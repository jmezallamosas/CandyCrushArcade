cmake_minimum_required(VERSION 3.23)
project(CandyCrush_V3)

set(CMAKE_CXX_STANDARD 14)

add_executable(CandyCrush_V3 main.cpp SerialPort.h SerialPort.cpp Button.cpp Button.h Frame.cpp Frame.h Board.cpp Board.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(CandyCrush_V3 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
