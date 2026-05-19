#ifndef _LOGMSG_H_
#define _LOGMSG_H_

#define LOGLEVEL_EMERGENCY      0
#define LOGLEVEL_ALERT          1
#define LOGLEVEL_CRITICAL       2
#define LOGLEVEL_ERROR          3
#define LOGLEVEL_WARNING        4
#define LOGLEVEL_NOTICE         5
#define LOGLEVEL_INFORMATIONAL  6
#define LOGLEVEL_DEBUG          7

extern
void logmsg
  (unsigned severity, char* fmt, ...);

#endif
