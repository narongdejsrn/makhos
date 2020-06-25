cmake_minimum_required(VERSION 3.16)
project(makhos)

set(CMAKE_CXX_STANDARD 17)

find_library(SDL2 SDL2)
if (NOT SDL2)
    message(FATAL_ERROR "SDL2 not found")
endif()

find_library(TTF_SDL
        NAMES SDL2_ttf
        HINTS "/Library/Frameworks/SDL2_ttf.framework")
if (NOT TTF_SDL)
    message(FATAL_ERROR "SDL2_TTF not found")
endif()

find_library(IMAGE_SDL SDL2_image)
if (NOT IMAGE_SDL)
    message(FATAL_ERROR "SDL2_IMAGE not found")
endif()

include_directories(include)
file(GLOB SOURCES "src/*cpp")
add_executable(makhos ${SOURCES})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(makhos ${SDL2})
target_include_directories(makhos PRIVATE ${SDL2}/Headers)

target_link_libraries(makhos ${IMAGE_SDL})
target_include_directories(makhos PRIVATE ${IMAGE_SDL}/Headers)

target_link_libraries(makhos ${TTF_SDL})
target_include_directories(makhos PRIVATE ${TTF_SDL}/Headers)