/*
 * syslog.c
 *
 */

#include "syslog.h"

#include <stdarg.h>
#include <stdint.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* The currently enabled set of syslog priorities */

uint8_t g_syslog_mask = LOG_ALL;

/****************************************************************************
 * Name: vsyslog
 *
 * Description:
 *   The function vsyslog() performs the same task as syslog() with the
 *   difference that it takes a set of arguments which have been obtained
 *   using the stdarg variable argument list macros.
 *
 ****************************************************************************/

int vsyslog(int priority, const char *fmt, va_list ap)
{
  int ret = 0;

  /* Check if this priority is enabled */

  if ((g_syslog_mask & LOG_MASK(priority)) != 0)
    {
      /* Yes.. Perform the _vsyslog system call.
       *
       * NOTE:  The va_list parameter is passed by reference.  That is
       * because the va_list is a structure in some compilers and passing
       * of structures in the NuttX sycalls does not work.
       */

      printf(fmt, ap);
//      ret = _vsyslog(priority, fmt, &ap);
    }

  return ret;
}

/****************************************************************************
 * Name: syslog
 *
 * Description:
 *   syslog() generates a log message. The priority argument is formed by
 *   ORing the facility and the level values (see include/syslog.h). The
 *   remaining arguments are a format, as in printf and any arguments to the
 *   format.
 *
 *   The NuttX implementation does not support any special formatting
 *   characters beyond those supported by printf.
 *
 ****************************************************************************/

int syslog(int priority, const char *fmt, ...)
{
  va_list ap;
  int ret;

  /* Let vsyslog do the work */

  va_start(ap, fmt);
  ret = vsyslog(priority, fmt, ap);
  va_end(ap);

  return ret;
}
