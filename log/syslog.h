/*
 * syslog.h
 *
 *  Created on: 2017年6月9日
 *      Author: lichen
 */

#ifndef SYSLOG_H_
#define SYSLOG_H_


/* This determines the importance of the message. The levels are, in order
 * of decreasing importance:
 */

#define LOG_EMERG     0  /* System is unusable */
#define LOG_ALERT     1  /* Action must be taken immediately */
#define LOG_CRIT      2  /* Critical conditions */
#define LOG_ERR       3  /* Error conditions */
#define LOG_WARNING   4  /* Warning conditions */
#define LOG_NOTICE    5  /* Normal, but significant, condition */
#define LOG_INFO      6  /* Informational message */
#define LOG_DEBUG     7  /* Debug-level message */

/* Used with setlogmask() */

#define LOG_MASK(p)   (1 << (p))

#define LOG_ALL       0xff


#endif /* SYSLOG_H_ */
