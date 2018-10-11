#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <rtthread.h>


#include "tc_iot_inc.h"


//==============UDP API==================//
void *tc_iot_hal_udp_create(char *host, unsigned short port)
{
#define NETWORK_ADDR_LEN    (16)

    int                     rc = -1;
    long                    socket_id = -1;
    char                    port_ptr[6] = {0};
    struct addrinfo         hints;
    char                    addr[NETWORK_ADDR_LEN] = {0};
    struct addrinfo        *res, *ainfo;
    struct sockaddr_in     *sa = NULL;

    if (NULL == host) {
        return (void *)(-1);
    }

    sprintf(port_ptr, "%d", port);
    memset((char *)&hints, 0x00, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    rc = getaddrinfo(host, port_ptr, &hints, &res);
    if (0 != rc) {
        TC_IOT_LOG_ERROR("getaddrinfo error");
        return (void *)(-1);
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        if ((AF_INET == ainfo->ai_family) || (AF_AT == ainfo->ai_family)) {
            sa = (struct sockaddr_in *)ainfo->ai_addr;

            strncpy(addr, inet_ntoa(sa->sin_addr), NETWORK_ADDR_LEN);

            TC_IOT_LOG_TRACE("The host IP %s, port is %d\r\n", addr, ntohs(sa->sin_port));

            socket_id = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
            if (socket_id < 0) {
                TC_IOT_LOG_ERROR("create socket error");
                continue;
            }
            if (0 == connect(socket_id, ainfo->ai_addr, ainfo->ai_addrlen)) {
                break;
            }

            close(socket_id);
        }
		else
		{
			TC_IOT_LOG_ERROR("ai_family %d", ainfo->ai_family);
		}
    }
    freeaddrinfo(res);

    return (void *)socket_id;

#undef NETWORK_ADDR_LEN
}

void tc_iot_hal_udp_close(void *p_socket)
{
    long            socket_id = -1;

    socket_id = (long)p_socket;
    close(socket_id);
}

int tc_iot_hal_udp_write(void *p_socket,
                  const unsigned char *p_data,
                  unsigned int datalen)
{
    int             rc = -1;
    long            socket_id = -1;

    socket_id = (long)p_socket;
    rc = send(socket_id, (char *)p_data, (int)datalen, 0);
    if (-1 == rc) {
        return -1;
    }

    return rc;
}

int tc_iot_hal_udp_read(void *p_socket,
                 unsigned char *p_data,
                 unsigned int datalen)
{
    long            socket_id = -1;
    int             count = -1;

    if (NULL == p_data || NULL == p_socket) {
        return -1;
    }

    socket_id = (long)p_socket;
    count = (int)read(socket_id, p_data, datalen);

    return count;
}

int tc_iot_hal_udp_readTimeout(void *p_socket,
                        unsigned char *p_data,
                        unsigned int datalen,
                        unsigned int timeout)
{
    int                 ret;
    struct timeval      tv;
    fd_set              read_fds;
    long                socket_id = -1;

    if (NULL == p_socket || NULL == p_data) {
        return -1;
    }
    socket_id = (long)p_socket;

    if (socket_id < 0) {
        return -1;
    }

    FD_ZERO(&read_fds);
    FD_SET(socket_id, &read_fds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret = select(socket_id + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);

    /* Zero fds ready means we timed out */
    if (ret == 0) {
        return -2;    /* receive timeout */
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3;    /* want read */
        }

        return -4; /* receive failed */
    }

    /* This call will not block */
    return tc_iot_hal_udp_read(p_socket, p_data, datalen);
}

//===================================//




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
//				if (rc == -1)
//				{
//				   if (errno != ENOTCONN && errno != ECONNRESET)
//				   {
//					  recvLen = -1;		
//					  TC_IOT_LOG_TRACE("read err, no data return");
//				   }
//				}
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
    } else if (recvLen != len) {
        return TC_IOT_NET_READ_TIMEOUT;
    }
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

	
#if 0
    TC_IOT_LOG_TRACE("before send, len=%d, timeout_ms=%d, fd=%d", len, timeout_ms,n->net_context.fd);
    tc_iot_hal_timer_countdown_ms(&timer, timeout_ms);

//    if (FD_ISSET(n->net_context.fd, &fdset)) 
	{
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
        } while (sentLen < len && (!tc_iot_hal_timer_is_expired(&timer)));
    }
#endif

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
