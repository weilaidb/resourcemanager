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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int InitServer(QString ipaddr, quint16 listenport);
    void WriteCurrentSettings();
    void ReadHistorySettings();
    void closeEvent(QCloseEvent *event);
    QList<QString> Getifconfig(void);
    int CheckIPAddr(QString ipaddr);
    void SetIPADDR_UI();

    int readResTxt(const char *filepath);

    int AppendResInfo(QString line);
    QByteArray CMPINDEX(quint16 INDEX, QStringList &strlst);
    void T_ResourceUse_Print(T_ResourceUse *p);

public slots:
    void procClientMessage();


    void logsappendShow(QString log);
    void readfromremote(QString cltmsg, void *pthread);

    void ReplyResourceInfo(void *socket);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_flushipaddr_clicked();

private:
    QTcpServer *tcpServer;
    QTcpSocket *pCltSock;

    QString logs;

    QList<T_ResourceUse> lst_sources;

    //写数据统计
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;

    QByteArray outBlock; //用于暂存我们要发送的数据

    QTcpSocket *clientConnection;

//UI
private:
    Ui::MainWindow *ui;
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
