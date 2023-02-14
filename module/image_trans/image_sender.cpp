#include "image_trans.h"

IMGSender::IMGSender(const char* target_ip, uint16_t target_port, uint16_t local_port, int quality)
: udp_sender(target_ip, target_port), udp_receiver(local_port)
{
    jpg_quality = quality;
    time_cnt = 0;
}

void IMGSender::push(cv::Mat image)
{
    std::vector<uint8_t> buffer;
    
}

