#!/bin/bash
echo "Починаємо збірку проєкту..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "Збірка успішна!"