#include "FrameProcessor.hpp"

int FrameProcessor::brightness = 50;
bool FrameProcessor::isDrawing = false;
cv::Point FrameProcessor::drawingStart(0, 0);
cv::Rect FrameProcessor::drawnRect(0, 0, 0, 0);

FrameProcessor::FrameProcessor() {}

void FrameProcessor::setupCallbacks(const std::string& windowName) {
    cv::createTrackbar("Brightness", windowName, &brightness, 100);
    cv::setMouseCallback(windowName, FrameProcessor::onMouse, nullptr);
}

void FrameProcessor::onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        isDrawing = true;
        drawingStart = cv::Point(x, y);
        drawnRect = cv::Rect(x, y, 0, 0);
    } else if (event == cv::EVENT_MOUSEMOVE && isDrawing) {
        drawnRect = cv::Rect(drawingStart, cv::Point(x, y));
    } else if (event == cv::EVENT_LBUTTONUP) {
        isDrawing = false;
        drawnRect = cv::Rect(drawingStart, cv::Point(x, y));
    }
}

void FrameProcessor::process(cv::Mat& frame, Mode mode) {
    switch (mode) {
        case Mode::INVERT:
            cv::bitwise_not(frame, frame);
            break;
        case Mode::BLUR:
            cv::GaussianBlur(frame, frame, cv::Size(15, 15), 0);
            break;
        case Mode::CANNY:
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
            cv::Canny(frame, frame, 50, 150);
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
            break;
        case Mode::NORMAL:
        default:
            break;
    }

    int actualBrightness = brightness - 50;
    frame.convertTo(frame, -1, 1.0, actualBrightness);

    if (drawnRect.width != 0 && drawnRect.height != 0) {
        cv::rectangle(frame, drawnRect, cv::Scalar(0, 0, 255), 2);
    }

    cv::putText(frame, "1:Normal 2:Invert 3:Blur 4:Canny | ESC:Exit",
                cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}