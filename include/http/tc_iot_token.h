#ifndef TC_IOT_TOKEN_H
#define TC_IOT_TOKEN_H

#define TC_IOT_TOKEN_MAX_EXPIRE_SECOND    (86400*60)
#define TC_IOT_TOKEN_DEFAULT_EXPIRE_SECOND (86400*7)

/* �����ϰ汾 */
#define http_refresh_auth_token(a,r,t,n,d) tc_iot_refresh_auth_token(t,n,d, TC_IOT_TOKEN_DEFAULT_EXPIRE_SECOND)
#define http_refresh_auth_token_with_expire(a,r,t,n,d,x) tc_iot_refresh_auth_token(t,n,d,x)
#define http_get_device_secret tc_iot_get_device_secret

/**
 * @brief tc_iot_get_device_secret ���� MQTT ���ʷ���˵� device secrect *
 * @param api_url ����ӿڵĵ�ַ
 * @param root_ca_path �������֤���ַ��һ���� NULL ����
 * @param timestamp ʱ���
 * @param nonce ���ֵ
 * @param p_device_info �豸��Ϣ������ɹ��󣬻��Զ����¸ö���� device_secret ��Ա����
 * @param expire token ��Ч�ڵ�λΪs�������Чʱ��Ϊ2���� = 60*86400 s��
 *
 * @return ���������
 * @see tc_iot_sys_code_e
 */
int tc_iot_get_device_secret(const char* api_url, char* root_ca_path, long timestamp, long nonce,
        tc_iot_device_info* p_device_info);

#endif /* end of include guard */
