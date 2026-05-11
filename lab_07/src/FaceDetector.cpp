#include "FaceDetector.hpp"
#include <iostream>
#include <chrono>
FaceDetector::FaceDetector(const std::string& prototxtPath, const std::string& modelPath) {
    net = cv::dnn::readNetFromCaffe(prototxtPath, modelPath);
    isRunning = false;
    hasNewFrame = false;
}

FaceDetector::~FaceDetector() {
    stop();
}

void FaceDetector::start() {
    if (!isRunning) {
        isRunning = true;
        workerThread = std::thread(&FaceDetector::workerLoop, this);
    }
}

void FaceDetector::stop() {
    isRunning = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::setFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(dataMutex);
    frame.copyTo(currentFrame);
    hasNewFrame = true;
}

void FaceDetector::drawDetections(cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(dataMutex);
    if (detections.empty() || frame.empty()) return;

    cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > 0.5) {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 3);
            cv::putText(frame, "Face", cv::Point(x1, y1 - 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
        }
    }
}

void FaceDetector::workerLoop() {
    while (isRunning) {
        cv::Mat frameForInference;

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            if (hasNewFrame && !currentFrame.empty()) {
                currentFrame.copyTo(frameForInference);
                hasNewFrame = false;
            }
        }

        if (!frameForInference.empty()) {
            cv::Mat blob = cv::dnn::blobFromImage(frameForInference, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);

            cv::Mat currentDetections = net.forward();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            {
                std::lock_guard<std::mutex> lock(dataMutex);
                currentDetections.copyTo(detections);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}