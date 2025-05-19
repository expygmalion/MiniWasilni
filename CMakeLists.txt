cmake_minimum_required(VERSION 3.10)
project(MiniWasilni VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 COMPONENTS Core Widgets Svg REQUIRED)

# Enable Qt Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set include directories
include_directories(include)

# Core library files
set(CORE_SOURCES
    src/Graph.cpp
    src/Traversal.cpp
    src/IOManager.cpp
    src/Dijkstra.cpp
    src/AStar.cpp
    src/Utilities.cpp
    src/FindPath.cpp
)

# GUI files - specify headers explicitly for MOC processing
set(GUI_SOURCES
    src/gui/MainWindow.cpp
    src/gui/main.cpp
)

set(GUI_HEADERS
    include/gui/MainWindow.hpp
)

# Command-line application
add_executable(MiniWasilni src/main.cpp ${CORE_SOURCES})

# GUI application - add headers explicitly for MOC
add_executable(MiniWasilniGUI ${GUI_SOURCES} ${GUI_HEADERS} ${CORE_SOURCES})
target_include_directories(MiniWasilniGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MiniWasilniGUI Qt5::Core Qt5::Widgets Qt5::Svg)

# Create data directory for saving/loading graph files
add_custom_command(
    TARGET MiniWasilniGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
    COMMENT "Creating data directory for graph storage"
)

# Add installation rules
install(TARGETS MiniWasilni MiniWasilniGUI
    RUNTIME DESTINATION bin
)

# Print message about GraphViz requirement
message(STATUS "Note: GraphViz must be installed for the GUI application to visualize graphs.") 