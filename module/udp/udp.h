#ifndef __UDP_H__
#define __UDP_H__

#include "global_config.h"
#include "bufqueue.h"

#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>


#if UDP_SWITCH == 1
    #define UDP_DEBUG(order) order
#else 
    #define UDP_DEBUG(order) {}
#endif

struct UDPHead
{
    struct timeval tv;
    uint16_t id;
    uint16_t total;
    uint16_t size;
};

struct RecvHead
{
    double delay;
    uint32_t size;
    uint8_t* buffer;
};

class UDPSender
{
private:
    int fd;
    uint16_t pack_size;
    uint8_t* buffer;
    BufQueue que;

    bool work_handle;
    std::thread work_thread;
    void work();

    struct sockaddr_in target_addr;

public:
    UDPSender(const char* target_ip, const int target_port, 
    uint16_t que_size=UDP_QUE_SIZE, uint16_t pack_size=UDP_PACK_SIZE);
    ~UDPSender();

    void send(uint8_t* buf, uint32_t size);
};

class UDPReceiver
{
private:
    int fd;
    uint16_t pack_size;
    uint8_t* buffer;

    bool work_handle;
    bool asynch;
    std::thread work_thread;
    void work();
    void (*callback_func) (RecvHead&);

    struct sockaddr_in ser_addr;
    void init(const int recv_port, uint16_t pack_size);

public:
    UDPReceiver(const int recv_port, uint16_t pack_size=UDP_PACK_SIZE);
    UDPReceiver(const int recv_port, void (*callback)(RecvHead&), uint16_t pack_size=UDP_PACK_SIZE);
    ~UDPReceiver();

    void recv(RecvHead &recv_head);
};

#endif // __UDP_H__