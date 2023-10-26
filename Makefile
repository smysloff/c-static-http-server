all: clean build run

clean:
	@rm -rf c_http_server

build:
	@gcc src/*\.c -o c_http_server -Wall -Werror -Wextra -pedantic -O3

run:
	@./c_http_server

server:
	@./c_http_server 1>/dev/null 2>/dev/null & disown

ps:
	@ps aux | grep c_http_server

.PHONY: all clean build run server ps
