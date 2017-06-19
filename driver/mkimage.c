/*
 * (C) Copyright 2000-2004
 * DENX Software Engineering
 * Wolfgang Denk, wd@denx.de
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>




#ifndef	O_BINARY		/* should be define'd on __WIN32__ */
#define O_BINARY	0
#endif

#include "image.h"

typedef struct table_entry {
	int	val;		/* as defined in image.h	*/
	char	*sname;		/* short (input) name		*/
	char	*lname;		/* long (output) name		*/
} table_entry_t;

table_entry_t arch_name[] = {
    {   IH_CPU_INVALID,     NULL,       "Invalid CPU",  },
    {   IH_CPU_ALPHA,       "alpha",    "Alpha",    },
    {   IH_CPU_ARM,     "arm",      "ARM",      },
    {   IH_CPU_I386,        "x86",      "Intel x86",    },
    {   IH_CPU_IA64,        "ia64",     "IA64",     },
    {   IH_CPU_M68K,        "m68k",     "MC68000",  },
    {   IH_CPU_MICROBLAZE,  "microblaze",   "MicroBlaze",   },
    {   IH_CPU_MIPS,        "mips",     "MIPS",     },
    {   IH_CPU_MIPS64,      "mips64",   "MIPS 64 Bit",  },
    {   IH_CPU_NIOS,        "nios",     "NIOS",     },
    {   IH_CPU_NIOS2,       "nios2",    "NIOS II",  },
    {   IH_CPU_PPC,     "ppc",      "PowerPC",  },
    {   IH_CPU_S390,        "s390",     "IBM S390", },
    {   IH_CPU_SH,      "sh",       "SuperH",   },
    {   IH_CPU_SPARC,       "sparc",    "SPARC",    },
    {   IH_CPU_SPARC64,     "sparc64",  "SPARC 64 Bit", },
    {   IH_CPU_BLACKFIN,    "blackfin", "Blackfin", },
    {   IH_CPU_AVR32,       "avr32",    "AVR32",    },
    {   -1,         "",     "",     },
};

table_entry_t os_name[] = {
    {   IH_OS_INVALID,  NULL,       "Invalid OS",       },
    {   IH_OS_4_4BSD,   "4_4bsd",   "4_4BSD",       },
    {   IH_OS_ARTOS,    "artos",    "ARTOS",        },
    {   IH_OS_DELL, "dell",     "Dell",         },
    {   IH_OS_ESIX, "esix",     "Esix",         },
    {   IH_OS_FREEBSD,  "freebsd",  "FreeBSD",      },
    {   IH_OS_IRIX, "irix",     "Irix",         },
    {   IH_OS_LINUX,    "linux",    "Linux",        },
    {   IH_OS_LYNXOS,   "lynxos",   "LynxOS",       },
    {   IH_OS_NCR,  "ncr",      "NCR",          },
    {   IH_OS_NETBSD,   "netbsd",   "NetBSD",       },
    {   IH_OS_OPENBSD,  "openbsd",  "OpenBSD",      },
    {   IH_OS_PSOS, "psos",     "pSOS",         },
    {   IH_OS_QNX,  "qnx",      "QNX",          },
    {   IH_OS_RTEMS,    "rtems",    "RTEMS",        },
    {   IH_OS_SCO,  "sco",      "SCO",          },
    {   IH_OS_SOLARIS,  "solaris",  "Solaris",      },
    {   IH_OS_SVR4, "svr4",     "SVR4",         },
    {   IH_OS_U_BOOT,   "u-boot",   "U-Boot",       },
    {   IH_OS_VXWORKS,  "vxworks",  "VxWorks",      },
    {   -1,     "",     "",         },
};

table_entry_t type_name[] = {
    {   IH_TYPE_INVALID,    NULL,     "Invalid Image",  },
    {   IH_TYPE_FILESYSTEM, "filesystem", "Filesystem Image",   },
    {   IH_TYPE_FIRMWARE,   "firmware",   "Firmware",       },
    {   IH_TYPE_KERNEL,     "kernel",     "Kernel Image",   },
    {   IH_TYPE_MULTI,      "multi",      "Multi-File Image",   },
    {   IH_TYPE_RAMDISK,    "ramdisk",    "RAMDisk Image",  },
    {   IH_TYPE_SCRIPT,     "script",     "Script",     },
    {   IH_TYPE_STANDALONE, "standalone", "Standalone Program", },
    {   IH_TYPE_FLATDT,     "flat_dt",    "Flat Device Tree",   },
    {   -1,         "",       "",           },
};

table_entry_t comp_name[] = {
    {   IH_COMP_NONE,   "none",     "uncompressed",     },
    {   IH_COMP_BZIP2,  "bzip2",    "bzip2 compressed", },
    {   IH_COMP_GZIP,   "gzip",     "gzip compressed",  },
    {   -1,     "",     "",         },
};

void	print_header (image_header_t *);
static	void	print_type (image_header_t *);
static	char	*put_table_entry (table_entry_t *, char *, int);
static	char	*put_arch (int);
static	char	*put_type (int);
static	char	*put_os   (int);
static	char	*put_comp (int);


void
print_header (image_header_t *hdr)
{
	time_t timestamp;
	uint32_t size;

	timestamp = (time_t)ntohl(hdr->ih_time);
	size = ntohl(hdr->ih_size);

	printf ("Image Name:   %.*s\n", IH_NMLEN, hdr->ih_name);
//	printf ("Created:      %s", ctime(&timestamp));
	printf ("Image Type:   "); print_type(hdr);
	printf ("Data Size:    %d Bytes = %.2f kB = %.2f MB\n",
		size, (double)size / 1.024e3, (double)size / 1.048576e6 );
	printf ("Load Address: 0x%08X\n", ntohl(hdr->ih_load));
	printf ("Entry Point:  0x%08X\n", ntohl(hdr->ih_ep));
}


static void
print_type (image_header_t *hdr)
{
	printf ("%s %s %s (%s)\n",
		put_arch (hdr->ih_arch),
		put_os   (hdr->ih_os  ),
		put_type (hdr->ih_type),
		put_comp (hdr->ih_comp)
	);
}

static char *put_arch (int arch)
{
	return (put_table_entry(arch_name, "Unknown Architecture", arch));
}

static char *put_os (int os)
{
	return (put_table_entry(os_name, "Unknown OS", os));
}

static char *put_type (int type)
{
	return (put_table_entry(type_name, "Unknown Image", type));
}

static char *put_comp (int comp)
{
	return (put_table_entry(comp_name, "Unknown Compression", comp));
}

static char *put_table_entry (table_entry_t *table, char *msg, int type)
{
	for (; table->val>=0; ++table) {
		if (table->val == type)
			return (table->lname);
	}
	return (msg);
}
