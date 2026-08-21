#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int srv_clientsocket_tcp
  (uint32_t ip, uint16_t port)
{
  int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in sin = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = ip
  };

  if (connect(fd, (struct sockaddr*)&sin, sizeof(sin))) {
    fprintf(stderr, "Could not connect socket.\n");
    close(fd);
    return -1;
  }
  return fd;
}
