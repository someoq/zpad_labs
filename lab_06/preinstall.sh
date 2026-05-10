#!/bin/bash
echo "Встановлюємо необхідні бібліотеки для OpenCV та збірки..."
sudo apt update
sudo apt install -y build-essential cmake git libopencv-dev
echo "Встановлення завершено!"