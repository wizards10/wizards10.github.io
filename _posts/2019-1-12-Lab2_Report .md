---
layout: post
title: "Lab2_Report"
date: 2019-1-12
excerpt: "Lab2"
tags: [操作系统，ucore]
comments: true
---

## 练习1

> 实现first-fit连续物理内存分配算法

```c
free_area_t free_area;
#define free_list (free_area.free_list)
#define nr_free (free_area.nr_free)

static void 
default_init(void)
{
    list_init(&free_list);
    nr_free = 0;
}
static void
default_init_memmap(struct Page *base , size_t n)
{
    assert(n > 0);
    struct Page* p = base;
    for( ; p != base + n; p++)
    {
        assert(PageReserved(p));
        set_page_ref(p , 0);
        p->flags = 0;
        p->property = 0;
        list_add_before(&free_list , &(p->page_link));
    }
    base->property = n;
    nr_free += n;
}
static struct Page*
default_alloc_pages(size_t n)
{
    assert( n > 0);
    if( n > nrfree)
    {
        return NULL;
    }
    struct Page* page = NULL;
    list_entry_t* le = &free_list;
    while((le = list_next(le)) != &free_list)
    {
        struct Page* p = le2page(le , page_link);
        if(p->property >= n)
        {
            page = p;
            break;
        }
        // just jump
        if(p->property != 0)
        {
            unsigned int len = p->property;
            p += (size_t)(len - 1);
            le = &(p->page_link)
        }
    }
    if(page != NULL)
    {
        list_entry_t* le_next = NULL;
        int i;
        for(i = 0 ; i < n ; i++)
        {
            le_next = list_next(le);
            struct Page* p_tmp = le2page(le , page_link);
            ClearPageProperty(p_tmp);
            SetPageReserved(p_tmp);
            list_del(le);
            le = le_next;
        }
        //updata the block
        if(page->property > n)
        {
            (le2page(le , page_link))->property = page->property - n;
        }
        page->property = n;
        nr_free -= n;
    }
}
```
























