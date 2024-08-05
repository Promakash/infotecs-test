cmake_minimum_required(VERSION 3.13)

project (infotecs_task)

SET(CMAKE_CXX_STANDARD 14)

ADD_EXECUTABLE(server server/main.cpp
        server/handler.cpp
        server/handler.h)
ADD_EXECUTABLE(client client/main.cpp
        client/socketclient.h)