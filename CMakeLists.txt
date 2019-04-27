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

set(SOURCE_FILES_5 mps/05/mdriver.c)

set(mdriver.o mps/05/mdriver.c mps/05/fsecs.h mps/05/fcyc.h mps/05/clock.h mps/05/memlib.h mps/05/config.h mps/05/mm.h)
set(memlib.o mps/05/memlib.c mps/05/memlib.h)
set(mm.o mps/05/mm.c mps/05/mm.h mps/05/memlib.h)
set(fsecs.o mps/05/fsecs.c mps/05/fsecs.h mps/05/config.h)
set(fcyc.o mps/05/fcyc.c mps/05/fcyc.h)
set(ftimer.o mps/05/ftimer.c mps/05/ftimer.h mps/05/config.h)
set(clock.o mps/05/clock.c mps/05/clock.h)

add_executable(mdriver ${mdriver.o} ${memlib.o} ${mm.o} ${fsecs.o} ${fcyc.o} ${ftimer.o} ${clock.o})


set(SOURCE_FILES_5_1 mps/05/test.c)
add_executable(test ${SOURCE_FILES_5_1} ${memlib.o} ${mm.o})


#set (CMAKE_C_FLAGS -Wall -Werror -g)
#set (CMAKE_C_FLAGS -g)
#set (CMAKE_C_FLAGS -Werror)
