#include <cstdlib>
#include <cstring>

typedef unsigned int	t_uint;
typedef unsigned char	t_uchar;



template<typename T>
class MemoryBlock
{
    public:
        MemoryBlock(size_t element_Count)
        {
            this->element_Count = element_Count;
            this->element_size = sizeof(T);
            this->free_blocks = element_Count;
            this->used_blocks = 0;
            this->pool_start = (t_uchar *)(new T[element_Count]);
            this->pool_end = this->pool_start + this->element_Count * this->element_size;
            this->next_empty = this->pool_start;
            this->next_pool = NULL;
        }

    private:
        t_uint			element_Count;
        t_uint			element_size;
        t_uint			free_blocks;
        t_uint			used_blocks;
        t_uchar			*pool_start;
        t_uchar			*pool_end;
        t_uchar			*next_empty;
        MemoryBlock<T>	*next_pool;
};


template<typename T>
class MemoryPool
{
    public:
        MemoryPool(size_t element_Count)
        {
            this->element_Count = element_Count;
            this->head = new MemoryBlock<T>(element_Count);
        }

        T* allocate()
        {
            return (head->allocate());
        }

    private:
        t_uint			element_Count;
        MemoryBlock<T>	*head;
};

typedef struct s_mpool
{
	t_uint			element_Count;
	t_uint			element_size;
	t_uint			free_blocks;
	t_uint			used_blocks;
	t_uchar			*pool_start;
	t_uchar			*pool_end;
	t_uchar			*next_empty;
	struct s_mpool	*next_pool;
}	t_mpool;

t_mpool	*mpool_create(size_t element_size, t_uint element_Count);
void	mpool_destroy(t_mpool **pool);
void	*mpool_alloc(t_mpool *pool);
void	mpool_dealloc(t_mpool *pool, void *p);



void	mpool_destroy(t_mpool **pool)
{
	t_mpool	*cur;
	t_mpool	*next;

	cur = *pool;
	while (cur)
	{
		free(cur->pool_start);
		cur->pool_start = NULL;
		next = cur->next_pool;
		free(cur);
		cur = next;
	}
	*pool = NULL;
}


static t_uchar	*index_addr(const t_mpool *pool, t_uint i)
{
	return (pool->pool_start + (i * pool->element_size));
}

static t_uint	addr_index(const t_mpool *pool, const t_uchar *p)
{
	return (((t_uint)(p - pool->pool_start)) / pool->element_size);
}

void	*mpool_alloc(t_mpool *pool)
{
	t_uint	*p;
	void	*ret;

	if (pool->used_blocks < pool->element_Count)
	{
		p = (t_uint *)index_addr(pool, pool->used_blocks);
		*p = pool->used_blocks++ + 1;
	}
	ret = NULL;
	if (pool->free_blocks > 0)
	{
		ret = (void *)pool->next_empty;
		if (--pool->free_blocks != 0)
			pool->next_empty = index_addr(pool, *((t_uint *)pool->next_empty));
		else
			pool->next_empty = NULL;
	}
	else
	{
		if (!pool->next_pool)
			pool->next_pool = \
			mpool_create(pool->element_size, pool->element_Count);
		return (mpool_alloc(pool->next_pool));
	}
	return (ret);
}

void	mpool_dealloc(t_mpool *pool, void *to_free)
{
	if (!pool || !to_free)
		return ;
	if ((t_uchar *)to_free >= pool->pool_start \
	&& (t_uchar *)to_free < pool->pool_end)
	{
		if (pool->next_empty != NULL)
		{
			(*(t_uint *)to_free) = addr_index(pool, pool->next_empty);
			pool->next_empty = (t_uchar *)to_free;
		}
		else
		{
			*((t_uint *)to_free) = pool->element_Count;
			pool->next_empty = (t_uchar *)to_free;
		}
		++pool->free_blocks;
	}
	else
		mpool_dealloc(pool->next_pool, to_free);
}