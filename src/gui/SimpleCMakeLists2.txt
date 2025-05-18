cmake_minimum_required(VERSION 3.10)
project(SimpleMainWindowTest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Enable Qt Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Create executable
add_executable(SimpleMainWindowTest 
    simple_gui_main.cpp 
    SimpleMainWindow.cpp
)

target_link_libraries(SimpleMainWindowTest Qt5::Core Qt5::Widgets) 