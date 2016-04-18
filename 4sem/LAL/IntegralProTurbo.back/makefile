FLAGS=-Wall -Werror -O0 -pthread 
all: server client
server: server.o server_main.o integral.o
	gcc $(FLAGS) -o ./server ./ObjModules/server.o ./ObjModules/server_main.o ./ObjModules/integral.o -lm
client: client.o client_main.o
	gcc $(FLAGS) -o ./client ./ObjModules/client.o ./ObjModules/client_main.o -lm
server.o: ./Sources/server.c
	gcc $(FLAGS) -c -o ./ObjModules/server.o ./Sources/server.c -lm
client.o: ./Sources/client.c
	gcc $(FLAGS) -c -o ./ObjModules/client.o ./Sources/client.c -lm
server_main.o: ./Sources/server_main.c ./Headers/server.h ./Headers/common.h 
	gcc $(FLAGS) -c -o ./ObjModules/server_main.o ./Sources/server_main.c -lm
client_main.o: ./Sources/client_main.c ./Headers/client.h ./Headers/common.h 
	gcc $(FLAGS) -c -o ./ObjModules/client_main.o ./Sources/client_main.c -lm
integral.o: ./Sources/integral.c ./Headers/integral.h 
	gcc $(FLAGS) -c -o ./ObjModules/integral.o ./Sources/integral.c -lm
