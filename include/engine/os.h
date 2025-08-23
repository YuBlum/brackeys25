#ifndef __OS_LAYER_H__
#define __OS_LAYER_H__

void *os_mem_reserve(size_t amount);
bool os_mem_commit(void *buf, size_t amount);
bool os_mem_free(void *buf, size_t amount);
size_t os_page_size(void);
size_t os_reasonable_default_capacity(void);

#endif/*__OS_LAYER_H__*/
