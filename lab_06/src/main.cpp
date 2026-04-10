#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

int main() {
    CameraProvider camera(0);
    Display display("Lab 6 - OpenCV Magic");
    KeyProcessor keyProc;
    FrameProcessor frameProc;

    FrameProcessor::setupCallbacks(display.getWindowName());

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            break;
        }

        frameProc.process(frame, keyProc.getCurrentMode());

        display.show(frame);

        int key = cv::waitKey(30);
        if (!keyProc.processKey(key)) {
            break;
        }
    }

    return 0;
}