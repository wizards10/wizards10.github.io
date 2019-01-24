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

## 练习2：实现寻找虚拟地址对应的页表项（需要编程）

- t通过设置页表和对应的页表项，可建立虚拟内存地址和物理内存地址的对应关系。其中get_pte函数是设置页表项环节中的一个重要步骤。此函数找到一个虚拟地址对应的二级页表项的内核虚地址。

  ```c
  pte_t* get_pte(pde_t* pgdir , uintptr_t la , bool create)
  {
      pde* pdep = &pgdir[PDX(la)]; //PDX(la)返回Page_dir的index , pgdir存储着Page_table									//的基址
      uintptr_t pg_pa;             //page_dir存储着的是Page_table的基址是物理地址!!!
      void* kernal_address;
      if(!(*pd_entry & PTE_P))                      //如果该Page_Table不存在	
      {
          struct Page* page_table = alloc_page();
          if(page_table == NULL || !create)
          {
              return NULL;
          }
          set_page_ref(pg_table, 1);
          uintptr_t pa = page2pa(pg_table);      //将申请的页返回的地址转化为物理页帧地址
          memset(KADDR(pa) , 0 , PGSIZE);
          *pa_pa = pa |PTE_U | PTE_W | PTE_P; 
      }
      pg_pa = PDE_ADDR(*pd_entry);
      kenal_address = KADDR(pg_pa);           //返回内核虚地址
      return &((pte*)kenal_address)[PTX(la)]       //PTX(la) 返回Page_Table的index
      
  }
  ```





















