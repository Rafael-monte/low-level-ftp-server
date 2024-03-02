#include "../log.h"
#include "server.hpp"

int main() 
{ 
	SOCKET_FILE_DESCRIPTOR socket_file_descriptor;
	CLIENT_FILE_DESCRIPTOR client_file_descriptor;
	struct sockaddr_in servaddr, clientaddr; 
	auto creation_status = CreateSocket(&socket_file_descriptor);
	if (creation_status == SOCKET_CREATION_STATUS::FAILURE) {
		exit(EXIT_FAILURE);
	}
	LogSuccess("Socket do servidor criado com sucesso!");
	bzero(&servaddr, sizeof(servaddr)); 
	ConfigureServerAddress(servaddr);
	// Conversão de tamanho de memória de server address in -> server address
	SA* server_socket_address_ptr = reinterpret_cast<SA*>(&servaddr);
	socklen_t server_socket_address_len = static_cast<socklen_t>(sizeof(servaddr));
	BindSocket(socket_file_descriptor, server_socket_address_ptr, server_socket_address_len);
	PrepareSocketToListen(socket_file_descriptor);
	socklen_t client_socket_address_len = static_cast<socklen_t>(sizeof(clientaddr));
	SA* client_socket_address_ptr = reinterpret_cast<SA*>(&clientaddr);
	client_file_descriptor = AcceptClient(socket_file_descriptor, client_socket_address_ptr, client_socket_address_len);
	CommunicateWithClient(client_file_descriptor); 
	close(socket_file_descriptor); 
}

SOCKET_CREATION_STATUS CreateSocket(SOCKET_FILE_DESCRIPTOR* socket_file_descriptor) noexcept {
	LogInfo("Criando socket para o servidor...");
	*socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (*socket_file_descriptor == SOCKET_CREATION_STATUS::FAILURE) { 
		LogError("Não foi possível iniciar o servidor");
		return SOCKET_CREATION_STATUS::FAILURE;
	}
	return SOCKET_CREATION_STATUS::SUCCESS;
	
}

void ConfigureServerAddress(sockaddr_in& serverAddress) noexcept {
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
	serverAddress.sin_port = htons(PORT);
}

void BindSocket(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor, SA* server_address, socklen_t& socket_length) {
	auto socket_binding_status = bind(socket_file_descriptor, server_address, socket_length);
	if (socket_binding_status != SOCKET_BIND_SUCCESS) {
		LogError("Não foi possível ligar o socket do servidor...");
		exit(EXIT_FAILURE);
	}
	LogInfo("Socket ligado com sucesso");
}

void PrepareSocketToListen(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor) {
	LogInfo("Preparando o servidor para escutar requisições...");
	if ((listen(socket_file_descriptor, NUM_OF_REQUESTS_QUEUED)) != SOCKET_SET_TO_LISTEN) { 
		LogError("Não foi possível preparar o socket do servidor para escutar as requisições do cliente.");
		exit(EXIT_FAILURE); 
	}
	LogInfo("Socket preparado para escutar requisições.");
}

CLIENT_FILE_DESCRIPTOR AcceptClient(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor, SA* client_address, socklen_t& client_socket_len) {
	CLIENT_FILE_DESCRIPTOR connfd = accept(socket_file_descriptor, client_address, &client_socket_len); 
	if (connfd < 0) { 
		LogError("Ocorreu um erro ao conectar com o client.");
		exit(EXIT_FAILURE); 
	} 
	LogSuccess("Client conectado com sucesso.");
	return connfd;
}

using std::string;
void CommunicateWithClient(CLIENT_FILE_DESCRIPTOR& client_socket_file_descriptor) {
	char buffer[MAX_MESSAGE_SIZE];
	string exit_command{"exit"};
	string goodbye_message{"Goodbye"};
	string listen_message{"Listen"};
	for (;;) {
		string client_message{"Mensagem vinda do client: "};
		read(client_socket_file_descriptor, buffer, sizeof(buffer));
		buffer[MAX_MESSAGE_SIZE - 1] = '\0';
		string client_command{buffer};
		client_message.append(client_command);
		LogInfo(client_message.c_str());
		if (client_command == exit_command) {
			WriteToClient(client_socket_file_descriptor, goodbye_message);
			LogWarning("Servidor finalizando conexão...");
			break;
		}
		LogInfo("Enviando sinal de vida...");
		WriteToClient(client_socket_file_descriptor, listen_message);
		bzero(buffer, MAX_MESSAGE_SIZE);
	}
}
