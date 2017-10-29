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


//UDP Process
#include<QtNetwork>
//#define GET_HOST_COMMAND "GetCYHost"
#define GET_HOST_COMMAND "GetIPAddr"
#define LOCAL_PORT 11121
#define DEST_PORT 12811

#define TRY_TIMES 1




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pthreadsock(NULL)
{
    ui->setupUi(this);
    lst_sources.clear();

    ReadHistorySettings();
    logs.clear();

    QObject::connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this,
                     SLOT(setPushBtnEnable(QString)));

    QFile styleSheet(":/qss/myStyle.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");

    }
    else
    {
//        qApp->setStyleSheet(styleSheet.readAll());
        this->setStyleSheet(styleSheet.readAll());
    }


    // UDP Process
    initBroadcast();
    Sleep(2000);
    on_pushButton_clicked();


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
    ui->pushButton->setEnabled(false);

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
    ShowTipsInfoWithShowTime(str_china("正在连接中..."), 2000);
    qApp->processEvents();

    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    socket->connectToHost(ipaddr, BINDPORT);
    connect(socket, SIGNAL(connected()), this, SLOT(hellosocket()));




    qDebug() <<"socket err:" << socket->errorString();
    qDebug() <<"remote ip addr:" << ipaddr;
    logsappendShow(QString("connecting ip:%1").arg(ipaddr));


    //当返回false时可以调用error来确定无法连接的原因
    if(!socket->waitForConnected(3000))
    {
        logsappendShow(str_china("连接超时，请检查服务器地址是否正确"));
        ui->pushButton->setEnabled(true);
        //出错后，广播查找sever
        BroadcastGetIpCommand();
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

    if(pthreadsock == NULL)
    {
        pthreadsock = new sockthread(this);
        pthreadsock->setSocketConnect(socket);
        QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString,void*)),
                         this,SLOT(readfromremote(QString,void*)));
        QObject::connect(pthreadsock,SIGNAL(emitErrInfo(QString,void*)),
                         this,SLOT(procErrMsg(QString,void*)));
        pthreadsock->start();
        pthreadsock->sendmsg(CMD_FETCH_SRC);
    }



}

void MainWindow::procErrMsg(QString errmsg,void*)
{
    logsappendShow(errmsg);
    ui->pushButton->setEnabled(true);

    for(it_ShowUi = lst_ShowUi.begin(); it_ShowUi != lst_ShowUi.end(); it_ShowUi++)
    {
        T_LocalUiForShow & rowUi = *it_ShowUi;
        rowUi.prequestBtn->setEnabled(false);
    }
}


void MainWindow::readfromremote(QString cltmsg, void*)
{
    logsappendShow(QString("read clt msg:%1").arg(cltmsg));
    if(cltmsg.contains(CMD_REPLY_SRC))
    {//处理收到的资源信息并显示到界面
        qDebug() << "Show @ UI";

        deleteBeforeShow();
        showTitle();
#ifdef TESTSHOW
        test_showoneresource(cltmsg);
#else
        showResources(cltmsg.mid(strlen(CMD_REPLY_SRC)));
#endif
        //        setLayout(ui->verticalLayout_resource);


        ShowTipsInfoWithShowTime(str_china("更新完成"), 1500);
    }
}


void MainWindow::showResources(QString cltmsg)
{
    lst_sources.clear();

    cltmsg = cltmsg.left(cltmsg.length() - 1);

    QStringList splitmsg = cltmsg.mid(1).split("\",\"");
    qDebug() << "splist cltmsg size:" << splitmsg.size();



#define COLUMNSIZE (9)

    quint16 index = 0;
    T_ResourceUse tSrc = {0};
    for(index = 0; index < splitmsg.size(); index++)
    {
        switch( index % COLUMNSIZE )
        {
        case 0:
            tSrc.devname = splitmsg.at(index);
            break;
        case 1:
            tSrc.devip = splitmsg.at(index);
            break;
        case 2:
            tSrc.netip = splitmsg.at(index);
            break;
        case 3:
            tSrc.timelst = splitmsg.at(index);
            break;
        case 4:
            tSrc.time = splitmsg.at(index);
            break;
        case 5:
            tSrc.usrlist = splitmsg.at(index);
            break;
        case 6:
            tSrc.usr = splitmsg.at(index);
            break;
        case 7:
            tSrc.notice = splitmsg.at(index);
            break;
        case 8:
            tSrc.right = splitmsg.at(index);
            break;
        default:

            break;
        }
        if((index + 1) % COLUMNSIZE == 0 && 0 != index)
        {
            T_ResourceUse_Print(&tSrc);
            lst_sources.push_back(tSrc);
        }

    }

    logsappendShow(QString("lst_sources size:%1").arg(lst_sources.size()));

    //这个数据特别长了，导致整体界面变长，怪不得布局半天没效果呢
    //    QLabel *infolable = new QLabel;
    //    infolable->setText(cltmsg);
    //    //    infolable->show();

    //    ui->verticalLayout_resource->addWidget(infolable);
    //    showuilist.push_back(infolable);

    QStringList opstime;
    opstime << "day" <<  "am" << "pm";

    QStringList usrlist;
    usrlist << "" << str_china("小屁孩") <<  str_china("龙龙") << str_china("奇奇");


    lst_ShowUi.clear();

    //    lst_sources.size();
    for(it_src = lst_sources.begin(); it_src != lst_sources.end(); it_src++)
    {
        T_ResourceUse &tmp = *it_src;
        opstime  = getlistbydouhao(tmp.timelst);
        usrlist  = getlistbydouhao(tmp.usrlist);
        showOneRowUI(tmp.devname,
                     tmp.devip,
                     tmp.netip,
                     opstime,
                     tmp.time,
                     usrlist,
                     tmp.usr,
                     tmp.notice);
    }

}

void MainWindow::test_showoneresource(QString cltmsg)
{
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
                 "day",
                 usrlist,
                 "weiwei",
                 "beijing");
}



void MainWindow::showTitle()
{

    //devname  devip           netip          time   usr       notice
    QLabel *title = new QLabel;
    title->setText(str_china("名称              设备IP         网元IP           申请时间         用户          备注                               请申请"));
    // 设置字体：微软雅黑、点大小20
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(20);
    font.setItalic(false);

    title->setFont(font);
    title->setStyleSheet("QLabel{"
                         "background:transparent;"
                         "border:2px solid green;"
                         "border-radius:4px;"
                         "padding:2px;"
                         "color:blue;"
                         "}   ");
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
                              QString timeindex,
                              QStringList usrlist,
                              QString usrindex,
                              QString notice)
{
    // 设置字体：微软雅黑、点大小14
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(14);
    font.setItalic(false);

#define SETFONT(WIDGET, WIDTHSIZE)\
    WIDGET->setFont(font);\
    WIDGET->setFixedWidth(WIDTHSIZE);

#define CONVERT_CHIN(TEXT)\
    str_china_utf8(TEXT.toUtf8().data())

#define WIDTHSIZE (150)

    QLabel *pDevName = new QLabel(CONVERT_CHIN(devname));
    SETFONT(pDevName, WIDTHSIZE);
    pDevName->setStyleSheet("QLabel{"
                         "background:transparent;"
                         "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                          "stop: 0 #007500, stop: 1 #a6ffa6);"
                         "border-radius:4px;"
                         "padding:2px;"
                         "color:white;"
                         "}   ");

    QLineEdit *pDevIP = new QLineEdit(CONVERT_CHIN(devip));
    SETFONT(pDevIP, WIDTHSIZE);
    QLineEdit *pNetIP = new QLineEdit(CONVERT_CHIN(netip));
    SETFONT(pNetIP, WIDTHSIZE);

    QComboBox *pTime = new QComboBox;
    SETFONT(pTime, WIDTHSIZE);
    pTime->addItems(timelist);
    pTime->setCurrentText(CONVERT_CHIN(timeindex));

    QComboBox *pUsr = new QComboBox;
    SETFONT(pUsr, WIDTHSIZE);
    pUsr->addItems(usrlist);
    pUsr->setCurrentText(CONVERT_CHIN(usrindex));

    QLineEdit *pNotice = new QLineEdit(CONVERT_CHIN(notice));
    pNotice->setFixedWidth(WIDTHSIZE * 2);
    SETFONT(pNotice, WIDTHSIZE * 2);

    KeyButton *prequestBtn = new KeyButton(QString(QString::fromLocal8Bit("申请") + devname));
    connect(prequestBtn, SIGNAL(keyClicked(QString)), this, SLOT(Proc_RequestSrcItem(QString)));
    SETFONT(prequestBtn, WIDTHSIZE);
    prequestBtn->setStyleSheet("QPushButton {"
                               "    /* Let's make the size of the button 1,5 times of font size. */"
                               "    min-height: 1.5em;"
                               "    /* Font size just 1.*/"
                               "    font: 1em;"
                               "    /* Margins so that we get a little space on the left and right. */"
                               "    margin: 0 1px 0 1px;"
                               "    /* The font color */ "
                               "    color: white;"
                               "    /* Here's the background gradient with start point, end point, "
                               "       stop \"percentage\" and color, stop percentage and color. */"
                               "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                               "                                      stop: 0 #2198c0, stop: 1 #0d5ca6);"
                               "    /* We'll round the borders. */"
                               "    border-style: outset;"
                               "    /* Round radius will be 3px */"
                               "    border-radius: 3px;"
                               "    /* Border is only one pixel */"
                               "    border-width: 1px;"
                               "    /* Border color is now set */"
                               "    border-color: #0c457e;"
                               "}"
                               " "
                               "/* This is style when button is pressed */"
                               "QPushButton:pressed {"
                               "    /* We'll just invert the gradient by changing the colors around. */"
                               "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                               "                                      stop: 0 #0d5ca6, stop: 1 #2198c0);"
                               "}"
                               " "
                               "QPushButton:flat {"
                               "    border: none;"
                               "}"
                               "QPushButton:disabled {"
                               "	color: grey"
                               "}"
                               );


    //水平排列
    QHBoxLayout *pHItemLay = new QHBoxLayout;
    pHItemLay->addWidget(pDevName);
    pHItemLay->addStretch(30);
    pHItemLay->addWidget(pDevIP);
    pHItemLay->addStretch(30);
    pHItemLay->addWidget(pNetIP);
    pHItemLay->addStretch();
    pHItemLay->addWidget(pTime);
    pHItemLay->addStretch();
    pHItemLay->addWidget(pUsr);
    pHItemLay->addStretch();
    pHItemLay->addWidget(pNotice);
    pHItemLay->addStretch();
    pHItemLay->addWidget(prequestBtn);
    pHItemLay->addStretch();



    ui->verticalLayout_resource->addLayout(pHItemLay);
    ui->verticalLayout_resource->setSpacing(10);


    showuilist.push_back(pDevName);
    showuilist.push_back(pDevIP);
    showuilist.push_back(pNetIP);
    showuilist.push_back(pTime);
    showuilist.push_back(pUsr);
    showuilist.push_back(pNotice);
    showuilist.push_back(prequestBtn);

//保存到结构列表中
    T_LocalUiForShow rowUi;
    rowUi.pDevName = pDevName;
    rowUi.pDevIP = pDevIP;
    rowUi.pNetIP = pNetIP;
    rowUi.pTime = pTime;
    rowUi.pUsr = pUsr;
    rowUi.pNotice = pNotice;
    rowUi.prequestBtn = prequestBtn;

    lst_ShowUi.push_back(rowUi);


}

void MainWindow::Proc_RequestSrcItem(QString text)
{
    qDebug() << "btn pressed:" << str_china(text.toLocal8Bit().data());

    for(it_ShowUi = lst_ShowUi.begin(); it_ShowUi != lst_ShowUi.end(); it_ShowUi++)
    {
        T_LocalUiForShow & rowUi = *it_ShowUi;
        if(text == rowUi.prequestBtn->text())
        {
            Proc_RequestUpdate(rowUi);
        }
    }
}



void MainWindow::Proc_RequestUpdate(T_LocalUiForShow &rowUi)
{
#define ADDRESULT(VAL)\
    result +=  AddYinHao(VAL) + ",";

    QString result;
    result.clear();

    qDebug() << "request for update " << str_china(rowUi.prequestBtn->text().toLocal8Bit().data());
    logsappendShow("request for update " + str_china(rowUi.prequestBtn->text().toLocal8Bit().data()));

    T_ResourceUse tmp;
    tmp.devname = rowUi.pDevName->text();
    tmp.devip   = rowUi.pDevIP->text();
    tmp.netip   = rowUi.pNetIP->text();
    tmp.time    = rowUi.pTime->currentText();
    tmp.usr     = rowUi.pUsr->currentText();
    tmp.notice  = rowUi.pNotice->text();
    tmp.right   = "";

    ADDRESULT(tmp.devname);
    ADDRESULT(tmp.devip);
    ADDRESULT(tmp.netip);
    ADDRESULT(tmp.timelst);
    ADDRESULT(tmp.time);
    ADDRESULT(tmp.usrlist);
    ADDRESULT(tmp.usr);
    ADDRESULT(tmp.notice);
    ADDRESULT(tmp.right);


    result = result.left(result.length() -1 );


    pthreadsock->sendmsg(CMD_UPDATE_SRC + result);
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

    qDebug() << "timelist:" << p->timelst;
    qDebug() << "usrlist :" << p->usrlist;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // Your code here
}


QStringList MainWindow::getlistbydouhao(QString str)
{
    QStringList lst;
    lst.clear();
    if(str.isEmpty())
        return lst;
    lst = str.split(",");
    return lst;
}


void MainWindow::setPushBtnEnable(QString st)
{
    ui->pushButton->setEnabled(true);
}

QString MainWindow::AddYinHao(QString str)
{
    return ("\"" + str.replace("\"", "\"\"") + "\"");
}



//QLabel *pDevName = new QLabel(CONVERT_CHIN(devname));
//QLineEdit *pDevIP = new QLineEdit(CONVERT_CHIN(devip));
//QLineEdit *pNetIP = new QLineEdit(CONVERT_CHIN(netip));
//QComboBox *pTime = new QComboBox;
//QComboBox *pUsr = new QComboBox;
//QLineEdit *pNotice = new QLineEdit(CONVERT_CHIN(notice));
//KeyButton *prequestBtn = new KeyButton(QString(QString::fromLocal8Bit("申请") + devname));





void MainWindow::initBroadcast()
{
    receiver = new QUdpSocket(this);
    /////绑定，第一个参数为端口号，第二儿表示允许其它地址链接该广播
    receiver->bind(LOCAL_PORT,QUdpSocket::ShareAddress);

    //readyRead:每当有数据报来时发送这个信号
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPengingDatagram()));

    BroadcastGetIpCommand();
}

void MainWindow::BroadcastGetIpCommand()
{
    //QByteArray datagram = "Hello World!";
    qDebug() << "BroadcastGetIpCommand !!!" ;
    QByteArray datagram = GET_HOST_COMMAND;
    int times = TRY_TIMES;
    while(times--)
    {
        //sender->writeDatagram(datagram.data(),datagram.size(),QHostAddress::Broadcast,1066);
        receiver->writeDatagram(datagram.data(),datagram.size(),QHostAddress::Broadcast,DEST_PORT);
    }
}

void MainWindow::processPengingDatagram()
{
    //数据报不为空
    while( receiver->hasPendingDatagrams() )
    {
        QByteArray datagram;
        //datagram大小为等待处理数据报的大小才能就收数据;
        datagram.resize( receiver->pendingDatagramSize() );
        //接收数据报
        receiver->readDatagram(datagram.data(),datagram.size());
        //label->setText(datagram);
        addIpItem(datagram);
    }
}

void MainWindow::addIpItem(QByteArray data)
{
    logsappendShow(data);
    QList<QByteArray> serverip = data.split(':');
    qDebug() << "server ip :" << serverip.at(1);
    if(serverip.size() > 1)
    {
        QString actualsvrip = serverip.at(1).simplified();
        ui->comboBox->setCurrentText(serverip.at(1).simplified());

        on_pushButton_clicked();

    }
//    QListWidgetItem* lst1 = new QListWidgetItem(data, mlistWidget);
//    mlistWidget->insertItem(1, lst1);
}

