#ifndef __CLIENT_H__ //Header guard
#define __CLIENT_H__
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 80
#define SERVER_RESPONSE_MAX_LENGTH 256
#define DEFAULT_PORT 8080
#define SA struct sockaddr
#define LOCALHOST "127.0.0.1"
#define EXIT_COMM "exit"
#define EXIT_COMM_SIZE 4
#define CONNECTION_SUCCESS 0
#define DEFAULT_PROTOCOL 0
#define BAD_DESCRIPTOR -1
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

typedef enum SOCKET_CREATION_STATUS {
    SUCCESS = 1,
    FAILURE = 0
} SOCKET_CREATION_STATUS;

// Descritor da conexao do socket, se for -1 (BAD_DESCRIPTOR), falhou
typedef int SOCKET_FILE_DESCRIPTOR;

// Status da conexão FTP
typedef int FTP_CONNECTION_STATUS;

// Conecta ao servidor FTP
void CommunicateWithFTPServer(SOCKET_FILE_DESCRIPTOR socket_file_desc);

// Verifica se foi digitado "exit" em ambas as partes (servidor e cliente)
int TypedExit(char* command);

// Espera o comando do usuário enquanto digita
void WaitTyping(char* buff, int keyboard_pivot);

// Cria o socket do cliente
SOCKET_CREATION_STATUS CreateSocket(SOCKET_FILE_DESCRIPTOR* socket_file_desc);

// Conecta o socket do cliente ao servidor FTP
FTP_CONNECTION_STATUS ConnectToFTPServer(SOCKET_FILE_DESCRIPTOR* socket_file_desc, SA* server_address, socklen_t* socket_len);

//Insere as informações do socket
socklen_t ConfigureSocketAddress(struct sockaddr_in* server_address);

void PrintInput(const char* message) {
    printf(BLUE "%s" RESET, message);
}

#endif //__CLIENT_H__