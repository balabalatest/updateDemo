/*
 * debug.h
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "config.h"
#include "syslog.h"

void assert_func(const char *file, int line, const char *func, const char *failedExpr);


#define assert        debug_assert
#define DEBUGASSERT   debug_assert
#define ASSERT        debug_assert

#define debug_assert(expression) \
  ((expression) ? (void)0 : assert_func (__FILE__, __LINE__, NULL, #expression))



#if 0

#ifdef CONFIG_HAVE_FUNCTIONNAME
#  define EXTRA_FMT "%s: "
#  define EXTRA_ARG ,__FUNCTION__
#else
#  define EXTRA_FMT
#  define EXTRA_ARG
#endif

#ifdef CONFIG_DEBUG_ALERT
#  define _alert(format, ...) \
   syslog(LOG_EMERG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else /* CONFIG_DEBUG_ERROR */
#  define  _alert(x...)
#endif


#ifdef CONFIG_DEBUG_ERROR
#  define  _err(format, ...) \
   syslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define  _err(x...)
#endif

#ifdef CONFIG_DEBUG_WARN
#  define _warn(format, ...) \
   syslog(LOG_WARNING, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define _warn(x...)
#endif

#ifdef CONFIG_DEBUG_INFO
#  define _info(format, ...) \
   syslog(LOG_INFO, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define _info(x...)
#endif


/*
 * Debug for net
 */
#ifdef CONFIG_DEBUG_NET_ERROR
#  define nerr(format, ...)     _err(format, ##__VA_ARGS__)
#else
#  define nerr(x...)
#endif

#ifdef CONFIG_DEBUG_NET_WARN
#  define nwarn(format, ...)   _warn(format, ##__VA_ARGS__)
#else
#  define nwarn(x...)
#endif

#ifdef CONFIG_DEBUG_NET_INFO
#  define ninfo(format, ...)   _info(format, ##__VA_ARGS__)
#else
#  define ninfo(x...)
#endif

#else
#define nerr   printf
#define nwarn  printf
#define ninfo
#endif


#endif /* DEBUG_H_ */
