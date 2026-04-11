#include <iostream>
#include <opencv2/opencv.hpp>
#include "FaceDetector.hpp"

void applyFilters(cv::Mat& frame, int mode) {
    if (mode == 2) {
        cv::bitwise_not(frame, frame);
    } else if (mode == 3) {
        cv::GaussianBlur(frame, frame, cv::Size(15, 15), 0);
    } else if (mode == 4) {
        cv::Mat gray, edges;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 50, 150);
        cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
    }
}

int main() {
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "Помилка: Неможливо відкрити веб-камеру!" << std::endl;
        return -1;
    }

    std::cout << "Запуск програми..." << std::endl;
    std::cout << "Гарячі клавіші:" << std::endl;
    std::cout << "1-4: Фільтри" << std::endl;
    std::cout << "F: Детекція обличчя (On/Off)" << std::endl;
    std::cout << "ESC: Вихід" << std::endl;

    FaceDetector faceDetector("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    faceDetector.start();

    bool faceMode = false;
    int currentFilterMode = 1;

    cv::namedWindow("Lab 7: Camera & Face Detection", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        if (faceMode) {
            faceDetector.setFrame(frame);
        }

        applyFilters(frame, currentFilterMode);

        if (faceMode) {
            faceDetector.drawDetections(frame);
        }

        cv::imshow("Lab 7: Camera & Face Detection", frame);

        int key = cv::waitKey(1) & 0xFF;
        if (key == 27) {
            break;
        } else if (key >= '1' && key <= '4') {
            currentFilterMode = key - '0';
        } else if (key == 'f' || key == 'F') {
            faceMode = !faceMode;
            std::cout << "Режим детекції обличчя: " << (faceMode ? "УВІМКНЕНО" : "ВИМКНЕНО") << std::endl;
        }
    }

    faceDetector.stop();
    cap.release();
    cv::destroyAllWindows();
    return 0;
}