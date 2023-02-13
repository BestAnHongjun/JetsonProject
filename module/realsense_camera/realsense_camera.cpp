#include "realsense_camera.h"


RealsenseCam::RealsenseCam(uint16_t mode, uint32_t width, 
uint32_t height, uint16_t fps, bool enable_color, bool enable_depth)
: que_color(REALSENSE_QUE_SIZE), que_depth(REALSENSE_QUE_SIZE)
{
    capture_handle = false;
    this->mode = mode;
    this->width = width;
    this->height = height;
    this->enable_color = enable_color;
    this->enable_depth = enable_depth;

    if (enable_color)
    {
        pipe_config.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, fps);
    }
    if (enable_depth)
    {
        pipe_config.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, fps);
    }
    
    profile = pipe.start(pipe_config);
    capture_handle = true;
    capture_thread = std::thread(&RealsenseCam::capture_work, this);
}


void RealsenseCam::capture_work()
{
    rs2::frameset data;
    rs2::frame color;
    rs2::frame depth;

    uint16_t color_w, color_h;
    uint16_t depth_w, depth_h;

    cv::Mat color_image, depth_image;
    std::vector<uint8_t> buffer;
    
    while (capture_handle)
    {
        data = pipe.wait_for_frames();

        if (enable_color)
        {
            color = data.get_color_frame();
        }

        if (enable_depth)
        {
            depth = data.get_depth_frame().apply_filter(color_map);
        }

        if (enable_color)
        {
            color_w = color.as<rs2::video_frame>().get_width();
            color_h = color.as<rs2::video_frame>().get_height();
            color_image = cv::Mat(cv::Size(color_w,color_h), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
            if (mode == MODE_MAT)
            {
                que_color.push((uint8_t*)color_image.data, height * color_image.step);
            }
            else
            {
                MatToJpgBuffer(color_image, buffer);
                que_color.push((uint8_t*)&buffer, buffer.size());
            }
        }

        if (enable_depth)
        {
            depth_w = depth.as<rs2::video_frame>().get_width();
            depth_h = depth.as<rs2::video_frame>().get_height();
            depth_image = cv::Mat(cv::Size(depth_w, depth_h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
            if (mode = MODE_MAT)
            {
                que_depth.push((uint8_t*)depth_image.data, height * depth_image.step);
            }
            else
            {
                MatToJpgBuffer(depth_image, buffer);
                que_depth.push((uint8_t*)&buffer, buffer.size());
            }
        }
    }
}


void RealsenseCam::start()
{
    profile=pipe.start(pipe_config);
}