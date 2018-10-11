#ifndef TC_IOT_CONFIG_H
#define TC_IOT_CONFIG_H

#include "tc_iot_compile_flags.h"


/* buffer ���ȵı�������, ������̫��json̫����ʱ��, һЩbuffer������Ҫ����, �������ó� 2 , 4 ,8 ����ֵ */
#define BUF_MULTIPLE 			(1)

#define TC_IOT_SDK_VERSION     "2.6"

/* mqtt��Ϣ���ջ��������� */
#define TC_IOT_CLIENT_READ_BUF_SIZE (1024*BUF_MULTIPLE)
/* mqtt��Ϣ���ͻ��������� */
#define TC_IOT_CLIENT_SEND_BUF_SIZE (1024*BUF_MULTIPLE)

/* �����쳣��ʱ�������� */
/* ��С�ȴ�ʱ�� */
#define TC_IOT_MIN_RECONNECT_WAIT_INTERVAL 1000
/* ���ȴ�ʱ�� */
#define TC_IOT_MAX_RECONNECT_WAIT_INTERVAL 60000

/* ���յ� MQTT ��ͷ�Ժ󣬽���ʣ�೤�ȼ�ʣ���������ӳٵȴ�ʱ�� */
#define TC_IOT_MQTT_MAX_REMAIN_WAIT_MS     2000

/* �豸���Ƴ��� */
#define TC_IOT_MAX_DEVICE_NAME_LEN 20

/* �豸��Ϣ���Ƴ��� */
#define TC_IOT_MAX_FIRM_INFO_NAME_LEN 256

/* �豸��Ϣȡֵ���� */
#define TC_IOT_MAX_FIRM_INFO_VALUE_LEN 256

/* clientid��topic�Ķ��������ƣ� */
/* https://cloud.tencent.com/document/product/646/12657 */
/* client id ��󳤶� */
#define TC_IOT_MAX_CLIENT_ID_LEN 64
/* ����topic��󳤶� */
#define TC_IOT_MAX_MQTT_TOPIC_LEN 128

/*
 * �������ȣ�{"method":"get","passthrough":{"sid":"3ffe0001"},"metadata":false}
 * */
#define TC_IOT_GET_MSG_LEN   96

/*
 * �������ȣ�{"method":"update","passthrough":{"sid":"3ffe0001"},"state":{"reported":{}}}
 *    +
 * ���ݵ����Գ���*���ݵ���
 * */
#define TC_IOT_REPORT_UPDATE_MSG_LEN   (1024*BUF_MULTIPLE)

/**
 * �������ȣ�{"method":"delete","passthrough":{"sid":"40b20006"},"state":{"desired":{}}}
 *    +
 * ���ݵ����Գ���*���ݵ���
 * */
#define TC_IOT_UPDATE_DESIRED_MSG_LEN   (1024*BUF_MULTIPLE)

/**
 * �������ȣ�{"method":"report_firm","payload":{"sdk-ver":"2.5","firm-ver":"LINUXV1.0"}}
 *
 */

#define TC_IOT_REPORT_FIRM_MSG_LEN  128

/**
 * �������ȣ�{"method":"report_upgrade","passthrough":{"mid":1},"payload":{"ota_id":"ota-k7pz1fxo_0","ota_code":"1","ota_status":"OTA
 * command received","ota_message":"success"}}
 *
 */
#define TC_IOT_REPORT_UPGRADE_MSG_LEN  256

/* username��password������� */
/* https://cloud.tencent.com/document/product/646/12661 */
/* �û������� */
#define TC_IOT_MAX_USER_NAME_LEN 128
/* ���볤�� */
#define TC_IOT_MAX_PASSWORD_LEN 128
/* ǩ����Կ���� */
#define TC_IOT_MAX_SECRET_LEN 64
/* product id ���� */
#define TC_IOT_MAX_PRODUCT_ID_LEN 20
/* product key ���� */
#define TC_IOT_MAX_PRODUCT_KEY_LEN 20

/* TLS ���ֳ�ʱʱ�� */
#define TC_IOT_DEFAULT_TLS_HANSHAKE_TIMEOUT_MS  10000

// HTTPS ����ʱ���Ƿ��ϸ�������֤��
#define TC_IOT_HTTPS_CERT_STRICT_CHECK       0

// ���� �����豸 Active �ӿڱ�����
#define TC_IOT_HTTP_ACTIVE_REQUEST_FORM_LEN  256
// ���� �����豸 Active �ӿ��ܳ���
#define TC_IOT_HTTP_ACTIVE_REQUEST_LEN       1024
// ���� �����豸 Active �ӿ���Ӧ����
#define TC_IOT_HTTP_ACTIVE_RESPONSE_LEN      512  

#define TC_IOT_HTTP_OTA_REQUEST_LEN          1024

// ���� Token �ӿڱ�����
#define TC_IOT_HTTP_TOKEN_REQUEST_FORM_LEN  256
// ���� Token �ӿ���Ӧ����
#define TC_IOT_HTTP_TOKEN_RESPONSE_LEN      512  

// ���� Rpc �ӿڱ�����
#define TC_IOT_HTTP_MQAPI_REQUEST_FORM_LEN  (1024*BUF_MULTIPLE)
// ���� Rpc �ӿ� HTTP �������Ӧ��󳤶�
#define TC_IOT_HTTP_MQAPI_RESPONSE_LEN      (1024*BUF_MULTIPLE)


#define TC_IOT_MAX_JSON_TOKEN_COUNT         (120*BUF_MULTIPLE)

#define TC_IOT_TLS_ERROR_STR_LEN            64
#define TC_IOT_TLS_INFO_LEN                 64

#endif /* end of include guard */
