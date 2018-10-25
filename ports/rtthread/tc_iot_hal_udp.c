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


//====================================================================

int tc_iot_hal_udp_read(tc_iot_network_t* network, unsigned char* buffer, int len, int timeout_ms)
{
    int   count;

    if (NULL == buffer) 
	{
        return TC_IOT_INVALID_PARAMETER;
    }

    count = (int)read(network->net_context.fd, buffer, len);

	if (count == 0) 
	{
        return TC_IOT_NET_NOTHING_READ;
    }

    return count;
}


int tc_iot_hal_udp_write(tc_iot_network_t* network, const unsigned char* buffer, int len, int timeout_ms)
{
    return send(network->net_context.fd, (char *)buffer, len, 0);
}

int tc_iot_hal_udp_connect(tc_iot_network_t* network, const char* host, uint16_t port) {
    int type = SOCK_DGRAM;    
	char        port_ptr[6] = {0};
    sa_family_t family = AF_INET;
    struct addrinfo* result = NULL;
	struct addrinfo* ainfo = NULL;
	struct addrinfo* res = NULL;
    struct addrinfo hints;
	struct sockaddr_in     *sa = NULL;
	int rc;


    if (host) {
        network->net_context.host = (char *)host;
    }

    if (port) {
        network->net_context.port = port;
    }

	sprintf(port_ptr, "%d",  network->net_context.port);
    memset((char *)&hints, 0x00, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;


	rc = getaddrinfo(network->net_context.host, port_ptr, &hints, &res);
    if (0 != rc) 
	{
        TC_IOT_LOG_ERROR("getaddrinfo failed for host:%s", network->net_context.host);
        return TC_IOT_NET_UNKNOWN_HOST;
    }

	for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) 
	{
		if ((AF_INET == ainfo->ai_family) || (AF_AT == ainfo->ai_family)) 
		{
		    sa = (struct sockaddr_in *)ainfo->ai_addr;

		    TC_IOT_LOG_TRACE("The host IP %s, port is %d\r\n", inet_ntoa(sa->sin_addr), ntohs(sa->sin_port));

		    network->net_context.fd  = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
		    if (network->net_context.fd  < 0) 
			{
		        TC_IOT_LOG_ERROR("create socket error");
		        continue;
		    }
		    if (0 == connect(network->net_context.fd , ainfo->ai_addr, ainfo->ai_addrlen)) 
			{
				network->net_context.is_connected = 1;
				rc = TC_IOT_SUCCESS;
		        break;
		    }

		    close(network->net_context.fd);
		}
		else
		{
			TC_IOT_LOG_ERROR("ai_family %d", ainfo->ai_family);
		}
	}
    freeaddrinfo(res);


	if(TC_IOT_SUCCESS != rc)
	{
		rc = TC_IOT_NET_CONNECT_FAILED;
	}

	return rc;    
}

int tc_iot_hal_udp_is_connected(tc_iot_network_t* network) {
    return network->net_context.is_connected;
}

int tc_iot_hal_udp_disconnect(tc_iot_network_t* network) {
    TC_IOT_LOG_TRACE("network disconnecting...");
    close(network->net_context.fd);
    network->is_connected = 0;
    TC_IOT_LOG_TRACE("network disconnected");
    return TC_IOT_SUCCESS;
}

int tc_iot_hal_udp_destroy(tc_iot_network_t* network) {
    if (tc_iot_hal_udp_is_connected(network)) {
        tc_iot_hal_udp_disconnect(network);
    }

    TC_IOT_LOG_TRACE("network destroying...");
    TC_IOT_LOG_TRACE("network destroied...");
    return TC_IOT_SUCCESS;
}

int tc_iot_hal_udp_init(tc_iot_network_t* network,
                        tc_iot_net_context_init_t* net_context) {
    if (NULL == network) {
        return TC_IOT_NETWORK_PTR_NULL;
    }

    network->do_read = tc_iot_hal_udp_read;
    network->do_write = tc_iot_hal_udp_write;
    network->do_connect = tc_iot_hal_udp_connect;
    network->do_disconnect = tc_iot_hal_udp_disconnect;
    network->is_connected = tc_iot_hal_udp_is_connected;
    network->do_destroy = tc_iot_hal_udp_destroy;
    tc_iot_copy_net_context(&(network->net_context), net_context);

    return TC_IOT_SUCCESS;
}


#ifdef __cplusplus
}
#endif
