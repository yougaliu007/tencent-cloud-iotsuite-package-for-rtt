#ifndef TC_IOT_SHADOW_H
#define TC_IOT_SHADOW_H

#define TC_IOT_MAX_PROPERTY_COUNT   128
#define TC_IOT_MAX_FIRM_INFO_COUNT  5

#define TC_IOT_SHADOW_JSON_TRUE   "true"
#define TC_IOT_SHADOW_JSON_FALSE   "false"

typedef double tc_iot_shadow_number;
typedef int tc_iot_shadow_int;
typedef int tc_iot_shadow_enum;
typedef char tc_iot_shadow_bool;
typedef char * tc_iot_shadow_string;

typedef enum _tc_iot_shadow_data_type_e {
    TC_IOT_SHADOW_TYPE_INVALID = 0,
    TC_IOT_SHADOW_TYPE_BOOL = 1,
    TC_IOT_SHADOW_TYPE_NUMBER = 2,
    TC_IOT_SHADOW_TYPE_ENUM = 3,
    TC_IOT_SHADOW_TYPE_INT = 4,
    TC_IOT_SHADOW_TYPE_STRING = 5,
} tc_iot_shadow_data_type_e;

typedef struct _tc_iot_shadow_property_def {
    const char * name;
    int id;
    tc_iot_shadow_data_type_e  type;
    int offset;
    int len;
} tc_iot_shadow_property_def;


/**
 * @brief Ӱ���豸����
 */
typedef struct _tc_iot_shadow_config {
    tc_iot_mqtt_client_config mqtt_client_config;  /**< MQTT �������*/
    char sub_topic[TC_IOT_MAX_MQTT_TOPIC_LEN]; /**< Ӱ���豸���� Topic*/
    char pub_topic[TC_IOT_MAX_MQTT_TOPIC_LEN];  /**< Ӱ���豸��Ϣ Publish Topic*/
    message_handler on_receive_msg; /**< Ӱ���豸��Ϣ�ص�*/
    int property_total;
    tc_iot_shadow_property_def * properties;
    tc_iot_event_handler event_notify;
    void * p_current_device_data;
    void * p_reported_device_data;
    void * p_desired_device_data;
} tc_iot_shadow_config;

typedef enum _tc_iot_command_ack_status_e {
    TC_IOT_ACK_SUCCESS,
    TC_IOT_ACK_TIMEOUT,
} tc_iot_command_ack_status_e;

typedef void (*message_ack_handler)(tc_iot_command_ack_status_e ack_status, tc_iot_message_data * md , void * session_context);


#define TC_IOT_SESSION_ID_LEN     8
#define TC_IOT_MAX_SESSION_COUNT  10

typedef struct _tc_iot_shadow_session{
    char sid[TC_IOT_SESSION_ID_LEN+1];
    tc_iot_timer        timer;
    message_ack_handler handler;
    void * session_context;
}tc_iot_shadow_session;


/**
 * @brief Ӱ���豸�ͻ���
 */
typedef struct _tc_iot_shadow_client {
    tc_iot_shadow_config* p_shadow_config; /**< Ӱ���豸����*/
    tc_iot_mqtt_client mqtt_client; /**< MQTT �ͻ���*/
    tc_iot_shadow_session sessions[TC_IOT_MAX_SESSION_COUNT];
    char desired_bits[(TC_IOT_MAX_PROPERTY_COUNT/8)+1];
    char reported_bits[(TC_IOT_MAX_PROPERTY_COUNT/8)+1];
} tc_iot_shadow_client;


/**
 * @brief tc_iot_shadow_doc_pack_for_get ���� get ����������ڶ�ȡӰ���豸��������ݡ�
 *
 * @param buffer ������������
 * @param buffer_len ��������������С
 * @param c Ӱ���豸�ͻ���
 *
 * @return ����������
 */
int tc_iot_shadow_doc_pack_for_get(char * buffer, int buffer_len, tc_iot_shadow_client *c);


/**
 * @brief tc_iot_shadow_doc_pack_for_update ����Ӱ���豸�ĵ��������ϱ������
 *
 * @param buffer ������������
 * @param buffer_len ��������������С
 * @param c Ӱ���豸�ͻ���
 * @param reported Reported �ĵ��ϱ�����
 * @param desired  Desired �ĵ��ϱ�����
 *
 * @return ����������
 */
int tc_iot_shadow_doc_pack_for_update(char * buffer, int buffer_len, tc_iot_shadow_client *c, const char * reported, const char * desired);

/**
 * @brief tc_iot_shadow_doc_pack_for_delete ����Ӱ���豸ɾ���ĵ������������
 *
 * @param buffer ������������
 * @param buffer_len ��������������С
 * @param c Ӱ���豸�ͻ���
 * @param reported Reported ��������ɾ��ָ��
 * @param desired  Desired ��������ɾ��ָ��
 *
 * @return ����������
 */
int tc_iot_shadow_doc_pack_for_delete(char * buffer, int buffer_len, tc_iot_shadow_client *c, const char * reported, const char * desired);


/*--- begin Ӱ���豸������Ӧ�� method �ֶ�ȡֵ----*/
/* ������ */
/**< ��ȡ�����Ӱ���豸����
 * {"method":"get"}
 * */
#define TC_IOT_MQTT_METHOD_GET       "get"
/**< ���·����Ӱ���豸����
 * {"method":"update","state"{"reported":{"a":1}}}
 *
 * */
#define TC_IOT_MQTT_METHOD_UPDATE    "update"

/**< ɾ�������Ӱ���豸����
 * {"method":"delete","state"{"desired":{"a":null}}}
 *
 * */
#define TC_IOT_MQTT_METHOD_DELETE    "delete"

/**< �����豸״̬����ָ��
 * {"method":"control","state"{"reported":{"a":1}, "desired":{"a":2}}}
 * */
#define TC_IOT_MQTT_METHOD_CONTROL   "control"

/* ��Ӧ�� */
/**< ��ȡ������Ӧ*/
#define TC_IOT_MQTT_METHOD_REPLY     "reply"


/*--- end Ӱ���豸������Ӧ�� method �ֶ�ȡֵ----*/

/**
 * @brief tc_iot_shadow_construct �����豸Ӱ�Ӷ���
 *
 * @param p_shadow_client �豸Ӱ�Ӷ���
 * @param p_config ��ʼ���豸Ӱ�Ӷ��������������
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_shadow_construct(tc_iot_shadow_client * p_shadow_client,
                            tc_iot_shadow_config *p_config);


/**
 * @brief tc_iot_shadow_destroy �ر� Shadow client ���ӣ������� Shadow client
 *
 * @param p_shadow_client �豸Ӱ�Ӷ���
 */
void tc_iot_shadow_destroy(tc_iot_shadow_client *p_shadow_client);


/**
 * @brief tc_iot_shadow_isconnected �ж��豸Ӱ�Ӷ����Ƿ��ѳɹ����ӷ�����
 *
 * @param p_shadow_client �豸Ӱ�Ӷ���
 *
 * @return 1 ��ʾ�����ӣ�0 ��ʾδ���ӡ�
 */
char tc_iot_shadow_isconnected(tc_iot_shadow_client *p_shadow_client);

/**
 * @brief tc_iot_shadow_yield �ڵ�ǰ�߳�Ϊ�ײ�����ó�һ�� CPU ִ
 * ��ʱ��
 *
 * @param  p_shadow_client �豸Ӱ�Ӷ���
 * @param timeout_ms �ȴ�ʱ�ӣ���λ����
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_shadow_yield(tc_iot_shadow_client *p_shadow_client, int timeout_ms);


/**
 * @brief tc_iot_shadow_get �첽��ʽ��ȡ�豸Ӱ���ĵ�
 *
 * @param c �豸Ӱ�Ӷ���
 * @param buffer �豸Ӱ���ĵ�����
 * @param buffer_len �豸Ӱ���ĵ�������󳤶�
 * @param callback ������Ӧ���ݻص�����ѡ���� NULL ���ʾ��ָ����Ӧ�ص���δָ��
 * �ص�ʱ���������Ӧ������ shadow ��Ĭ�ϻص����� on_receive_msg ����
 * @param timeout_ms �������ȴ�ʱ�ӣ���ѡ����ָ�� callback ����ʱ����ָ���ûص����
 * �ȴ�ʱ�䡣
 * @param session_context ������� context����ѡ������͸��ʱ���ɴ�Ĭ�ϵ� NULL ��
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_shadow_get(tc_iot_shadow_client *c, char * buffer, int buffer_len,
         message_ack_handler callback, int timeout_ms, void * context);

/**
 * @brief tc_iot_shadow_update �첽��ʽ�����豸Ӱ���ĵ�
 *
 * @param c �豸Ӱ�Ӷ���
 * @param buffer �豸Ӱ���ĵ�����
 * @param buffer_len �豸Ӱ���ĵ�������󳤶�
 * @param reported reported �ֶ��ϱ����ݣ��ɴ����������ݣ�
 * 1. NULL : ������Ҫ�ϱ� reported �ֶ�ʱ���� NULL ��
 * 2. TC_IOT_JSON_NULL : ����Ҫ��� reported �ֶ�����ʱ���� TC_IOT_JSON_NULL ��
 * 3. {"a":1,"b":"some string"} : ����Ҫ�����ϱ� reported ����ʱ������Ч�� json �ַ�����
 *
 * @param desired desired �ֶ��ϱ����ݣ��ɴ����������ݣ�
 * 1. NULL : ������Ҫ�ϱ� desired �ֶ�ʱ���� NULL ��
 * 2. TC_IOT_JSON_NULL : ����Ҫ��� desired �ֶ�����ʱ���� TC_IOT_JSON_NULL ��
 * 3. {"a":1,"b":"some string"} : ����Ҫ�����ϱ� desired ����ʱ������Ч�� json �ַ�����
 *
 * @param callback ������Ӧ���ݻص�����ѡ���� NULL ���ʾ��ָ����Ӧ�ص���δָ��
 * �ص�ʱ���������Ӧ������ shadow ��Ĭ�ϻص����� on_receive_msg ����
 * @param timeout_ms �������ȴ�ʱ�ӣ���ѡ����ָ�� callback ����ʱ����ָ���ûص����
 * �ȴ�ʱ�䡣
 * @param session_context ������� context����ѡ������͸��ʱ���ɴ�Ĭ�ϵ� NULL ��
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_shadow_update(tc_iot_shadow_client *c, char * buffer, int buffer_len,
        const char * reported, const char * desired,
        message_ack_handler callback, int timeout_ms, void * session_context);


/**
 * @brief tc_iot_shadow_delete ɾ���豸���ԡ�
 *
 * @param c �豸Ӱ�Ӷ���
 * @param buffer �豸Ӱ���ĵ�����
 * @param buffer_len �豸Ӱ���ĵ�������󳤶�
 * @param reported reported �ֶ��ϱ����ݣ��ɴ����������ݣ�
 * 1. NULL : ������Ҫ�ϱ� reported �ֶ�ʱ���� NULL ��
 * 2. TC_IOT_JSON_NULL : ����Ҫ��� reported �ֶ�����ʱ���� TC_IOT_JSON_NULL ��
 * 3. {"a":1,"b":"some string"} : ����Ҫ�����ϱ� reported ����ʱ������Ч�� json �ַ�����
 *
 * @param desired desired �ֶ��ϱ����ݣ��ɴ����������ݣ�
 * 1. NULL : ������Ҫ�ϱ� desired �ֶ�ʱ���� NULL ��
 * 2. TC_IOT_JSON_NULL : ����Ҫ��� desired �ֶ�����ʱ���� TC_IOT_JSON_NULL ��
 * 3. {"a":1,"b":"some string"} : ����Ҫ�����ϱ� desired ����ʱ������Ч�� json �ַ�����
 *
 * @param callback ������Ӧ���ݻص�����ѡ���� NULL ���ʾ��ָ����Ӧ�ص���δָ��
 * �ص�ʱ���������Ӧ������ shadow ��Ĭ�ϻص����� on_receive_msg ����
 * @param timeout_ms �������ȴ�ʱ�ӣ���ѡ����ָ�� callback ����ʱ����ָ���ûص����
 * �ȴ�ʱ�䡣
 * @param session_context ������� context����ѡ������͸��ʱ���ɴ�Ĭ�ϵ� NULL ��
 *
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_shadow_delete(tc_iot_shadow_client *c, char * buffer, int buffer_len,
        const char * reported, const char * desired,
        message_ack_handler callback, int timeout_ms, void * session_context);
int tc_iot_shadow_doc_pack_for_get_with_sid(char *buffer, int buffer_len,
                                    char * session_id, int session_id_len,
                                    tc_iot_shadow_client *c);
int tc_iot_shadow_doc_pack_for_update_with_sid(char *buffer, int buffer_len,
                                    char * session_id, int session_id_len,
                                    const char * reported, const char * desired,
                                    tc_iot_shadow_client *c) ;
int tc_iot_shadow_doc_pack_for_delete_with_sid(char *buffer, int buffer_len,
                                    char * session_id, int session_id_len,
                                    const char * reported, const char * desired,
                                    tc_iot_shadow_client *c) ;

int tc_iot_shadow_doc_pack_start(char *buffer, int buffer_len,
                                 char * session_id, int session_id_len,
                                 const char * method,
                                 tc_iot_shadow_client *c);

int tc_iot_shadow_doc_pack_format(char *buffer, int buffer_len,
        const char * reported,
        const char * desired);
int tc_iot_shadow_doc_pack_end(char *buffer, int buffer_len, tc_iot_shadow_client *c);

int tc_iot_shadow_update_state(tc_iot_shadow_client *c, char * buffer, int buffer_len,
        message_ack_handler callback, int timeout_ms, void * session_context,
         const char * state_name, int count, va_list p_args);

int tc_iot_shadow_update_firm_info(tc_iot_shadow_client *c, char * buffer, int buffer_len,
        message_ack_handler callback, int timeout_ms, void * session_context,
         va_list p_args);
tc_iot_shadow_session * tc_iot_find_empty_session(tc_iot_shadow_client *c);
void tc_iot_release_session(tc_iot_shadow_session * p_session);

void tc_iot_device_on_message_received(tc_iot_message_data* md);
void _device_on_message_received(tc_iot_message_data* md);
int _tc_iot_sync_shadow_property(tc_iot_shadow_client * p_shadow_client,
        int property_total, tc_iot_shadow_property_def * properties, bool reported,
        const char * doc_start, jsmntok_t * json_token, int tok_count);

int tc_iot_shadow_doc_parse(tc_iot_shadow_client * p_shadow_client,
        const char * payload, jsmntok_t * json_token, int token_count, char * field_buf, int field_buf_len);

int tc_iot_shadow_event_notify(tc_iot_shadow_client * p_shadow_client, tc_iot_event_e event, void * data, void * context);

int tc_iot_shadow_cmp_local(tc_iot_shadow_client * c, int property_id, void * src, void * dest);
int tc_iot_shadow_cmp_local_with_reported(tc_iot_shadow_client * c, int property_id);
int tc_iot_shadow_cmp_local_with_desired(tc_iot_shadow_client * c, int property_id);
void * tc_iot_shadow_save_to_cached(tc_iot_shadow_client * c, int property_id, const void * p_data, void * p_cache);
void * tc_iot_shadow_save_string_to_cached(tc_iot_shadow_client * c, int property_id, const void * p_data, int len, void * p_cache);

int tc_iot_shadow_report_property(tc_iot_shadow_client * c, int property_id, char * buffer, int buffer_len);
int tc_iot_shadow_check_and_report(tc_iot_shadow_client *c, char * buffer, int buffer_len,
        message_ack_handler callback, int timeout_ms, void * session_context, bool do_confirm);

tc_iot_shadow_property_def * tc_iot_shadow_get_property_def(tc_iot_shadow_client * p_shadow_client, int property_id);
const char * tc_iot_shadow_get_property_name(tc_iot_shadow_client * p_shadow_client, int property_id);
int tc_iot_shadow_get_property_type(tc_iot_shadow_client * p_shadow_client, int property_id);
int tc_iot_shadow_get_property_offset(tc_iot_shadow_client * p_shadow_client, int property_id);
int tc_iot_shadow_pending_session_count(tc_iot_shadow_client *c);

int tc_iot_report_device_data(tc_iot_shadow_client* p_shadow_client);
int tc_iot_confirm_devcie_data(tc_iot_shadow_client* p_shadow_client);

int tc_iot_server_init(tc_iot_shadow_client* p_shadow_client, tc_iot_shadow_config * p_client_config);
int tc_iot_server_loop(tc_iot_shadow_client* p_shadow_client, int yield_timeout);
int tc_iot_server_destroy(tc_iot_shadow_client* p_shadow_client);

#endif /* end of include guard */

