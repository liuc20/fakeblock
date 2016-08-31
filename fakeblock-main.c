#include "fakeblock.h"

static struct fakeblock g_fakeblock;

static int fakeblock_open(struct block_device *bdev, fmode_t mode)
{
		return 0;
}

static void fakeblock_release(struct gendisk *gd, fmode_t mode)
{
		return;
}

static struct block_device_operations fakeblock_ops = {
		.owner = THIS_MODULE,
		.open = fakeblock_open,
		.release = fakeblock_release,
};

static void fakeblock_make_request(struct request_queue *queue, struct bio *bio)
{
		bio_endio(bio, 0);
		return;
}

static int fakeblock_create_disk(struct fakeblock *block)
{
	int ret = 0;

	block->major_num = register_blkdev(block->major_num, "fakeblock");
	if (block->major_num < 0) {
			printk("register major number for block device failed!\n");
			ret = -1;
			goto out0;
	}

	block->queue = blk_alloc_queue(GFP_KERNEL);
	if (!block->queue) {
			printk("alloc request queue failed!\n");
			ret = -1;
			goto out1;
	}
	blk_queue_make_request(block->queue, fakeblock_make_request);

	block->disk = alloc_disk(FAKEBLOCK_MINORS);
	if (!block->disk) {
			printk("alloc gendisk failed!\n");
			ret = -1;
			goto out2;
	}

	block->disk->major = block->major_num;
	block->disk->first_minor = 0;
	block->disk->fops = &fakeblock_ops;
	block->disk->private_data = &block;
	strcpy(block->disk->disk_name, FAKEBLOCK_NAME);
	set_capacity(block->disk, FAKEBLOCK_SIZE);
	block->disk->queue = block->queue;

	add_disk(block->disk);

	return ret;

	del_gendisk(block->disk);
out2:
	blk_cleanup_queue(block->queue);
out1:
	unregister_blkdev(block->major_num, "fakeblock");
out0:
	return ret;
}

static __init int fakeblock_init(void)
{
	int ret = 0;

	ret = fakeblock_create_disk(&g_fakeblock);
	if (ret) {
			printk("create fakeblock failed!\n");
	}
	return ret;
}

static void fakeblock_destroy_disk(struct fakeblock *block)
{
	del_gendisk(block->disk);
	blk_cleanup_queue(block->queue);
	unregister_blkdev(block->major_num, "fakeblock");
}

static __exit void fakeblock_exit(void)
{
	fakeblock_destroy_disk(&g_fakeblock);
}

module_init(fakeblock_init);
module_exit(fakeblock_exit);

