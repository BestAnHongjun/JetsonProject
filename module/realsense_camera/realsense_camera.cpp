#include "realsense_camera.h"


RealsenseCam::RealsenseCam(uint32_t width, uint32_t height, uint16_t fps)
{
    this->width = width;
    this->height = height;
    pipe_config.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, fps);
    pipe_config.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, fps);
    profile = pipe.start(pipe_config);
}

void RealsenseCam::get_all(cv::Mat &color_image, cv::Mat &depth_image)
{
    rs2::frameset data=pipe.wait_for_frames();//等待下一帧
 
    rs2::frame depth = data.get_depth_frame().apply_filter(color_map);//获取深度图，加颜色滤镜
    rs2::frame color = data.get_color_frame();

    //获取宽高
    const int depth_w=depth.as<rs2::video_frame>().get_width();
    const int depth_h=depth.as<rs2::video_frame>().get_height();
    const int color_w=color.as<rs2::video_frame>().get_width();
    const int color_h=color.as<rs2::video_frame>().get_height();

    //创建OPENCV类型 并传入数据
    depth_image = cv::Mat(cv::Size(depth_w,depth_h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
    color_image = cv::Mat(cv::Size(color_w,color_h), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
}
