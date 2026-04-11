#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"
#include <string>

class FrameProcessor {
public:
    FrameProcessor();
    void process(cv::Mat& frame, Mode mode);

    static void setupCallbacks(const std::string& windowName);
    static void onMouse(int event, int x, int y, int flags, void* userdata);

    static int brightness;

private:
    static bool isDrawing;
    static cv::Point drawingStart;
    static cv::Rect drawnRect;
};