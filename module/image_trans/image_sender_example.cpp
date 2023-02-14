#include "image_trans.h"


int main()
{
    IMGSender img_sender("192.168.137.97", 8888, 8889);
    cv::Mat mat = cv::Mat::zeros(cv::Size(1920, 1080), CV_8UC3);
    for (int i = 0; i < 1000; i++)
    {
        img_sender.push(mat);
    }
    return 0;
}