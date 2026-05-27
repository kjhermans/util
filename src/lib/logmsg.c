/**
 *

Copyright (c) 2024, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Kees-Jan Hermans BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "../include/util/vec_t.h"
#include "../include/util/util_functions.h"

static pid_t pid = 0;
static uint64_t count = 0;
static const unsigned facility = 16;
static char sinstr[ 64 ] = { 0 };
static int sinstr_init = 0;
static int logfd = 0;

/**
 * Implements RFC5424 syslog message formatting, and sending such a
 * message out to either a filedescriptor, or print it to stderr.
 *
 * \param severity Severity (0-7 inclusive, as per the RFC).
 *
 * TODO: Fill in the 'host' field.
 */
void logmsg
  (unsigned severity, char* fmt, ...)
{
  char buf1[ 1024 ];
  char buf2[ 2048 ];
  va_list ap;
  char tim[ 64 ];
  time_t t = time(0);
  struct tm* tm = localtime(&t);
  char defaulthost[] = "localhost";
  char* hoststring = 0;

  severity = (facility * 8) + (severity % 8);

  if (!pid) {
    pid = getpid();
  }

  ++count;

  va_start(ap, fmt);
  vsnprintf(buf1, sizeof(buf1), fmt, ap);
  va_end(ap);

  strftime(tim, sizeof(tim), "%Y-%m-%dT%H:%M:%S%z", tm);

  if (logfd) {
    if (!sinstr_init) {
      struct sockaddr_in sin;
      socklen_t sinlen = sizeof(sin);
      if (getsockname(logfd, (struct sockaddr*)&sin, &sinlen) == 0) {
        uint32_t srcip = sin.sin_addr.s_addr;
        char* tmp = ipv4_tostring(srcip);
        strncpy(sinstr, tmp, sizeof(sinstr)-1);
        hoststring = sinstr;
        sinstr_init = 1;
      } else {
        hoststring = defaulthost;
      }
    } else {
      hoststring = sinstr;
    }
  } else {
    hoststring = defaulthost;
  }

  snprintf(buf2, sizeof(buf2),
    "<%u> 1 %s %s kmd %u %"PRIu64" - %s"
    , severity
    , tim
    , hoststring
    , pid
    , count
    , buf1
  );

  if (logfd) {
    ssize_t s = send(logfd, buf2, strlen(buf2), 0); (void)s;
  }
  fprintf(stderr, "%s\n", buf2);
}
