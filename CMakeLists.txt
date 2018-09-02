cmake_minimum_required(VERSION 3.10)
project(rwlock C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-O3")
add_executable(rwlock main.c list.c rwlock.c my_rand.c)
target_link_libraries(rwlock pthread)
