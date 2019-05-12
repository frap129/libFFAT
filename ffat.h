// vim: noet:ts=8:sts=8
#ifndef FFAT_H
#define FFAT_H

#include <sys/types.h>

struct fat_boot_fat32 {
	/*36*/	uint32_t	sectors_per_fat;//BPB_FATSz32
	/*40*/	uint16_t	fat_flags;
	/*42*/	uint16_t	version;
	/*44*/	uint32_t	root_cluster;//BPB_RootClus
	/*48*/	uint16_t	fsinfo_sector;
	/*50*/	uint16_t	backup_sector;
	/*52*/	uint8_t		reserved2[12];
	/*64*/	uint8_t		drive_number;
	/*65*/	uint8_t		reserved3;
	/*66*/	uint8_t		ext_sig;
	/*67*/	uint32_t	serial;
	/*71*/	char		label[11];
	/*82*/	char		type[8];
} __attribute__ ((__packed__));

// Boot sector
struct fat_boot {
	/* 0*/	uint8_t		jmp_boot[3];
	/* 3*/	char		oemname[8];
	/*11*/	uint16_t	bytes_per_sector;//BPB_BytsPerSec
	/*13*/	uint8_t		sectors_per_cluster;//BPB_SecPerClus
	/*14*/	uint16_t	reserved_sectors;//BPB_RsvdSecCnt
	/*16*/	uint8_t		fat_count;//BPB_NumFATs
	/*17*/	uint16_t	root_max_entries; //BPB_RootEntCnt
	/*19*/	uint16_t	total_sectors_small;//BPB_TotSec16
	/*21*/	uint8_t		media_info;//BPB_Media
	/*22*/	uint16_t	sectors_per_fat_small;//BPB_FATSz16
	/*24*/	uint16_t	sectors_per_track;
	/*26*/	uint16_t	head_count;
	/*28*/	uint32_t	fs_offset;
	/*32*/	uint32_t	total_sectors;//BPB_TotSec32
	/*36*/	struct fat_boot_fat32  fat32;
} __attribute__ ((__packed__));


struct fat32_direntry {
	/* 0*/	union {
			struct {
				char		name[8];
				char		ext[3];
			};
			char			nameext[11];
		};
	/*11*/	uint8_t		attr;
	/*12*/	uint8_t		res;
	/*13*/	uint8_t		ctime_ms;
	/*14*/	uint16_t	ctime_time;
	/*16*/	uint16_t	ctime_date;
	/*18*/	uint16_t	atime_date;
	/*20*/	uint16_t	cluster_hi;
	/*22*/	uint16_t	mtime_time;
	/*24*/	uint16_t	mtime_date;
	/*26*/	uint16_t	cluster_lo;
	/*28*/	uint32_t	size;
} __attribute__ ((__packed__));

// A kitchen sink for all important data about filesystem
struct ffat_data {
	const char	*dev;
	int		fs;
	struct fat_boot  fat_boot;
	/* XXX add your code here */
	uint32_t root_cluster;
};

// Used by ffat_search_entry()
struct ffat_search_data {
	const char	*name;
	int		found;
	struct stat	*st;
};

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define FFAT_ATTR_LFN 0xf
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define FFAT_ATTR_INVAL (0x80|0x40|0x08)

struct fat32_direntry_long {
	/* 0*/	uint8_t		seq;
	/* 1*/	uint16_t	name1[5];
	/*11*/	uint8_t		attr;
	/*12*/	uint8_t		type;
	/*13*/	uint8_t		csum;
	/*14*/	uint16_t	name2[6];
	/*26*/	uint16_t	reserved2;
	/*28*/	uint16_t	name3[2];
} __attribute__ ((__packed__));

#define FFAT_LFN_SEQ_START	0x40
#define FFAT_LFN_SEQ_DELETED	0x80
#define FFAT_LFN_SEQ_MASK	0x3f

void seek_cluster(uint32_t cluster_no);
static int read_cluster(uint32_t cluster_no, fuse_fill_dir_t filler, void *fillerdata,bool first_cluster);
char* getfilename(char* nameext, char* filename);
uint32_t next_cluster(uint32_t cluster_no);
void setStat(struct fat32_direntry dir_entry, char* buffer, fuse_fill_dir_t filler, void *fillerdata, uint32_t cluster_no);
static void ffat_init(const char *dev);
unsigned char chkSum (unsigned char *pFcbName);
time_t conv_time(uint16_t date_entry, uint16_t time_entry);
static int ffat_readdir(uint32_t cluster_no, fuse_fill_dir_t filler, void *fillerdata);
static int ffat_search_entry(void *data, const char *name, const struct stat *st, off_t offs);
static int ffat_resolve(const char *path, struct stat *st);
static int ffat_fuse_getattr(const char *path, struct stat *st);
static int ffat_fuse_readdir(const char *path, void *buf,
            fuse_fill_dir_t filler, off_t offs, struct fuse_file_info *fi);
static int ffat_fuse_read(const char *path, char *buf, size_t size, off_t offs,
            struct fuse_file_info *fi);
static int ffat_opt_args(void *data, const char *arg, int key, struct fuse_args *oargs);
void int_handler(int sig);
#endif