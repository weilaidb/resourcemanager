#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msgtips.h"

#define BINDPORT (88888)


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logs.clear();
    //读数据统计
    TotalReadBytes = 0;
    bytesReceived  = 0;
    bytesNeedRecv  = 0;

    ReadHistorySettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}



/*============================================
* FuncName    : MainWindow::ReadHistorySettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::ReadHistorySettings()
{
    QSettings m_settings("resourcemanagerclt.com.cn", "resourcemanagerclt");
    //    qDebug() << "read setting filename:" << m_settings.fileName();

    ui->comboBox->setEditText(m_settings.value("curipaddr").toString());
    ui->textBrowser->setText(m_settings.value("logs").toString());
    logs = m_settings.value("logs").toString();

    qDebug() <<  "reading from history settings, of comboBox_keytips :" << m_settings.value("comboBox_keytips").toInt();

    this->restoreGeometry(m_settings.value("resourcemanagerclt_Geometry").toByteArray());
}

/*============================================
* FuncName    : MainWindow::WriteCurrentSettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::WriteCurrentSettings()
{
    QSettings m_settings("resourcemanagerclt.com.cn", "resourcemanagerclt");
    m_settings.setValue("curipaddr", ui->comboBox->currentText());
    m_settings.setValue("logs", logs);

//    qDebug() <<  "writing from history settings, of comboBox_keytips :" << ui->comboBox_keytips->currentIndex();

    m_settings.setValue("resourcemanagerclt_Geometry", this->saveGeometry());

    //    qDebug() << "setting filename:" << m_settings.fileName();

}



/*============================================
* FuncName    : MainWindow::closeEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainWindow closeEvent";
    WriteCurrentSettings();
    event->accept();
}


void MainWindow::logsappendShow(QString log)
{
    QDate date;
    QTime time;
    logs += date.currentDate().toString("yyyy-MM-dd");
    logs += time.currentTime().toString(" HH:mm:ss");
    logs +=":";

    logs +=log + "\n";
    ui->textBrowser->setText(logs);

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);

}
int MainWindow::CheckIPAddr(QString ipaddr)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!regExp.exactMatch(ipaddr))
    {
        ShowTipsInfo(str_china("提示, ip地址错误"));
        logsappendShow(QString("ip %1  invalid!!!").arg(ipaddr));
        return -1;
    }

    return 0;
}

void MainWindow::on_pushButton_clicked()
{
    QString ipaddr = ui->comboBox->currentText();
    if(0 != CheckIPAddr(ipaddr))
    {
        return;
    }

    tcpSocket = new QTcpSocket();

    newConnect(ipaddr);

}

void MainWindow::newConnect(QString ipaddr)
{
    blockSize = 0; //初始化其为0
    tcpSocket->abort(); //取消已有的连
    //tcpSocket->connectToHost(ui->hostLineEdit->text(),ui->portLineEdit->text().toInt());
    //tcpSocket->connectToHost(QHostAddress("192.168.1.100"),ui->portLineEdit->text().toInt());
    //tcpSocket->connectToHost(QHostAddress::LocalHost,ui->portLineEdit->text().toInt());
    tcpSocket->connectToHost((ipaddr), BINDPORT);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(updateReadMsgProgress()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayErr(QAbstractSocket::SocketError)));

    //连接到主机，这里从界面获取主机地址和端口号
    logsappendShow(QString("new connect ip:%1").arg(ipaddr));
}

void MainWindow::readMessage()
{
    qDebug() << "read message:" << endl;

    QByteArray qba = tcpSocket->readAll();
    QString ss=QVariant(qba).toString();

    qDebug() << ss << endl;
    logsappendShow(QString("read message size :%1").arg(ss.length()));
//    ui->textBrowser->setText(ss);

//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_6);
//    //设置数据流版本，这里要和服务器端相同
//    if(blockSize==0) //如果是刚开始接收数据
//    {
//        //判断接收的数据是否有两字节，也就是文件的大小信息
//        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
//        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//        in >> blockSize;
//        logsappendShow(QString("read message size :%1").arg(blockSize));
//    }
//    if(tcpSocket->bytesAvailable() < blockSize) return;

//    //如果没有得到全部的数据，则返回，继续接收数据
////    message = tcpSocket->readAll();
//    in >> message;
//    qDebug() << "message :" << message << ", size:" << message.size();
//    //将接收到的数据存放到变量中
//    logsappendShow(message.toLocal8Bit());
////    ui->messageLabel->setText(message);
//    //显示接收到的数据
}


/*============================================
* FuncName    : sockthread::updateReadMsgProgress
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::updateReadMsgProgress()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    qDebug() << "updateReadMsgProgress !!!" << endl;

    static bool recvdone = READ_DONE;

    if(bytesReceived <= sizeof(qint16)*1  && (recvdone == READ_DONE)){
        if((tcpSocket->bytesAvailable() >= sizeof(qint16)*1)){
            in>>TotalReadBytes;
            qDebug() << "TotalReadBytes:" << TotalReadBytes << endl;
//            bytesReceived += sizeof(qint16)*1;
            inBlock.resize(0);
            recvdone = READING;
        }
    }


    if (bytesReceived < TotalReadBytes){
        /* 实际需要接收的字节数 */
        bytesNeedRecv = TotalReadBytes - bytesReceived;
        bytesReceived += tcpSocket->bytesAvailable();

        if(bytesReceived >= TotalReadBytes){
            inBlock.append(tcpSocket->read(bytesNeedRecv));
            bytesReceived = TotalReadBytes;
        }else{
            inBlock.append(tcpSocket->readAll());
        }

        qDebug() << "bytesReceived:"<< bytesReceived;
        qDebug() << "TotalReadBytes   :"<< TotalReadBytes;
        qDebug() << "inBlock size          :"<< inBlock.size();
    }

    if (bytesReceived == TotalReadBytes) {
        QString  bigmsg = inBlock;
//        ui->textBrowser->setText(inBlock);
        ui->textBrowser->append(inBlock);
        //入库
//        readfromremote(bigmsg);
//        emit emitMsgDoneSignal(bigmsg);
        qDebug() << "bigmsg size:" << bigmsg.size() << endl;
//        logsappendShow(bigmsg);
        qDebug() << "read done !!! bigmsg:" << bigmsg.toUtf8().data() << endl;

        TotalReadBytes = 0;
        bytesReceived = 0;
//        fileNameSize = 0;
        bytesNeedRecv = 0;
        inBlock.resize(0);

        recvdone = READ_DONE;
    }


}




void MainWindow::displayErr(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error message:" << endl;
    qDebug() << tcpSocket->errorString() << endl;
    logsappendShow(QString("error message :%1").arg(tcpSocket->errorString()));
    tcpSocket->abort();
    tcpSocket->close();
}
