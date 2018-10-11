#include "tc_iot_inc.h"


/**
 * @brief _tc_iot_get_message_ack_callback shadow_get �ص�����
 *
 * @param ack_status �ص�״̬����ʶ��Ϣ�Ƿ������յ���Ӧ�������Ѿ���ʱ�ȡ�
 * @param md �ص�״̬Ϊ TC_IOT_ACK_SUCCESS ʱ����������Ӱ������������Ӧ��Ϣ��
 * @param session_context �ص� context��
 */
void _tc_iot_get_message_ack_callback(tc_iot_command_ack_status_e ack_status, tc_iot_message_data * md , void * session_context) {

    /* tc_iot_mqtt_message* message = NULL; */

    if (ack_status != TC_IOT_ACK_SUCCESS) {
        if (ack_status == TC_IOT_ACK_TIMEOUT) {
            TC_IOT_LOG_ERROR("request timeout");
        }
        return;
    }

    /* message = md->message; */
    tc_iot_device_on_message_received(md);
}

/**
 * @brief _tc_iot_report_message_ack_callback shadow_update �ϱ���Ϣ�ص�
 *
 * @param ack_status �ص�״̬����ʶ��Ϣ�Ƿ������յ���Ӧ�������Ѿ���ʱ�ȡ�
 * @param md �ص�״̬Ϊ TC_IOT_ACK_SUCCESS ʱ����������Ӱ������������Ӧ��Ϣ��
 * @param session_context �ص� context��
 */
void _tc_iot_report_message_ack_callback(tc_iot_command_ack_status_e ack_status,
        tc_iot_message_data * md , void * session_context) {
    tc_iot_mqtt_message* message = NULL;

    if (ack_status != TC_IOT_ACK_SUCCESS) {
        if (ack_status == TC_IOT_ACK_TIMEOUT) {
            TC_IOT_LOG_ERROR("request timeout");
        } else {
            TC_IOT_LOG_ERROR("request return ack_status=%d", ack_status);
        }
        return;
    }

    message = md->message;
    TC_IOT_LOG_TRACE("[s->c] %s", (char*)message->payload);
}

/**
 * @brief _tc_iot_update_firm_message_ack_callback shadow_update �ϱ���Ϣ�ص�
 *
 * @param ack_status �ص�״̬����ʶ��Ϣ�Ƿ������յ���Ӧ�������Ѿ���ʱ�ȡ�
 * @param md �ص�״̬Ϊ TC_IOT_ACK_SUCCESS ʱ����������Ӱ������������Ӧ��Ϣ��
 * @param session_context �ص� context��
 */
void _tc_iot_update_firm_message_ack_callback(tc_iot_command_ack_status_e ack_status,
        tc_iot_message_data * md , void * session_context) {
    tc_iot_mqtt_message* message = NULL;

    if (ack_status != TC_IOT_ACK_SUCCESS) {
        if (ack_status == TC_IOT_ACK_TIMEOUT) {
            TC_IOT_LOG_ERROR("request timeout");
        } else {
            TC_IOT_LOG_ERROR("request return ack_status=%d", ack_status);
        }
        return;
    }

    message = md->message;
    TC_IOT_LOG_TRACE("[s->c] %s", (char*)message->payload);
}


/**
 * @brief _tc_iot_sync_shadow_property ���ݷ�����·���Ӱ�����ݣ�ͬ���������豸״̬���ݣ�����
 * ���ϱ���
 *
 * @param property_count �豸״̬�ֶ���
 * @param properties �豸״̬����
 * @param reported ͬ���������ͣ�Ϊ true ��ʾͬ�� reported ���ݣ�Ϊ
 * false ��ʾͬ�� current ���ݡ�
 * @param doc_start reported or desired ������ʼλ�á�
 * @param json_token json token ������ʼλ��
 * @param tok_count ��Ч json token ����
 *
 * @return
 */
int _tc_iot_sync_shadow_property(tc_iot_shadow_client * p_shadow_client,
        int property_total, tc_iot_shadow_property_def * properties, bool reported,
        const char * doc_start, jsmntok_t * json_token, int tok_count) {
    int i,j;
    jsmntok_t  * key_tok = NULL;
    jsmntok_t  * val_tok = NULL;
    char field_buf[TC_IOT_MAX_FIELD_LEN];
    tc_iot_shadow_number new_number = 0;
    tc_iot_shadow_bool new_bool = 0;
    tc_iot_shadow_enum new_enum = 0;
    tc_iot_shadow_int  new_int = 0;
    int  key_len = 0, val_len = 0;
    const char * key_start;
    const char * val_start;
    int ret = 0;
    tc_iot_shadow_property_def * p_prop = NULL;
    void  * ptr = NULL;

    if (!properties) {
        TC_IOT_LOG_ERROR("properties is null");
        return TC_IOT_NULL_POINTER;
    }

    if (!doc_start) {
        TC_IOT_LOG_ERROR("doc_start is null");
        return TC_IOT_NULL_POINTER;
    }

    if (!json_token) {
        TC_IOT_LOG_ERROR("json_token is null");
        return TC_IOT_NULL_POINTER;
    }

    if (!tok_count) {
        TC_IOT_LOG_ERROR("tok_count is invalid");
        return TC_IOT_INVALID_PARAMETER;
    }

    memset(field_buf, 0, sizeof(field_buf));

    for (i = 0; i < tok_count/2; i++) {
        /* λ�� 0 ��object��������Ҫ��λ�� 1 ��ʼȡ����*/
        /* 2*i+1 Ϊ key �ֶΣ�2*i + 2 Ϊ value �ֶ�*/
        key_tok = &(json_token[2*i + 1]);
        key_start = doc_start + key_tok->start;
        key_len = key_tok->end - key_tok->start;

        val_tok = &(json_token[2*i + 2]);
        val_start = doc_start + val_tok->start;
        val_len = val_tok->end - val_tok->start;
        for(j = 0; j < property_total; j++) {
            p_prop = &properties[j];
            if (key_len == strlen(p_prop->name) && strncmp(p_prop->name, key_start, key_len) == 0)  {
                if (val_len < sizeof(field_buf)) {
                    strncpy(field_buf, val_start, val_len);
                    field_buf[val_len] = '\0';
                }

                if ((val_len > 0) && strncmp(TC_IOT_JSON_NULL, field_buf, val_len) == 0) {
                    TC_IOT_LOG_WARN("%s recevied null value.", p_prop->name);
                    continue;
                }

                if (p_prop->type == TC_IOT_SHADOW_TYPE_BOOL) {
                    new_bool = (field_buf[0] != 'f') && (field_buf[0] != '0');
                    ptr = &new_bool;
                    TC_IOT_LOG_TRACE("state change:[%s=%s]", p_prop->name, 
                            (*(tc_iot_shadow_bool *) ptr)? TC_IOT_SHADOW_JSON_TRUE:TC_IOT_SHADOW_JSON_FALSE);
                } else if (p_prop->type == TC_IOT_SHADOW_TYPE_NUMBER) {
                    new_number = atof(field_buf);
                    ptr = &new_number;
                    TC_IOT_LOG_TRACE("state change:[%s=%f]", p_prop->name, (*(tc_iot_shadow_number *) ptr));
                } else if (p_prop->type == TC_IOT_SHADOW_TYPE_ENUM) {
                    new_enum = atoi(field_buf);
                    ptr = &new_enum;
                    TC_IOT_LOG_TRACE("state change:[%s=%d]", p_prop->name, (*(tc_iot_shadow_enum *) ptr));
                } else if (p_prop->type == TC_IOT_SHADOW_TYPE_INT) {
                    new_int = atoi(field_buf);
                    ptr = &new_int;
                    TC_IOT_LOG_TRACE("state change:[%s=%d]", p_prop->name, (*(tc_iot_shadow_int *) ptr));
                } else if (p_prop->type == TC_IOT_SHADOW_TYPE_STRING) {
                    ptr = tc_iot_shadow_save_string_to_cached(p_shadow_client, p_prop->id, val_start, val_len, 
                            p_shadow_client->p_shadow_config->p_desired_device_data);
                    if (ptr) {
                        TC_IOT_LOG_TRACE("state change:[%s=%s]", p_prop->name, (const char *)ptr);
                    }
                } else {
                    TC_IOT_LOG_ERROR("%s type=%d invalid.", p_prop->name, p_prop->type);
                    continue;
                }

                if (p_prop->type != TC_IOT_SHADOW_TYPE_STRING) {
                    tc_iot_shadow_save_to_cached(p_shadow_client, p_prop->id, ptr, p_shadow_client->p_shadow_config->p_desired_device_data);
                }
                TC_IOT_BIT_SET(p_shadow_client->desired_bits, p_prop->id);
                ret = tc_iot_shadow_event_notify(p_shadow_client, TC_IOT_SHADOW_EVENT_SERVER_CONTROL, ptr, p_prop);
                if (ret == TC_IOT_SUCCESS) {
                    tc_iot_shadow_save_to_cached(p_shadow_client, p_prop->id, ptr, p_shadow_client->p_shadow_config->p_current_device_data);
                }
            }
        }
    }
    return tc_iot_confirm_devcie_data(p_shadow_client);
}



/**
 * @brief tc_iot_device_on_message_received ���ݻص������� shadow_get ��ȡ����״̬����
 * ��Ӱ�ӷ������͵����¿���ָ�����ݡ�
 *
 * @param md Ӱ��������Ϣ
 */
void tc_iot_device_on_message_received(tc_iot_message_data* md) {
    jsmntok_t  json_token[TC_IOT_MAX_JSON_TOKEN_COUNT];
    char field_buf[TC_IOT_MAX_FIELD_LEN];
    int field_index = 0;
    int ret = 0;
    tc_iot_shadow_client * p_shadow_client = TC_IOT_CONTAINER_OF(md->mqtt_client, tc_iot_shadow_client, mqtt_client);

    memset(field_buf, 0, sizeof(field_buf));

    tc_iot_mqtt_message* message = md->message;
    TC_IOT_LOG_TRACE("[s->c] %s", (char*)message->payload);

    /* ��Ч�Լ�� */
    ret = tc_iot_json_parse(message->payload, message->payloadlen, json_token, TC_IOT_ARRAY_LENGTH(json_token));
    if (ret <= 0) {
        return ;
    }

    tc_iot_mem_usage_log("json_token[TC_IOT_MAX_JSON_TOKEN_COUNT]", sizeof(json_token), sizeof(json_token[0])*ret);

    field_index = tc_iot_json_find_token((char*)message->payload, json_token, ret, "method", field_buf, sizeof(field_buf));
    if (field_index <= 0 ) {
        TC_IOT_LOG_ERROR("field method not found in JSON: %s", (char*)message->payload);
        return ;
    }

    if (strncmp(TC_IOT_MQTT_METHOD_CONTROL, field_buf, strlen(field_buf)) == 0) {
        TC_IOT_LOG_TRACE("Control data receved.");
    } else if (strncmp(TC_IOT_MQTT_METHOD_REPLY, field_buf, strlen(field_buf)) == 0) {
        TC_IOT_LOG_TRACE("Reply pack recevied.");
    }

    tc_iot_shadow_doc_parse(p_shadow_client, (const char *)message->payload, json_token, ret, field_buf, sizeof(field_buf));
}

int tc_iot_shadow_doc_parse(tc_iot_shadow_client * p_shadow_client,
        const char * payload, jsmntok_t * json_token, int token_count, char * field_buf, int field_buf_len) {

    const char * reported_start = NULL;
    int reported_len = 0;
    const char * desired_start = NULL;
    int desired_len = 0;
    int field_index = 0;
    int ret = 0;

    /* ��� reported �ֶ��Ƿ���� */
    field_index = tc_iot_json_find_token(payload, json_token, token_count, "payload.state.reported", NULL, 0);
    if (field_index <= 0 ) {
        /* TC_IOT_LOG_TRACE("payload.state.reported not found"); */
    } else {
        reported_start = payload + json_token[field_index].start;
        reported_len = json_token[field_index].end - json_token[field_index].start;
        TC_IOT_LOG_TRACE("payload.state.reported found:%s", tc_iot_log_summary_string(reported_start, reported_len));
    }

    /* ��� desired �ֶ��Ƿ���� */
    field_index = tc_iot_json_find_token(payload, json_token, token_count, "payload.state.desired", NULL, 0);
    if (field_index <= 0 ) {
        /* TC_IOT_LOG_TRACE("payload.state.desired not found"); */
    } else {
        desired_start = payload + json_token[field_index].start;
        desired_len = json_token[field_index].end - json_token[field_index].start;
        TC_IOT_LOG_TRACE("payload.state.desired found:%s", tc_iot_log_summary_string(desired_start, desired_len));
    }

    /* ���ݿ���̨���� APP �˵�ָ��趨�豸״̬ */
    if (desired_start) {
        ret = tc_iot_json_parse(desired_start, desired_len, json_token, token_count);
        if (ret <= 0) {
            return TC_IOT_FAILURE;
        }

        TC_IOT_LOG_TRACE("---synchronizing desired status---");
        ret = _tc_iot_sync_shadow_property(
                p_shadow_client,
                p_shadow_client->p_shadow_config->property_total,
                p_shadow_client->p_shadow_config->properties, false,
                desired_start, json_token, ret);
        if (ret == TC_IOT_SUCCESS)  {
            TC_IOT_LOG_TRACE("---synchronizing desired status success---");
        } else {
            TC_IOT_LOG_ERROR("---synchronizing desired status failed, ret=%d---", ret);
        }
    }
    return TC_IOT_SUCCESS;
}

int tc_iot_report_device_data(tc_iot_shadow_client* p_shadow_client) {
    char buffer[TC_IOT_REPORT_UPDATE_MSG_LEN];
    int buffer_len = sizeof(buffer);
    int ret = 0;

    ret = tc_iot_shadow_check_and_report(p_shadow_client, buffer, buffer_len,
            _tc_iot_report_message_ack_callback, p_shadow_client->mqtt_client.command_timeout_ms, NULL, false);
    if (TC_IOT_BUFFER_OVERFLOW == ret) {
        TC_IOT_LOG_ERROR("buffer overflow, please check TC_IOT_REPORT_UPDATE_MSG_LEN");
    } else if (TC_IOT_REPORT_SKIPPED_FOR_NO_CHANGE == ret) {
        ret = TC_IOT_SUCCESS;
    }
    tc_iot_mem_usage_log("buffer[TC_IOT_REPORT_UPDATE_MSG_LEN]", sizeof(buffer), strlen(buffer));
    return ret;
}

int tc_iot_confirm_devcie_data(tc_iot_shadow_client* p_shadow_client) {
    char buffer[TC_IOT_UPDATE_DESIRED_MSG_LEN];
    int buffer_len = sizeof(buffer);
    int ret = 0;

    ret = tc_iot_shadow_check_and_report(p_shadow_client, buffer, buffer_len,
            _tc_iot_report_message_ack_callback, p_shadow_client->mqtt_client.command_timeout_ms, NULL, false);
    tc_iot_mem_usage_log("buffer[TC_IOT_UPDATE_DESIRED_MSG_LEN]", sizeof(buffer), strlen(buffer));
    if ((ret != TC_IOT_SUCCESS) && (ret != TC_IOT_REPORT_SKIPPED_FOR_NO_CHANGE)) {
        if (TC_IOT_BUFFER_OVERFLOW == ret) {
            TC_IOT_LOG_ERROR("buffer overflow, please check TC_IOT_UPDATE_DESIRED_MSG_LEN");
        }
        return ret;
    }
    ret = tc_iot_shadow_check_and_report(p_shadow_client, buffer, buffer_len,
            _tc_iot_report_message_ack_callback, p_shadow_client->mqtt_client.command_timeout_ms, NULL, true);
    if (TC_IOT_BUFFER_OVERFLOW == ret) {
        TC_IOT_LOG_ERROR("buffer overflow, please check TC_IOT_UPDATE_DESIRED_MSG_LEN");
    } else if (TC_IOT_REPORT_SKIPPED_FOR_NO_CHANGE == ret) {
        ret = TC_IOT_SUCCESS;
    }
    return ret;
}

int tc_iot_shadow_event_notify(tc_iot_shadow_client * p_shadow_client, tc_iot_event_e event, void * data, void * context) {
    tc_iot_event_message event_msg;

    if (p_shadow_client
            && p_shadow_client->p_shadow_config
            && p_shadow_client->p_shadow_config->event_notify) {
        event_msg.event = event;
        event_msg.data = data;
        return p_shadow_client->p_shadow_config->event_notify(&event_msg, p_shadow_client, context);
    } else {
        TC_IOT_LOG_TRACE("no event_notify callback, skip calling event_notify.");
        return TC_IOT_SUCCESS;
    }
}

/*
���ݳ�ʼ���ã���ʼ�� client��
1. ���� MQ ��������
2. ���� shadow/get/${product_id}/${device_name} Topic����������������Ϣ��
3. ֪ͨ�豸Ӧ���ϱ��豸������Ϣ��
4. ���� {"method":"get"} ���󣬻�ȡ�����ڼ��ۻ��ģ�����������ָ�
5. �ϱ������豸״̬��
*/
int tc_iot_server_init(tc_iot_shadow_client* p_shadow_client, tc_iot_shadow_config * p_client_config) {
    int ret = 0;
    char buffer[TC_IOT_GET_MSG_LEN];
    int buffer_len = sizeof(buffer);

    /* ��ʼ�� shadow client */
    TC_IOT_LOG_INFO("constructing mqtt shadow client.");
    ret = tc_iot_shadow_construct(p_shadow_client, p_client_config);
    if (ret != TC_IOT_SUCCESS) {
        return ret;
    }

    TC_IOT_LOG_INFO("construct mqtt shadow client success.");
	
#if 1	
    TC_IOT_LOG_INFO("yield waiting for server push.");
    /* ִ�� yield ��ȡӰ�ӷ����ǰ��ָ����Ϣ��������ʷ״̬�� */
    tc_iot_shadow_yield(p_shadow_client, 200);
    TC_IOT_LOG_INFO("yield waiting for server finished.");

    /* ͨ��get����������ȡ�����Ӱ���豸״̬���Ա��豸��ͬ������������״̬*/
    ret = tc_iot_shadow_get(p_shadow_client, buffer, buffer_len, _tc_iot_get_message_ack_callback,
            p_shadow_client->mqtt_client.command_timeout_ms, p_shadow_client);
    TC_IOT_LOG_TRACE("[c->s] shadow_get%s", buffer);
    tc_iot_mem_usage_log("buffer[TC_IOT_GET_MSG_LEN]", sizeof(buffer), strlen(buffer));

    tc_iot_report_device_data(p_shadow_client);
#endif	

    return ret;
}

tc_iot_shadow_property_def * tc_iot_shadow_get_property_def(tc_iot_shadow_client * p_shadow_client, int property_id) {
    tc_iot_shadow_property_def * p_prop = NULL;

    if (NULL == p_shadow_client) {
        TC_IOT_LOG_ERROR("p_shadow_client = null");
        return NULL;
    }

    if (property_id < p_shadow_client->p_shadow_config->property_total) {
        p_prop = &p_shadow_client->p_shadow_config->properties[property_id];
        return p_prop;
    } else {
        TC_IOT_LOG_ERROR("invalid property_id=%d, property_total=%d", property_id, p_shadow_client->p_shadow_config->property_total);
        return NULL;
    }
}

const char * tc_iot_shadow_get_property_name(tc_iot_shadow_client * p_shadow_client, int property_id) {
    tc_iot_shadow_property_def * p_prop = NULL;

    if (NULL == p_shadow_client) {
        TC_IOT_LOG_ERROR("p_shadow_client = null");
        return NULL;
    }

    p_prop = tc_iot_shadow_get_property_def(p_shadow_client, property_id);
    if (p_prop) {
        return p_prop->name;
    } else {
        return "not_found";
    }
}

int tc_iot_shadow_get_property_type(tc_iot_shadow_client * p_shadow_client, int property_id) {
    tc_iot_shadow_property_def * p_prop = NULL;

    if (NULL == p_shadow_client) {
        TC_IOT_LOG_ERROR("p_shadow_client = null");
        return TC_IOT_SHADOW_TYPE_INVALID;
    }

    p_prop = tc_iot_shadow_get_property_def(p_shadow_client, property_id);
    if (p_prop) {
        return p_prop->type;
    } else {
        return TC_IOT_SHADOW_TYPE_INVALID;
    }
}

int tc_iot_shadow_get_property_offset(tc_iot_shadow_client * p_shadow_client, int property_id) {
    tc_iot_shadow_property_def * p_prop = NULL;

    if (NULL == p_shadow_client) {
        TC_IOT_LOG_ERROR("p_shadow_client = null");
        return 0;
    }

    p_prop = tc_iot_shadow_get_property_def(p_shadow_client, property_id);
    if (p_prop) {
        return p_prop->offset;
    } else {
        return 0;
    }
}


/*
������ѭ����
1. ���շ������Ӧ��Ϣ�Ϳ���ָ��ȣ�
2. ��� session ��������ĳ�ʱ�������ʱ��ص�֪ͨӦ�ã�
3. ���keep alive �������ʱ����keep alive ping ��������ʧ�ܣ������������
 * */
int tc_iot_server_loop(tc_iot_shadow_client* p_shadow_client, int yield_timeout) {
    return tc_iot_shadow_yield(p_shadow_client, yield_timeout);
}

/*
�����˳����ͷŹر����ӡ������Դ�ȡ�
 * */
int tc_iot_server_destroy(tc_iot_shadow_client* p_shadow_client) {

    TC_IOT_LOG_TRACE("Stopping");
    tc_iot_shadow_destroy(p_shadow_client);
    TC_IOT_LOG_TRACE("Exit success.");
    return 0;
}
