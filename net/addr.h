/*
 * addr.h
 *
 *  Created on: 2017年6月1日
 *      Author: lichen
 */

#ifndef ADDR_H_
#define ADDR_H_

#include <stdint.h>
#include <stddef.h>



#define IPPROTO_IP            0    /* Dummy protocol for TCP */
#define IPPROTO_HOPOPTS       0    /* IPv6 Hop-by-Hop options.  */
#define IPPROTO_ICMP          1    /* Internet Control Message Protocol */
#define IPPROTO_IGMP          2    /* Internet Group Management Protocol */
#define IPPROTO_IPIP          4    /* IPIP tunnels (older KA9Q tunnels use 94) */
#define IPPROTO_TCP           6    /* Transmission Control Protocol */
#define IPPROTO_EGP           8    /* Exterior Gateway Protocol */
#define IPPROTO_PUP           12   /* PUP protocol */
#define IPPROTO_UDP           17   /* User Datagram Protocol */
#define IPPROTO_IDP           22   /* XNS IDP protocol */
#define IPPROTO_TP            29   /* SO Transport Protocol Class 4.  */
#define IPPROTO_DCCP          33   /* Datagram Congestion Control Protocol */
#define IPPROTO_IPV6          41   /* IPv6-in-IPv4 tunnelling */
#define IPPROTO_ROUTING       43   /* IPv6 routing header. */
#define IPPROTO_FRAGMENT      44   /* IPv6 fragmentation header.  */
#define IPPROTO_RSVP          46   /* Reservation Protocol. */
#define IPPROTO_GRE           47   /* General Routing Encapsulation. */
#define IPPROTO_ESP           50   /* Encapsulation Security Payload protocol */
#define IPPROTO_AH            51   /* Authentication Header protocol */
#define IPPROTO_ICMPV6        58   /* ICMPv6 */
#define IPPROTO_NONE          59   /* IPv6 no next header. */
#define IPPROTO_DSTOPTS       60   /* IPv6 destination options. */
#define IPPROTO_MTP           92   /* Multicast Transport Protocol.  */
#define IPPROTO_ENCAP         98   /* Encapsulation Header. */
#define IPPROTO_BEETPH        94   /* IP option pseudo header for BEET */
#define IPPROTO_PIM           103  /* Protocol Independent Multicast */
#define IPPROTO_COMP          108  /* Compression Header protocol */
#define IPPROTO_SCTP          132  /* Stream Control Transport Protocol */
#define IPPROTO_UDPLITE       136  /* UDP-Lite (RFC 3828) */
#define IPPROTO_MPLS          137  /* MPLS in IP (RFC 4023) */
#define IPPROTO_RAW           255  /* Raw IP packets */


#define PF_UNSPEC      0 /* Protocol family unspecified */
#define PF_UNIX        1 /* Local communication */
#define PF_LOCAL       1 /* Local communication */
#define PF_INET        2 /* IPv4 Internet protocols */
#define PF_INET6       3 /* IPv6 Internet protocols */
#define PF_PACKET      4 /* Low level packet interface */
/* Supported Address Families. Opengroup.org requires only AF_UNSPEC,
 * AF_UNIX, AF_INET and AF_INET6.
 */
#define AF_UNSPEC      PF_UNSPEC
#define AF_UNIX        PF_UNIX
#define AF_LOCAL       PF_LOCAL
#define AF_INET        PF_INET
#define AF_INET6       PF_INET6
#define AF_PACKET      PF_PACKET


#define SOCK_STREAM    0 /* Provides sequenced, reliable, two-way, connection-based byte streams.
                          * An  out-of-band data transmission mechanism may be supported. */
#define SOCK_DGRAM     1 /* Supports  datagrams (connectionless, unreliable messages of a fixed
                          * maximum length). */
#define SOCK_SEQPACKET 2 /* Provides a sequenced, reliable, two-way connection-based data
                          * transmission path for datagrams of fixed maximum length; a consumer
                          * is required to read an entire packet with each read system call. */
#define SOCK_RAW       3 /* Provides raw network protocol access. */
#define SOCK_RDM       4 /* Provides a reliable datagram layer that does not guarantee ordering. */
#define SOCK_PACKET    5 /* Obsolete and should not be used in new programs */


/* Socket options */

#define SO_DEBUG        0 /* Enables recording of debugging information (get/set).
                           * arg: pointer to integer containing a boolean value */
#define SO_ACCEPTCONN   1 /* Reports whether socket listening is enabled (get only).
                           * arg: pointer to integer containing a boolean value */
#define SO_BROADCAST    2 /* Permits sending of broadcast messages (get/set).
                           * arg: pointer to integer containing a boolean value */
#define SO_REUSEADDR    3 /* Allow reuse of local addresses (get/set)
                           * arg: pointer to integer containing a boolean value */
#define SO_KEEPALIVE    4 /* Keeps connections active by enabling the periodic transmission
                           * of messages (get/set).
                           * arg: pointer to integer containing a boolean value */
#define SO_LINGER       5 /* Lingers on a close() if data is present (get/set)
                           * arg: struct linger */
#define SO_OOBINLINE    6 /* Leaves received out-of-band data (data marked urgent) inline
                           * (get/set) arg: pointer to integer containing a boolean value */
#define SO_SNDBUF       7 /* Sets send buffer size. arg: integer value (get/set). */
#define SO_RCVBUF       8 /* Sets receive buffer size. arg: integer value (get/set). */
#define SO_ERROR        9 /* Reports and clears error status (get only).  arg: returns
                           * an integer value */
#define SO_TYPE        10 /* Reports the socket type (get only). return: int */
#define SO_DONTROUTE   11 /* Requests that outgoing messages bypass standard routing (get/set)
                           * arg: pointer to integer containing a boolean value */
#define SO_RCVLOWAT    12 /* Sets the minimum number of bytes to process for socket input
                           * (get/set). arg: integer value */
#define SO_RCVTIMEO    13 /* Sets the timeout value that specifies the maximum amount of time
                           * an input function waits until it completes (get/set).
                           * arg: struct timeval */
#define SO_SNDLOWAT    14 /* Sets the minimum number of bytes to process for socket output
                           * (get/set). arg: integer value */
#define SO_SNDTIMEO    15 /* Sets the timeout value specifying the amount of time that an
                           * output function blocks because flow control prevents data from
                           * being sent(get/set). arg: struct timeval */

/* Protocol levels supported by get/setsockopt(): */

#define SOL_SOCKET     0  /* Only socket-level options supported */




/* directly map this to the lwip internal functions */
#define inet_addr(cp)         ipaddr_addr(cp)

/** IPv4 only: set the IP address given as an u32_t */
#define ip4_addr_set_u32(dest_ipaddr, src_u32) ((dest_ipaddr)->addr = (src_u32))
/** IPv4 only: get the IP address as an u32_t */
#define ip4_addr_get_u32(src_ipaddr) ((src_ipaddr)->addr)

/** 255.255.255.255 */
#define IPADDR_NONE         ((u32_t)0xffffffffUL)
/** 127.0.0.1 */
#define IPADDR_LOOPBACK     ((u32_t)0x7f000001UL)
/** 0.0.0.0 */
#define IPADDR_ANY          ((u32_t)0x00000000UL)
/** 255.255.255.255 */
#define IPADDR_BROADCAST    ((u32_t)0xffffffffUL)



typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int    u32_t;
typedef signed     int    s32_t;


/* This is the aligned version of ip_addr_t,
   used as local variable, on the stack, etc. */
struct ip_addr {
  u32_t addr;
};

typedef struct ip_addr ip_addr_t;


typedef uint16_t sa_family_t;

typedef uint16_t in_port_t;

/* IPv4 Internet address */

typedef uint32_t in_addr_t;

struct in_addr
{
  in_addr_t       s_addr;      /* Address (network byte order) */
};

struct sockaddr_in
{
  sa_family_t     sin_family;  /* Address family: AF_INET */
  uint16_t        sin_port;    /* Port in network byte order */
  struct in_addr  sin_addr;    /* Internet address */
};

struct sockaddr
{
  sa_family_t sa_family;       /* Address family: See AF_* definitions */
  char        sa_data[14];     /* 14-bytes of address data */
};

typedef uint32_t socklen_t;


#define h_addr h_addr_list[0]

struct hostent
{
  char  *h_name;       /* Official name of the host. */
  char **h_aliases;    /* A pointer to an array of pointers to alternative
                            * host names, terminated by a null pointer. */
  int        h_addrtype;   /* Address type. */
  int        h_length;     /* The length, in bytes, of the address. */
  char **h_addr_list;  /* A pointer to an array of pointers to network
                            * addresses (in network byte order) for the host,
                            * terminated by a null pointer. */
};




#define htons(x) lwip_htons(x)
#define ntohs(x) lwip_ntohs(x)
#define htonl(x) lwip_htonl(x)
#define ntohl(x) lwip_ntohl(x)

u16_t lwip_htons(u16_t x);
u16_t lwip_ntohs(u16_t x);
u32_t lwip_htonl(u32_t x);
u32_t lwip_ntohl(u32_t x);


u32_t ipaddr_addr(const char *cp);
int ipaddr_aton(const char *cp, ip_addr_t *addr);
int inet_pton(int af, const char *src, void *dest);


#endif /* ADDR_H_ */
