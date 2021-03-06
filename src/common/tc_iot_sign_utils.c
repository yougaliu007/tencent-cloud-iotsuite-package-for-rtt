#include "tc_iot_inc.h"

int tc_iot_calc_sign(unsigned char * output, int output_len, const char * secret, const char * format, ...) {
    va_list ap;
    const unsigned char * pos  = (const unsigned char *)format;
    const unsigned char * prev = (const unsigned char *)format;
    char num_buf[20];
    char format_str[4];
    const char * var_str;

    tc_iot_hmac_sha256_t hmac;
    tc_iot_hmac_sha256_init(&hmac, (unsigned char *)secret, strlen(secret));
    TC_IOT_LOG_TRACE("secret=%s,format=%s",secret, format);

    if (output_len < TC_IOT_SHA256_DIGEST_SIZE) {
        return TC_IOT_BUFFER_OVERFLOW;
    }

    va_start(ap, format);
    
    while(*pos) {
        if ('%' == *pos) {
            pos++;
            if ((pos-1) > prev) {
                tc_iot_sha256_update(&(hmac.sha), prev, pos-1-prev);
                prev = pos;
            }
            switch(*pos) {
                case 'c':
                case 'd':
                case 'i':
                    tc_iot_hal_snprintf(format_str, sizeof(format_str), "%%%c", *pos);
                    tc_iot_hal_snprintf(num_buf, sizeof(num_buf), format_str, va_arg(ap, int));
                    tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)num_buf, strlen(num_buf));
                    break;
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    tc_iot_hal_snprintf(format_str, sizeof(format_str), "%%%c", *pos);
                    tc_iot_hal_snprintf(num_buf, sizeof(num_buf), format_str, va_arg(ap, unsigned int));
                    tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)num_buf, strlen(num_buf));
                    break;
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                    tc_iot_hal_snprintf(format_str, sizeof(format_str), "%%%c", *pos);
                    tc_iot_hal_snprintf(num_buf, sizeof(num_buf), format_str, va_arg(ap, double));
                    tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)num_buf, strlen(num_buf));
                    break;
                case 's':
                    var_str = va_arg(ap, const char *);
                    tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)var_str, strlen(var_str));
                    break;

                case 'l':
                    pos++;
                    switch(*pos) {
                        case 'd':
                        case 'i':
                            tc_iot_hal_snprintf(format_str, sizeof(format_str), "%%l%c", *pos);
                            tc_iot_hal_snprintf(num_buf, sizeof(num_buf), format_str, va_arg(ap, long int));
                            tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)num_buf, strlen(num_buf));
                            break;
                        case 'u':
                        case 'o':
                        case 'x':
                        case 'X':
                            tc_iot_hal_snprintf(format_str, sizeof(format_str), "%%l%c", *pos);
                            tc_iot_hal_snprintf(num_buf, sizeof(num_buf), format_str, va_arg(ap, unsigned long int));
                            tc_iot_sha256_update(&(hmac.sha), (const unsigned char *)num_buf, strlen(num_buf));
                            break;
                        default:
                            TC_IOT_LOG_ERROR("unkown *pos=%c", *pos);
                            va_end(ap);
                            return TC_IOT_INVALID_PARAMETER;
                    }
                    break;
                case '%':
                    pos++;
                    continue;
                default:
                    TC_IOT_LOG_ERROR("unkown *pos=%c", *pos);
                    va_end(ap);
                    return TC_IOT_INVALID_PARAMETER;
            }
            pos++;
            prev = pos;
        } else {
            pos++;
        }
    }

    if ((pos-1) > prev) {
        tc_iot_sha256_update(&(hmac.sha), prev, pos-1-prev);
    }

    tc_iot_hmac_sha256_finish(&hmac, NULL, 0);

    memcpy(output, hmac.digest, TC_IOT_SHA256_DIGEST_SIZE);

    va_end(ap);

    return TC_IOT_SHA256_DIGEST_SIZE;    
}

int tc_iot_calc_active_device_sign(char* sign_out, int max_sign_len, 
                            const char* product_secret,
                            const char* device_name,  
                            const char* product_id,
                            long nonce, 
                            long timestamp    ) {
    unsigned char sha256_digest[TC_IOT_SHA256_DIGEST_SIZE];
    int ret;
    char b64_buf[TC_IOT_BASE64_ENCODE_OUT_LEN(TC_IOT_SHA256_DIGEST_SIZE)];
    int url_ret;

    IF_NULL_RETURN(sign_out, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(product_secret, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(device_name, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(product_id, TC_IOT_NULL_POINTER);
    IF_EQUAL_RETURN(max_sign_len, 0, TC_IOT_INVALID_PARAMETER);

    ret = tc_iot_calc_sign(
        sha256_digest, sizeof(sha256_digest),
        product_secret,
        "deviceName=%s&nonce=%d&productId=%s&timestamp=%d",
        device_name, (int)nonce,
        product_id, (int)timestamp);

    ret = tc_iot_base64_encode((unsigned char *)sha256_digest, sizeof(sha256_digest), b64_buf,
                               sizeof(b64_buf));

    tc_iot_mem_usage_log("b64_buf", sizeof(b64_buf), ret);

    url_ret = tc_iot_url_encode(b64_buf, ret, sign_out, max_sign_len);
    
    /* TC_IOT_LOG_DEBUG(" tc_iot_url_encoded sign\n %.*s\n, url_ret=%d", url_ret, sign_out, url_ret);  */
    if (url_ret < max_sign_len) {
        sign_out[url_ret] = '\0';
    }
    TC_IOT_LOG_DEBUG(" tc_iot_calc_active_device_sign deviceName=%s&nonce=%d&productId=%s&timestamp=%d sign:%s", 
            device_name, (int)nonce,product_id, (int)timestamp , sign_out);
    return url_ret;
}

int tc_iot_calc_auth_sign(char* sign_out, int max_sign_len, const char* secret, const char* client_id, const char* device_name,
                          long expire, long nonce,
                          const char* product_id,
                          long timestamp) {
    unsigned char sha256_digest[TC_IOT_SHA256_DIGEST_SIZE];
    int ret;
    char b64_buf[TC_IOT_BASE64_ENCODE_OUT_LEN(TC_IOT_SHA256_DIGEST_SIZE)];
    int url_ret;

    IF_NULL_RETURN(sign_out, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(secret, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(client_id, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(device_name, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(product_id, TC_IOT_NULL_POINTER);
    IF_EQUAL_RETURN(max_sign_len, 0, TC_IOT_INVALID_PARAMETER);

    ret = tc_iot_calc_sign(
        sha256_digest, sizeof(sha256_digest),
        secret,
        "clientId=%s&deviceName=%s&expire=%d&nonce=%d&productId=%s&timestamp=%d",
        client_id, device_name, (int)expire, (int)nonce,
        product_id, (int)timestamp);

    ret = tc_iot_base64_encode((unsigned char *)sha256_digest, sizeof(sha256_digest), b64_buf,
                               sizeof(b64_buf));
    if (ret < sizeof(b64_buf) && ret > 0) {
       b64_buf[ret] = '\0'; 
       tc_iot_mem_usage_log("b64_buf", sizeof(b64_buf), ret);
    }

    TC_IOT_LOG_TRACE("tc_iot_calc_auth_sign source clientId=%s&deviceName=%s&expire=%d&nonce=%d&productId=%s&timestamp=%d sec %s sig %s\n", 
            client_id, device_name, (int)expire, (int)nonce,product_id, (int)timestamp,
            secret, b64_buf);

    url_ret = tc_iot_url_encode(b64_buf, ret, sign_out, max_sign_len);
    if (url_ret < max_sign_len) {
        sign_out[url_ret] = '\0';
    }

    return url_ret;
}

int tc_iot_calc_mqtt_dynamic_sign(char* sign_out, int max_sign_len, 
        const char* secret, const char* client_id, const char* device_name,
        long nonce,
        const char* product_id,
        long timestamp) {

    unsigned char sha256_digest[TC_IOT_SHA256_DIGEST_SIZE];
    int ret;

    IF_NULL_RETURN(sign_out, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(secret, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(client_id, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(device_name, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(product_id, TC_IOT_NULL_POINTER);
    IF_EQUAL_RETURN(max_sign_len, 0, TC_IOT_INVALID_PARAMETER);

    ret = tc_iot_calc_sign(
        sha256_digest, sizeof(sha256_digest),
        secret,
        "clientId=%s&deviceName=%s&nonce=%d&productId=%s&timestamp=%d",
        client_id, device_name, nonce,
        product_id, timestamp);

    ret = tc_iot_base64_encode((unsigned char *)sha256_digest, sizeof(sha256_digest), sign_out,
                               max_sign_len);
    if (ret < max_sign_len && ret > 0) {
       sign_out[ret] = '\0'; 
    } else {
        TC_IOT_LOG_ERROR("ret=%d", ret);
        return ret;
    }

    TC_IOT_LOG_TRACE("tc_iot_calc_auth_sign source clientId=%s&deviceName=%s&nonce=%d&productId=%s&timestamp=%d sec %s sig %s\n", 
            client_id, device_name, (int)nonce,product_id, (int)timestamp,
            secret, sign_out);

    /* char b64_buf[TC_IOT_BASE64_ENCODE_OUT_LEN(TC_IOT_SHA256_DIGEST_SIZE)]; */
    /* int url_ret = 0; */
    /* strcpy(b64_buf, sign_out); */
    /* url_ret = tc_iot_url_encode(b64_buf, ret, sign_out, max_sign_len); */
    /* if (url_ret < max_sign_len) { */
    /*     sign_out[url_ret] = '\0'; */
    /* } */

    /* return url_ret; */

    return TC_IOT_SUCCESS;
}
