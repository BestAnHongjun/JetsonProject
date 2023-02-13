#include "opencv.h"


void MatToJpgBuffer(const cv::Mat mat, std::vector<uint8_t>& buff, int quality)
{
    if (mat.empty())
    {
        return;
    }
    std::vector<int> param = std::vector<int>(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = quality;
    cv::imencode(".jpg", mat, buff, param);
    return;
}
