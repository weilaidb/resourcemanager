#ifndef KOULING_H
#define KOULING_H

//命令码
#define CMD_FETCH_SRC "FETCH SRC"
#define CMD_REPLY_SRC "REPLY SRC"



#define INSIZE (64)

typedef struct T_ResourceUse{
    char devname[INSIZE];    /* 资源名称 */
    char devip[INSIZE]  ;    /* 资源IP  */
    char netip[INSIZE]  ;    /* 网元IP  */
    char time[INSIZE]   ;    /* 申请时间 */
    char usr[INSIZE]    ;    /* 用   户  */
    char notice[INSIZE] ;    /* 备   注 */
    char right[INSIZE] ;     /* 是否可用  */
}T_ResourceUse, *P_ResourceUse;


#endif // KOULING_H
