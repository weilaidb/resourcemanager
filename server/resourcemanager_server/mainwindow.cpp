#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>


#define BINDPORT (88888)
#define RESOURCEPATH "./resource.txt"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(NULL)
{
    ui->setupUi(this);
    SetIPADDR_UI();
    logs.clear();
    lst_sources.clear();


    ReadHistorySettings();

    readResTxt(RESOURCEPATH);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::InitServer( QString ipaddr, quint16 listenport)
{
    if(NULL != tcpServer)
    {
        tcpServer->disconnect(this);
        tcpServer->close();
        tcpServer->deleteLater();
        tcpServer = NULL;
    }

    tcpServer = new QTcpServer(this);
    //if(!tcpServer->listen(QHostAddress::LocalHost,6666))
    //    if(!tcpServer->listen(QHostAddress("192.168.1.100"),6666))
    if(!tcpServer->listen(QHostAddress(ipaddr),listenport))
    {  //��������������6666�˿ڣ������������������Ϣ�����ر�

        qDebug() << tcpServer->errorString();
        //        close();
        return -1;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendResourceInfoMessage()));
    return 0;
}

void MainWindow::on_pushButton_clicked()
{
    QString ipaddr = ui->comboBox->currentText();
    if(ipaddr.isEmpty())
    {
        logsappendShow(QString("ip addr(%1) invalid").arg(ipaddr));
        return;
    }
    if(0 != CheckIPAddr(ipaddr))
    {
        logsappendShow(QString("ip addr(%1) invalid").arg(ipaddr));
        return;
    }
    if(0 != InitServer(ipaddr, BINDPORT))
    {
        ShowTipsInfo(str_china("����Ϣ, ��ʧ��"));
        logsappendShow(QString("bind addr(%1) fail!!").arg(ipaddr));

    }
    else
    {
        ShowTipsInfo(str_china("����Ϣ, �󶨳ɹ�"));
        logsappendShow(QString("bind addr(%1) ok").arg(ipaddr));
    }

}


void MainWindow::sendResourceInfoMessage()
{
    QByteArray block; //�����ݴ�����Ҫ���͵�����
    QDataStream out(&block,QIODevice::WriteOnly);
    //ʹ��������д������
    out.setVersion(QDataStream::Qt_4_0);
    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ
    //Ҫ���͵����ݷŵ�out
    QString sendstr("hello");
    out<<(quint16) 0;
    out<<sendstr;
    quint16 skpos = 0;
//    foreach(T_ResourceUse src, lst_sources)
//    {

//        T_ResourceUse_Print(&src);
//        out << src.devname << " ";
//        out << src.devip << " ";
//        out << src.netip << " ";
//        out << src.time << " ";
//        out << src.usr << " ";
//        out << src.notice << " ";
//        out << src.right << " ";
//        //Ҫ���͵����ݷŵ�out
////        out.device()->seek(skpos);
////        out<<(quint16)(block.size()-sizeof(quint16));
//        qDebug() << "@ block size :" << block.size();
//        //���¶�д��λ��
////        skpos = block.size();
//    }

    //Ҫ���͵����ݷŵ�out
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));
    qDebug() << "@ block size :" << block.size();
    //���¶�д��λ��
//    skpos = block.size();

    //Ҫ���͵����ݷŵ�out

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    //���ǻ�ȡ�Ѿ����������ӵ����׽���
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    clientConnection->write(block);

//    clientConnection->disconnectFromHost();

    ui->statusBar->showMessage("send message successful!!!");
    logsappendShow(QString("got new client(%1), send msg size:%2")
                   .arg(clientConnection->peerAddress().toString())
                   .arg(block.size()));
    //�������ݳɹ�����ʾ��ʾ
}



//void MainWindow::sendMessage()
//{
//    QByteArray block; //�����ݴ�����Ҫ���͵�����
//    QDataStream out(&block,QIODevice::WriteOnly);
//    //ʹ��������д������
//    out.setVersion(QDataStream::Qt_4_6);
//    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ
//    out<<(quint16) 0;
//    //Ҫ���͵����ݷŵ�out
//    out<<tr("hello Tcp!!!");
//    //Ҫ���͵����ݷŵ�out
//    out.device()->seek(0);
//    out<<(quint16)(block.size()-sizeof(quint16));
//    //Ҫ���͵����ݷŵ�out

//    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
//    //���ǻ�ȡ�Ѿ����������ӵ����׽���
//    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
//    clientConnection->write(block);

//    clientConnection->disconnectFromHost();

//    ui->statusBar->showMessage("send message successful!!!");
//    logsappendShow(QString("got new client(%1), send msg size:%2")
//                   .arg(clientConnection->peerAddress().toString())
//                   .arg(block.size()));
//    //�������ݳɹ�����ʾ��ʾ
//}

/*============================================
* FuncName    : MainWindow::ReadHistorySettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::ReadHistorySettings()
{
    QSettings m_settings("resourcemanager.com.cn", "resourcemanager");
    //    qDebug() << "read setting filename:" << m_settings.fileName();

    ui->comboBox->setEditText(m_settings.value("curipaddr").toString());
    ui->textBrowser->setText(m_settings.value("logs").toString());
    logs = m_settings.value("logs").toString();

    qDebug() <<  "reading from history settings, of comboBox_keytips :" << m_settings.value("comboBox_keytips").toInt();

    this->restoreGeometry(m_settings.value("resourcemanager_Geometry").toByteArray());
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
    QSettings m_settings("resourcemanager.com.cn", "resourcemanager");
    m_settings.setValue("curipaddr", ui->comboBox->currentText());
    m_settings.setValue("logs", logs);

    //    qDebug() <<  "writing from history settings, of comboBox_keytips :" << ui->comboBox_keytips->currentIndex();

    m_settings.setValue("resourcemanager_Geometry", this->saveGeometry());

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





//qtʵ��������ifconfig -a����
QList<QString> MainWindow::Getifconfig(void)
{
    QStringList envVariables;
    QByteArray username;
    QList<QHostAddress> broadcastAddresses;
    QList<QString> ipAddresses;
    QString ipAddressesstr;

    envVariables << "USERNAME.*" <<"USER.*" <<"USERDOMAIN.*"
                 <<"HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        if(-1 != index){
            QStringList stringList = environment.at(index).split("=");
            if(stringList.size() == 2){
                username = stringList.at(1).toUtf8();
                qDebug() << username.data();
                break;
            }
        }
    }

    broadcastAddresses.clear();
    ipAddresses.clear();
    qDebug() << "Interface numbers:"
             <<QNetworkInterface::allInterfaces().count();

    foreach (QNetworkInterface interface,
             QNetworkInterface::allInterfaces()) {
        qDebug() << "Interface name:" << interface.name() <<endl
                 <<"Interface hardwareAddress:"
                <<interface.hardwareAddress()<<endl
               <<"entry numbers:" << interface.addressEntries().count();
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            QHostAddress broadcastAddress = entry.broadcast();
            qDebug() << "entry ip:" << entry.ip()
                     <<"entry netmask:" <<entry.netmask();
            qDebug() << "ip addr:" << entry.ip().toString();
            //            ipAddressesstr = entry.ip().toString();
            //            if(ipAddressesstr.contains("192.168"))
            //            {
            //                return ipAddressesstr;
            //            }
            if(broadcastAddress != QHostAddress::Null){
                broadcastAddresses << broadcastAddress;
                ipAddresses << entry.ip().toString();
            }
        }

    }
    return ipAddresses;
}

int MainWindow::CheckIPAddr(QString ipaddr)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!regExp.exactMatch(ipaddr))
    {
        ShowTipsInfo(str_china("��ʾ, ip��ַ����"));
        return -1;
    }

    return 0;
}



void MainWindow::SetIPADDR_UI()
{
    QList<QString> machineiplist = Getifconfig();
    ui->comboBox->clear();
    foreach (QString ipaddr, machineiplist) {
        ui->comboBox->addItem(ipaddr);
    }
}

void MainWindow::on_pushButton_flushipaddr_clicked()
{
    logsappendShow(QString("flush ipaddr"));
    SetIPADDR_UI();
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

int MainWindow::readResTxt(const char *filepath)
{
    QFile file(filepath);
    if(!file.exists())
    {
        QString errinfo = QString("resource file no exist!! %1").arg(filepath);
        ShowTipsInfo(errinfo);
        logsappendShow(errinfo);
        return -1;
    }

    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    in.setCodec("UTF-8"); //��ע������
    while(!in.atEnd())     {
        QString line = in.readLine();
        qDebug() << line;
        AppendResInfo(line);
    }
    file.close();

}
QByteArray MainWindow::CMPINDEX(quint16 INDEX, QStringList &strlst)
{
    QString indexstr = strlst.at(INDEX);
    indexstr.replace(QRegExp("\\s+"), QString(""));
    return ((INDEX < strlst.size()) ? (indexstr.toLatin1().data()) : (""));
}

int MainWindow::AppendResInfo(QString line)
{
//#define CMPINDEX(INDEX, strlst)  \
//    strlst.at(INDEX).replace(" ", "") \
//    ((INDEX < strlst.size()) ? (strlst.at(INDEX).toLatin1().data()) : (""))


    T_ResourceUse tSrc = {0};
    QStringList strlst = line.split(QRegExp("\\s+"));
    logsappendShow(QString("line splist size:%1").arg(strlst.size()));

    quint16 index = 0;
    memcpy(tSrc.devname, CMPINDEX(index, strlst), sizeof(tSrc.devname));
    index++;
    memcpy(tSrc.devip, CMPINDEX(index, strlst), sizeof(tSrc.devip));
    index++;
    memcpy(tSrc.netip, CMPINDEX(index, strlst), sizeof(tSrc.netip));
    index++;
    memcpy(tSrc.time, CMPINDEX(index, strlst), sizeof(tSrc.time));
    index++;
    memcpy(tSrc.usr, CMPINDEX(index, strlst), sizeof(tSrc.usr));
    index++;
    memcpy(tSrc.notice, CMPINDEX(index, strlst), sizeof(tSrc.notice));
    index++;
    memcpy(tSrc.right, CMPINDEX(index, strlst), sizeof(tSrc.right));
    index++;

    T_ResourceUse_Print(&tSrc);
    lst_sources.push_back(tSrc);
    logsappendShow(QString("lst_sources size:%1").arg(lst_sources.size()));

}


void MainWindow::T_ResourceUse_Print(T_ResourceUse *p)
{
    if(!p)
    {
        return;
    }
    qDebug() << "=================";
    qDebug() << "devname :" << p->devname;
    qDebug() << "devip   :" << p->devip;
    qDebug() << "netip   :" << p->netip;
    qDebug() << "time    :" << p->time;
    qDebug() << "usr     :" << p->usr;
    qDebug() << "notice  :" << p->notice;
    qDebug() << "right   :" << p->right;

}


