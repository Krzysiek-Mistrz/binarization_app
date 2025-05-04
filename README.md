# Binarization App

A Qt5 + OpenCV C++ application that lets you manually or automatically threshold each color channel of an image.

## Project Structure

```
.
├── CMakeLists.txt
├── include
│   └── mainwindow.hpp
├── src
│   ├── main.cpp
│   └── mainwindow.cpp
├── resources
│   ├── resources.qrc
│   └── style.qss
└── README.md
```

## Requirements

- Qt5 Widgets
- OpenCV 4
- CMake ≥ 3.10
- A C++17-capable compiler

## Build & Run

```bash
mkdir build
cd build
cmake ..
make
./binarization_app
```