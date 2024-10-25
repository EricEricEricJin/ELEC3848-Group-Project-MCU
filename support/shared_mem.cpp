#include "shared_mem.h"

struct mem_line
{
    int id;
    size_t size;
    void *pointer;
};

static struct mem_line id_addr_table[SHARED_MEM_MAX_OBJ] = {0};

int shared_mem_create(int id, size_t size)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].pointer == NULL)
        {
            // assign ID and size
            id_addr_table[i].id = id;
            id_addr_table[i].size = size;

            id_addr_table[i].pointer = malloc(size);
            memset(id_addr_table[i].pointer, 0, size);

            return 0;
        }
    }
    return -1;
}

int shared_mem_update(int id, const void *pointer)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            memcpy(id_addr_table[i].pointer, pointer, id_addr_table[i].size);
            return 0;
        }
    }
    return -1;
}

int shared_mem_get(int id, void* dst)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            memcpy(dst, id_addr_table[i].pointer, id_addr_table[i].size);
            return 0;
        }
    }
    return -1;
}

int shared_mem_delete(int id)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            // free memory
            free(id_addr_table[i].pointer);
            // reset struct
            memset(&id_addr_table[i], 0, sizeof(struct mem_line));
            return 0;
        }
    }
    return -1;
}
