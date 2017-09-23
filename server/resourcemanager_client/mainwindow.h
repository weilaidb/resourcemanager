#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "sockthread.h"
#include "kouling.h"
#include <QtWidgets>
#include "keybutton.h"



//单行保存的界面信息
typedef struct T_LocalUiForShow{
    QLabel *pDevName      ;    /*  */
    QLineEdit *pDevIP     ;    /*  */
    QLineEdit *pNetIP     ;    /*  */
    QComboBox *pTime      ;    /*  */
    QComboBox *pUsr       ;    /*  */
    QLineEdit *pNotice    ;    /*  */
    KeyButton *prequestBtn;    /*  */
}T_LocalUiForShow, *P_LocalUiForShow;



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
    void readfromremote(QString cltmsg, void *);
    void procErrMsg(QString errmsg, void *);


    void showTitle();
    void deleteBeforeShow();
    void showOneRowUI(QString devname, QString devip, QString netip, QStringList time, QString timeindex, QStringList usr, QString usrindex, QString notice);
    void Proc_RequestSrcItem(QString text);
    void Proc_RequestUpdate(T_LocalUiForShow &rowUi);

    void test_showoneresource(QString cltmsg);
    void showResources(QString cltmsg);
    void T_ResourceUse_Print(T_ResourceUse *p);

    void setPushBtnEnable(QString st);

    QString AddYinHao(QString str);
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

    QList<T_LocalUiForShow> lst_ShowUi;
    QList<T_LocalUiForShow>::iterator it_ShowUi;
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
