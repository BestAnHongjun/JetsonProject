#include "image_trans.h"

IMGSender::IMGSender(const char* target_ip, uint16_t target_port, uint16_t local_port, int quality)
: udp_sender(target_ip, target_port), udp_receiver(local_port)
{
    jpg_quality = quality;

    ttl_handle = true;
    ttl_thread = std::thread(&IMGSender::ttl_func, this);
}

IMGSender::~IMGSender()
{
    ttl_handle = false;
    ttl_thread.join();
}

void IMGSender::ttl_func()
{
    RecvHead head;
    while (ttl_handle)
    {
        udp_receiver.recv(head);
        if (head.buffer == NULL) continue;
        udp_sender.send(head.buffer, head.size);
        delete[] head.buffer;
    }
}

void IMGSender::push(cv::Mat image)
{
    std::vector<uint8_t> buffer;
    MatToJpgBuffer(image, buffer, jpg_quality);
    udp_sender.send((uint8_t*)&buffer, buffer.size());
}
