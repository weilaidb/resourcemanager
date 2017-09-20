#ifndef KOULING_H
#define KOULING_H

//������
#define CMD_FETCH_SRC "FETCH SRC"
#define CMD_REPLY_SRC "REPLY SRC"



#define INSIZE (64)

typedef struct T_ResourceUse{
    char devname[INSIZE];    /* ��Դ���� */
    char devip[INSIZE]  ;    /* ��ԴIP  */
    char netip[INSIZE]  ;    /* ��ԪIP  */
    char time[INSIZE]   ;    /* ����ʱ�� */
    char usr[INSIZE]    ;    /* ��   ��  */
    char notice[INSIZE] ;    /* ��   ע */
    char right[INSIZE] ;     /* �Ƿ����  */
}T_ResourceUse, *P_ResourceUse;


#endif // KOULING_H
