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

    void logsappendShow(QString log);

    int readResTxt(const char *filepath);

    int AppendResInfo(QString line);
    QByteArray CMPINDEX(quint16 INDEX, QStringList &strlst);
    void T_ResourceUse_Print(T_ResourceUse *p);

public slots:
    void sendResourceInfoMessage();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_flushipaddr_clicked();

private:
    QTcpServer *tcpServer;
    QTcpSocket *pCltSock;

    QString logs;

    QList<T_ResourceUse> lst_sources;

//UI
private:
    Ui::MainWindow *ui;
};


#if 1
//±àÂëºº×Ö
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#endif // MAINWINDOW_H
