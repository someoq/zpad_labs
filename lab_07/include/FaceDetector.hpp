#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>

class FaceDetector {
public:
    FaceDetector(const std::string& prototxtPath, const std::string& modelPath);
    ~FaceDetector();
    void start();
    void stop();
    void setFrame(const cv::Mat& frame);
    void drawDetections(cv::Mat& frame);

private:
    void workerLoop();
    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex dataMutex;
    std::atomic<bool> isRunning;

    cv::Mat currentFrame;
    bool hasNewFrame;
    cv::Mat detections;
};

#endif