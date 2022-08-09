cmake_minimum_required(VERSION 3.10)
project(Xonix)

set(CMAKE_CXX_STANDARD 11)

configure_file(images/enemy.png images/enemy.png COPYONLY)
configure_file(images/gameover.png images/gameover.png COPYONLY)
configure_file(images/tiles.png images/tiles.png COPYONLY)

configure_file(sounds/sound.ogg sounds/sound.ogg COPYONLY)

configure_file(fonts/Arcade.ttf fonts/Arcade.ttf COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "Xonix")

add_executable(${EXECUTABLE_NAME} main.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()