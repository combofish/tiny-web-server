cmake_minimum_required(VERSION 3.12)

#project(Tiny_Web_Server_CPP)
#set(CMAKE_BUILD_TYPE DEBUG)

# set(CMAKE_C_FLAGS "-lpthread")


include_directories(.)

find_package(Threads REQUIRED)

#add_executable(pthread_create pthread_create.c)
#target_link_libraries(pthread_create PRIVATE Threads::Threads)
#
#add_executable(pthread_exit pthread_exit.c)
#target_link_libraries(pthread_exit PRIVATE Threads::Threads)
#
#add_executable(pthread_join pthread_join.c)
#target_link_libraries(pthread_join PRIVATE Threads::Threads)
#
#add_executable(pthread_detach pthread_detach.c)
#target_link_libraries(pthread_detach PRIVATE Threads::Threads)
#
#add_executable(pthread_cancel pthread_cancel.c)
#target_link_libraries(pthread_cancel PRIVATE Threads::Threads)
#
#add_executable(pthread_attr pthread_attr.c)
#target_link_libraries(pthread_attr PRIVATE Threads::Threads)
#
#add_executable(sell_tickets sell_tickets.c)
#target_link_libraries(sell_tickets PRIVATE Threads::Threads)
#
#add_executable(mutex mutex.c)
#target_link_libraries(mutex PRIVATE Threads::Threads)
#
#add_executable(dead_lock dead_lock.c)
#target_link_libraries(dead_lock PRIVATE Threads::Threads)
#
#add_executable(dead_lock_ii dead_lock_ii.c)
#target_link_libraries(dead_lock_ii PRIVATE Threads::Threads)
#
#add_executable(rwlock rwlock.c)
#target_link_libraries(rwlock PRIVATE Threads::Threads)
#
#add_executable(producer_customer producer_customer.c)
#target_link_libraries(producer_customer PRIVATE Threads::Threads)
#
#add_executable(cond cond.c)
#target_link_libraries(cond PRIVATE Threads::Threads)
#
#add_executable(semaphore semaphore.c)
#target_link_libraries(semaphore PRIVATE Threads::Threads)