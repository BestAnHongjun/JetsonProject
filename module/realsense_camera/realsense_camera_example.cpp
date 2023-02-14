#include "realsense_camera.h"
#include "timer.h"
#include <unistd.h>

int main() try
{
    RealsenseCam cam;
    TIMER(Timer timer(std::string("main_loop")));
    
    const char* depth_win = "depth_Image";
    cv::namedWindow(depth_win, cv::WINDOW_AUTOSIZE);
    const char* color_win = "color_Image";
    cv::namedWindow(color_win, cv::WINDOW_AUTOSIZE);

    while (cv::waitKey(1) && cvGetWindowHandle(depth_win))
    {
        TIMER(timer.start());
        cv::Mat color, depth;
        
        cam.get_all(color, depth);
        cv::imshow(depth_win, depth);
        cv::imshow(color_win, color);

        TIMER(timer.end());
    }

    return EXIT_SUCCESS;
}

catch (const rs2::error &e){
    std::cout<<"RealSense error calling"<<e.get_failed_function()<<"("<<e.get_failed_args()<<"):\n"
            <<e.what()<<std::endl;
    return EXIT_FAILURE;
}

catch (const std::exception &e){
    std::cout<<e.what()<<std::endl;
    return EXIT_FAILURE;
}