#include "image_trans.h"
#include "realsense_camera.h"
#include "timer.h"
#include <unistd.h>

int main() try
{
    RealsenseCam cam;
    IMGSender img_sender("192.168.137.28", 8888, 8889, 60);
    TIMER(Timer timer(std::string("main_loop")));

    while (1)
    {
        TIMER(timer.start());
        cv::Mat color, depth;
        
        cam.get_all(color, depth);
        img_sender.push(color);

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