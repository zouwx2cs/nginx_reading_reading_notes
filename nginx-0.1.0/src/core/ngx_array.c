
/*
 * Copyright (C) Igor Sysoev
 */


#include <ngx_config.h>
#include <ngx_core.h>

/**
 * @function: 建立array
 * @param ngx_pool_t *p: 分配内存使用的内存池
 * @param ngx_uint_t n: array初始capacity（元素个数）
 * @param size_t size: 单个元素字节数
 * @return boolean: 数组结构地址
 */
ngx_array_t *ngx_create_array(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

	// 分配ngx_array_t那么大的内存空间
    ngx_test_null(a, ngx_palloc(p, sizeof(ngx_array_t)), NULL);

	// 给数组的数据分配内存空间
    ngx_test_null(a->elts, ngx_palloc(p, n * size), NULL);

	// 更新内存池
    a->pool = p;
	// 当前元素个数为0
    a->nelts = 0;
	// capacity（元素个数）
    a->nalloc = n;
	// 单个元素字节数
    a->size = size;

    return a;
}

/**
 * @function: 销毁array
 * @param ngx_pool_t *p: 分配内存使用的内存池
 * @param ngx_uint_t n: array初始capacity（元素个数）
 * @param size_t size: 单个元素字节数
 * @return boolean: 数组结构地址
 */
void ngx_destroy_array(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    if ((char *) a->elts + a->size * a->nalloc == p->last) {
        p->last -= a->size * a->nalloc;
    }

    if ((char *) a + sizeof(ngx_array_t) == p->last) {
        p->last = (char *) a;
    }
}


void *ngx_push_array(ngx_array_t *a)
{
    void        *elt, *new;
    ngx_pool_t  *p;

    /* array is full */
    if (a->nelts == a->nalloc) {
        p = a->pool;

        /* array allocation is the last in the pool */
        if ((char *) a->elts + a->size * a->nelts == p->last
            && (unsigned) (p->end - p->last) >= a->size)
        {
            p->last += a->size;
            a->nalloc++;

        /* allocate new array */
        } else {
            ngx_test_null(new, ngx_palloc(p, 2 * a->nalloc * a->size), NULL);

            ngx_memcpy(new, a->elts, a->nalloc * a->size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (char *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}
