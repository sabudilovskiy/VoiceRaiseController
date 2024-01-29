cmake_minimum_required(VERSION 3.27)
project(VoiceRaiseController)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

##################
####libraries#####
##################
FetchContent_Declare(hello_imgui GIT_REPOSITORY https://github.com/pthom/hello_imgui.git GIT_TAG master)
FetchContent_MakeAvailable(hello_imgui)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

##################
#######src########
##################
add_library(${PROJECT_NAME}_src
        src/DecibelListener.cpp
        src/DecibelListener.hpp)
target_link_libraries(${PROJECT_NAME}_src PRIVATE sfml-audio)
##################

##################
#######GUI########
##################

hello_imgui_add_app(${PROJECT_NAME}_gui main_gui.cpp)
target_link_libraries(${PROJECT_NAME}_gui PRIVATE ${PROJECT_NAME}_src)
target_include_directories(${PROJECT_NAME}_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)