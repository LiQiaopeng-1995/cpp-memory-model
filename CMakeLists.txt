cmake_minimum_required(VERSION 3.21)

project(cpp_demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /ZI /Od")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
    "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE
    "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

add_executable(cpu_reorder 1_cpu_reorder.cpp)

add_executable(atomic_relaxed 2_atomic_relaxed.cpp)

# # 设置自己的OpenCV库路径
# set(OpenCV_DIR
#     "D:/project/scc-nt-fakepoint-sdk/build/_deps/opencv-src/x64/vc15/lib")

find_package(OpenCV)
find_package(OpenMP REQUIRED)

add_executable(mat_cal 3_mat_cal.cpp)

target_include_directories(mat_cal PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(mat_cal ${OpenCV_LIBS}  OpenMP::OpenMP_CXX)

add_executable(atomic_acq_rel 4_atomic_acq_rel.cpp)

add_executable(double_rel_acq 5_double_rel_acq.cpp)

add_executable(sync_acq_rel 6_sync_acq_rel.cpp)

add_executable(fence 7_fence.cpp)
