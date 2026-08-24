#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int srv_serversocket_tcp
  (uint32_t ip, uint16_t port)
{
  int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in sin = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = htonl(ip)
  };

  if (fd < 0) {
    fprintf(stderr, "Could not create socket.\n");
    return -1;
  }

#define _SRV_SET_REUSEPORT
#ifdef _SRV_SET_REUSEPORT
  {
    int flag = 1;
    if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag))) {
      fprintf(stderr,
          "Inet: Warning: Failed to set (server) socket option SO_REUSEPORT "
          "(continuing), errno=%d.", errno);
      abort();  /* FIXME temp */
    }
  }
#endif

  if (bind(fd, (struct sockaddr*)&sin, sizeof(sin))) {
    fprintf(stderr, "Could not bind socket.\n");
    close(fd);
    return -1;
  } else if (listen(fd, 1)) {
    fprintf(stderr, "Could not call listen() on socket.\n");
    close(fd);
    return -1;
  }

#define _SRV_SET_TCPNODELAY
#ifdef _SRV_SET_TCPNODELAY
  {
    int flag = 1;
    if (-1 == setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))) {
      fprintf(stderr,
          "Inet: Warning: Failed to set (server) socket option TCP_NODELAY "
          "(continuing), errno=%d.", errno);
    }
  }
#endif

  return fd;
}
