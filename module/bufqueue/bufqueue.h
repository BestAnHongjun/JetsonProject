#ifndef __BUFQUEUE_H__
#define __BUFQUEUE_H__

#include "global_config.h"

#include "pv.h"
#include <stdint.h>

class BufQueue
{
private:
    uint8_t** que;
    uint32_t* item_size;
    uint16_t max_size;
    uint16_t front, back;
    PV* mutex;
    PV mutex_pointer;
    PV full, empty;

public:
    BufQueue(uint16_t max_size);
    ~BufQueue();

    void push(uint8_t* buf, uint32_t size);
    void push_drop_old(uint8_t* buf, uint32_t size);
    void take(uint8_t** buf, uint32_t *size);

    bool is_empty();
    bool is_full();
    uint32_t size();
};

#endif // __BUFQUEUE_H__