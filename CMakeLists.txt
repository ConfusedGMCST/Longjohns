cmake_minimum_required(VERSION 3.14)
project(longjohns LANGUAGES CXX)

# Find SDL3 and its components
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Define your executable
add_executable(my_game
    main.cpp
    longjohnmon.cpp
    texthandler.cpp
    buttonhandler.cpp
)

# Link the libraries
target_link_libraries(my_game PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)  
