# CMakeLists.txt
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.15)

set(VERSION "0.1.1")

# 项目信息
project(OpenGLStudyDemo)

# include路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include )

# 把 第三方 库 的 .a 文件和 .h 文件放到 代码的 lib 文件夹
link_directories(
    ${PROJECT_SOURCE_DIR}/lib)

# 设置存放编译时产生的链接库输出到lib文件夹下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

# 输出 main.exe 与 vscode 配置保持一致，即可进行调试
add_executable(main src/main.cpp)