#include "udp.h"

#include <iostream>
#include <memory.h>


void UDPReceiver::init(const int recv_port, uint16_t pack_size)
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        std::cout << "Create socket fail at " << ERR_INFO << std::endl;
        exit(-1);
    }

    struct timeval timeout = {UDP_TIMEOUT, 0};
    int ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (ret < 0)
    {
        std::cout << "Set timeout fail at " << ERR_INFO << std::endl;
        exit(-1);
    }

    this->pack_size = pack_size;

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(recv_port);  //端口号，需要网络序转换

    ret = bind(fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if (ret < 0)
    {
        std::cout << "socket bind fail at " << ERR_INFO << std::endl;
        exit(-1);
    }

    buffer = new uint8_t[sizeof(UDPHead) + pack_size];
}

UDPReceiver::UDPReceiver(const int recv_port, uint16_t pack_size)
{
    init(recv_port, pack_size);
    asynch = false;
}

UDPReceiver::UDPReceiver(const int recv_port, void (*callback)(RecvHead&), uint16_t pack_size)
{
    init(recv_port, pack_size);
    callback_func = callback;
    work_handle = true;
    asynch = true;
    work_thread = std::thread(&UDPReceiver::work, this);
}

UDPReceiver::~UDPReceiver()
{
    if (asynch)
    {
        work_handle = false;
        work_thread.join();
        UDP_DEBUG(printf("UDPReceiver work thread has ended.\n"));
    }
    if (buffer) delete[] buffer;
    UDP_DEBUG(printf("~UDPReceiver() finished.\n"));
}

void UDPReceiver::work()
{
    RecvHead head;
    while (work_handle)
    {
        recv(head);
        callback_func(head);
    }
}

void UDPReceiver::recv(RecvHead &recv_head)
{
    int count = 0, ret;
    double current_time, delay;
    struct sockaddr_in clent_addr;
    socklen_t len = sizeof(clent_addr);

    uint16_t buf_len = sizeof(UDPHead) + pack_size;
start_recv:
    ret = recvfrom(fd, buffer, buf_len, 0, (struct sockaddr*)&clent_addr, &len);
    if (ret < 0 || ret != buf_len)
    {
        recv_head = (RecvHead){0, 0, NULL};
        return;
    }
    
    UDPHead* head = (UDPHead*)buffer;
    double time = (double)(*head).tv.tv_sec * 1e3 + (double)(*head).tv.tv_usec * 1e-3;
    uint16_t id = (*head).id;
    if (id != 1) goto start_recv;
    uint16_t total = (*head).total;
    uint16_t size = (*head).size;
    count += size;
    UDP_DEBUG(printf("Recv id=%d, total=%d, size=%d\n", id, total, size));
    
    uint8_t* recv_buf = new uint8_t[total * pack_size];
    memcpy(recv_buf, buffer + sizeof(UDPHead), size);

    for (int i = 2; i <= total; i++)
    {
        ret = recvfrom(fd, buffer, buf_len, 0, (struct sockaddr*)&clent_addr, &len);
        if (ret < 0 || ret != buf_len)
        {
            UDP_DEBUG(std::cout << "recieve data fail at" << ERR_INFO << std::endl);
            exit(-1);
        }
        
        head = (UDPHead*)buffer;
        id = (*head).id;
        if (id != i) 
        {
            recv_head = (RecvHead){0, 0, NULL};
            return;
        }
        size = (*head).size;
        count += size;
        UDP_DEBUG(printf("Recv id=%d, size=%d\n", id, size));
        memcpy(recv_buf + (i - 1) * pack_size, buffer + sizeof(UDPHead), size);
    }

    struct timeval tp;
    gettimeofday(&tp, NULL);
    current_time = (double)tp.tv_sec * 1e3 + (double)tp.tv_usec * 1e-3;
    delay = current_time - time;
    UDP_DEBUG(printf("Total delay: %.6lf ms, size=%d\n", delay, count));

    recv_head.delay = delay;
    recv_head.size = count;
    recv_head.buffer = recv_buf;
}