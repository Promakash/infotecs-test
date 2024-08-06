build:
	mkdir build && cd build && cmake .. && make

run_server:
	build/server_executable

run_client:
	build/client_executable
