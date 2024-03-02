# Makefile for ftp-server

# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -g
CXXFLAGS = -Wall -Wextra -g

# Directories
SERVER_DIR = server
CLIENT_DIR = client

# Executables
SERVER_EXEC = server_executable
CLIENT_EXEC = client_executable

# Source files
SERVER_SRC = $(SERVER_DIR)/server.cpp
CLIENT_SRC = $(CLIENT_DIR)/client.c

# Object files
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Header files
SERVER_HEADER = $(SERVER_DIR)/server.hpp
CLIENT_HEADER = $(CLIENT_DIR)/client.h

all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_OBJ): $(SERVER_SRC) $(SERVER_HEADER)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SERVER_SRC)

$(CLIENT_OBJ): $(CLIENT_SRC) $(CLIENT_HEADER)
	$(CC) $(CFLAGS) -c -o $@ $(CLIENT_SRC)

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_EXEC) $(CLIENT_EXEC)

.PHONY: all clean
