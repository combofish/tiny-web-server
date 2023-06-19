cmake_minimum_required(VERSION 3.1.2)

project(Tiny_Web_Server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer \
                    -fno-optimize-sibling-calls -Wall -Werror -Wreturn-type")

#add_subdirectory(Preparation/Code_03)
#add_subdirectory(Preparation/Code_04)

find_package(Threads REQUIRED)

aux_source_directory(Preparation/Code_05 DIR_SRCS)
#add_executable(${PROJECT_NAME} ${DIR_SRCS})

#add_executable(tiny-web-server-with-timer Preparation/Code_05/no_active/nonactive_conn.cpp
#        Preparation/Code_05/locker.hpp
#        Preparation/Code_05/http_conn.hpp
#        Preparation/Code_05/thread_pool.hpp
#        )

#target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# （分时）循环服务器
#add_executable(udp_server Learn/Code01/main.cpp)
#add_executable(udp_client Learn/Code01/udp_client.cpp)

# add_executable(tcp_client Learn/chatSrv/tcp_client.cpp)
# add_executable(tcp_server Learn/TCP-Com/tcp_server.cpp)

#  多进程并发服务器
#add_executable(test_01 Learn/thread/test_01.cpp)
#target_link_libraries(test_01 PRIVATE Threads::Threads)

add_executable(epoll_srv Learn/epoll/epoll_srv.cpp)
add_executable(test_cgi Learn/cgi-bin-srv/test_cgi.cpp)

include_directories(/usr/include/mysql)

#add_executable(test_mysql Learn/mysql-use/test_mysql.cpp)
#target_link_libraries(test_mysql mysqlclient)

#add_executable(mydb Learn/chatSrv/mydb.cpp)
#target_link_libraries(mydb mysqlclient)

add_executable(chatSrv Learn/chatSrv/chatSrv.cpp Learn/chatSrv/mydb.cpp Learn/chatSrv/mydb.h)
target_link_libraries(chatSrv mysqlclient)

add_executable(tcp_client Learn/chatSrv/tcp_client.cpp)


add_executable(tcp_client_read Learn/chatSrv/tcp_client_read.cpp)
add_executable(test_fileno Learn/fileno/test_fileno.cpp)

# find_package(Libevent REQUIRED)
add_executable(test_event Learn/event/test_event.cpp)
target_link_libraries(test_event event)