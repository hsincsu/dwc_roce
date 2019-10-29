/* dwcroce_pool.c  --2019/10/28 hs*/
#include <rdma/ib_verbs.h>
#include "dwcroce.h"

struct dwcroce_type_info dwcroce_type_info[DWCROCE_NUM_TYPES] = {
	[DWCROCE_TYPE_PD] = {
		.name = "dwcroce-pd",
		.size = sizeof(struct dwcroce_pd),
	},
	[DWCROCE_TYPE_QP] = {
		.name = "dwcroce-qp",
		.size = sizeof(struct dwcroce_qp),
		.cleanup = dwcroce_qp_cleanup,
		.flags = DWCROCE_POOL_INDEX,
		.min_index = DWCROCE_MIN_QP_INDEX,
		.max_index = DWCROCE_MAX_QP_INDEX,
	},
	[DWCROCE_TYPE_CQ] = {
		.name = "dwcroce-cq",
		.size = sizeof(struct dwcroce_cq),
		.cleanup = dwcroce_cq_cleanup,
	},
	[DWCROCE_TYPE_MR] = {
		.name = "dwcroce-mr",
		.size = sizeof(struct dwcroce_mr),
		.cleanup = dwcroce_mem_cleanup,
		.flags = DWCROCE_POOL_INDEX,
		.max_index = DWCROCE_MAX_MR_INDEX,
		.min_index = DWCROCE_MIN_MR_INDEX,
	},
};

static inline const char *pool_name(struct dwcroce_pool *pool)
{
	return dwcroce_type_info[pool->type].name;
}

static inline struct kmem_cache *pool_cache(struct dwcroce_pool *pool)
{
	return dwcroce_type_info[pool->type].cache;
}

static void dwcroce_cache_clean(size_t cnt)
{
	int i;
	struct dwcroce_type_info *type;

	for (i = 0; i < cnt; i++) {
		type = &dwcroce_type_info[i];
		kmem_cache_destroy(type->cache);
		type->cache = NULL;
	}
}

int dwcroce_cache_init(void)
{
	int err;
	int i;
	size_t size;
	struct dwcroce_type_info *type;

	for (i = 0; i < DWCROCE_NUM_TYPES; i++) {
		type = &dwcroce_type_info[i];
		size = ALIGN(type->size, DWCROCE_POOL_ALIGN);
		type->cache = kmem_cache_create(type->name, size,
			DWCROCE_POOL_ALIGN,
			DWCROCE_POOL_CACHE_FLAGS, NULL);
		if (!type->cache) {
			pr_err("Unable to init kmem cache for %s\n",
				type->name);
			err = -ENOMEM;
			goto err1;
		}
	}

	return 0;

err1:
	dwcroce_cache_clean(i);

	return err;
}

void dwcroce_cache_exit(void)
{
	dwcroce_cache_clean(DWCROCE_NUM_TYPES);
}

static int dwcroce_pool_init_index(struct dwcroce_pool *pool, u32 max, u32 min)
{
	int err = 0;
	size_t size;

	if ((max - min + 1) < pool->max_elem) {
		pr_warn("not enough indices for max_elem\n");
		err = -EINVAL;
		goto out;
	}

	pool->max_index = max;
	pool->min_index = min;

	size = BITS_TO_LONGS(max - min + 1) * sizeof(long);
	pool->table = kmalloc(size, GFP_KERNEL);
	if (!pool->table) {
		err = -ENOMEM;
		goto out;
	}

	pool->table_size = size;
	bitmap_zero(pool->table, max - min + 1);

out:
	return err;
}


int dwcroce_pool_init(struct dwcroce_dev *dev, struct dwcroce_pool *pool,
	enum dwcroce_elem_type type, u32 max_elem)
{
	int			err = 0;
	size_t			size = dwcroce_type_info[type].size;

	memset(pool, 0, sizeof(*pool));

	pool->dev = dev;
	pool->type = type;
	pool->max_elem = max_elem;
	pool->elem_size = ALIGN(size, DWCROCE_POOL_ALIGN);
	pool->flags = dwcroce_type_info[type].flags;
	pool->tree = RB_ROOT;
	pool->cleanup = dwcroce_type_info[type].cleanup;

	atomic_set(&pool->num_elem, 0);

	kref_init(&pool->ref_cnt);
	printk("dwcroce: pool_lock init \n");//added by hs 
	rwlock_init(&pool->pool_lock);

	if (dwcroce_type_info[type].flags & DWCROCE_POOL_INDEX) {
		err = dwcroce_pool_init_index(pool,
			dwcroce_type_info[type].max_index,
			dwcroce_type_info[type].min_index);
		if (err)
			goto out;
	}

	if (dwcroce_type_info[type].flags & DWCROCE_POOL_KEY) {
		pool->key_offset = dwcroce_type_info[type].key_offset;
		pool->key_size = dwcroce_type_info[type].key_size;
	}

	pool->state = DWCROCE_POOL_STATE_VALID;

out:
	return err;

}

static void dwcroce_pool_release(struct kref *kref)
{
	struct dwcroce_pool *pool = container_of(kref, struct dwcroce_pool, ref_cnt);

	pool->state = DWCROCE_POOL_STATE_INVALID;
	kfree(pool->table);
}

static void dwcroce_pool_put(struct dwcroce_pool *pool)
{
	kref_put(&pool->ref_cnt, dwcroce_pool_release);
}

void dwcroce_pool_cleanup(struct dwcroce_pool *pool)
{
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	pool->state = DWCROCE_POOL_STATE_INVALID;
	if (atomic_read(&pool->num_elem) > 0)
		pr_warn("%s pool destroyed with unfree'd elem\n",
			pool_name(pool));
	write_unlock_irqrestore(&pool->pool_lock, flags);

	dwcroce_pool_put(pool);
}

static u32 alloc_index(struct dwcroce_pool *pool)
{
	printk("dwcroce: alloc_index start\n");//added by hs 
	u32 index;
	u32 range = pool->max_index - pool->min_index + 1;

	index = find_next_zero_bit(pool->table, range, pool->last);
	if (index >= range)
		index = find_first_zero_bit(pool->table, range);

	WARN_ON_ONCE(index >= range);
	set_bit(index, pool->table);
	pool->last = index;
	printk("dwcroce: alloc_index end!\n");//added by hs 
	return index + pool->min_index;
}

static void insert_index(struct dwcroce_pool *pool, struct dwcroce_pool_entry *new)
{
	struct rb_node **link = &pool->tree.rb_node;
	struct rb_node *parent = NULL;
	struct dwcroce_pool_entry *elem;

	printk("dwcroce: insert_index start \n");//added by hs 
	while (*link) {
		parent = *link;
		elem = rb_entry(parent, struct dwcroce_pool_entry, node);

		if (elem->index == new->index) {
			pr_warn("element already exists!\n");
			goto out;
		}

		if (elem->index > new->index)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

	rb_link_node(&new->node, parent, link);
	rb_insert_color(&new->node, &pool->tree);
out:
	printk("dwcroce: insert_index end\n");//added by hs 
	return;
}

static void insert_key(struct dwcroce_pool *pool, struct dwcroce_pool_entry *new)
{
	struct rb_node **link = &pool->tree.rb_node;
	struct rb_node *parent = NULL;
	struct dwcroce_pool_entry *elem;
	int cmp;

	while (*link) {
		parent = *link;
		elem = rb_entry(parent, struct dwcroce_pool_entry, node);

		cmp = memcmp((u8 *)elem + pool->key_offset,
			(u8 *)new + pool->key_offset, pool->key_size);

		if (cmp == 0) {
			pr_warn("key already exists!\n");
			goto out;
		}

		if (cmp > 0)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

	rb_link_node(&new->node, parent, link);
	rb_insert_color(&new->node, &pool->tree);
out:
	return;
}

void dwcroce_add_key(void *arg, void *key)
{
	struct dwcroce_pool_entry *elem = arg;
	struct dwcroce_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	memcpy((u8 *)elem + pool->key_offset, key, pool->key_size);
	insert_key(pool, elem);
	write_unlock_irqrestore(&pool->pool_lock, flags);
}

void dwcroce_drop_key(void *arg)
{
	struct dwcroce_pool_entry *elem = arg;
	struct dwcroce_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	rb_erase(&elem->node, &pool->tree);
	write_unlock_irqrestore(&pool->pool_lock, flags);
}

void dwcroce_add_index(void *arg)
{
	printk("dwcroce: dwcroce_add_index start\n");//added by hs 
	struct dwcroce_pool_entry *elem = arg;
	struct dwcroce_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	elem->index = alloc_index(pool);
	insert_index(pool, elem);
	write_unlock_irqrestore(&pool->pool_lock, flags);
	printk("dwcroce: dwcroce_add_index end!\n");//added by hs 
}

void dwcroce_drop_index(void *arg)
{
	struct dwcroce_pool_entry *elem = arg;
	struct dwcroce_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	clear_bit(elem->index - pool->min_index, pool->table);
	rb_erase(&elem->node, &pool->tree);
	write_unlock_irqrestore(&pool->pool_lock, flags);
}

void *dwcroce_alloc(struct dwcroce_pool *pool)
{
	printk("dwcroce: --------------------dwcroce_alloc start--------------- \n");//added by hs 
	struct dwcroce_pool_entry *elem;
	unsigned long flags;
	printk("dwcroce: next is might_sleep_if\n");//added by hs 
	might_sleep_if(!(pool->flags & DWCROCE_POOL_ATOMIC));
	printk("dwcroce: next is read_lock_irqsave\n");//added by hs 
	read_lock_irqsave(&pool->pool_lock, flags);
	if (pool->state != DWCROCE_POOL_STATE_VALID) {
		printk("dwcroce: pool->state is not valid\n");//added by hs 
		read_unlock_irqrestore(&pool->pool_lock, flags);
		return NULL;
	}
	printk("dwcroce: next is kref_get\n");//added by hs 
	kref_get(&pool->ref_cnt);
	printk("dwcroce: next is read_unlock_irq\n");//added by hs
	read_unlock_irqrestore(&pool->pool_lock, flags);

	//kref_get(&pool->dwcroce->ref_cnt);
	printk("dwcroce: next is atomic_inc_return\n");//added by hs 
	printk("dwcorce: pool->num_elem is %x\n, max_elem is %x\n",pool->num_elem,pool->max_elem);//added by hs 
	if (atomic_inc_return(&pool->num_elem) > pool->max_elem)
		goto out_put_pool;

	printk("dwcroce: next is kmem_cache_zalloc\n");//added by hs 
	elem = kmem_cache_zalloc(pool_cache(pool),
		(pool->flags & DWCROCE_POOL_ATOMIC) ?
		GFP_ATOMIC : GFP_KERNEL);
	if (!elem)
		goto out_put_pool;

	elem->pool = pool;
	printk("dwcroe: next is kref_init \n");//added by hs 
	kref_init(&elem->ref_cnt);
	printk("dwcroce: dwcroce_alloc end\n");//added by hs 

	return elem;

out_put_pool:
	atomic_dec(&pool->num_elem);
	//dwcroce_dev_put(pool->dwcroce);
	dwcroce_pool_put(pool);
	return NULL;
}

void dwcroce_elem_release(struct kref *kref)
{
	struct dwcroce_pool_entry *elem =
		container_of(kref, struct dwcroce_pool_entry, ref_cnt);
	struct dwcroce_pool *pool = elem->pool;

	if (pool->cleanup)
		pool->cleanup(elem);

	kmem_cache_free(pool_cache(pool), elem);
	atomic_dec(&pool->num_elem);
	//dwcroce_dev_put(pool->dwcroce);
	dwcroce_pool_put(pool);
}

void *dwcroce_pool_get_index(struct dwcroce_pool *pool, u32 index)
{
	struct rb_node *node = NULL;
	struct dwcroce_pool_entry *elem = NULL;
	unsigned long flags;

	read_lock_irqsave(&pool->pool_lock, flags);

	if (pool->state != DWCROCE_POOL_STATE_VALID)
		goto out;

	node = pool->tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct dwcroce_pool_entry, node);

		if (elem->index > index)
			node = node->rb_left;
		else if (elem->index < index)
			node = node->rb_right;
		else {
			kref_get(&elem->ref_cnt);
			break;
		}
	}

out:
	read_unlock_irqrestore(&pool->pool_lock, flags);
	return node ? elem : NULL;
}

void *dwcroce_pool_get_key(struct dwcroce_pool *pool, void *key)
{
	struct rb_node *node = NULL;
	struct dwcroce_pool_entry *elem = NULL;
	int cmp;
	unsigned long flags;

	read_lock_irqsave(&pool->pool_lock, flags);

	if (pool->state != DWCROCE_POOL_STATE_VALID)
		goto out;

	node = pool->tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct dwcroce_pool_entry, node);

		cmp = memcmp((u8 *)elem + pool->key_offset,
			key, pool->key_size);

		if (cmp > 0)
			node = node->rb_left;
		else if (cmp < 0)
			node = node->rb_right;
		else
			break;
	}

	if (node)
		kref_get(&elem->ref_cnt);

out:
	read_unlock_irqrestore(&pool->pool_lock, flags);
	return node ? elem : NULL;
}

static u8 dwcroce_get_key(void)
{
	static u32 key = 1;

	key = key << 1;

	key |= (0 != (key & 0x100)) ^ (0 != (key & 0x10))
		^ (0 != (key & 0x80)) ^ (0 != (key & 0x40));

	key &= 0xff;

	return key;
}
#define IB_ACCESS_REMOTE        (IB_ACCESS_REMOTE_READ          \
                                | IB_ACCESS_REMOTE_WRITE        \
                                | IB_ACCESS_REMOTE_ATOMIC)

static void dwcroce_mem_init(int access, struct dwcroce_mr *mr)
{
	u32 lkey = mr->pelem.index << 8 | dwcroce_get_key();
	u32 rkey = (access & IB_ACCESS_REMOTE) ? lkey : 0;

	if (mr->pelem.pool->type == DWCROCE_TYPE_MR) {
		mr->ibmr.lkey = lkey;
		mr->ibmr.rkey = rkey;
	}

	mr->lkey = lkey;
	mr->rkey = rkey;
	mr->state = DWCROCE_MEM_STATE_INVALID;
	mr->type = DWCROCE_MEM_TYPE_NONE;
	mr->map_shift = ilog2(DWCROCE_BUF_PER_MAP);

}

static int dwcroce_mem_alloc(struct dwcroce_mr *mr, int num_buf)
{
	printk("dwcroce: dwcroce_mem_alloc start\n");//added by hs 
	int i;
	int num_map;
	struct dwcroce_map **map = mr->map;
	num_map = (num_buf + DWCROCE_BUF_PER_MAP - 1) / DWCROCE_BUF_PER_MAP;
	mr->map = kmalloc_array(num_map, sizeof(*map), GFP_KERNEL);
	if (!mr->map)
		goto err1;

	for (i = 0; i < num_map; i++) {
		mr->map[i] = kmalloc(sizeof(**map), GFP_KERNEL);
		if (!mr->map[i])
			goto err2;
	}

	BUILD_BUG_ON(!is_power_of_2(DWCROCE_BUF_PER_MAP));

	mr->map_shift = ilog2(DWCROCE_BUF_PER_MAP);
	mr->map_mask = DWCROCE_BUF_PER_MAP - 1;

	mr->num_buf = num_buf;
	mr->num_map = num_map;
	mr->max_buf = num_map * DWCROCE_BUF_PER_MAP;
	printk("dwcroce: dwcroce_mem_alloc end\n");//added by hs 
	return 0;

err2:
	for (i--; i >= 0; i--)
		kfree(mr->map[i]);

	kfree(mr->map);
err1:
	return -ENOMEM;


}


int dwcroce_mem_init_fast(struct dwcroce_pd *pd, int max_pages, struct dwcroce_mr *mr)
{
	int err;

	dwcroce_mem_init(0, mr);

	/* In fastreg, we also set the rkey */
	mr->ibmr.rkey = mr->ibmr.lkey;

	err = dwcroce_mem_alloc(mr, max_pages);
	if (err)
		goto err1;

	mr->pd = pd;
	mr->max_buf = max_pages;
	mr->state = DWCROCE_MEM_STATE_FREE;
	mr->type = DWCROCE_MEM_TYPE_MR;

	return 0;

err1:
	return err;

}


int dwcroce_mem_init_dma(struct dwcroce_pd *pd,int access, struct dwcroce_mr *mr)
{
	printk("dwcroce: dwcroce_mem_init_dma start \n");//added by hs 
	dwcroce_mem_init(access,mr);

	mr->pd = pd;
	mr->access = access;
	mr->state = DWCROCE_MEM_STATE_VALID;
	mr->type = DWCROCE_MEM_TYPE_DMA;
	printk("dwcroce: dwcroce_mem_init_dma ");//added by hs 
	return 0;
}
