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

int srv_clientsocket_tcp
  (uint32_t ip, uint16_t port)
{
  int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in remote = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = ip
  };

#define _SRV_SET_REUSEPORT
#ifdef _SRV_SET_REUSEPORT
  {
    int flag = 1;
    if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag))) {
      fprintf(stderr,
          "Inet: Warning: Failed to set (client) socket option SO_REUSEPORT "
          "(continuing), errno=%d.", errno);
    }
  }
#endif

#define _SRV_TCP_CLIENT_CLONE_PORTNUMBER
#ifdef _SRV_TCP_CLIENT_CLONE_PORTNUMBER
  struct sockaddr_in local = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = INADDR_ANY,
    .sin_port = htons(port)
  };
  if (bind(fd, (struct sockaddr*)&local, sizeof(local)) != 0) {
    fprintf(stderr,
      "Inet: Warning: Failed to bind TCP client source port to %u "
      "(continuing), errno=%d.", ntohs(local.sin_port), errno);
  } else {
    fprintf(stderr, "Inet: Bound TCP client source port to %u.",
        ntohs(local.sin_port));
  }
#endif

  if (connect(fd, (struct sockaddr*)&remote, sizeof(remote))) {
    close(fd);
    return -1;
  }

#define _SRV_SET_TCPNODELAY
#ifdef _SRV_SET_TCPNODELAY
  {
    int flag = 1;
    if (-1 == setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))) {
      fprintf(stderr,
          "Inet: Warning: Failed to set (client) socket option TCP_NODELAY "
          "(continuing), errno=%d.", errno);
    }
  }
#endif

#ifdef FORCE_SO_RCVBUF
  {
    /* Mind the other copy */
    int r = setsockopt(
              fd,
              SOL_SOCKET,
              SO_RCVBUF,
	      &(int){ FORCE_SO_RCVBUF },
              sizeof(int)
            );
    if (r) {
      perror("Set socket option SO_RCVBUF failed");
    } else {
      fprintf(stderr, FORCE_SO_RCVBUF);
    }
  }
#endif

  return fd;
}
