cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

project(TechnicalHack) # change the name here

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# this is so stupid
set(CMAKE_SIZEOF_VOID_P 4)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  # ensure 32 bit on clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
  add_definitions("--target=i386-pc-windows-msvc")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  libraries/minhook/include
  libraries/cocos-headers/cocos2dx/include
  libraries/imgui-hook
  libraries/imgui-hook/imgui
  libraries/libdetours
  libraries/libdetours/Detours/src
  libraries/gd.h
  libraries/gd.h/include
)

add_subdirectory(libraries/minhook)
add_subdirectory(libraries/cocos-headers)
add_subdirectory(libraries/imgui-hook)
add_subdirectory(libraries/libdetours)

target_link_libraries(${PROJECT_NAME} minhook cocos2d imgui-hook libdetours)