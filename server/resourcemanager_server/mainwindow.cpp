#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>


#define BINDPORT (88888)
#define RESOURCEPATH "resource.txt"
#define RESOURCEBACKPATH "resource.txt.backup"
#define USRLIST "usrlist.txt"


/**
服务器侧环境信息的内容 RESOURCEPATH

devname  devip           netip          time   usr       notice
S3-2     10.85.159.20    70.70.70.70    day    xxxx     北京在使用
S3-3     10.85.159.50    90.90.90.90    day    yyy
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(NULL),
    dateTimer(NULL)
{
    ui->setupUi(this);
    SetIPADDR_UI();
    lst_sources.clear();
    //写数据统计
    TotalBytes   = 0;
    byteWritten  = 0;
    bytesToWrite = 0;

    ReadHistorySettings();
    logs.clear();
    socklist.clear();

    reLoadUsrList();
    reLoadResource();

    QObject::connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this,
                     SLOT(enablebindpushtbn(QString)));

    dateTimer = new QTimer(this);
    QObject::connect(dateTimer,SIGNAL(timeout()), this,
                     SLOT(Proc_DateChange()));
    dateTimer->start(1000 * 60 * 2);//2分钟检测一次时期是否变化
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::InitServer( QString ipaddr, quint16 listenport)
{
    if(NULL != tcpServer)
    {
        closeallclientsocket();
        tcpServer->disconnect(this);
        tcpServer->close();
//        tcpServer->deleteLater();
        tcpServer = NULL;
    }

    tcpServer = new QTcpServer(this);
    //if(!tcpServer->listen(QHostAddress::LocalHost,6666))
    //    if(!tcpServer->listen(QHostAddress("192.168.1.100"),6666))
    if(!tcpServer->listen(QHostAddress(ipaddr),listenport))
    {  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭

        qDebug() << tcpServer->errorString();
        //        close();
        return -1;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(procClientMessage()));
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
        ShowTipsInfo(str_china("绑定信息, 绑定失败"));
        qApp->processEvents();
        logsappendShow(QString("bind addr(%1) fail!!").arg(ipaddr));
        ui->pushButton->setEnabled(true);

    }
    else
    {
        ShowTipsInfo(str_china("绑定信息, 绑定成功"));
        qApp->processEvents();
        logsappendShow(QString("bind addr(%1) ok").arg(ipaddr));
        ui->pushButton->setEnabled(false);
    }

}


void MainWindow::procClientMessage()
{
    clientConnection = tcpServer->nextPendingConnection();

    qDebug() << "-->>client socket:" << clientConnection;
    sockthread *pthreadsock = new sockthread(this);
    pthreadsock->setSocketConnect(clientConnection);
    QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString,void*)),
                     this,SLOT(readfromremote(QString, void *)));
    QObject::connect(pthreadsock,SIGNAL(emitErrInfo(QString,void*)),
                     this,SLOT(dealclienterror(QString, void *)));


    pthreadsock->start();
    socklist.push_back(pthreadsock);
    qDebug() << "-->>socklist size:" << socklist.size();


    ui->statusBar->showMessage("proc new client message...");
    logsappendShow(QString("got new client(%1)")
                   .arg(clientConnection->peerAddress().toString()));
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
    saveResource();
    event->accept();
}





//qt实现类似于ifconfig -a功能
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

    foreach (QNetworkInterface netinterface,
             QNetworkInterface::allInterfaces()) {
        qDebug() << "Interface name:" << netinterface.name() <<endl
                 <<"Interface hardwareAddress:"
                <<netinterface.hardwareAddress()<<endl
               <<"entry numbers:" << netinterface.addressEntries().count();
        foreach (QNetworkAddressEntry entry, netinterface.addressEntries()) {
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
        ShowTipsInfo(str_china("提示, ip地址错误"));
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
    logsappendShow(QString("flush ipaddr and reload resources infos"));
    SetIPADDR_UI();
    reLoadUsrList();
    reLoadResource();
    replyclientwhenflush();
}

void MainWindow::whendatechange_flushipaddr()
{
    logsappendShow(QString("when date change flush ipaddr and reload resources infos"));
//    SetIPADDR_UI();
    reLoadUsrList();
    reLoadResource();
    replyclientwhenflush();
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
    lst_sources.clear();

    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    in.setCodec("UTF-8"); //请注意这行
    while(!in.atEnd())     {
        QString line = in.readLine();
        qDebug() << line;
        if(line.contains("devname"))
            continue;
        AppendResInfo(line);
    }
    file.close();
    return 0;
}
int MainWindow::writeResTxt(const char *filepath)
{
    QFile file(filepath);
//    if(!file.exists())
//    {
//        QString errinfo = QString("save resource file no exist!! %1").arg(filepath);
//        ShowTipsInfo(errinfo);
//        logsappendShow(errinfo);
//        return -1;
//    }
//    lst_sources.clear();

//    devname  devip           netip          time   usr       notice
//    S3-1     10.85.159.20    70.70.70.70    day    佳佳     数据变更



    file.open(QIODevice::WriteOnly);

    QTextStream in(&file);
    in.setCodec("UTF-8"); //请注意这行
    QString TITLE= "devname  devip           netip          time   usr       notice right";
    in << TITLE + "\n";

#define ADDSPACE(VAL)\
    result +=  (VAL) + "   ";

    QString result;
    result.clear();
//    lst_sources.size();
    for(it_src = lst_sources.begin(); it_src != lst_sources.end(); it_src++)
    {
        T_ResourceUse &tmp = *it_src;
        result.clear();

        ADDSPACE(tmp.devname);
        ADDSPACE(tmp.devip);
        ADDSPACE(tmp.netip);
//        ADDSPACE(tmp.timelst);
        ADDSPACE(tmp.time);
//        ADDSPACE(tmp.usrlist);
        ADDSPACE(tmp.usr);
        ADDSPACE(tmp.notice);
        ADDSPACE(tmp.right);
        in << result + "\n";

    }

    file.close();

    return 0;
}




QString MainWindow::readUsrInfoTxt(const char *filepath)
{
    QString usrlist;
    usrlist.clear();

    QFile file(filepath);
    if(!file.exists())
    {
        QString errinfo = QString("usrlist file no exist!! %1").arg(filepath);
        ShowTipsInfo(errinfo);
        logsappendShow(errinfo);
        usrlist = "";
        return usrlist;
    }



    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    in.setCodec("UTF-8"); //请注意这行
    while(!in.atEnd())     {
        QString line = in.readLine();
        qDebug() << line;
        usrlist += line.replace("\"", "\"\"").simplified() + ",";
    }
    file.close();

    usrlist = usrlist.left(usrlist.length() - 1 );

    return usrlist;

}

int MainWindow::reLoadResource()
{
    QDate date;
    QString datename = date.currentDate().toString("rcmgryyyy-MM-dd");
    QString filename = datename + RESOURCEPATH ;

    if(0 != readResTxt(filename.toLocal8Bit().data()))
    {
        readResTxt(RESOURCEPATH);
    }
    return 0;
}
int MainWindow::saveResource()
{
    QDate date;
    QString datename = date.currentDate().toString("rcmgryyyy-MM-dd");
    QString filename = datename + RESOURCEPATH ;

    writeResTxt(filename.toLocal8Bit().data());

//    writeResTxt(RESOURCEBACKPATH);
    return 0;
}



int MainWindow::reLoadUsrList()
{
    UserList.clear();
    UserList =  readUsrInfoTxt(USRLIST);
    return 0;
}



QByteArray MainWindow::CMPINDEX(quint16 INDEX, QStringList &strlst)
{
    QString indexstr = strlst.at(INDEX);
    indexstr.replace(QRegExp("\\s+"), QString(""));
    return ((INDEX < strlst.size()) ? (indexstr.toLatin1().data()) : (""));
}

//使用松散数据.csv "abc","s3-2","a,b,c,d"
int MainWindow::AppendResInfo(QString line)
{
//#define CMPINDEX(INDEX, strlst)  \
//    strlst.at(INDEX).replace(" ", "") \
//    ((INDEX < strlst.size()) ? (strlst.at(INDEX).toLatin1().data()) : (""))


    T_ResourceUse tSrc = {0};
    QStringList strlst = line.split(QRegExp("\\s+"));
//    logsappendShow(QString("line splist size:%1").arg(strlst.size()));

    quint16 index = 0;
    for(index = 0; index < strlst.size(); index++)
    {
        switch( index )
        {
        case 0:
            tSrc.devname = strlst.at(index);
            break;
        case 1:
            tSrc.devip = strlst.at(index);
            break;
        case 2:
            tSrc.netip = strlst.at(index);
            break;
        case 3:
            tSrc.time = strlst.at(index);
            break;
        case 4:
            tSrc.usr = strlst.at(index);
            break;
        case 5:
            tSrc.notice = strlst.at(index);
            break;
        case 6:
            tSrc.right = strlst.at(index);
            break;
        default:

            break;
        }

    }

    tSrc.timelst = "day,am,pm";
    tSrc.usrlist = UserList;

    T_ResourceUse_Print(&tSrc);
    lst_sources.push_back(tSrc);
//    logsappendShow(QString("lst_sources size:%1").arg(lst_sources.size()));

    ComBineResource();
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


void MainWindow::readfromremote(QString cltmsg, void * pthread)
{
    logsappendShow(QString("read clt msg:%1").arg(cltmsg));
    qDebug() << (QString("read clt msg:%1").arg(cltmsg));
    if(CMD_FETCH_SRC == cltmsg)//直接发送获取，没有报文体
    {
        qDebug() << "clt  cmd code: CMD_FETCH_SRC";
        ReplyResourceInfo(pthread);
    }
    else if(cltmsg.contains((CMD_UPDATE_SRC)))
    {
        qDebug() << "clt  cmd code: CMD_UPDATE_SRC";
        ReplyUpdateInfo(pthread, cltmsg.mid(sizeof(CMD_UPDATE_SRC) - 1));
        saveResource();
    }
    else
    {
        qDebug() << "invalid cmd code";
    }
}

void MainWindow::dealclienterror(QString cltmsg, void * pthread)
{
    logsappendShow(QString("error clt msg:%1").arg(cltmsg));
    sockthread * inthread = (sockthread * )pthread;

    for(it_sklst = socklist.begin(); it_sklst != socklist.end(); )
    {
        if(inthread == *it_sklst)
        {
            qDebug() << "delete clt pthread :" << inthread;
            socklist.erase(it_sklst);
            break;
        }
        else
        {
            it_sklst++;
        }
    }
}

void MainWindow::closeallclientsocket()
{
    logsappendShow(QString("close all clt socket"));


    for(it_sklst = socklist.begin(); it_sklst != socklist.end(); it_sklst++)
    {
        sockthread * inthread = (sockthread * )*it_sklst;
        inthread->closeSocketConnect();
    }
    socklist.clear();
}




void MainWindow::replyclientwhenflush()
{
    qDebug() << "socklist size:" << socklist.size();
    for(it_sklst = socklist.begin(); it_sklst != socklist.end(); it_sklst++)
    {
        readfromremote(CMD_FETCH_SRC, *it_sklst);
    }
}


void MainWindow::ReplyResourceInfo(void * pthread)
{
    sockthread * inthread = (sockthread * )pthread;

    inthread->sendmsg(CMD_REPLY_SRC + ComBineResource());
//    pthreadsock
}

void MainWindow::ReplyUpdateInfo(void *pthread, QString row)
{
    UpdateResources(row);
    sockthread * inthread = (sockthread * )pthread;

//    inthread->sendmsg(CMD_REPLY_SRC + ComBineResource());//依然回应全部资源信息
    replyclientwhenflush();//全部连接客户端数据回应
//    pthreadsock
}



QString MainWindow::ComBineResource()
{
#define ADDRESULT(VAL)\
    result +=  AddYinHao(VAL) + ",";

    QString result;
    result.clear();
//    lst_sources.size();
    for(it_src = lst_sources.begin(); it_src != lst_sources.end(); it_src++)
    {
        T_ResourceUse &tmp = *it_src;
        ADDRESULT(tmp.devname);
        ADDRESULT(tmp.devip);
        ADDRESULT(tmp.netip);
        ADDRESULT(tmp.timelst);
        ADDRESULT(tmp.time);
        ADDRESULT(tmp.usrlist);
        ADDRESULT(tmp.usr);
        ADDRESULT(tmp.notice);
        ADDRESULT(tmp.right);

    }

    qDebug() << "combine resource result:" << result;

    return result;
}

void MainWindow::UpdateResources(QString row)
{
    QStringList splitmsg = row.mid(1).split("\",\"");
    qDebug() << "splist clt's update msg size:" << splitmsg.size();



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
//            tSrc.timelst = splitmsg.at(index);
            break;
        case 4:
            tSrc.time = splitmsg.at(index);
            break;
        case 5:
//            tSrc.usrlist = splitmsg.at(index);
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
//            lst_sources.push_back(tSrc);
        }

    }





    for(it_src = lst_sources.begin(); it_src != lst_sources.end(); it_src++)
    {
        T_ResourceUse &tmp = *it_src;
        if(tmp.devname == tSrc.devname)
        {
            tmp.devname = tSrc.devname;
            tmp.devip   = tSrc.devip;
            tmp.netip   = tSrc.netip;
            tmp.time    = tSrc.time;
            tmp.usr     = tSrc.usr;
            tmp.notice  = tSrc.notice;
            tmp.right   = tSrc.right;
            logsappendShow(QString("update src :%1").arg(tSrc.devname));
            break;
        }
    }
}

QString MainWindow::AddYinHao(QString str)
{
    return ("\"" + str.replace("\"", "\"\"") + "\"");
}

QStringList MainWindow::getusrlist()
{
    QStringList usrlist;
    usrlist << "" << str_china("小屁孩") <<  str_china("龙龙") << str_china("奇奇");
    return usrlist;
}


void MainWindow::enablebindpushtbn(QString str)
{
    ui->pushButton->setEnabled(true);
}

void MainWindow::Proc_DateChange()
{
    static QString oldate;
    QString curdate;
    QDate date;
    curdate = date.currentDate().toString("yyyy-MM-dd");

    qDebug() << "oldate  date:" << oldate;
    qDebug() << "current date:" << curdate;

    if(curdate != oldate && !oldate.isEmpty())
    {
        whendatechange_flushipaddr();
    }
    oldate = curdate;
}
