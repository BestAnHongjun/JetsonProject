#ifndef __IMAGE_TRANS_H__
#define __IMAGE_TRANS_H__

#include "opencv.h"
#include "udp.h"
#include <thread>


class IMGSender
{
private:
    UDPSender udp_sender;
    UDPReceiver udp_receiver;

    bool recv_handle;
    std::thread recv_thread;



public:
    IMGSender(const char* target_ip, uint16_t target_port, uint16_t local_port);
    void push(cv::Mat image);
};

#endif