add_library(client_modules
        socketclient.cpp
        reader.cpp
        processor.cpp
)

add_executable(client_executable main.cpp)

target_link_libraries(client_executable client_modules)