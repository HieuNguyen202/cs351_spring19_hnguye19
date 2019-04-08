cmake_minimum_required(VERSION 3.8)
project(cs351_spring19_hnguye19)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES mps/02/hashtable.c mps/02/main.c)
add_executable(hashtable ${SOURCE_FILES})

set(SOURCE_FILES_3 mps/03/tsh.c)
add_executable(tsh ${SOURCE_FILES_3})

set(SOURCE_FILES_4 mps/04/csim.c mps/04/cachelab.c)
add_executable(csim ${SOURCE_FILES_4})

set(SOURCE_FILES_4_1 mps/04/mytrans.c)
add_executable(mytrans ${SOURCE_FILES_4_1})