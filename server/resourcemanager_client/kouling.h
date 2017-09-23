#ifndef KOULING_H
#define KOULING_H

#include <QString>
/* 映射关系
 * 客户侧              服务侧
** CMD_FETCH_SRC -->
**               <-- CMD_REPLY_SRC
** CMD_UPDATE_SRC -->>
**              <--  CMD_REPLY_UPDATE
*/
//命令码
#define CMD_FETCH_SRC "FETCH SRC" //客户侧取资源数据
#define CMD_REPLY_SRC "REPLY SRC" //应答给客户侧回应

#define CMD_UPDATE_SRC   "UPDATE SRC" //客户侧更新一项资源数据
#define CMD_REPLY_UPDATE "REPLY UPDATE" //应答给客户侧回应更新结果



typedef struct T_ResourceUse{
    QString devname;    /* 资源名称 */
    QString devip  ;    /* 资源IP  */
    QString netip  ;    /* 网元IP  */
    QString timelst;    /* 时间列表 */
    QString time   ;    /* 申请时间 */
    QString usrlist;    /* 用户列表  */
    QString usr    ;    /* 用   户  */
    QString notice ;    /* 备   注 */
    QString right ;     /* 是否可用  */
}T_ResourceUse, *P_ResourceUse;


#endif // KOULING_H
