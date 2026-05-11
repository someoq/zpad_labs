#include "KeyProcessor.hpp"
#include <iostream>

KeyProcessor::KeyProcessor() : currentMode(Mode::NORMAL) {}

bool KeyProcessor::processKey(int key) {
    if (key == 27) return false;

    switch (key) {
        case '1': currentMode = Mode::NORMAL; std::cout << "Режим: NORMAL\n"; break;
        case '2': currentMode = Mode::INVERT; std::cout << "Режим: INVERT\n"; break;
        case '3': currentMode = Mode::BLUR; std::cout << "Режим: BLUR\n"; break;
        case '4': currentMode = Mode::CANNY; std::cout << "Режим: CANNY\n"; break;
    }
    return true;
}

Mode KeyProcessor::getCurrentMode() const {
    return currentMode;
}