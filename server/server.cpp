#include "../log.h"
#include "server.hpp"
#include "../db/database_connector.cpp"
#include "server_factory.cpp"
int main() 
{
	ServerFactory sf{};
	std::vector<AbstractServer*> servers{sf.CreateEnglishServer(8080), sf.CreatePortugueseServer(8081)};
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
	CommunicateWithClient(client_file_descriptor, servers);
	for (auto server: servers) {
		delete server;
	}
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
	LogInfo("Ligando socket...");
	auto socket_binding_status = bind(socket_file_descriptor, server_address, socket_length);
	if (socket_binding_status != SOCKET_BIND_SUCCESS) {
		LogError("Não foi possível ligar o socket do servidor...");
		exit(EXIT_FAILURE);
	}
	LogSuccess("Socket ativado com sucesso.");
}

void PrepareSocketToListen(SOCKET_FILE_DESCRIPTOR& socket_file_descriptor) {
	LogInfo("Preparando o servidor para escutar requisições...");
	if ((listen(socket_file_descriptor, NUM_OF_REQUESTS_QUEUED)) != SOCKET_SET_TO_LISTEN) { 
		LogError("Não foi possível preparar o socket do servidor para escutar as requisições do cliente.");
		exit(EXIT_FAILURE); 
	}
	LogSuccess("Socket preparado para escutar requisições na porta 8080.");
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
void CommunicateWithClient(CLIENT_FILE_DESCRIPTOR& client_socket_file_descriptor, std::vector<AbstractServer*>& servers) {
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
		if (TypedExit(client_command)) {
			WriteToClient(client_socket_file_descriptor, goodbye_message);
			LogWarning("Servidor finalizando conexao...");
			break;
		}
		auto response = InterpretateCommand(client_command, servers);
		ShowMessageSent(response);
		WriteToClient(client_socket_file_descriptor, response);
		bzero(buffer, MAX_MESSAGE_SIZE);
	}
}

std::string InterpretateCommand(std::string& command, std::vector<AbstractServer*>& servers) {
	const string NAME_SEARCH_COMMAND{"find -n"};
	const string OPERATION_NOT_FOUND{"Operacao nao encontrada..."};
	size_t name_search_string = command.find(NAME_SEARCH_COMMAND);
	if (name_search_string == string::npos) {
		return OPERATION_NOT_FOUND;
	}
	std::string name = command.substr(name_search_string + NAME_SEARCH_COMMAND.size() + 1);
	name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
	return FindStringInServers(name, servers).value_or("Not found");
}


std::optional<std::string> FindStringInServers(const std::string& name, std::vector<AbstractServer*>& servers) {
	constexpr int PIPE_OPERATIONS{2}; // Representa as 2 partes do pipe ("leitura e escrita")
    constexpr int READ_END = 0; // Parte de leitura
    constexpr int WRITE_END = 1; // Parte de escrita
	std::vector<std::array<int, PIPE_OPERATIONS>> pipes{servers.size()};  // Array com vários pipes ("Um para cada servidor")
    // Começa a criar processos filhos de cada servidor
    for (std::size_t i = 0; i < servers.size(); ++i) {
        if (pipe(pipes[i].data()) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork(); // Inicia um novo processo via fork
        if (pid == -1) { // Se o retorno do fork é -1, então ocorreu um erro
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {  // Verifica se é processo filho
            close(pipes[i][READ_END]);  // Fecha a parte de leitura do pipe

            // Redireciona a parte de leitura do pipe para o standard output (stdout)
            dup2(pipes[i][WRITE_END], STDOUT_FILENO);
            close(pipes[i][WRITE_END]);  // Fecha a parte de leitura do pipe

            // Faz a busca no servidor conectado ao pipe
            auto opt_person = servers[i]->FindByName(name);
            if (opt_person.has_value()) {
                std::string result{opt_person.value()};
                write(STDOUT_FILENO, result.c_str(), result.size());
            }
            _exit(EXIT_SUCCESS);  // Finaliza o processo filho
        } else {  // Processo pai
            close(pipes[i][WRITE_END]);  // Fecha o a parte de leitura do processo pai (ele não precisa escrever, apenas ler)
        }
    }

	// Espera um processo finalizar a busca e captura a mensagem pelo pipe
    for (std::size_t i = 0; i < servers.size(); ++i) {
        char buffer[MAX_MESSAGE_SIZE];
        ssize_t bytesRead = read(pipes[i][READ_END], buffer, sizeof(buffer)); // Lê o conteudo do pipe e manda pra um buffer
        if (bytesRead > 0) { // Se o buffer tiver alguma coisa...
            close(pipes[i][READ_END]);  // Fecha a parte de leitura do pipe
            return std::string(buffer, bytesRead); // Retorna a mensagem capturada da mensagem
        }
        close(pipes[i][READ_END]);  // Fecha os outros pipes que não capturaram nada
    }

    return std::nullopt; // Se nada for capturado, retorna um null option
}


void ShowMessageSent(const std::string& message) {
	const string PREFIX{"Mensagem sendo enviada: "};
	const string BITS{" bits)."};
	string log{PREFIX};
	log.append(message);
	log.append(" (");
	string message_size{std::to_string(message.size())};
	log.append(message_size);
	log.append(BITS);
	LogInfo(log.c_str());
}