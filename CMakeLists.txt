cmake_minimum_required(VERSION 3.8)
project(cs351_spring19_hnguye19)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES mps/02/hashtable.c mps/02/main.c)
add_executable(hashtable ${SOURCE_FILES})