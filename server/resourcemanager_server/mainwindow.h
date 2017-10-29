#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "msgtips.h"
#include "kouling.h"
#include <QRegExp>
#include "sockthread.h"
#include <QUdpSocket>
#include <QSystemTrayIcon>
#include <QCloseEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void UIBeauty();
    int InitServer(QString ipaddr, quint16 listenport);
    void WriteCurrentSettings();
    void ReadHistorySettings();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void createActions();
    void createTrayIcon();


    QList<QString> Getifconfig(void);
    int CheckIPAddr(QString ipaddr);
    void SetIPADDR_UI();
    QString GetFilteredIpAddr();
    void AutoBindAddress();

    int readResTxt(const char *filepath);
    int writeResTxt(const char *filepath);
    int writeDefaultResTxt(const char *filepath);
    int reLoadResource();
    int saveResource();

    QString readUsrInfoTxt(const char *filepath);
    int reLoadUsrList();
    QStringList getusrlist();

    int AppendResInfo(QString line);
    QByteArray CMPINDEX(quint16 INDEX, QStringList &strlst);
    void T_ResourceUse_Print(T_ResourceUse *p);

    void changeEvent(QEvent *e);


private:
    void closeEvent(QCloseEvent *event);

#if 1
    QSystemTrayIcon *trayIcon;
    char *msg;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *savetofileAction;
    QAction *quitAction;
    QAction *startAction;
    QAction *stopAction;
    QMenu *trayIconMenu;
#endif


public slots:
    void procClientMessage();


    void logsappendShow(QString log);
    void readfromremote(QString cltmsg, void *pthread);
    void replyclientwhenflush();
    void dealclienterror(QString cltmsg, void * pthread);
    void closeallclientsocket();

    void ReplyResourceInfo(void *socket);
    void ReplyUpdateInfo(void *pthread, QString row);

    QString ComBineResource(void);
    void UpdateResources(QString row);

    QString AddYinHao(QString str);
    void enablebindpushtbn(QString str);

    void Proc_DateChange();
    void whendatechange_flushipaddr();

    //udp process
    void initBroadcastListener();
    void processPengingDatagram();
    QString getIp();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_flushipaddr_clicked();

private:
    QTcpServer *tcpServer;
    QTcpSocket *pCltSock;

    QString logs;

    QList<T_ResourceUse> lst_sources;
    QList<T_ResourceUse>::iterator it_src;
    QString UserList;//用户列表

    QList<sockthread *> socklist;
    QList<sockthread *>::iterator it_sklst;
    //写数据统计
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;

    QByteArray outBlock; //用于暂存我们要发送的数据

    QTcpSocket *clientConnection;

//UI
private:
    Ui::MainWindow *ui;

private:
    QTimer *dateTimer;

private:
    //接收客户侧广播数据
    QUdpSocket * receiver;
    QUdpSocket * sender;
};


#if 1
//编码汉字
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#endif // MAINWINDOW_H
