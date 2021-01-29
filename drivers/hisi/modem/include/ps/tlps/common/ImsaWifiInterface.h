


#ifndef __IMSA_WIFI_INTERFACE_H__
#define __IMSA_WIFI_INTERFACE_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    �����ֽڶ��뷽ʽ
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/

#define IMSA_WIFI_MAX_APN_LEN           (99)

#define IMSA_WIFI_IPV4_ADDR_LEN         (4)
#define IMSA_WIFI_IPV6_ADDR_LEN         (16)

/* added by Liqingtao for vowifi emc, begin in 2016-07-15 */
#define IMSA_WIFI_NONCID_SCHEME_URI_LEN 128
#define IMSA_WIFI_CIVICLOC_COUNTRY_LEN  4

#define IMSA_WIFI_CIVICLOC_STATE_LEN  48
#define IMSA_WIFI_CIVICLOC_CITY_LEN  48
#define IMSA_WIFI_CIVICLOC_ZIP_LEN  48
#define IMSA_WIFI_CIVICLOC_RADIUS_LEN  48

#define IMSA_WIFI_CIVICLOC_COUNTRY_A5_LEN  48
#define IMSA_WIFI_CIVICLOC_ROADSTREET_LEN  100
#define IMSA_WIFI_CIVICLOC_OTHER_ADDRESS_LEN  100
#define IMSA_WIFI_COORDINATE_LATITUDE_LEN  16
#define IMSA_WIFI_COORDINATE_LONGITUDE_LEN 16
#define IMSA_WIFI_COORDINATE_ALTITUDE_LEN  16
#define IMSA_WIFI_TIMESTAMP_LEN              24
/* added by Liqingtao for vowifi emc, end in 2016-07-15 */
#define IMSA_WIFI_EXTENTION_VER_MAXNUM  6

#define  WIFI_IMSA_SSID_LEN             33
#define  IMSA_WIFI_MAX_IPV4_EPDG_NUM            2
#define  IMSA_WIFI_MAX_IPV6_EPDG_NUM            2

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/



/*****************************************************************************
  4 Enum
*****************************************************************************/

/*****************************************************************************
 ö������: IMSA_WIFI_PDN_TYPE_ENUM
 ö��˵��: PDP����
*****************************************************************************/
enum IMSA_WIFI_PDN_TYPE_ENUM
{
    IMSA_WIFI_PDN_TYPE_NORMAL           = 0x00,
    IMSA_WIFI_PDN_TYPE_EMC              = 0x01,

    IMSA_WIFI_PDN_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_PDN_TYPE_ENUM_INT8;


enum WIFI_IMSA_MSG_ID_ENUM
{
    /* WIFI����IMSA����Ϣ */
    ID_WIFI_IMSA_POWERON_REQ            = 0x0000,           /* _H2ASN_MsgChoice WIFI_IMSA_POWERON_REQ_STRU */
    ID_WIFI_IMSA_POWEROFF_REQ           = 0x0001,           /* _H2ASN_MsgChoice WIFI_IMSA_POWEROFF_REQ_STRU */
    ID_WIFI_IMSA_IMS_PDN_ACTIVATE_CNF   = 0x0002,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU*/
    ID_WIFI_IMSA_IMS_PDN_READY_IND      = 0x0003,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_READY_IND_STRU */
    ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF = 0x0004,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU */
    ID_WIFI_IMSA_ERROR_IND              = 0x0005,           /* _H2ASN_MsgChoice WIFI_IMSA_ERROR_IND_STRU */
    ID_WIFI_IMSA_STATE_IND              = 0x0006,           /* _H2ASN_MsgChoice WIFI_IMSA_STATE_IND_STRU */
    ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_IND = 0x0007,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU */
    ID_IMSA_WIFI_LOCATION_UPDATE_IND    = 0x0008,           /* _H2ASN_MsgChoice WIFI_IMSA_LOCATION_UPDATE_IND_STRU */

    ID_WIFI_IMSA_OPEN_VOWIFI_RSP        = 0x0009,           /* _H2ASN_MsgChoice WIFI_IMSA_OPEN_VOWIFI_RSP_STRU */
    ID_WIFI_IMSA_ABNORMAL_HOLD_STATE_IND = 0x000A,         /*  _H2ASN_MsgChoice WIFI_IMSA_ABNORMAL_HOLD_STATE_IND_STRU  */

    ID_WIFI_IMSA_VOWIFI_SWITCH_IND       = 0x000B,         /* _H2ASN_MsgChoice WIFI_IMSA_VOWIFI_SWITCH_IND_STRU */
    ID_WIFI_IMSA_RTP_FAIL_IND             = 0x000C,         /* _H2ASN_MsgChoice WIFI_IMSA_RTP_FAIL_IND_STRU */

    /* IMSA����WIFI����Ϣ */
    ID_IMSA_WIFI_POWERON_CNF            = 0x0100,           /* _H2ASN_MsgChoice IMSA_WIFI_POWERON_CNF_STRU */
    ID_IMSA_WIFI_POWEROFF_CNF           = 0x0101,           /* _H2ASN_MsgChoice IMSA_WIFI_POWEROFF_CNF_STRU */
    ID_IMSA_WIFI_IMS_PDN_ACTIVATE_REQ   = 0x0102,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU */
    ID_IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ = 0x0103,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU */
    ID_IMSA_WIFI_IMS_AUDIO_INFO_IND     = 0x0104,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU */
    ID_IMSA_WIFI_IMS_VEDIO_INFO_IND     = 0x0105,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_VEDIO_INFO_IND_STRU */

    ID_IMSA_WIFI_OPEN_VOWIFI_IND        = 0x0106,           /* _H2ASN_MsgChoice IMSA_WIFI_OPEN_VOWIFI_IND_STRU */
    ID_IMSA_WIFI_CLOSE_VOWIFI_IND       = 0x0107,           /* _H2ASN_MsgChoice IMSA_WIFI_CLOSE_VOWIFI_IND_STRU */

    ID_IMSA_WIFI_CELLULAR_OPEN_IND      = 0x0108,           /* _H2ASN_MsgChoice IMSA_WIFI_CELLULAR_OPEN_IND_STRU */
    ID_IMSA_WIFI_LOCATION_UPDATE_REQ    = 0x0109,           /* _H2ASN_MsgChoice IMSA_WIFI_LOCATION_UPDATE_REQ_STRU */

    ID_IMSA_WIFI_DNS_QUERY_NTF          = 0x010A,           /* _H2ASN_MsgChoice IMSA_WIFI_DNS_QUERY_NTF_STRU */

    ID_IMSA_WIFI_EPDG_INFO_IND          = 0x010B,           /* _H2ASN_MsgChoice IMSA_WIFI_EPDG_INFO_IND_STRU */
    ID_IMSA_WIFI_THRESHOLD_CONF_NTF     = 0x010C,           /* _H2ASN_MsgChoice IMSA_WIFI_THRESHOLD_CONF_NTF_STRU */
    ID_IMSA_WIFI_RTT_INFO_IND           = 0x010D,
    ID_IMSA_WIFI_NV_REFRESH_IND         = 0x010e,           /* _H2ASN_MsgChoice IMSA_WIFI_NV_REFRESH_IND_STRU*/
    ID_WIFI_IMSA_MSG_BUTT
};
typedef VOS_INT32 WIFI_IMSA_MSG_ID_ENUM_INT32;

/*****************************************************************************
 ö������: IMSA_WIFI_IP_TYPE_ENUM
 ö��˵��: PDP����
*****************************************************************************/
enum IMSA_WIFI_IP_TYPE_ENUM
{
    IMSA_WIFI_IP_IPV4                   = 0x01,
    IMSA_WIFI_IP_IPV6                   = 0x02,
    IMSA_WIFI_IP_IPV4V6                 = 0x03,

    IMSA_WIFI_IP_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_IP_TYPE_ENUM_INT8;

/*****************************************************************************
 ö������: IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
 ö��˵��: PDP����
*****************************************************************************/
enum IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
{
    IMSA_WIFI_PDN_RELEASE_TYPE_NW_CMD   = 0x00,
    IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL    = 0x01,

    IMSA_WIFI_PDN_RELEASE_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_PDN_RELEASE_TYPE_ENUM_INT8;

/*****************************************************************************
 ö������: IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
 ö��˵��: PDP����
*****************************************************************************/
enum IMSA_WIFI_SERVICE_TYPE_ENUM
{
    IMSA_WIFI_NORMAL_SERVICE_IN_NORMAL_PDN   = 0x00,
    IMSA_WIFI_EMC_SERVICE_IN_NORMAL_APN,
    IMSA_WIFI_EMC_SERVICE_IN_EMERGECY_APN,

    IMSA_WIFI_SERVICE_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_SERVICE_TYPE_ENUM_INT8;


/*****************************************************************************
    ö����    : WIFI_QUALITY_LEVEL_ENUM
    ö��˵��  : WIFI�����ȼ�ö��
*****************************************************************************/
enum WIFI_QUALITY_LEVEL_ENUM
{
    WIFI_IMSA_QUALITY_LEVEL_BAD         = 0x00,
    WIFI_IMSA_QUALITY_LEVEL_NORMAL      = 0x01,
    WIFI_IMSA_QUALITY_LEVEL_GOOD        = 0x02,

    WIFI_IMSA_QUALITY_LEVEL_BUTT
};
typedef VOS_INT8 WIFI_IMSA_QUALITY_LEVEL_ENUM_INT8;

/*****************************************************************************
    ö����    : WIFI_IMSA_SERVICE_STATUS_ENUM
    ö��˵��  : WIFI�ϱ���AP����״̬
*****************************************************************************/
enum WIFI_IMSA_SERVICE_STATUS_ENUM
{
    WIFI_IMSA_SERVICE_STATUS_NORMAL_SEVICE         = 0x00,
    WIFI_IMSA_SERVICE_STATUS_LIMITED_SERVICE       = 0x01,
    WIFI_IMSA_SERVICE_STATUS_NO_SERVICE            = 0x02,

    WIFI_IMSA_SERVICE_STATUS_BUTT
};
typedef VOS_INT8 WIFI_IMSA_SERVICE_STATUS_ENUM_INT8;


/*****************************************************************************
    ö����    : WIFI_IMSA_PDN_ACT_RESULT_ENUM
    ö��˵��  : IMS PDN������ö��
*****************************************************************************/
enum WIFI_IMSA_PDN_ACT_RESULT_ENUM
{
    WIFI_IMSA_PDN_ACT_RESULT_SUCCESS    = 0x00,
    WIFI_IMSA_PDN_ACT_RESULT_PARAM_ERR,                       /* ָ���������� */
    WIFI_IMSA_PDN_ACT_RESULT_LINKLOST,                        /* ��·��ʧ */
    WIFI_IMSA_PDN_ACT_RESULT_TIMEOUT,                         /* ������ʱ */
    WIFI_IMSA_PDN_ACT_RESULT_MAPCON_STATE_ERR,                /* MAPCON״̬���� */
    WIFI_IMSA_PDN_ACT_RESULT_OTHER_FAILURE,                   /* ��������*/
    WIFI_IMSA_PDN_ACT_RESULT_SIM_AUTH_NO_RESP = 0x06,
    WIFI_IMSA_PDN_ACT_RESULT_AUTH_NETWORK_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_PARSE_NETWORK_PAYLOAD_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_ADD_SA_AP_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_CLIENT_PAYLOAD_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_MSG_NETWORK_OVERTIME_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_ENC_ALG_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_APN_ID_ERROR,

    WIFI_IMSA_PDN_ACT_RESULT_NETWORK_AUTH_SIM_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_STATUS_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_SA_INIT_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED,
    WIFI_IMSA_PDN_ACT_RESULT_USER_UNKNOWN  = 0x12,
    WIFI_IMSA_PDN_ACT_RESULT_PLMN_NOT_ALLOWED,
    WIFI_IMSA_PDN_ACT_RESULT_AUTHORIZATION_REJECTED,
    WIFI_IMSA_PDN_ACT_RESULT_RAT_TYPE_NOT_ALLOWED,

    WIFI_IMSA_PDN_ACT_RESULT_NETWORK_FAILURE,
    WIFI_IMSA_PDN_ACT_RESULT_NO_APN_SUBSCRIPTION,
    WIFI_IMSA_PDN_ACT_RESULT_PDN_CONNECTION_REJECTION,
    WIFI_IMSA_PDN_ACT_RESULT_CONNECTION_REACHED,
    WIFI_IMSA_PDN_ACT_RESULT_PROTECLE_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_INTERNAL_ADDRESS_ERROR,
    WIFI_IMSA_PDN_ACT_RESULT_IPV4_ADDRESS_CONFLICT = 0x1C, /* ˫vowifi�·�����ͬIPV4��ַʱ������ */
    WIFI_IMSA_PDN_ACTIVE_RESULT_CERT_ERROR         = 0x1D, /* ֤����֤���� */
    WIFI_IMSA_PDN_ACTIVE_RESULT_BEARS_CONCURRENT_NOT_SUPPORT = 0x1E, /*��֧�ֳ��ز���*/
    WIFI_IMSA_PDN_ACTIVE_RESULT_SHUTDOWN            = 0x1F,
    WIFI_IMSA_PDN_ACTIVE_RESULT_DONE_ERROR             = 0x20,
    WIFI_IMSA_PDN_ACTIVE_RESULT_REDIRECT_FAILURE       = 0x21,
    WIFI_IMSA_PDN_ACTIVE_RESULT_INNER_ERROR_DIR_RETRY  = 0x31,
    /*!!!���������������50�ˣ���ɾԭ��ֵʱ��Ҫͬ���޸ĺ���: IMSA_CONN_InitWifiPdnRejTblList   */
    WIFI_IMSA_PDN_ACT_RESULT_ROAM_FORBID = 0x32,
    WIFI_IMSA_PDN_ACT_RESULT_HIFI_STATUS_OVERTIME = 0x33,
    WIFI_IMSA_PDN_ACT_RESULT_IKED_ABNORMAL = 0x34,
    WIFI_IMSA_PDN_ACT_RESULT_UNKNOWN_NETWORK_ERROR = 0x35,
    WIFI_IMSA_PDN_ACT_RESULT_SWITCH_CACHE_MAX_REACHED   = 0x36,
    WIFI_IMSA_PDN_ACT_RESULT_APN_MAX_REACHED            = 0x37,
    WIFI_IMSA_PDN_ACT_RESULT_REQUEST_REPEATED           = 0x38,
    /*!!!��ɾԭ��ֵʱ��Ҫͬ���޸ĺ���: IMSA_CONN_InitWifiPdnRejTblList   */
    WIFI_IMSA_PDN_ACT_RESULT_BUTT
};
typedef VOS_INT8 WIFI_IMSA_PDN_ACT_RESULT_ENUM_INT8;

/*****************************************************************************
    ö����    : WIFI_IMSA_PDN_ACT_RESULT_ENUM
    ö��˵��  : IMS PDN������ö��(����ԭ��ֵ)
*****************************************************************************/
enum WIFI_IMSA_PDN_ACT_NW_RESULT_ENUM
{
    WIFI_IMSA_PDN_ACT_NW_RSLT_SUCCESS                     = 0,
    WIFI_IMSA_PDN_ACT_NW_RSLT_UNSPTED_CRITICAL_PAYLOAD    = 1,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_IKE_SPI             = 4,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_MAJOR_VERSION       = 5,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_SYNTAX              = 7,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_MESSAGE_ID          = 9,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_SPI                 = 11,
    WIFI_IMSA_PDN_ACT_NW_RSLT_NO_PROPOSAL_CHOSEN          = 14,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_KE_PAYLOAD          = 17,
    WIFI_IMSA_PDN_ACT_NW_RSLT_AUTHENTICATION_FAILED       = 24,

    WIFI_IMSA_PDN_ACT_NW_RSLT_SINGLE_PAIR_REQUIRED        = 34,
    WIFI_IMSA_PDN_ACT_NW_RSLT_NO_ADDITIONAL_SAS           = 35,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INTERNAL_ADDRESS_FAILURE    = 36,
    WIFI_IMSA_PDN_ACT_NW_RSLT_FAILED_CP_REQUIRED          = 37,
    WIFI_IMSA_PDN_ACT_NW_RSLT_TS_UNACCEPTABLE             = 38,
    WIFI_IMSA_PDN_ACT_NW_RSLT_INVALID_SELECTORS           = 39,
    WIFI_IMSA_PDN_ACT_NW_RSLT_UNACCEPTABLE_ADDRESSES      = 40,     //RFC4555
    WIFI_IMSA_PDN_ACT_NW_RSLT_UNEXPECTED_NAT_DETECTED     = 41,     //RFC4555
    WIFI_IMSA_PDN_ACT_NW_RSLT_USE_ASSIGNED_HOA            = 42,     //RFC5026

    
    WIFI_IMSA_PDN_ACT_NW_RSLT_PDN_CONNECTION_REJECTION    = 8192,   //R13
    WIFI_IMSA_PDN_ACT_NW_RSLT_MAX_CONNECTION_REACHED      = 8193,   //R13

    WIFI_IMSA_PDN_ACT_NW_RSLT_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED = 9000,  //R13
    WIFI_IMSA_PDN_ACT_NW_RSLT_USER_UNKNOWN                = 9001,   //R13
    WIFI_IMSA_PDN_ACT_NW_RSLT_NO_APN_SUBSCRIPTION         = 9002,   //R13
    WIFI_IMSA_PDN_ACT_NW_RSLT_AUTHORIZATION_REJECTED      = 9003,   //R13

    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_MISSING_OR_UNKNOWN_APN  = 9048,   //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_NO_SUBSCRIPTION         = 9049,   //R1

    WIFI_IMSA_PDN_ACT_NW_RSLT_NETWORK_TOO_BUSY            = 10000,  //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_PGW_OVERLOAD                = 10003,  //R1      

    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_REQUEST_REJECTED        = 10048,  //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_PROTOCOL_ERROR          = 10049,  //R1

    WIFI_IMSA_PDN_ACT_NW_RSLT_NETWORK_FAILURE             = 10500,  //R13

    WIFI_IMSA_PDN_ACT_NW_RSLT_ROAMING_NOT_ALLOWED         = 11000,  //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_RAT_DISALLOWED              = 11001,  //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_PERMENENT_FAILURE           = 11002,  //R1
    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_RAT_DISALLOWED          = 11003,  //R1

    WIFI_IMSA_PDN_ACT_NW_RSLT_PLMN_NOT_ALLOWED            = 11011,  //R13

    WIFI_IMSA_PDN_ACT_NW_RSLT_S2B_UNSUPPORTED_PDN_TYPE    = 11049,  //R1

    WIFI_IMSA_PDN_ACT_NW_RSLT_BUTT                        = 16384,  //MAX error
};
typedef VOS_UINT16 WIFI_IMSA_PDN_ACT_NW_RESULT_ENUM_UINT16;


/*****************************************************************************
    ö����    : WIFI_IMSA_PDN_REACT_ACTION_ENUM
    ö��˵��  : IMS PDN������ö��
*****************************************************************************/
enum WIFI_IMSA_PDN_REACT_ACTION_ENUM
{
    WIFI_IMSA_PDN_REACT_ACTION_NO    = 0x00,               /* PDN�����쳣, MAPCONû��ָ��������Ϊ */
    WIFI_IMSA_PDN_REACT_ACTION_RETRY_NODELAY,              /* PDN�����쳣, MAPCONָ���������� */
    WIFI_IMSA_PDN_REACT_ACTION_RETRY_DELAY,                /* PDN�����쳣, MAPCONָ���ӳ�����,���ReactTimerLenָ��ʱ��,��IMSAʹ�����ʱ�䳤�� */
    WIFI_IMSA_PDN_REACT_ACTION_PERM_REJ,                   /* PDN�����쳣, MAPCONָ����Զ��Ҫ���� */

    WIFI_IMSA_PDN_REACT_ACTION_BUTT
};
typedef VOS_INT8 WIFI_IMSA_PDN_REACT_ACTION_ENUM_INT8;

/*****************************************************************************
    ö����    : WIFI_IMS_ERROR_CAUSE_ENUM
    ö��˵��  : IMS PDN������ö��
*****************************************************************************/
enum WIFI_IMSA_ERROR_CAUSE_ENUM
{
    /* ָʾWIFI IMSͨ������ */
    WIFI_IMSA_ERROR_CAUSE_LINK_LOST         =0x00,          /* WIFI��·��ʧ */
    WIFI_IMSA_ERROR_CAUSE_SIP_PORT_FAILURE,                 /* SIP PORT��ʧ��*/
    WIFI_IMSA_ERROR_CAUSE_AUDIO_PORT_FAILURE,               /* MEDIA PORT��ʧ�� */
    WIFI_IMSA_ERROR_CAUSE_MAPCON_RESET_FAILURE,              /* MAPCON�쳣*/
    WIFI_IMSA_ERROR_CAUSE_RTT_PORT_FAILURE,                 /* rtt port��ʧ�� */

    WIFI_IMSA_ERROR_CAUSE_BUTT
};
typedef VOS_INT8 WIFI_IMSA_ERROR_CAUSE_ENUM_INT8;

/*****************************************************************************
    ö����    : WIFI_IMSA_PDN_DEACT_CAUSE_ENUM
    ö��˵��  : IMS PDNȥ����ԭ��ֵö��
*****************************************************************************/
enum WIFI_IMSA_PDN_DEACT_CAUSE_ENUM
{
    WIFI_IMSA_PDN_DEACT_CAUSE_KEEP_LIVE        =    0x01,     /* �����û�лظ� */
    WIFI_IMSA_PDN_DEACT_CAUSE_NETWORK_DELETE,                 /* ���������ͷ���� */
    WIFI_IMSA_PDN_DEACT_CAUSE_IKED_RESET,                     /* IKED�쳣���� */
    WIFI_IMSA_PDN_DEACT_CAUSE_REKEY_ERROR,                    /* REKEYʧ�� */
    WIFI_IMSA_PDN_DEACT_CAUSE_MAPCON_TEAR,                    /* MAPCON�����ͷ���� */
    WIFI_IMSA_PDN_DEACT_CAUSE_BEARS_CONCURRENT_NOT_SUPPORT, /*��֧�ֳ��ز���*/

    WIFI_IMSA_PDN_DEACT_CAUSE_BUTT
};
typedef VOS_INT8 WIFI_IMSA_PDN_DEACT_CAUSE_ENUM_INT8;


/*****************************************************************************
    ö����    : WIFI_IMS_ACCESS_TYPE_ENUM
    ö��˵��  : WIFI��������ö��
*****************************************************************************/
enum WIFI_IMS_ACCESS_TYPE_ENUM
{
    WIFI_IMSA_ACCESS_TYPE_802_11    = 0,
    WIFI_IMSA_ACCESS_TYPE_802_11_a  = 1,
    WIFI_IMSA_ACCESS_TYPE_802_11_b  = 2,
    WIFI_IMSA_ACCESS_TYPE_802_11_g  = 3,
    WIFI_IMSA_ACCESS_TYPE_802_11_n  = 4,
    WIFI_IMSA_ACCESS_TYPE_802_11_ac = 5,
    WIFI_IMSA_ACCESS_TYPE_802_11_ad = 6,
    WIFI_IMSA_ACCESS_TYPE_802_11_ah = 7,

    WIFI_IMSA_ACCESS_TYPE_BUTT
};
typedef VOS_INT32 WIFI_IMS_ACCESS_TYPE_ENUM_INT32;


/*****************************************************************************
    ö����    : IMSA_WIFI_POWEROFF_REASON_ENUM
    ö��˵��  : WIFI�ͷ�ԭ������ö��
*****************************************************************************/
enum IMSA_WIFI_POWEROFF_REASON_ENUM
{
    IMSA_WIFI_POWEROFF_REASON_WIFI_POWEROFF    = 0,
    IMSA_WIFI_POWEROFF_REASON_SYSTEM_DOWN      = 1,
    IMSA_WIFI_POWEROFF_REASON_VOWIFI_STOP      = 2,
    IMSA_WIFI_POWEROFF_REASON_ROAMING_FORB     = 3,
    IMSA_WIFI_POWEROFF_REASON_SIM_DISABLE      = 4,

    IMSA_WIFI_POWEROFF_REASON_BUTT
};
typedef VOS_INT8 IMSA_WIFI_POWEROFF_REASON_ENUM_INT8;
/*****************************************************************************
    ö����    : IMSA_WIFI_TUNNEL_HOLD_STATUS_ENUM
    ö��˵��  : �������ģʽ״̬������/�˳�
*****************************************************************************/
enum IMSA_WIFI_TUNNEL_HOLD_STATUS_ENUM
{
    IMSA_WIFI_TUNNEL_HOLD_STATUS_EXIT     = 0x00,/*�˳����ģʽ*/
    IMSA_WIFI_TUNNEL_HOLD_STATUS_ENTER    = 0x01,/*��ȥ�������ģʽ*/
    IMSA_WIFI_TUNNEL_HOLD_STATUS_BUTT
};
typedef VOS_INT8 IMSA_WIFI_TUNNEL_HOLD_STATUS_ENUM_INT8;



/*****************************************************************************
    ö����    : IMSA_WIFI_INTER_ROAMING_STATUS_ENUM
    ö��˵��  : WIFI��������״̬
*****************************************************************************/
enum IMSA_WIFI_INTER_ROAMING_STATUS_ENUM
{
    IMSA_WIFI_INTER_ROAMING_STATUS_NOT_SUPPORT     = 0x00,/*��֧������*/
    IMSA_WIFI_INTER_ROAMING_STATUS_FALSE              = 0x01,/*û�й�������*/
    IMSA_WIFI_INTER_ROAMING_STATUS_TRUE               = 0x02,/*��������*/

    IMSA_WIFI_INTER_ROAMING_STATUS_BUTT
};
typedef VOS_INT8 IMSA_WIFI_INTER_ROAMING_STATUS_ENUM_INT8;

/*****************************************************************************
  5 STRUCT
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : IMSA_WIFI_IP_ADDRESS_STRU
 �ṹ˵��  : IP��ַ�Ľṹ��
*****************************************************************************/
typedef struct
{
    IMSA_WIFI_IP_TYPE_ENUM_INT8        enIpType;
    VOS_INT8                           ucReserved[3];
    VOS_INT8                           aucIpV4Addr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_INT8                           aucIpV6Addr[IMSA_WIFI_IPV6_ADDR_LEN];
}IMSA_WIFI_IP_ADDRESS_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_MEDIA_INFO_STRU
 �ṹ˵��  : media��Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT32                          ulChannelId;
    VOS_INT32                          usLocalRTPPort;
    VOS_INT32                          usLocalRTCPPort;
    VOS_INT32                          usRemoteRTPPort;       /* 0��ʾ��δ��� */
    VOS_INT32                          usRemoteRTCPPort;
    IMSA_WIFI_IP_ADDRESS_STRU          stRemoteAddr;
    IMSA_WIFI_IP_ADDRESS_STRU          stLocalAddr;
} IMSA_WIFI_MEDIA_INFO_STRU;


/*****************************************************************************
 �ṹ����: IMSA_WIFI_PDP_APN_STRU
 �ṹ˵��: APN�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucLength;
    VOS_UINT8                           aucValue[IMSA_WIFI_MAX_APN_LEN];
} IMSA_WIFI_PDP_APN_STRU;


/*****************************************************************************
 �ṹ����: IMSA_WIFI_PDP_IPV4_DNS_STRU
 �ṹ˵��: IPv4 DNS��ַ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPriDns;
    VOS_INT8                            cOpSecDns;
    VOS_INT8                            cOpSpare[2];

    VOS_UINT8                           aucPriDns[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecDns[IMSA_WIFI_IPV4_ADDR_LEN];
}IMSA_WIFI_PDP_IPV4_DNS_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_PDP_IPV4_PCSCF_STRU
 �ṹ˵��: IPv4 P-CSCF��ַ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT8                             cOpPrimPcscfAddr;
    VOS_INT8                             cOpSecPcscfAddr;
    VOS_INT8                             cOpThiPcscfAddr;
    VOS_INT8                             cOpSpare;

    VOS_UINT8                           aucPrimPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucThiPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
} IMSA_WIFI_PDP_IPV4_PCSCF_STRU;

/*****************************************************************************
 �ṹ����   : IMSA_WIFI_PDP_IPV6_DNS_STRU
 �ṹ˵��   : IPv6 DNS��ַ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPriDns;
    VOS_INT8                            cOpSecDns;
    VOS_INT8                            cOpSpare[2];

    VOS_UINT8                           aucPriDns[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucSecDns[IMSA_WIFI_IPV6_ADDR_LEN];
} IMSA_WIFI_PDP_IPV6_DNS_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_PDP_IPV6_PCSCF_STRU
 �ṹ˵��: IPv6 P-CSCF��ַ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPrimPcscfAddr;
    VOS_INT8                            cOpSecPcscfAddr;
    VOS_INT8                            cOpThiPcscfAddr;
    VOS_INT8                            cOpSpare;

    VOS_UINT8                           aucPrimPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucSecPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucThiPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
} IMSA_WIFI_PDP_IPV6_PCSCF_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_AP_MAC_STRU
 �ṹ˵��: WIFI AP MAC�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMS_ACCESS_TYPE_ENUM_INT32     enAccessType;
    VOS_CHAR                            acApMac[16];
}IMSA_WIFI_AP_MAC_STRU;

/*****************************************************************************
 �ṹ��    : WIFI_IMSA_POWERON_REQ_STRU
 �ṹ˵��  : WIFI����IMSA�Ŀ����Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            cFunctionVersion;/*�����汾�ţ���ʾAP֧�ָð汾�Ŷ�Ӧ�����Լ����°汾�Ŷ�Ӧ����*/


    /*��չ�汾����Ҫ����汾��Ծ����:

      ��������:һ��vowifi������һ����֧A��AP��modemά���İ汾��Ϊ13��������Ժ���Ҫ���뵽B��֧�ϣ�
             ��B��֧��modem�İ汾��Ϊ10(AP��Ӧ�����ǰ汾����13)����ô�����Ժ��뵽B��֧�Ϻ�,��modemֱ�Ӹ�AP���
             funcionVersionΪ13��AP����13�����µ�����ȫ���򿪣���modem�ಢû�а���11,12��Ӧ�İ汾�ŵ����ԣ�����
             �����������쳣

      �������:�ӿ���������չ�汾�ţ����ڱ�ʾ��������ڻ��߰汾������Ծ��vowifi���Զ�Ӧ�İ汾�š�
             �����������⣬modem��AP����cFunctionVersion����10��������չ�汾���б��н�13����汾�Ŵ���AP,AP���յ��Ժ�
             ��Ҫ��10��10���°汾�Ŷ�Ӧ�����ԺͰ汾��Ϊ13������


      �ӿ�Լ��:
        1.���������ֶ�ΪԤ���ֶΣ�ĿǰAP����Ҫ��modem�����չ�汾�ţ�(��cExtentionNum��cExtentionVersion���鶼Ӧ��0)
           2017-04-14

    */
    VOS_INT8                            cExtentionNum;/*��Ч��չ�汾�Ÿ���*/
    VOS_INT8                            cExtentionVersion[IMSA_WIFI_EXTENTION_VER_MAXNUM];/*��չ�汾�ţ����ڱ�ʾ��������ڻ��߰汾������Ծ��vowifi���Զ�Ӧ�İ汾��*/
    VOS_INT8                            cVoWiFiSwitch;       /*1: ����VoWiFi�򿪣�0:����VoWiFi�ر�  */
    VOS_INT8                            acRsv[3];
} WIFI_IMSA_POWERON_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WIFI_IMSA_VOWIFI_SWITCH_IND_STRU
 �ṹ˵��  : WIFI����IMSA��VOWIFI����Switch״̬�Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            cVoWiFiSwitch;       /*1: ����VoWiFi�򿪣�0:����VoWiFi�ر�  */
    VOS_INT8                            acRsv[3];
} WIFI_IMSA_VOWIFI_SWITCH_IND_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_DNS_QUERY_NTF_STRU
 �ṹ˵��  : IMSA����WIFI֪ͨ���·���DNS��ѯ�Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            acRsv[4];
} IMSA_WIFI_DNS_QUERY_NTF_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_THRESHOLD_CONF_NTF_STRU
 �ṹ˵��  : IMSA����WIFI֪ͨ���·���DNS��ѯ�Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT16                           sThresholdWiFiOut; /* ��WiFi�����ߵ����� */
    VOS_INT16                           sThresholdWiFiIn;  /* ����WiFi������� */
    VOS_INT8                            cImsaThrshFlag;    /* 1:ָʾʹ��IMSA��������, 0:����ʹ��MAPCON�ڲ����� */
    VOS_INT8                            acRsv[3];
} IMSA_WIFI_THRESHOLD_CONF_NTF_STRU;






/*****************************************************************************
 �ṹ��    : WIFI_IMSA_RTP_FAIL_IND_STRU
 �ṹ˵��  : WIFI����IMSA֪ͨRTP���������л��Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            acRsv[4];
} WIFI_IMSA_RTP_FAIL_IND_STRU;



/*****************************************************************************
 �ṹ��    : WIFI_IMSA_POWERON_CNF_STRU
 �ṹ˵��  : IMSA�ظ�WIFI�Ŀ����Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            cFunctionVersion;/*�����汾�ţ���ʾmodem֧�ָð汾�Ŷ�Ӧ�����Լ����°汾�Ŷ�Ӧ����*/

    /*��չ�汾����Ҫ����汾��Ծ����:
    ��������:һ��vowifi������һ����֧A��AP��modemά���İ汾��Ϊ13��������Ժ���Ҫ���뵽B��֧�ϣ�
           ��B��֧��modem�İ汾��Ϊ10(AP��Ӧ�����ǰ汾����13)����ô�����Ժ��뵽B��֧�Ϻ�,��modemֱ�Ӹ�AP���
           funcionVersionΪ13��AP����13�����µ�����ȫ���򿪣���modem�ಢû�а���11,12��Ӧ�İ汾�ŵ����ԣ�����
           �����������쳣

    �������:�ӿ���������չ�汾�ţ����ڱ�ʾ��������ڻ��߰汾������Ծ��vowifi���Զ�Ӧ�İ汾�š�
           �����������⣬modem��AP����cFunctionVersion����10��������չ�汾���б��н�13����汾�Ŵ���AP,AP���յ��Ժ�
           ��Ҫ��10��10���°汾�Ŷ�Ӧ�����ԺͰ汾��Ϊ13������

    �ӿ�Լ��:
    1.������ڰ汾��Ծ������ʱ�Ż��õ���չ�汾������ֶΣ����������ӦЯ��(��cExtentionNum��cExtentionVersion���鶼Ӧ��0)
    2.������ڰ汾��Ծ������ʱ��modem�ڸýӿ��У���Ҫ����Ч��չ�汾�Ÿ���cExtentionNum��������չ�汾�������������չ�汾�ţ�����ȷ��չ�汾��
      ������ÿ��Ԫ�ض�Ӧ��vowifi��������
    3.������ڰ汾��Ծ������ʱ��modem��ͨ���ýӿڴ�������cFunctionVersionӦ�Ǹ����Ժ���ǰ��modem�汾��

    4.
       4.1.��modem�ظ�POWER-ON-CNF��Ϣ��δЯ����Ч��չ�汾�ţ�APֻ��ƥ������汾��cFunctionVersion;
           ��cFunctionVersion�汾�Ŷ�Ӧ�����Ժ͸ð汾���������а汾�Ŷ�Ӧ�����ԣ�

       4.2.��modem�ָ�POWER-ON-CNF��Ϣ����Я����Ч��չ�汾�ţ�AP����Ҫ��cFunctionVersion�汾�Ŷ�Ӧ�����Ժ͸ð汾���������а汾�Ŷ�Ӧ�����ԣ�
           ����Ҫ����չ�汾�Ŷ�Ӧ������

    */
    VOS_INT8                           cExtentionNum;/*��Ч��չ�汾�Ÿ���*/

    VOS_INT8                           acExtentionVersion[IMSA_WIFI_EXTENTION_VER_MAXNUM];/*��չ�汾�ţ����ڱ�ʾ��������ڻ��߰汾������Ծ��vowifi���Զ�Ӧ�İ汾��*/


} IMSA_WIFI_POWERON_CNF_STRU;

/*****************************************************************************
 �ṹ��    : WIFI_IMSA_POWEROFF_REQ_STRU
 �ṹ˵��  : WIFI����IMSA�ػ��Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    IMSA_WIFI_POWEROFF_REASON_ENUM_INT8 enReason;         /* 0:WiFi�رգ�1��ϵͳ�رգ�2: VoWiFi�رգ�3:���ν�ֹ�ر� */
    VOS_INT8                           acReserve[3];
} WIFI_IMSA_POWEROFF_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WIFI_IMSA_POWEROFF_CNF_STRU
 �ṹ˵��  : WIFI����IMSA�ػ��ظ��Ľṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];
} IMSA_WIFI_POWEROFF_CNF_STRU;



/*****************************************************************************
 �ṹ����: IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU
 �ṹ˵��: ��������ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/

    IMSA_WIFI_IP_TYPE_ENUM_INT8         enIpType;
    VOS_INT8                            cIsHandover;       /* 1��Handover��0��Initial */
    VOS_INT8                            cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    VOS_INT8                            acReServe;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    IMSA_WIFI_IP_ADDRESS_STRU           stPdpAddr;
    IMSA_WIFI_PDP_IPV4_DNS_STRU         stDns;
    IMSA_WIFI_PDP_IPV4_PCSCF_STRU       stPcscf;
    IMSA_WIFI_PDP_IPV6_DNS_STRU         stIPv6Dns;
    IMSA_WIFI_PDP_IPV6_PCSCF_STRU       stIPv6Pcscf;
} IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU
 �ṹ˵��: WIFI PDN�������ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_IP_TYPE_ENUM_INT8         enPdnType;
    VOS_INT8                            cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    WIFI_IMSA_PDN_ACT_RESULT_ENUM_INT8  enCause;
    WIFI_IMSA_PDN_REACT_ACTION_ENUM_INT8  enRetryAction;

    IMSA_WIFI_PDP_APN_STRU              stApn;
    IMSA_WIFI_IP_ADDRESS_STRU           stPdpAddr;
    IMSA_WIFI_PDP_IPV4_DNS_STRU         stDns;
    IMSA_WIFI_PDP_IPV4_PCSCF_STRU       stPcscf;
    IMSA_WIFI_PDP_IPV6_DNS_STRU         stIPv6Dns;
    IMSA_WIFI_PDP_IPV6_PCSCF_STRU       stIPv6Pcscf;
    IMSA_WIFI_AP_MAC_STRU               stApMac;
    VOS_INT32                           lPdnReActTimerLen; /* ����Ϊ��λ�����·���WifiPdn�������̵Ķ�ʱ�� */
    VOS_INT32                           lWiFiMtu; /* WIFI�´�MTU */
    VOS_INT32                           lNetworkReason;
} WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_IMS_PDN_READY_IND_STRU
 �ṹ˵��: WIFI��·�����ɹ��ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32                     lMsgId;
    VOS_INT8                                        cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    VOS_INT8                                        acReserve[3];
}WIFI_IMSA_IMS_PDN_READY_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU
 �ṹ˵��: WIFI PDNȥ��������ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    VOS_INT8                            cIsLocal;          /* 1�������ͷţ�0�������ཻ�� */
    VOS_INT8                            cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    VOS_INT8                            cIsHandover;/* ָʾ�Ƿ����л����µĳ����ͷ�,1���л��ͷţ�0����ͨ�ͷ� */
    VOS_INT8                            acReserve[1];
} IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU;


/*****************************************************************************
 �ṹ����: WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU
 �ṹ˵��: WIFI PDNȥ�������ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    VOS_INT8                            cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    VOS_INT8                            acReserve[3];
} WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU
 �ṹ˵��: WIFI PDNȥ�������ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    VOS_INT8                            cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    WIFI_IMSA_PDN_DEACT_CAUSE_ENUM_INT8 enCause;           /* ֪ͨ��ǰ��������ͷŵ�ԭ�� */
    VOS_INT8                            acReserve[2];
} WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU;


/*****************************************************************************
 �ṹ����: WIFI_IMSA_STATE_IND_STRU
 �ṹ˵��: WIFI״̬�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    WIFI_IMSA_QUALITY_LEVEL_ENUM_INT8  enWifiQuality;
    WIFI_IMSA_SERVICE_STATUS_ENUM_INT8 enServiceStatus;
    VOS_INT8                           acReserve[2];
    IMSA_WIFI_AP_MAC_STRU              stApMac;
    VOS_CHAR                           acSsid[WIFI_IMSA_SSID_LEN];
    VOS_CHAR                           acReserve1[3];
} WIFI_IMSA_STATE_IND_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_ERROR_IND_STRU
 �ṹ˵��: WIFI����ָʾ�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    WIFI_IMSA_ERROR_CAUSE_ENUM_INT8    enCause;
    VOS_INT8                           cServiceType;      /* 0����ͨAPN�ϵ���ͨҵ��1����ͨAPN�ϵĽ���ҵ��, 2: ����APN�ϵĽ���ҵ�� */
    VOS_INT8                           acReserve[2];
} WIFI_IMSA_ERROR_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU
 �ṹ˵��: IMS audio��Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    VOS_INT8                           cAudioInfoCnt;
    IMSA_WIFI_SERVICE_TYPE_ENUM_INT8   enServiceType;/*0,��ͨAPN�ϵ���ͨWIFIҵ��1����ͨAPN�ϵĽ���WIFIҵ��3������APN�ϵ�WIFI����ҵ��*/
    VOS_INT8                           acReserved[2];
    IMSA_WIFI_MEDIA_INFO_STRU          stMediaInfo[4];
} IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU
 �ṹ˵��: IMS RTT��Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    VOS_INT8                           cRttInfoCnt;
    IMSA_WIFI_SERVICE_TYPE_ENUM_INT8   enServiceType;/*0,��ͨAPN�ϵ���ͨWIFIҵ��1����ͨAPN�ϵĽ���WIFIҵ��3������APN�ϵ�WIFI����ҵ��*/
    VOS_INT8                           acReserved[2];
    IMSA_WIFI_MEDIA_INFO_STRU          stMediaInfo[4];
} IMSA_WIFI_IMS_RTT_INFO_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_IMS_VIDEO_INFO_IND_STRU
 �ṹ˵��: IMS video��Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    VOS_INT8                           cVideoInfoCnt;
    VOS_INT8                           acReserved[3];
    IMSA_WIFI_MEDIA_INFO_STRU          stMediaInfo[2];
} IMSA_WIFI_IMS_VIDEO_INFO_IND_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_OPEN_VOWIFI_IND_STRU
 �ṹ˵��  : IMSA֪ͨMAPCON��VOWIFI��Ϣ�ṹ
 ��ϢԼ��  :
    1)IMSAֻ����VOWIFIδ��������IMSA��Ϊ��Ҫ��VOWIFIʱ��ʹ�ô���Ϣ֪ͨMAPCON��
    2)MAPCON���յ�����Ϣʱ�������ǰWIFI״̬һ����������Ҫ��IMSA�·�ID_WIFI_IMSA_POWERON_REQ
      ��Ϣָʾ��������ʱ������IMSA�ظ�ID_WIFI_IMSA_OPEN_VOWIFI_RSP��
    3)���MAPCON��ĳ���쳣ԭ���ֵ�ǰ���ܴ�VOWIFI�������յ�����Ϣ����Ҫ��IMSA�ظ�
      ID_WIFI_IMSA_OPEN_VOWIFI_RSP֪ͨIMSA��VOWIFIʧ�ܣ�
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];      /* ����λ */
} IMSA_WIFI_OPEN_VOWIFI_IND_STRU;

/*****************************************************************************
 �ṹ��    : WIFI_IMSA_OPEN_VOWIFI_RSP_STRU
 �ṹ˵��  : MAPCON֪ͨIMSA��VOWIFI���
 ��ϢԼ��  : ��ǰֻ��MAPCON���޷���VOWIFIʱ��ʹ�ô���Ϣ�ظ�IMSA
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT32                          lResult;           /* 0 ���쳣��1 ��WIFIʧ�� */
} WIFI_IMSA_OPEN_VOWIFI_RSP_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_CLOSE_VOWIFI_IND_STRU
 �ṹ˵��  : IMSA֪ͨMAPCON�ر�VOWIFI��Ϣ�ṹ
 ��ϢԼ��  :
    1)IMSAֻ����MAPCON���͹�ID_IMSA_WIFI_OPEN_VOWIFI_IND�Ż���MAPCON���ʹ���Ϣ��
    2)IMSAֻҪ��MAPCON���͹�ID_IMSA_WIFI_OPEN_VOWIFI_IND�����ǳ���MAPCON��IMSAֱ���·�
      ID_WIFI_IMSA_POWEROFF_REQ(���ⳡ�����û��ڴ�videoʱֱ�Ӱ�����WIFI�ر�)���������
      IMSA�ں���һ������ĳ������ҪVOWIFI��ʱ����MAPCON���ʹ���Ϣ��
    3)IMSA��MAPCON�ϱ�����Ϣ��MAPCON���ͬ��ر�VOWIFI�������IMSA�·�ID_WIFI_IMSA_POWEROFF_REQ��
      �����ͬ��ر�VOWIFI�������ѡ����IMSA�·��κ���Ϣ��
      (���ⳡ�������û���video�ڼ����ֶ���VOWIFI���˴����ж��߼���MAPCON��֤)
    4)���֮ǰIMSA��MAPCON���͹�ID_IMSA_WIFI_OPEN_VOWIFI_IND���������յ�MAPCON�·�
      ID_WIFI_IMSA_POWERON_REQ֮ǰ��IMSA����ر�VOWIFI����ʱIMSAֻҪֱ����MAPCON�ϱ�����Ϣ���ɣ�
      ����IMSA��MAPCON���ᰴ�յ�����Ϣ˳����д���
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];      /* ����λ */
} IMSA_WIFI_CLOSE_VOWIFI_IND_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_WIFI_CELLULAR_OPEN_IND_STRU
 �ṹ˵��  : IMSA֪ͨMAPCON��ǰCelluar������Ϣ�ṹ
 ��ϢԼ��  :
    1)IMSAֻ����Cellular������ʹ�ô���Ϣ֪ͨMAPCON;
    2)MAPCON���յ�����Ϣʱ�������ǰWIFI״̬�����״̬��һ��,��Ҫ���ش���WiFi״̬��WiFi���״̬
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           cWiFiStatus;       /* 1: WiFi On; 0:WiFi Off */
    VOS_INT8                           cWiFiTunnelStatus; /* 1: WiFi Tunnel Exist; 0:WiFi Tunnel non-exist */
    VOS_INT8                           acReserve[2];      /* ����λ */
} IMSA_WIFI_CELLULAR_OPEN_IND_STRU;
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];      /* ����λ */
} IMSA_WIFI_LOCATION_UPDATE_REQ_STRU;
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];      /* ����λ */
} IMSA_WIFI_NV_REFRESH_IND_STRU;
typedef struct
{
    VOS_UINT8                           aucIpV4Addr[IMSA_WIFI_IPV4_ADDR_LEN];
}IMSA_WIFI_IPV4_EPDG_STRU;

typedef struct
{
    VOS_UINT8                           aucIpV6Addr[IMSA_WIFI_IPV6_ADDR_LEN];
}IMSA_WIFI_IPV6_EPDG_STRU;

/*****************************************************************************
 �ṹ����: IMSA_WIFI_EPDG_INFO_STRU
 �ṹ˵��: IMSA֪ͨwifi����Я����EPDG��ַ��Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usIpv4EpdgNum;
    VOS_UINT16                          usIpv6EpdgNum;
    IMSA_WIFI_IPV4_EPDG_STRU            astIpv4EpdgList[IMSA_WIFI_MAX_IPV4_EPDG_NUM];
    IMSA_WIFI_IPV6_EPDG_STRU            astIpv6EpdgList[IMSA_WIFI_MAX_IPV6_EPDG_NUM];
}IMSA_WIFI_EPDG_INFO_STRU;

typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_EPDG_INFO_STRU            stEpdgInfo;  
}IMSA_WIFI_EPDG_INFO_IND_STRU;


/*****************************************************************************
 �ṹ����: WIFI_IMSA_LOCATION_UPDATE_IND_STRU
 �ṹ˵��: IMSλ����Ϣ��Ϣ�ṹ��
*****************************************************************************/

typedef struct
{
   VOS_CHAR acCountry[IMSA_WIFI_CIVICLOC_COUNTRY_LEN];/*��������Ϣ*/
   VOS_CHAR acState[IMSA_WIFI_CIVICLOC_STATE_LEN];/*����Ϣ*/
   VOS_CHAR acCity[IMSA_WIFI_CIVICLOC_CITY_LEN];/*����Ϣ*/
   VOS_CHAR acZip[IMSA_WIFI_CIVICLOC_ZIP_LEN];/*�ʱ���Ϣ*/
   VOS_CHAR acRadius[IMSA_WIFI_CIVICLOC_RADIUS_LEN];/*radius��Ϣ*/
   VOS_CHAR acCountryA5[IMSA_WIFI_CIVICLOC_COUNTRY_A5_LEN];
   VOS_CHAR acRoadStreet[IMSA_WIFI_CIVICLOC_ROADSTREET_LEN];/*�ֵ���Ϣ*/
   VOS_CHAR acOtherAddress[IMSA_WIFI_CIVICLOC_OTHER_ADDRESS_LEN];
}stCivicLocInfo;

typedef struct
{
   VOS_CHAR acLatitude[IMSA_WIFI_COORDINATE_LATITUDE_LEN];
   VOS_CHAR acLongitude[IMSA_WIFI_COORDINATE_LONGITUDE_LEN];
   VOS_CHAR acAltitude[IMSA_WIFI_COORDINATE_ALTITUDE_LEN];
}stCoordinateLocInfo;


typedef struct
{
    VOS_UINT32                       ulMsgId;
    VOS_INT8                         cIsCivicLocInfoExist;
    VOS_INT8                         cIsCoordinateLocInfoExist;
    IMSA_WIFI_INTER_ROAMING_STATUS_ENUM_INT8  cWifiInterRoamingFlag; /* 0:��֧�����ԣ�1:û�й������Σ� 2:�������� */
    VOS_INT8                         cReserved;
    stCivicLocInfo                   stCivicLocInfo;
    stCoordinateLocInfo              stCoordinateLocInfo;
    VOS_CHAR                         acRetentionExpiry[IMSA_WIFI_TIMESTAMP_LEN];
    VOS_CHAR                         acTimeStamp[IMSA_WIFI_TIMESTAMP_LEN];
} WIFI_IMSA_LOCATION_UPDATE_IND_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_MSG_STRU
 �ṹ˵��: WIFI-IMSA��Ľṹ��
*****************************************************************************/

typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    VOS_INT8                            acReserve[4];
} WIFI_IMSA_MSG_STRU;

/*****************************************************************************
 �ṹ����: WIFI_IMSA_ABNORMAL_HOLD_STATE_IND_STRU
 �ṹ˵��: WIFI����,MAPCON֪ͨIMSA������˳����������Ϣ
*****************************************************************************/
/*��֣�����۽ӿڵ������������ֶηŵ��ṹ��int8���ͺ���*/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32                      lMsgId;
    IMSA_WIFI_TUNNEL_HOLD_STATUS_ENUM_INT8           cAbnormalState;/*1����ʾ�����������ģʽ��0����ʾ�˳����ģʽ*/
    VOS_INT8                                         cReserve;
    VOS_INT16                                        sAbNormalTime;
}WIFI_IMSA_ABNORMAL_HOLD_STATE_IND_STRU;


/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif



#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif


#endif //__IMSA_WIFI_INTERFACE_H__
