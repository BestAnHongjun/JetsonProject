#ifndef __REALSENSE_CAMERA_H__
#define __REALSENSE_CAMERA_H__


#include "global_config.h"

#include "opencv.h"
#include <thread>
#include <librealsense2/rs.hpp>


class RealsenseCam
{
private:
    uint32_t width;
    uint32_t height;

    rs2::colorizer  color_map;      // 彩色图
    rs2::pipeline   pipe;           // realsense管道
    rs2::config     pipe_config;    // 数据流配置信息
    rs2::pipeline_profile profile;

    cv::Mat color_image, depth_image;

public:
    RealsenseCam(uint32_t width=REALSENSE_WIDTH, uint32_t height=REALSENSE_HEIGHT,
    uint16_t fps=REALSENSE_FPS);
    void get_all(cv::Mat &color_image, cv::Mat &depth_image);
};

#endif // __REALSENSE_CAMERA_H__