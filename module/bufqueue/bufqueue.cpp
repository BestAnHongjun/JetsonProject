#include "bufqueue.h"
#include <memory.h>


BufQueue::BufQueue(uint16_t max_size)
{
    this->max_size = max_size + 1;
    
    que = new uint8_t*[this->max_size];
    item_size = new uint32_t[this->max_size];
    mutex = new PV[this->max_size];
    
    front = back = 0;
    PV_INIT(mutex_pointer, 1);
    PV_INIT(empty, max_size);
    PV_INIT(full, 0);
    for (int i = 0; i < this->max_size; i++)
    {
        que[i] = NULL;
        PV_INIT(mutex[i], 1);
    }
}

BufQueue::~BufQueue()
{
    for (int i = front; i != back; i = (i + 1) % max_size)
    {
        if (que[i]) 
        {
            delete[] que[i];
            que[i] = NULL;
        }
    }
    delete[] que;
    delete[] item_size;
    delete[] mutex;
}

void BufQueue::push(uint8_t* buf, uint32_t size)
{
    uint16_t this_back;

    P(empty);
    
    P(mutex_pointer);
    this_back = back;
    back = (back + 1) % max_size;
    P(mutex[this_back]);
    V(mutex_pointer);

    que[this_back] = new uint8_t[size];
    memcpy(que[this_back], buf, size);
    item_size[this_back] = size;
    V(mutex[this_back]);

    V(full);
}

void BufQueue::push_drop_old(uint8_t* buf, uint32_t size)
{
    bool fullque = false;
    uint16_t this_front, this_back;
    
    P(mutex_pointer);
    this_front = front;
    this_back = back;
    if ((back + 1) % max_size == front)
    {
        fullque = true;
        front = (front + 1) % max_size;
        P(mutex[this_front]);
    } 
    else
    {
        P(empty);
    }
    back = (back + 1) % max_size;
    V(mutex_pointer);

    if (fullque)
    {
        delete[] que[this_front];
        que[this_front] = NULL;
        item_size[this_front] = 0;
        V(mutex[this_front]);
    }

    P(mutex[this_back]);
    que[this_back] = new uint8_t[size];
    memcpy(que[this_back], buf, size);
    item_size[this_back] = size;
    V(mutex[this_back]);

    if (!fullque)
    {
        V(full);
    }
}

void BufQueue::take(uint8_t** buf, uint32_t *size)
{
    uint16_t this_front;

    P(full);

    P(mutex_pointer);
    this_front = front;
    front = (front + 1) % max_size;
    P(mutex[this_front]);
    V(mutex_pointer);

    *size = item_size[this_front];
    *buf = new uint8_t[*size];
    memcpy(*buf, que[this_front], *size);
    delete[] que[this_front];
    que[this_front] = NULL;
    item_size[this_front] = 0;
    V(mutex[this_front]);

    V(empty);
}

bool BufQueue::is_empty()
{
    uint16_t this_front, this_back;

    P(mutex_pointer);
    this_front = front;
    this_back = back;
    V(mutex_pointer);
    
    return this_front == this_back;
}

bool BufQueue::is_full()
{
    uint16_t this_front, this_back;

    P(mutex_pointer);
    this_front = front;
    this_back = back;
    V(mutex_pointer);

    return (back + 1) % max_size == front;
}

uint32_t BufQueue::size()
{
    uint16_t this_front, this_back;

    P(mutex_pointer);
    this_front = front;
    this_back = back;
    V(mutex_pointer);

    return (this_back + max_size - this_front) % max_size;
}
