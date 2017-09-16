#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QtGui>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QtWidgets/QMainWindow>

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

public slots:
    void readMessage();
    void updateReadMsgProgress();
    void displayErr(QAbstractSocket::SocketError socketError);

private slots:
    void on_pushButton_clicked();

private:
    QString logs;
private:
    Ui::MainWindow *ui;

private:
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    QString message;  //存放从服务器接收到的字符串
    //读数据统计
//    qint64 TotalReadBytes;
//    qint64 byteReadden;
//    qint64 bytesToRead;
//    QByteArray outReadBlock;

    qint16  TotalReadBytes;
    qint16  bytesReceived;
//    qint64  fileNameSize;
    qint16  bytesNeedRecv;
    QByteArray inBlock;
};


#if 1
//编码汉字
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif



enum
{
    READ_DONE,
    READING,
    READ_OTHER
};




#endif // MAINWINDOW_H
