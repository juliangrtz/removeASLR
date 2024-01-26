/*
These definitions are taken from the XNU kernel:
https://opensource.apple.com/source/xnu/
*/

#include <stdint.h>

#define MH_PIE 0x200000 /* When this bit is set, the OS will load the main executable at a random address. */

#define MH_MAGIC 0xFEEDFACE /* the mach magic number (little endian) */
#define MH_CIGAM 0xCEFAEDFE /* the mach magic number (big endian) --> NXSwapInt(MH_MAGIC), look at byte_order.h */

#define MH_MAGIC_64 0xFEEDFACF /* the 64-bit mach magic number (little endian) */
#define MH_CIGAM_64 0xCFFAEDFE /* the 64-bit mach magic number (big endian) --> NXSwapInt(MH_MAGIC), look at byte_order.h */

typedef int32_t integer_t;
typedef integer_t cpu_type_t;
typedef integer_t cpu_subtype_t;

struct mach_header
{
	uint32_t magic;			  /* mach magic number identifier */
	cpu_type_t cputype;		  /* cpu specifier */
	cpu_subtype_t cpusubtype; /* machine specifier */
	uint32_t filetype;		  /* type of file */
	uint32_t ncmds;			  /* number of load commands */
	uint32_t sizeofcmds;	  /* the size of all the load commands */
	uint32_t flags;			  /* flags */
};

struct mach_header_64
{
	uint32_t magic;			  /* mach magic number identifier */
	cpu_type_t cputype;		  /* cpu specifier */
	cpu_subtype_t cpusubtype; /* machine specifier */
	uint32_t filetype;		  /* type of file */
	uint32_t ncmds;			  /* number of load commands */
	uint32_t sizeofcmds;	  /* the size of all the load commands */
	uint32_t flags;			  /* flags */
	uint32_t reserved;		  /* reserved */
};
