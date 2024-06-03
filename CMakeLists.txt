cmake_minimum_required(VERSION 3.5)

project(Canvas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置安装路径
set(INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/install)

add_subdirectory(submodule/redcgl)
add_subdirectory(libs)
add_subdirectory(app)

