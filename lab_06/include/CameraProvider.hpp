#pragma once
#include <opencv2/opencv.hpp>

class CameraProvider {
public:
    CameraProvider(int cameraId = 0);
    ~CameraProvider();
    cv::Mat getFrame();
private:
    cv::VideoCapture cap;
};