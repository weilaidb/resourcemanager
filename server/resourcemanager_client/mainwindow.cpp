#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msgtips.h"
#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sockthread.h"
#include "kouling.h"
#include "keybutton.h"
#include <QtWidgets>

using namespace std;

#define BINDPORT (88888)


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pthreadsock(NULL)
{
    ui->setupUi(this);
    logs.clear();

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

    socket = new QTcpSocket();

    newConnect(ipaddr);

}

void MainWindow::newConnect(QString ipaddr)
{
    qDebug() << "-->>client socket:" << socket;
    if(socket)
    {
        socket->disconnectFromHost();
        socket->abort();
        socket->close();
        socket = NULL;
        if(pthreadsock)
        {
            pthreadsock->deleteLater();
            pthreadsock = NULL;
        }
    }
    ShowTipsInfoWithShowTime(str_china("正在连接中..."), 3000);
    qApp->processEvents();

    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    socket->connectToHost(ipaddr, BINDPORT);
    connect(socket, SIGNAL(connected()), this, SLOT(hellosocket()));




    qDebug() <<"socket err:" << socket->errorString();
    qDebug() <<"remote ip addr:" << ipaddr;
    logsappendShow(QString("connecting ip:%1").arg(ipaddr));

    if(pthreadsock == NULL)
    {
        pthreadsock = new sockthread(this);
        pthreadsock->setSocketConnect(socket);
        QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString)),
                         this,SLOT(readfromremote(QString)));
        QObject::connect(pthreadsock,SIGNAL(emitErrInfo(QString)),
                         this,SLOT(procErrMsg(QString)));
        pthreadsock->start();

//        ui->verticalLayout_resource->removeItem();

    }

    //当返回false时可以调用error来确定无法连接的原因
    if(!socket->waitForConnected(3000))
    {
        logsappendShow(str_china("连接超时，请检查服务器地址是否正确"));
        return;
    }



}
/*============================================
* FuncName    : autoCCode::hellosocket
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::hellosocket()
{
    logsappendShow(QString::fromLocal8Bit("连接服务器成功！！"));
    pthreadsock->sendmsg(CMD_FETCH_SRC);
}

void MainWindow::procErrMsg(QString errmsg)
{
    logsappendShow(errmsg);
}


void MainWindow::readfromremote(QString cltmsg)
{
    logsappendShow(QString("read clt msg:%1").arg(cltmsg));
    if(cltmsg == CMD_REPLY_SRC)
    {
        qDebug() << "Show @ UI";
        deleteBeforeShow();
        showTitle();
        QLabel *infolable = new QLabel;
        infolable->setText(cltmsg);
        infolable->show();

        ui->verticalLayout_resource->addWidget(infolable);
        showuilist.push_back(infolable);

        QStringList opstime;
        opstime << "day" <<  "am" << "pm";

        QStringList usrlist;
        usrlist << "" << str_china("小屁孩") <<  str_china("龙龙") << str_china("奇奇");

        showOneRowUI("S3-2",
                     "192.168.1.12",
                     "70.70.70.70",
                     opstime,
                     1,
                     usrlist,
                     2,
                     "beijing");

//        setLayout(ui->verticalLayout_resource);


    }
}

void MainWindow::showTitle()
{

    //devname  devip           netip          time   usr       notice
    QLabel *title = new QLabel;
    title->setText("devname      devip   netip          time   usr       notice");
    // 设置字体：微软雅黑、点大小36
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(36);
    font.setItalic(false);

    title->setFont(font);
    ui->verticalLayout_resource->addWidget(title);
    showuilist.push_back(title);
}

void MainWindow::deleteBeforeShow()
{
    foreach (QWidget *q, showuilist) {
        ui->verticalLayout_resource->removeWidget(q);
        delete q;
    }
    showuilist.clear();

}

void MainWindow::showOneRowUI(QString devname,
                              QString devip,
                              QString netip,
                              QStringList timelist,
                              quint32 timeindex,
                              QStringList usrlist,
                              quint32 usrindex,
                              QString notice)
{
    // 设置字体：微软雅黑、点大小36
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(30);
    font.setItalic(false);

#define SETFONT(WIDGET)\
    WIDGET->setFont(font);


    QLabel *pDevName = new QLabel(devname);
    SETFONT(pDevName);
    QLineEdit *pDevIP = new QLineEdit(devip);
    SETFONT(pDevIP);
    QLineEdit *pNetIP = new QLineEdit(netip);
    SETFONT(pNetIP);

    QComboBox *pTime = new QComboBox;
    SETFONT(pTime);
    pTime->addItems(timelist);
    pTime->setCurrentIndex(timeindex);

    QComboBox *pUsr = new QComboBox;
    SETFONT(pUsr);
    pUsr->addItems(usrlist);
    pUsr->setCurrentIndex(usrindex);

    QLineEdit *pNotice = new QLineEdit(notice);
    SETFONT(pNotice);

    KeyButton *prequestBtn = new KeyButton(QString(QString::fromLocal8Bit("申请") + devname));
    connect(prequestBtn, SIGNAL(keyClicked(QString)), this, SLOT(Proc_RequestSrcItem(QString)));
    SETFONT(prequestBtn);

    //水平排列
    QHBoxLayout *pHItemLay = new QHBoxLayout;
    pHItemLay->addWidget(pDevName);
    pHItemLay->addWidget(pDevIP);
    pHItemLay->addWidget(pNetIP);
    pHItemLay->addWidget(pTime);
    pHItemLay->addWidget(pUsr);
    pHItemLay->addWidget(pNotice);
    pHItemLay->addWidget(prequestBtn);
//    pHItemLay->addWidget(pDevName);




    ui->verticalLayout_resource->addLayout(pHItemLay);

    showuilist.push_back(pDevName);
    showuilist.push_back(pDevIP);
    showuilist.push_back(pNetIP);
    showuilist.push_back(pTime);
    showuilist.push_back(pUsr);
    showuilist.push_back(pNotice);
    showuilist.push_back(prequestBtn);

}

void MainWindow::Proc_RequestSrcItem(QString text)
{
    qDebug() << "btn pressed:" << str_china(text.toLocal8Bit().data());
}


