#include "Display.hpp"

Display::Display(const std::string& windowName) : name(windowName) {
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& frame) {
    cv::imshow(name, frame);
}

std::string Display::getWindowName() const {
    return name;
}