/*
*	 this file is to alloc memory pool for resources
*
*
*
*/

#ifndef DWCROCE_POOL_H
#define DWCROCE_POOL_H

#define DWCROCE_POOL_ALIGN		  (16)
#define DWCROCE_POOL_CACHE_FLAGS  (0)

enum dwcroce_pool_flags {
	DWCROCE_POOL_ATOMIC			=BIT(0),
	DWCROCE_POOL_INDEX			=BIT(1),
	DWCROCE_POOL_KEY			=BIT(2),
};

enum dwcroce_elem_type {
//	DWCROCE_TYPE_UC,
	DWCROCE_TYPE_PD,
//	DWCROCE_TYPE_AH,
//	DWCROCE_TYPE_SRQ,
	DWCROCE_TYPE_QP,
	DWCROCE_TYPE_CQ,
	DWCROCE_TYPE_MR,
//	DWCROCE_TYPE_MW,
//	DWCROCE_TYPE_MC_GRP,
//	DWCROCE_TYPE_MC_ELEM,
	DWCROCE_NUM_TYPES,
};

struct dwcroce_pool_entry;

struct dwcroce_type_info {
	const char *name;
	size_t			size;
	void			(*cleanup)(struct dwcroce_pool_entry *obj);
	enum dwcroce_pool_flags	flags;
	u32			max_index;
	u32			min_index;
	size_t			key_offset;
	size_t			key_size;
	struct kmem_cache *cache;
};

extern struct dwcroce_type_info dwcroce_type_info[];

enum dwcroce_mem_state {
	DWCROCE_MEM_STATE_ZOMBIE,
	DWCROCE_MEM_STATE_INVALID,
	DWCROCE_MEM_STATE_FREE,
	DWCROCE_MEM_STATE_VALID,
};

enum dwcroce_mem_type {
	DWCROCE_MEM_TYPE_NONE,
	DWCROCE_MEM_TYPE_DMA,
	DWCROCE_MEM_TYPE_MR,
	DWCROCE_MEM_TYPE_FMR,
	DWCROCE_MEM_TYPE_MW,
};

enum dwcroce_pool_state {
	DWCROCE_POOL_STATE_INVALID,
	DWCROCE_POOL_STATE_VALID,
};

struct dwcroce_pool_entry {
	struct dwcroce_pool			*pool;
	struct kref					ref_cnt;
	struct list_head			list;
	
	/*only userd if indexed or keyed*/
	struct rb_node				node;
	u32							index;
};

struct dwcroce_pool {
	struct dwcroce_dev			*dev;
	rwlock_t					pool_lock; /*protects pool add/del/search*/
	size_t						elem_size;
	struct kref					ref_cnt;
	void						(*cleanup)(struct dwcroce_pool_entry *obj);
	enum dwcroce_pool_state		state;
	enum dwcroce_pool_flags		flags;
	enum dwcroce_elem_type		type;

	unsigned int				max_elem;
	atomic_t					num_elem;

	/*only used if indexed or keyed*/
	struct rb_root				tree;
	unsigned long				*table;
	size_t						table_size;
	u32							max_index;
	u32							min_index;
	u32							last;
	size_t						key_offset;
	size_t						key_size;
};

#define DWCROCE_BUF_PER_MAP			(PAGE_SIZE/sizeof(struct dwcroce_phys_buf))

struct dwcroce_phys_buf {
	u64				addr;
	u64				size;
};

struct dwcroce_map {
	struct dwcroce_phys_buf		buf[DWCROCE_BUF_PER_MAP];
};



/*initialize slab caches for managed objects*/
int dwcroce_cache_init(void);

/* cleanup slab caches for managed objects */
void dwcroce_cache_exit(void);

/* initialize a pool of objects with given limit on
 * number of elements. gets parameters from dwcroce_type_info
 * pool elements will be allocated out of a slab cache
 */
int dwcroce_pool_init(struct dwcroce_dev *dwcroce, struct dwcroce_pool *pool,
	enum dwcroce_elem_type type, u32 max_elem);

/* free resources from object pool */
void dwcroce_pool_cleanup(struct dwcroce_pool *pool);

/* allocate an object from pool */
void *dwcroce_alloc(struct dwcroce_pool *pool);

/* assign an index to an indexed object and insert object into
 *  pool's rb tree
 */
void dwcroce_add_index(void *elem);

/* drop an index and remove object from rb tree */
void dwcroce_drop_index(void *elem);

/* assign a key to a keyed object and insert object into
 *  pool's rb tree
 */
void dwcroce_add_key(void *elem, void *key);

/* remove elem from rb tree */
void dwcroce_drop_key(void *elem);

/* lookup an indexed object from index. takes a reference on object */
void *dwcroce_pool_get_index(struct dwcroce_pool *pool, u32 index);

/* lookup keyed object from key. takes a reference on the object */
void *dwcroce_pool_get_key(struct dwcroce_pool *pool, void *key);

/* cleanup an object when all references are dropped */
void dwcroce_elem_release(struct kref *kref);

/* take a reference on an object */
#define dwcroce_add_ref(elem) kref_get(&(elem)->pelem.ref_cnt)

/* drop a reference on an object */
#define dwcroce_drop_ref(elem) kref_put(&(elem)->pelem.ref_cnt, dwcroce_elem_release)

#endif
