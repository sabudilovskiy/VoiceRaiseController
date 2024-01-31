cmake_minimum_required(VERSION 3.27)
include(CreatePart.cmake)
project(VRC)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

#-----------------#
#----libraries----#
#-----------------#
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
#----------------#

#-----------------#
#------audio------#
#-----------------#
CreateTrivialPart(
        name "audio"
        path "/src/audio"
)
IF(NOT WIN32)
    target_link_libraries(${audio_module} PRIVATE "-framework OpenAL")
ENDIF ()
target_link_libraries(${audio_module} PRIVATE sfml-audio)
#----------------#

#----------------#
#######GUI########
#----------------#
CreateTrivialPart(
        name "gui"
        path "/src/gui"
)
target_link_libraries(${gui_module} PRIVATE ImGui-SFML::ImGui-SFML)

CreateTrivialExe(
        name "main"
        path "/src/app"
)

target_link_libraries(${main_exe} PRIVATE ${audio_module})
target_link_libraries(${main_exe} PRIVATE ${gui_module})
target_link_libraries(${main_exe} PRIVATE implot::implot)