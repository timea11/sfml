cmake_minimum_required(VERSION 3.23)

project(box_game)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(box-app)

target_include_directories(box-app PRIVATE inc)

target_sources(box-app PRIVATE src/box_main.cpp src/box.cpp src/box_map.cpp)

target_link_libraries(box-app PRIVATE sfml-graphics sfml-window sfml-system)

