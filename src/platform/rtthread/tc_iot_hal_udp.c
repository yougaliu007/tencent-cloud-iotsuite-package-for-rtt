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


#ifdef __cplusplus
}
#endif
