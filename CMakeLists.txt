cmake_minimum_required(VERSION 3.10)
project(hybrid_BC)
set (CMAKE_CXX_STANDARD 17)



find_package(Boost 1.58 REQUIRED COMPONENTS program_options)
find_package(CUDA REQUIRED)


add_subdirectory(library/yaml-cpp)

add_library(hybrid_BC INTERFACE)
target_include_directories(
  hybrid_BC
  INTERFACE 
  include
  library/yaml-cpp/include
)

file(GLOB SOURCES "include/hybrid_BC/*")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11" )

enable_language(CUDA)
cuda_add_executable(bc example/main.cpp ${SOURCES})
set_property(TARGET bc PROPERTY CUDA_STANDARD 11)


target_link_libraries(
  bc
  hybrid_BC
  ${Boost_LIBRARIES}
  yaml-cpp
)
