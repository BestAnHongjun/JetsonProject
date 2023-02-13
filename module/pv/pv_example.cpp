#include "pv.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>


// 生产者-消费者模型
PV mutex;   // 临界区互斥信号量
PV empty;   // 空闲缓冲区
PV full;    // 缓冲区初始化为空

bool producer_handle = true;
bool consumer_handle = true;

void init()
{
    PV_INIT(mutex, 1);
    PV_INIT(empty, 5);
    PV_INIT(full, 0);
}

void producer()
{
    int cnt = 0;
    while (producer_handle)
    {
        printf("[%d]produce an item in nextp.\n", cnt); // 生产数据
        P(empty);                                       // 获取空缓冲区单元
        P(mutex);                                       // 进入临界区
        printf("[%d]add nextp to buffer.\n", cnt);      // 将数据放入缓冲区
        V(mutex);                                       // 离开临界区
        V(full);                                        // 满缓冲区加1
        cnt++;
    }
}

void consumer()
{
    int cnt = 0;
    while (consumer_handle)
    {
        P(full);                                        // 获取满缓冲区单元
        P(mutex);                                       // 进入临界区
        printf("[%d]rm an item from buffer.\n", cnt);   // 从缓冲区取出数据
        V(mutex);                                       // 离开临界区
        V(empty);                                       // 空缓冲区加1
        printf("[%d]consume the item.\n", cnt);         // 消费数据
        cnt++;
    }
}


int main()
{
    init();
    
    std::thread producer_thread = std::thread(producer);
    std::thread consumer_thread = std::thread(consumer);

    usleep(10 * 1000 * 1000);
    
    producer_handle = false;
    consumer_handle = false;

    producer_thread.join();
    consumer_thread.join();

    return 0;
}