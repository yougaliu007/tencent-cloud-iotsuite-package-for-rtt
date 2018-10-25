#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
//#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <rtthread.h>


#include "tc_iot_inc.h"


int tc_iot_hal_net_read(tc_iot_network_t* n, unsigned char* buffer,
                        int len, int timeout_ms) {

  	int recvLen = 0;
	int rc = 0;
	tc_iot_timer timer;

    struct timeval timeout;
    fd_set fdset;

    IF_NULL_RETURN(n, TC_IOT_NULL_POINTER);
    TC_IOT_LOG_TRACE("before read, len=%d, timeout_ms=%d", len, timeout_ms);
	
    tc_iot_hal_timer_countdown_ms(&timer, timeout_ms);
	do {
			rc = recv(n->net_context.fd, buffer + recvLen, len - recvLen, MSG_DONTWAIT);
			if(rc < 0) 
			{
				TC_IOT_LOG_TRACE("read failed: recvLen=%d, rc=%d", recvLen, rc);
				recvLen = rc;
				break;			  		
			}
			else
			{
				recvLen += rc;
			    TC_IOT_LOG_TRACE("read success: recvLen=%d, rc=%d", recvLen, rc);

				FD_ZERO(&fdset);
				FD_SET(n->net_context.fd, &fdset);

			    timeout.tv_sec = 2;
			    timeout.tv_usec = timeout_ms * 0;
				select(n->net_context.fd + 1, &fdset, NULL, NULL, &timeout);
			}          
	} while (recvLen < len && (!tc_iot_hal_timer_is_expired(&timer)));
	
    if (recvLen == 0) {
        return TC_IOT_NET_NOTHING_READ;
    }
//	else if (recvLen != len) {
//        return TC_IOT_NET_READ_TIMEOUT;
//    }
    TC_IOT_LOG_TRACE("recv result: recvLen=%d", recvLen);
    return recvLen;
}

int tc_iot_hal_net_write(tc_iot_network_t* n, const unsigned char* buffer,
                         int len, int timeout_ms) {
    int sentLen = 0;
    int rc = 0;
  	tc_iot_timer timer;
	struct timeval timeout;
    fd_set fdset;

    FD_ZERO(&fdset);
    FD_SET(n->net_context.fd, &fdset);

	timeout.tv_sec = 2;
    timeout.tv_usec = 0;

	 setsockopt(n->net_context.fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	 rc = send(n->net_context.fd, buffer, len, 0);
	 if (rc == len)
	 {
		sentLen = rc;
	 }
	 else
	 {
		 sentLen = -1;
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
    //sAddr.sin_family = AF_AT;
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
        closesocket(n->net_context.fd);
		n->net_context.fd = -1;
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
	if (network->net_context.fd >= 0)
    {
        closesocket(network->net_context.fd);
        network->net_context.fd = -1;
    }
    network->is_connected = 0;
    return TC_IOT_SUCCESS;
}

int tc_iot_hal_net_destroy(tc_iot_network_t* network) {
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
