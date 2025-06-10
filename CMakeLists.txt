cmake_minimum_required(VERSION 3.1)
project(log_demo)

# ************************ 设置路径变量 ************************ 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build)#设置可执行文件的输出目录
message("EXECUTABLE_OUTPUT_PATH is ${EXECUTABLE_OUTPUT_PATH}")
# 必须的编译文件路径
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")#cpp源码目录
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")#cpp头文件目录
set(LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")#编译后的链接库的目录

# 打印出一些信息
message("SRC_DIR is ${SRC_DIR}")
message("INCLUDE_DIR is ${INCLUDE_DIR}")
message("LIB_DIR is ${LIB_DIR}")

# ************************ 设置编译的配置 ************************ 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")#编译时产生调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -pthread")

set(CMAKE_CXX_COMPILER "/home/rksysu/Documents/cross_toolchain/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++")
set(CMAKE_C_COMPILER "/home/rksysu/Documents/cross_toolchain/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc")
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set(CMAKE_C_COMPILER "/usr/bin/gcc")
# set(CROSS_TOOLCHAIN_PREFIX "/opt/hisi-linux/x86-arm/aarch64-himix100-linux/bin/")
# set(CMAKE_CXX_COMPILER ${CROSS_TOOLCHAIN_PREFIX}aarch64-himix100-linux-g++)
# set(CMAKE_C_COMPILER ${CROSS_TOOLCHAIN_PREFIX}aarch64-himix100-linux-gcc)

# ************************ 加入第三方库 *********************** 
# set(OpenCV_DIR /home/zlx/Downloads/opencv-3.4/build_arm)
# find_package( OpenCV REQUIRED REQUIRED 
#                 PATHS  /home/rksysu/Documents/opencv4.10-contribe-arm/contrib-install
#                 NO_DEFAULT_PATH)

# message(STATUS "    OpenCV library status:")
# message(STATUS "    version: ${OpenCV_VERSION}")
# message(STATUS "    libraries: ${OpenCV_LIBS}")
# message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


find_package( spdlog  REQUIRED  
                PATHS ${CMAKE_CURRENT_LIST_DIR}/3rdparty/spdlog
                NO_DEFAULT_PATH)

# ************************ 把头文件添加进编译器中 ************
# include_directories( ${OpenCV_DIRS} )
include_directories(${INCLUDE_DIR})
# include_directories(${CMAKE_CURRENT_LIST_DIR}/include/spdlog/include)

# ************************ 添加源码 ************************ 
aux_source_directory(${SRC_DIR} SOURCES_VAR)

# ************************ 开始编译 ************************
# 编译静态库
# add_library(kcftracker STATIC ${SOURCES_VAR})

# 编译目标
add_executable(log_demo ${SOURCES_VAR})

# 添加额外的库
# target_link_libraries(log_demo.bin  ${OpenCV_LIBS})
target_link_libraries(log_demo PRIVATE spdlog::spdlog)


