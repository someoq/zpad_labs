#pragma once

enum class Mode {
    NORMAL,
    INVERT,
    BLUR,
    CANNY
};

class KeyProcessor {
public:
    KeyProcessor();
    bool processKey(int key);
    Mode getCurrentMode() const;
private:
    Mode currentMode;
};