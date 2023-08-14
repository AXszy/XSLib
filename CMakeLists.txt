# 指定最低版本的 CMake
cmake_minimum_required(VERSION 3.20)

# 定义工程名称
project(XSLib)

# 定义头文件路径
include_directories(${PROJECT_SOURCE_DIR}/inc)

# 定义库路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 获得源文件列表
file(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.*)

# 链接动态库
link_libraries(winmm msimg32 ole32 sapi gdi32)

# 设置输出的可执行文件名
add_executable(${PROJECT_NAME} ${SRC_LIST})