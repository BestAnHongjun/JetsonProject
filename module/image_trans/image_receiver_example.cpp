#include "image_trans.h"
#include "opencv.h"
#include <cstdio>

const char* win = "View";

void call_back(cv::Mat mat, double delay, double lost_rate)
{
    printf("Recv %ld bytes, delay:%.5lf ms, lost_rate: %.2lf% \n", mat.rows * mat.step[0], delay, lost_rate * 100);
    cv::imshow(win, mat);
    cv::waitKey(1);
}

int main()
{
    cv::namedWindow(win, cv::WINDOW_AUTOSIZE);
    IMGReceiver img_receiver(8888, "192.168.137.97", 8889, call_back);
    
    while (1);
    return 0;
}