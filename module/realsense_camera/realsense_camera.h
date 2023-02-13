#ifndef __REALSENSE_CAMERA_H__
#define __REALSENSE_CAMERA_H__


#include "global_config.h"

#include "opencv.h"
#include "bufqueue.h"
#include <thread>
#include <librealsense2/rs.hpp>

#define MODE_MAT    0
#define MODE_JPGBUF 1

class RealsenseCam
{
private:
    uint16_t mode;
    uint32_t width;
    uint32_t height;

    rs2::colorizer  color_map;      // 彩色图
    rs2::pipeline   pipe;           // realsense管道
    rs2::config     pipe_config;    // 数据流配置信息
    rs2::pipeline_profile profile;

    bool enable_color;
    bool enable_depth;

    BufQueue que_color;
    BufQueue que_depth;
    bool capture_handle;
    std::thread capture_thread;
    void capture_work();

public:
    RealsenseCam(uint16_t mode=MODE_MAT, uint32_t width=REALSENSE_WIDTH, 
    uint32_t height=REALSENSE_HEIGHT, uint16_t fps=REALSENSE_FPS, 
    bool enable_color=true, bool enable_depth=true);
    void start();
};

#endif // __REALSENSE_CAMERA_H__