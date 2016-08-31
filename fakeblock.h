#ifndef	__SUBMITER_H__
#define __SUBMITER_H__


#include <linux/init.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/gfp.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kmod.h>
#include <linux/version.h>
#include <linux/seq_file.h>

#define FAKEBLOCK_NAME		"fakeblock"
#define FAKEBLOCK_SIZE		(1024 * 1024 * 2 * 100) 	//100GB
#define FAKEBLOCK_MINORS	16


struct fakeblock {
		int major_num;
		struct gendisk *disk;
		struct request_queue *queue;
};

MODULE_LICENSE("Dual BSD/GPL");
#endif
