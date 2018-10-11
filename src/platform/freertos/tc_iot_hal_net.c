#ifdef __cplusplus
extern "C" {
#endif

#include "tc_iot_inc.h"

int tc_iot_hal_net_read(tc_iot_network_t* n, unsigned char* buffer,
                        int len, int timeout_ms) {
    portTickType xTicksToWait = timeout_ms / portTICK_RATE_MS; /* convert milliseconds to ticks */
    xTimeOutType xTimeOut;
    int recvLen = 0;
    int recv_timeout = timeout_ms;
    int rc = 0;

    struct timeval timeout;
    fd_set fdset;

    IF_NULL_RETURN(n, TC_IOT_NULL_POINTER);

    FD_ZERO(&fdset);
    FD_SET(n->net_context.fd, &fdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_ms * 1000;

    TC_IOT_LOG_TRACE("before read, len=%d, timeout_ms=%d", len, timeout_ms);
    vTaskSetTimeOutState(&xTimeOut); /* Record the time at which this function was entered. */
    if (select(n->net_context.fd + 1, &fdset, NULL, NULL, &timeout) > 0) {
        if (FD_ISSET(n->net_context.fd, &fdset)) {
            do {
                rc = recv(n->net_context.fd, buffer + recvLen, len - recvLen, MSG_DONTWAIT);
                if (rc > 0) {
                    recvLen += rc;
                    TC_IOT_LOG_TRACE("read success: recvLen=%d, rc=%d", recvLen, rc);
                } else if (rc < 0) {
                    recvLen = rc;
                    TC_IOT_LOG_TRACE("read failed: recvLen=%d, rc=%d", recvLen, rc);
                    break;
                } else if (rc == 0) {
                    TC_IOT_LOG_TRACE("peer closed: recvLen=%d, rc=%d", recvLen, rc);
                    if (recvLen > 0) {
                        return recvLen;
                    } else {
                        return TC_IOT_NET_READ_ERROR;
                    }
                }
                
            } while (recvLen < len && xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);
        }
    }


    if (recvLen == 0) {
        return TC_IOT_NET_NOTHING_READ;
    } else if (recvLen != len) {
        return TC_IOT_NET_READ_TIMEOUT;
    }
    TC_IOT_LOG_TRACE("recv result: recvLen=%d", recvLen);
    return recvLen;
}

int tc_iot_hal_net_write(tc_iot_network_t* n, const unsigned char* buffer,
                         int len, int timeout_ms) {
    portTickType xTicksToWait = timeout_ms / portTICK_RATE_MS; /* convert milliseconds to ticks */
    xTimeOutType xTimeOut;
    int sentLen = 0;
    int send_timeout = timeout_ms;
    int rc = 0;
    int readysock;

    struct timeval timeout;
    fd_set fdset;

    FD_ZERO(&fdset);
    FD_SET(n->net_context.fd, &fdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_ms * 1000;

    TC_IOT_LOG_TRACE("before send, len=%d, timeout_ms=%d, fd=%d", len, timeout_ms,n->net_context.fd);
    vTaskSetTimeOutState(&xTimeOut); /* Record the time at which this function was entered. */
    /* do { */
        /* TC_IOT_LOG_TRACE("selecting readysock"); */
        /* readysock = select(n->net_context.fd + 1, NULL, &fdset, NULL, &timeout); */
        /* if (readysock == 0) { */
            /* TC_IOT_LOG_TRACE("timeout no sock ready, but try send data."); */
            /* [> return TC_IOT_SEND_PACK_FAILED; <] */
        /* } else { */
            /* TC_IOT_LOG_TRACE("readysock = %d", readysock); */
        /* } */
    /* } while (readysock < 0); */
    if (FD_ISSET(n->net_context.fd, &fdset)) {
        do {
            TC_IOT_LOG_TRACE("send : sentLen=%d, len=%d, rc=%d", sentLen, len, rc);
            rc = send(n->net_context.fd, buffer + sentLen, len - sentLen, MSG_DONTWAIT);
            if (rc > 0) {
                sentLen += rc;
                TC_IOT_LOG_TRACE("sent success: sentLen=%d, rc=%d", sentLen, rc);
            } else if (rc < 0) {
                sentLen = rc;
                TC_IOT_LOG_TRACE("sent failed: sentLen=%d, rc=%d", sentLen, rc);
                break;
            } else {
                TC_IOT_LOG_TRACE("sent zero: sentLen=%d, rc=%d", sentLen, rc);
            }
        } while (sentLen < len && xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);
    }

    TC_IOT_LOG_TRACE("sent result: sentLen=%d", sentLen);
    return sentLen;
}

int tc_iot_hal_net_connect(tc_iot_network_t* n, const char* host,
                           uint16_t port) {
    struct sockaddr_in sAddr;
    int retVal = -1;
    struct hostent *ipAddress;

    if (host) {
        n->net_context.host = (char *)host;
    }

    if (port) {
        n->net_context.port = port;
    }

	TC_IOT_LOG_TRACE("tc_iot_hal_net_connect entry");

    if ((ipAddress = gethostbyname(n->net_context.host)) == 0)
    {
        TC_IOT_LOG_ERROR("getaddrinfo failed for host:%s", n->net_context.host);
        retVal = TC_IOT_NET_UNKNOWN_HOST;
        goto exit;
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = ((struct in_addr*)(ipAddress->h_addr))->s_addr;
    sAddr.sin_port = htons(n->net_context.port);

	TC_IOT_LOG_TRACE("ip[0x%08X],port[%d] [%d]",(u32_t)sAddr.sin_addr.s_addr, (u16_t)sAddr.sin_port, n->net_context.port);

    if ((n->net_context.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        retVal = TC_IOT_NET_SOCKET_FAILED;
        TC_IOT_LOG_ERROR("create socket failed for host:%s", n->net_context.host);
        goto exit;
    }

    if ((retVal = connect(n->net_context.fd, (struct sockaddr*)&sAddr, sizeof(sAddr))) < 0)
    {
        retVal = TC_IOT_NET_CONNECT_FAILED;
        TC_IOT_LOG_ERROR("connect failed for host:%s", n->net_context.host);
        close(n->net_context.fd);
        goto exit;
    }
	else
	{
		TC_IOT_LOG_TRACE("tc_iot_hal_net_connect success");
	}

    n->net_context.is_connected = 1;

exit:
    return retVal;
}

int tc_iot_hal_net_is_connected(tc_iot_network_t* network) {
    return network->net_context.is_connected;
}

int tc_iot_hal_net_disconnect(tc_iot_network_t* network) {
    close(network->net_context.fd);
    network->is_connected = 0;
    return TC_IOT_SUCCESS;
}

int tc_iot_hal_net_destroy(tc_iot_network_t* network) {
    return TC_IOT_SUCCESS;
}

int tc_iot_copy_net_context(tc_iot_net_context_t * net_context, tc_iot_net_context_init_t * init) {
    IF_NULL_RETURN(net_context, TC_IOT_NULL_POINTER);
    IF_NULL_RETURN(init, TC_IOT_NULL_POINTER);

    net_context->use_tls           = init->use_tls      ;
    net_context->host              = init->host         ;
    net_context->port              = init->port         ;
    net_context->fd                = init->fd           ;
    net_context->is_connected      = init->is_connected ;
    net_context->extra_context     = init->extra_context;

#ifdef ENABLE_TLS
    net_context->tls_config = init->tls_config;
#endif
    return TC_IOT_SUCCESS;
}

int tc_iot_hal_net_init(tc_iot_network_t* network,
                        tc_iot_net_context_init_t* net_context) {
    if (NULL == network) {
        return TC_IOT_NETWORK_PTR_NULL;
    }

    network->do_read = tc_iot_hal_net_read;
    network->do_write = tc_iot_hal_net_write;
    network->do_connect = tc_iot_hal_net_connect;
    network->do_disconnect = tc_iot_hal_net_disconnect;
    network->is_connected = tc_iot_hal_net_is_connected;
    network->do_destroy = tc_iot_hal_net_destroy;
    tc_iot_copy_net_context(&(network->net_context), net_context);

    return TC_IOT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
