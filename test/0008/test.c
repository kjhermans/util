#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <util/util.h>
#include <util/xtransport.h>

static
vec_t orig = { 0 };

static
int serverhandler
  (char* name, vec_t* msg, void* arg)
{
  fprintf(stderr, "Received a message.\n");
  int c = vec_compare(&orig, msg);
  fprintf(stderr, "Message is '%s'; size=%u compares %d\n", name, msg->size, c);
  if (c == 0 && 0 == strcmp(name, "foo")) {
    fprintf(stderr, "SUCCESS\n");
  } else {
    logmem(orig.data, orig.size);
    logmem(msg->data, msg->size);
  }
  return 0;
}

static
void* serverthread
  (void* arg)
{
  xt_t xt = { 0 };
  int fd = srv_serversocket_udp(0, 5001);
  if (fd < 0) {
    fprintf(stderr, "Server socket error.\n");
    return NULL;
  }

  while (1) {
    xtransport_server(&xt, fd, serverhandler, 0);
  }
  return NULL;
}

int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  {
    xt_t xt = { 0 };
    pthread_t t = { 0 };
    int fd, r;
    struct timespec ts = { 1, 0 };

    pthread_create(&t, 0, serverthread, 0);

    fd = srv_clientsocket_udp(0x7f000001, 5001);

    sleep(2); /* Give the server time to start up */
    for (unsigned i=0; i < 2 * 1024 * 1024; i++) {
      vec_appendchr(&orig, rand() & 0xff);
    }

    r = xtransport_client(&xt, fd, "foo", &orig, &ts);

    sleep(2); /* Give the server time to process */
  }

/*
  {
    if (
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");
*/

//  fprintf(stderr, "SUCCESS\n");

  return 0;
}
