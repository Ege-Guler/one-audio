# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Define the project name and language
project(one-audio LANGUAGES CXX)

# Set the C++ standard (e.g., C++17, C++20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    src/device/device.h
    src/audio/filter/filter.h
)


set(SOURCES
    src/main.cpp
    src/device/device.cpp
    src/audio/filter/filter.cpp

)
include_directories(include)
add_executable(one-audio ${SOURCES} ${HEADERS})

# Link the ALSA library
target_link_libraries(one-audio asound fftw3)