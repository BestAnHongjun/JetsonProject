#ifndef __OPENCV_H__
#define __OPENCV_H__

#include "global_config.h"

#include <stdint.h>
#include <opencv2/opencv.hpp>

void MatToJpgBuffer(const cv::Mat mat, std::vector<uint8_t>& buff, int quality=95);

#endif // __OPENCV_H__