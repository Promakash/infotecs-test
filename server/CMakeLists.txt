add_library(server_modules
        handler.cpp
        socketserver.cpp
)

add_executable(server_executable main.cpp)

target_link_libraries(server_executable server_modules)