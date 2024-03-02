#include "./client.h"
#include "../log.h"

int main()
{
	SOCKET_FILE_DESCRIPTOR socket_file_desc;
	struct sockaddr_in server_address;
    CreateSocket(&socket_file_desc);
	bzero(&server_address, sizeof(server_address));
    socklen_t socket_len = ConfigureSocketAddress(&server_address);
    FTP_CONNECTION_STATUS connection_status = ConnectToFTPServer(&socket_file_desc, (SA*)&server_address, &socket_len);
    if (connection_status != CONNECTION_SUCCESS) {
		LogWarning("Verifique se o servidor está sendo executado antes de iniciar a execução do cliente.");
        exit(EXIT_FAILURE);
    }
	CommunicateWithFTPServer(socket_file_desc);
	close(socket_file_desc);
    return EXIT_SUCCESS;
}

void CommunicateWithFTPServer(SOCKET_FILE_DESCRIPTOR socket_file_desc) {
	char buff[MAX_BUFFER_SIZE];
	char server_response[SERVER_RESPONSE_MAX_LENGTH];
	int keyboard_pivot;
	for (;;) {
		bzero(buff, sizeof(buff));
		bzero(server_response, sizeof(server_response));
		PrintInput("Comando>");
		keyboard_pivot = 0;
        WaitTyping(buff, keyboard_pivot); // Espera ocupada
        if (TypedExit(buff)) {
            LogError("Conexão abortada pelo cliente");
            break;
        }
		write(socket_file_desc, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(socket_file_desc, buff, sizeof(buff));
		char server_response[MAX_BUFFER_SIZE];
		snprintf(server_response, SERVER_RESPONSE_MAX_LENGTH, "From Server: %s", buff);
		LogInfo(server_response);
		if (TypedExit(buff)) {
			LogError("Conexão finalizada pelo servidor");
			break;
		}
	}
}

FTP_CONNECTION_STATUS ConnectToFTPServer(SOCKET_FILE_DESCRIPTOR* socket_file_desc, SA* server_address, socklen_t* socket_len) {
	LogInfo("Estabelecendo conexão com o servidor...");
    FTP_CONNECTION_STATUS connection_status = connect(*socket_file_desc, server_address, *socket_len);
    if (connection_status != CONNECTION_SUCCESS) {
		LogError("Erro ao se comunicar com o servidor FTP");
    } else {
        LogSuccess("Conectado ao servidor FTP!");
    }
    return connection_status;
}

socklen_t ConfigureSocketAddress(struct sockaddr_in* server_address) {
	server_address->sin_family = AF_INET;
	server_address->sin_addr.s_addr = inet_addr(LOCALHOST);
	server_address->sin_port = htons(DEFAULT_PORT);
	socklen_t socket_len = (socklen_t) sizeof(*server_address);
	return socket_len;
}

SOCKET_CREATION_STATUS CreateSocket(SOCKET_FILE_DESCRIPTOR* socket_file_desc) {
	LogInfo("Criando Socket...");
    *socket_file_desc = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	if (*socket_file_desc == BAD_DESCRIPTOR) {
        LogError("Erro ao criar socket...");
		return FAILURE;
	}
    LogSuccess("Socket criado com sucesso!");
    return SUCCESS;
}

void WaitTyping(char* buff, int keyboard_pivot) {
    while ((buff[keyboard_pivot++] = getchar()) != '\n');
    return;
}

int TypedExit(char* command) {
    return strncmp(command, EXIT_COMM, EXIT_COMM_SIZE) == 0;
}