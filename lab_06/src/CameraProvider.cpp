#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int cameraId) {
    cap.open(cameraId);
    if (!cap.isOpened()) {
        std::cerr << "Помилка: Неможливо відкрити камеру!" << std::endl;
        exit(1);
    }
}

CameraProvider::~CameraProvider() {
    cap.release();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}