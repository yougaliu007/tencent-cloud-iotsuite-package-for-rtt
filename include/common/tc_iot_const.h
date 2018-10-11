#ifndef IOT_CONST_H
#define IOT_CONST_H

/**
 * @brief ϵͳ���������붨�壬��������ԭ��
 * 1. =0 ��ʾSUCCESS״̬������������ִ�гɹ�
 * 2. <0 ��ʾϵͳ�����쳣������ֵΪ��Ӧ���´�����
 * 3. >0 ���ڶ�д���շ��ຯ������ʾ��ǰ������Ӧִ�гɹ����ֽ�����
 */
typedef enum _tc_iot_sys_code_e {
    TC_IOT_SUCCESS = 0,               /**< ִ�гɹ� */
    TC_IOT_FAILURE = -90,             /**< ִ��ʧ��*/
    TC_IOT_INVALID_PARAMETER = -91,   /**< ����ȡֵ�Ƿ� */
    TC_IOT_NULL_POINTER = -92,        /**< ����ָ��Ϊ�� */
    TC_IOT_TLS_NOT_SUPPORTED = -93,   /**< ϵͳ��֧�� TLS ���ӷ�ʽ*/
    TC_IOT_BUFFER_OVERFLOW = -94,     /**< ������������ڴ�ռ䲻�㣬�޷�����������*/
    TC_IOT_FUCTION_NOT_IMPLEMENTED = -99,     /**< ���亯��δ��Ǩ��ʵ�� */

    TC_IOT_NETWORK_ERROR_BASE = -100, /**< ���������*/
    TC_IOT_NET_UNKNOWN_HOST = -101, /**< ��������ʧ��*/
    TC_IOT_NET_CONNECT_FAILED = -102, /**< ��������ʧ��*/
    TC_IOT_NET_SOCKET_FAILED = -103, /**< Socket����ʧ��*/
    TC_IOT_SEND_PACK_FAILED = -104, /**< ��������ʧ��*/
    TC_IOT_NET_NOTHING_READ = -105, /**< δ��ȡ������*/
    TC_IOT_NET_READ_TIMEOUT = -106, /**< ��ȡ���ݳ�ʱ*/
    TC_IOT_NET_READ_ERROR = -107, /**< ��ȡ����ʧ��*/

    TC_IOT_MQTT_RECONNECT_TIMEOUT = -150, /**< MQTT ������ʱ*/
    TC_IOT_MQTT_RECONNECT_IN_PROGRESS = -151, /**< MQTT ���ڳ�������*/
    TC_IOT_MQTT_RECONNECT_FAILED = -152, /**< ����ʧ��*/
    TC_IOT_MQTT_NETWORK_UNAVAILABLE = -153,/**< ���粻����*/
    TC_IOT_MQTT_WAIT_ACT_TIMEOUT = -154, /**< �ȴ�ACK��Ӧ����*/

    TC_IOT_MQTT_CONNACK_ERROR = -160, /**< ����ʧ�ܣ�δ֪����*/
    TC_IOT_MQTT_CONNACK_PROTOCOL_UNACCEPTABLE = -161, /**< ����ʧ�ܣ���֧�������Э��汾*/
    TC_IOT_MQTT_CONNACK_CLIENT_ID_INVALID = -162, /**< ����ʧ�ܣ�Client Id �ж��Ƿ�*/
    TC_IOT_MQTT_CONNACK_SERVICE_UNAVAILABLE = -163,  /**< ����ʧ�ܣ���˷��񲻿���*/
    TC_IOT_MQTT_CONNACK_BAD_USER_OR_PASSWORD = -164,  /**< ����ʧ�ܣ��û���������У�鲻ͨ��*/
    TC_IOT_MQTT_CONNACK_NOT_AUTHORIZED = -165,  /**< ����ʧ�ܣ��û�����δ��Ȩ*/
    TC_IOT_MQTT_SUBACK_FAILED = -170,  /**< ����ʧ�ܣ��п��� Topic �����ڣ�������Ȩ�޵�*/

    TC_IOT_MBED_TLS_ERROR_BASE = -200, /**< TLS ����*/
    TC_IOT_CTR_DRBG_SEED_FAILED = -201, /**< TLS ��ʼ��ʧ��*/
    TC_IOT_X509_CRT_PARSE_FILE_FAILED = -202, /**< ����֤���ļ�ʧ��*/
    TC_IOT_PK_PRIVATE_KEY_PARSE_ERROR = -203, /**< ����˽Կ�ļ�ʧ��*/
    TC_IOT_PK_PARSE_KEYFILE_FAILED = -204, /**< ����֤��ʧ�� */
    TC_IOT_TLS_NET_SET_BLOCK_FAILED = -205,/**< ���� TLS Block״̬ʧ��*/
    TC_IOT_TLS_SSL_CONFIG_DEFAULTS_FAILED = -206,/**< ���� TLS ����ʧ��*/
    TC_IOT_TLS_SSL_CONF_OWN_CERT_FAILED = -207, /**< ���ÿͻ���֤��ʧ��*/
    TC_IOT_TLS_SSL_SETUP_FAILED = -208,/**< TLS ����ʧ��*/
    TC_IOT_TLS_SSL_SET_HOSTNAME_FAILED = -209,/**< TLS ���÷��������ʧ��*/
    TC_IOT_TLS_SSL_HANDSHAKE_FAILED = -210, /**< TLS ����ʧ��*/
    TC_IOT_TLS_X509_CRT_VERIFY_FAILED = -211, /**< ֤����֤ʧ��*/
    TC_IOT_TLS_SSL_WRITE_FAILED = -212, /**< TLS д�����ʧ��*/
    TC_IOT_TLS_SSL_WRITE_TIMEOUT = -213,/**< TLS д�������ʱ*/
    TC_IOT_TLS_SSL_READ_FAILED = -214,/**< TLS ��ȡʧ��*/
    TC_IOT_TLS_SSL_READ_TIMEOUT = -215,/**< TLS ��ȡ��ʱ*/

    TC_IOT_COAP_PACK_INVALID = -300, /**< COAP������ʧ��*/
    TC_IOT_COAP_MSG_VER_INVALID = -301, /**< COAP��ver�ֶ�ȡֵ�Ƿ�*/
    TC_IOT_COAP_MSG_TYPE_INVALID = -302, /**< COAP��type�ֶ�ȡֵ�Ƿ�*/
    TC_IOT_COAP_MSG_TOKEN_LEN_INVALID = -303, /**< COAP��TKLȡֵ�Ƿ�*/
    TC_IOT_COAP_MSG_OPTION_DELTA_INVALID = -304, /**< COAP��Option Deltaȡֵ�Ƿ�*/
    TC_IOT_COAP_MSG_OPTION_LENGTH_INVALID = -305, /**< COAP��Option Lengthȡֵ�Ƿ�*/
    TC_IOT_COAP_MSG_OPTION_COUNT_TOO_MUCH = -306, /**< COAP��Option ��������*/

    TC_IOT_LOGIC_ERROR_BASE = -1000, /**< ҵ���߼����� */
    TC_IOT_NETWORK_PTR_NULL = -1001,/**< network ����ָ��Ϊ��*/
    TC_IOT_NETCONTEXT_PTR_NULL = -1002, /**< netcontext ����ָ��Ϊ��*/
    TC_IOT_JSON_PATH_NO_MATCH = -1003, /**< json path ����ʧ�ܣ�δ�ҵ�ָ������*/
    TC_IOT_JSON_PARSE_FAILED = -1004, /**< json ����ʧ��*/
    TC_IOT_ERROR_HTTP_REQUEST_FAILED = -1005, /**<  http ����ʧ��*/
    TC_IOT_REFRESH_TOKEN_FAILED = -1006, /**< ��ȡ�û�Tokenʧ�� */
    TC_IOT_HTTP_RESPONSE_INVALID = -1007, /**< �������Ӧ�� HTTP ���ݸ�ʽ�Ƿ� */
    TC_IOT_SHADOW_SESSION_NOT_ENOUGH = -1008, /**< Ӱ�ӻص��Ự�ռ䲻���������Ƿ��͹��ಢ������
                                                �������� TC_IOT_MAX_SESSION_COUNT ��ֵ*/
    TC_IOT_HTTP_REDIRECT_TOO_MANY = -1009, /**< HTTP ��ת�������࣬�п�������תѭ�� */
    TC_IOT_REPORT_SKIPPED_FOR_NO_CHANGE = -1010, /**< �����ޱ䶯�������ϱ�*/
    TC_IOT_FIRM_INFO_NAME_TOO_LONG = -1011, /**< �ϱ����豸��Ϣ�ֶ�����̫��*/
    TC_IOT_FIRM_INFO_VALUE_TOO_LONG = -1012, /**< �ϱ����豸��Ϣ�ֶ�ȡֵ̫��*/
    TC_IOT_HTTP_RESPONSE_NO_CONTENT_LENGTH = -1013, /**< HTTP ��Ӧ��δ����Content-Length�ֶ�*/
    TC_IOT_JSON_PARSE_TOKEN_NO_MEM = -1014, /**< json ����Token������*/
    TC_IOT_HTTP_RPC_FAILED = -1015, /**< HTTP rpc �ӿ�����ʧ��*/

    TC_IOT_HTTP_INVALID_STATUS_CODE = -1201, /** ��������󣬲��� 1xx ~ 5xx ������ */
    TC_IOT_HTTP_BODY_START_FOUND = -1204, /** ���� HTTP ���� */

} tc_iot_sys_code_e;

#define TC_IOT_ARRAY_LENGTH(a)  (sizeof(a)/sizeof(a[0]))


typedef enum _tc_iot_event_e {
    TC_IOT_EVENT_UNKNOWN = 0,
    TC_IOT_SYS_EVENT_BASE = 1,
    TC_IOT_MQTT_EVENT_BASE = 50,
    TC_IOT_SHADOW_EVENT_BASE = 100,
    TC_IOT_SHADOW_EVENT_SERVER_CONTROL,
    TC_IOT_USER_EVENT_BASE = 200,
} tc_iot_event_e;


typedef struct _tc_iot_code_map {
    int code;
    const char * str;
} tc_iot_code_map;

typedef struct _tc_iot_event_message {
    tc_iot_event_e  event;
    void * data;
} tc_iot_event_message;

/**
* @brief tc_iot_event_handler ͨ���¼��ص�ԭ��
*
* @param msg ��Ϣ����
* @param src �����¼�����Դ
* @param context ���� context ��Ϣ��
*
* @return @see tc_iot_sys_code_e
*/
typedef int (*tc_iot_event_handler)(tc_iot_event_message *msg, void * client,  void * context);

#define TC_IOT_CONTAINER_OF(ptr, type, member)		\
				(type *)((char *)ptr - offsetof(type, member))

#define tc_iot_const_str_len(a)  (sizeof(a)-1)

#define tc_iot_str2_equal(m, c0, c1) \
(m[0] == c0 && m[1] == c1)

#define tc_iot_str3_equal(m, c0, c1, c2) \
(m[0] == c0 && m[1] == c1 && m[2] == c2)

#define tc_iot_str4equal(m, c0, c1, c2, c3) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3)

#define tc_iot_str5equal(m, c0, c1, c2, c3, c4) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3 && m[4] == c4)

#define tc_iot_str6equal(m, c0, c1, c2, c3, c4, c5) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3 \
    && m[4] == c4 && m[5] == c5)

#define tc_iot_str7equal(m, c0, c1, c2, c3, c4, c5, c6) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3 \
    && m[4] == c4 && m[5] == c5 && m[6] == c6)

#define tc_iot_str8equal(m, c0, c1, c2, c3, c4, c5, c6, c7) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3       \
    && m[4] == c4 && m[5] == c5 && m[6] == c6 && m[7] == c7)

#define tc_iot_str9equal(m, c0, c1, c2, c3, c4, c5, c6, c7, c8) \
(m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3            \
    && m[4] == c4 && m[5] == c5 && m[6] == c6 && m[7] == c7 && m[8] == c8)
#define TC_IOT_MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

#endif /* end of include guard */
