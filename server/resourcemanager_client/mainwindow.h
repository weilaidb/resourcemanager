#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTcpSocket>
#include <QAbstractSocket>
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
    void WriteCurrentSettings();
    void ReadHistorySettings();
    void closeEvent(QCloseEvent *event);
    void logsappendShow(QString log);
    int CheckIPAddr(QString ipaddr);
    void newConnect(QString ipaddr);
//    void initClientReceive();

public slots:
    void hellosocket();
    void readfromremote(QString cltmsg);
    void procErrMsg(QString errmsg);

    void showTitle();
    void deleteBeforeShow();
    void showOneRowUI(QString devname, QString devip, QString netip, QStringList time, quint32 timeindex, QStringList usr, quint32 usrindex, QString notice);
    void Proc_RequestSrcItem(QString text);

private slots:
    void on_pushButton_clicked();

private:
    QString logs;
private:
    Ui::MainWindow *ui;

private:
    QTcpSocket *socket;
    QString message;  //存放从服务器接收到的字符串

    sockthread *pthreadsock;

private:
    QList<QWidget *> showuilist;

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
