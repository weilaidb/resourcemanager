#ifndef KOULING_H
#define KOULING_H

#include <QString>
/* ӳ���ϵ
 * �ͻ���              �����
** CMD_FETCH_SRC -->
**               <-- CMD_REPLY_SRC
** CMD_UPDATE_SRC -->>
**              <--  CMD_REPLY_UPDATE
*/
//������
#define CMD_FETCH_SRC "FETCH SRC" //�ͻ���ȡ��Դ����
#define CMD_REPLY_SRC "REPLY SRC" //Ӧ����ͻ����Ӧ

#define CMD_UPDATE_SRC   "UPDATE SRC" //�ͻ������һ����Դ����
#define CMD_REPLY_UPDATE "REPLY UPDATE" //Ӧ����ͻ����Ӧ���½��



typedef struct T_ResourceUse{
    QString devname;    /* ��Դ���� */
    QString devip  ;    /* ��ԴIP  */
    QString netip  ;    /* ��ԪIP  */
    QString timelst;    /* ʱ���б� */
    QString time   ;    /* ����ʱ�� */
    QString usrlist;    /* �û��б�  */
    QString usr    ;    /* ��   ��  */
    QString notice ;    /* ��   ע */
    QString right ;     /* �Ƿ����  */
}T_ResourceUse, *P_ResourceUse;


#endif // KOULING_H
