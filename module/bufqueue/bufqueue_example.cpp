#include "bufqueue.h"
#include <thread>
#include <unistd.h>


// 生产者 - 消费者模型
BufQueue que(5);

bool producer_handle = true;
bool consumer_handle = true;

void producer()
{
    int cnt = 0;
    while (producer_handle)
    {
        printf("producer: make %d.\n", cnt);
        que.push((uint8_t*)&cnt, sizeof(int));
        cnt++;
    }
}

void consumer()
{
    while (consumer_handle)
    {
        int* buf;
        uint32_t size;
        que.take((uint8_t**)&buf, &size);
        printf("consumer: got %d\n", *buf);
        delete[] buf;
    }
}

int main()
{
    std::thread producer_thread = std::thread(producer);
    std::thread consumer_thread = std::thread(consumer);

    usleep(2 * 1000 * 1000);
    
    producer_handle = false;
    consumer_handle = false;

    producer_thread.join();
    consumer_thread.join();

    return 0;
}