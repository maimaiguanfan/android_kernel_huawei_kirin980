#ifndef _RIL_SIM_NETLINK_H
#define _RIL_SIM_NETLINK_H

#define RIL_NETLINK_INIT 1
#define RIL_NETLINK_EXIT 0
#define MAX_SIM_CNT      3

typedef enum Ril2KnlMsgType
{
    NETLINK_KNL_SIM_NULL_MSG                =  0,
    NETLINK_KNL_SIM_UPDATE_IND_MSG          =  1,
    NETLINK_KNL_SIM_GET_PIN_INFO_REQ        =  2,
    NETLINK_KNL_SIM_GET_PIN_INFO_CNF        =  3,
    NETLINK_KNL_SIM_QUERY_VARIFIED_REQ      =  4,
    NETLINK_KNL_SIM_QUERY_VARIFIED_CNF      =  5,
    NETLINK_KNL_SIM_CLEAR_ALL_VARIFIED_FLG  =  6,
    NETLINK_KNL_SIM_BUTT
}KNL_Ril2knlMsgType;

#define KNL_CIPHER_TEXT_LEN (32)
#define KNL_IV_LEN          (32)
#define KNL_HMAC_LEN        (64)

typedef struct tagSimPinInfo
{
    int  sim_slot_id;   /* ��Ӧ��SIM��������   */
    int  is_verified;   /* �Ƿ����PIN������֤ */
    int  is_valid_flg;  /* ���²����Ƿ���Ч */
    char cipher_text[KNL_CIPHER_TEXT_LEN+1]; /* PIN��������Ϣ */
    char init_vector[KNL_IV_LEN+1];          /* IV������Ϣ */
    char hmac[KNL_HMAC_LEN+1];               /* HMACУ����Ϣ */
    char reserved;
} KNL_SIM_PIN_INFO;

typedef struct tagKnlUpdatePinInfoInd
{
    struct nlmsghdr   hdr;
    KNL_SIM_PIN_INFO  knlPinInfo;
}KNL_MsgUpdatePinInfoInd;

typedef struct tagRilSimCardInfo
{
    int    sim_slot_id;         /* ��Ӧ��SIM������ */
}KNL_RilSimCardInfo;

typedef struct tagKnlSimVerifiedCnfMsg
{
    struct nlmsghdr hdr;
    int    verified_flg;         /* ����SIM���Ƿ��Զ�У���־λ */
}KNL_SimVerifiedCnfMsg;

typedef struct tagMsgGetPinInfoReq
{
    struct nlmsghdr     hdr;
    KNL_RilSimCardInfo  cardInfo;
}KNL_MsgGetPinInfoReq;

#endif /*_RIL_SIM_NETLINK_H*/
