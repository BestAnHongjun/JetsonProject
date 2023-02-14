#include "opencv.h"
#include <cstdio>


int main()
{
    cv::Mat mat = cv::Mat::zeros(cv::Size(1920, 1080), CV_8UC3);
    std::vector<uint8_t> jpg_buf;
    MatToJpgBuffer(mat, jpg_buf);
    printf("size:%ld bytes.\n", jpg_buf.size());
    return 0;
}
