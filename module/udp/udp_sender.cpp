#include "udp.h"
#include "bufqueue.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


UDPSender::UDPSender(const char* target_ip, const int target_port, 
uint16_t que_size, uint16_t pack_size)
: que(que_size)
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        std::cout << "Create socket fail at " << ERR_INFO << std::endl;
        exit(-1);
    }

    this->pack_size = pack_size;

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);
    target_addr.sin_port = htons(target_port);

    work_handle = true;
    work_thread = std::thread(&UDPSender::work, this);
}

UDPSender::~UDPSender()
{
    int res = 0;
    work_handle = false;
    que.push((uint8_t*)&res, sizeof(int));
    work_thread.join();
    UDP_DEBUG(printf("UDPSender work thread has ended.\n"));
    if (buffer) delete[] buffer;
    UDP_DEBUG(printf("~UDPSender() finished.\n"));
}

void UDPSender::send(uint8_t* buf, uint32_t size)
{
    que.push(buf, size);
}

void UDPSender::work()
{
    UDP_DEBUG(printf("UDPSender work thread has started.\n"));
    uint8_t* send_buf;
    uint16_t buf_len = sizeof(UDPHead) + pack_size;
    send_buf = new uint8_t[buf_len];
    UDPHead* head = (UDPHead*)send_buf;
    uint8_t* buf = send_buf + sizeof(UDPHead);

    while (work_handle)
    {
        uint32_t size;
        que.take(&buffer, &size);
        if (!work_handle)
        {
            if (buffer) delete[] buffer;
            buffer = NULL;
            break;
        }
        uint16_t pack_mod = size % pack_size;
        uint16_t pack_num = size / pack_size;
        (*head).total = pack_mod ? pack_num + 1 : pack_num;
        UDP_DEBUG(printf("size=%d pack_mod=%d pack_num=%d\n", size, pack_mod, pack_num));

        uint8_t* dst_p;
        uint8_t* src_p;
         
        for (int i = 0; i < pack_num; i++)
        {
            dst_p = buf;
            src_p = buffer + i * pack_size;
            memcpy(dst_p, src_p, pack_size);
            gettimeofday(&((*head).tv), NULL);
            (*head).id = i + 1;
            (*head).size = pack_size;
            UDP_DEBUG(printf("SEND[%d/%d]\n", (*head).id, (*head).total));
            sendto(fd, send_buf, buf_len, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
        }

        if (pack_mod)
        {
            dst_p = buf;
            src_p = buffer + pack_num * pack_size;
            memcpy(dst_p, src_p, pack_mod);
            gettimeofday(&((*head).tv), NULL);
            (*head).id = pack_num + 1;
            (*head).size = pack_mod;
            UDP_DEBUG(printf("SEND[%d/%d]\n", (*head).id, (*head).total));
            sendto(fd, send_buf, buf_len, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
        }

        if (buffer) delete[] buffer;
        buffer = NULL;
    }

    if (send_buf) delete[] send_buf;
    send_buf = NULL;
}
