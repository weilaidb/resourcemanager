#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "sockthread.h"
#include "kouling.h"

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

    void resizeEvent(QResizeEvent* event);
    QStringList getusrlist();
    QStringList getlistbydouhao(QString str);

public slots:
    void hellosocket();
    void readfromremote(QString cltmsg);
    void procErrMsg(QString errmsg);


    void showTitle();
    void deleteBeforeShow();
    void showOneRowUI(QString devname, QString devip, QString netip, QStringList time, QString timeindex, QStringList usr, QString usrindex, QString notice);
    void Proc_RequestSrcItem(QString text);

    void test_showoneresource(QString cltmsg);
    void showResources(QString cltmsg);
    void T_ResourceUse_Print(T_ResourceUse *p);

    void setPushBtnEnable(QString st);
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

private:
    QList<T_ResourceUse> lst_sources;
    QList<T_ResourceUse>::iterator it_src;

};


#if 1
//编码汉字
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
#define str_china_utf8(A)     QString::fromUtf8(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif







#endif // MAINWINDOW_H
