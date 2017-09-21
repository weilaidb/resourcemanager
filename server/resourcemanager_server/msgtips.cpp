#include "msgtips.h"
#include <QtGui>
#include "msgtipsautoshut.h"

msgtips::msgtips()
{
}

void ShowTipsInfo(QString s)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->setWindowFlags( Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    tipsinfo->show();
    //无任务栏图标，无工具栏，置顶显示

}

void ShowTipsInfoWithShowTime(QString s, quint32 ultimeout)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL, ultimeout);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->setWindowFlags( Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    tipsinfo->show();
    //无任务栏图标，无工具栏，置顶显示

}
