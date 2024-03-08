#ifndef __SERVER_HPP__ // Header guard
#define __SERVER_HPP__
#include <iostream>
#include <string> // resize e outras facilidades 
#include <algorithm> // std::fill
#include <typeinfo> // std::reinterpret_cast
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <utility>
#include "database_connector.hpp"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Constantes
constexpr int SERVER_PORT=8080;
constexpr int NUM_OF_REQUESTS_QUEUED=5; // Número de requisições de conexão que serão aceitas;
constexpr int SOCKET_BIND_SUCCESS=0;
constexpr int SOCKET_SET_TO_LISTEN=0;
constexpr int MAX_MESSAGE_SIZE=256;
constexpr char NUL_DELIMITER='\0';
const char* MESSAGE_READ="Mensagem Lida!";
constexpr size_t MESSAGE_READ_SIZE=sizeof(MESSAGE_READ);
const char* GOODBYE_MESSAGE="Goodbye";
constexpr size_t GOODBYE_MESSAGE_SIZE=sizeof(GOODBYE_MESSAGE);
// Definições de tipo
typedef int SOCKET_FILE_DESCRIPTOR;
typedef int CLIENT_FILE_DESCRIPTOR;
typedef enum SOCKET_CREATION_STATUS {
	SUCCESS = 0,
	FAILURE = -1
} SOCKET_CREATION_STATUS;
typedef enum SERVER_OPERATION {
	NAME_SEARCH,
	NOT_FOUND,
} SERVER_OPERATION;

// Funções inline
inline void BzeroCppString(std::string& str) {
	str.resize(MAX_MESSAGE_SIZE);
	std::fill(str.begin(), str.end(), NUL_DELIMITER);
}

inline void WriteToClient(CLIENT_FILE_DESCRIPTOR& client_socket_file_descriptor, std::string& message) {
	char* as_c_string=const_cast<char*>(message.data());
	size_t size_of_c_string = message.size();
	write(client_socket_file_descriptor, as_c_string, size_of_c_string);
}

inline bool TypedExit(std::string& client_message) {
	char exit_command[]="exit";
	constexpr size_t EXIT_SIZE=4;
	const char* as_c_string=client_message.c_str();
	return strncmp(exit_command, as_c_string, EXIT_SIZE) == 0;
}

// Funções
SOCKET_CREATION_STATUS CreateSocket(SOCKET_FILE_DESCRIPTOR* socket_file_descriptor) noexcept;

void ConfigureServerAddress(sockaddr_in& serverAddress) noexcept;

void BindSocket(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor, SA* server_address, socklen_t& socket_length);

void PrepareSocketToListen(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor);

CLIENT_FILE_DESCRIPTOR AcceptClient(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor, SA* client_address, socklen_t& client_socket_len);

void CommunicateWithClient(CLIENT_FILE_DESCRIPTOR& client_socket_file_descriptor, Database& db);

std::string InterpretateCommand(std::string& command_string, Database& db);

void ShowMessageSent(const std::string& message);
#endif // __SERVER_HPP__