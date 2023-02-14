#ifndef __IMAGE_TRANS_H__
#define __IMAGE_TRANS_H__

#include "opencv.h"
#include "udp.h"
#include <thread>


class IMGSender
{
private:
    int jpg_quality;

    UDPSender udp_sender;
    UDPReceiver udp_receiver;

    // 测传输延时
    bool ttl_handle;
    std::thread ttl_thread;
    void ttl_func();

public:
    IMGSender(const char* target_ip, uint16_t target_port, uint16_t local_port, int quality=95);
    ~IMGSender();
    void push(cv::Mat image);
};

class IMGReceiver
{
private:
    UDPSender udp_sender;
    UDPReceiver udp_receiver;

    // 测传输延时
    uint16_t time_cnt;
    double send_time[50];
    double trans_delay = 0;

    bool ttl_handle;
    std::thread ttl_thread;
    void ttl_func();

    // 接收函数
    bool recv_handle;
    std::thread recv_thread;
    void recv_func();

    // 测定丢包率
    uint64_t lost = 0;
    uint64_t recv = 0;

    // 回调函数
    void (*callback)(cv::Mat, double delay, double lost_rate);
    
public:
    IMGReceiver(uint16_t local_port, const char* target_ip, uint16_t target_port, 
    void (*callback_func)(cv::Mat, double, double));
    ~IMGReceiver();
};

#endif