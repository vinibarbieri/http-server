#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit
#include <unistd.h>     // close
#include <string.h>     // memset
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // struct sockaddr_in, INADDR_ANY, htons

#define PORT 8080
// Tamanho da fila de conexões pendentes (backlog)
#define BACKLOG 10

int main() {
    int server_fd;
    struct sockaddr_in address;

    // 1. CRIAR O SOCKET
    // AF_NET = IPv4
    //SOCK_STREAM = TCP 
    // 0 = Protocolo IP padrão
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // retorna um int que é o índice na tabela de arquivos abertos no processo
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Socket created with file descriptor: %d\n", server_fd);

    // 2. CONFIGURAR O ENDEREÇO (A estrutura "struct sockaddr_in)
    // Zera a memória da struct para evitar lixo
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer IP da máquina (localhost, wifi, etc)

    // htons = Host TO Network Short
    // Converte op inteira da porta para a ordem de bytes da rede (Big Endian)
    // O Mac (ARM) é Little Endian, a internet é Big Endian
    address.sin_port = htons(PORT);

    // 3. VINCULAR O SOCKET À PORTA (BIND)
    // O cast (struct sockaddr *)&address é necessário porque bind espera um ponteiro genérico
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha no bind");
        close(server_fd); // Boa prática: fechar o socket em caso de erro
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Binded to port %d\n", PORT);

    // 4. ESCUTAR POR CONEXÕES (LISTEN)
    // Coloca o socket em modo passivo, esperando conexões
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Falha no listen");
        close(server_fd); // Boa prática: fechar o socket em caso de erro
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Server Listening for connections on port %d\n", PORT);

    // Aqui entra o loop principal do servidor

    // Limpeza final (código inalcançável por enquanto, mas importante saber)
    close(server_fd);
    printf("[DEBUG] Server closed\n");
    return 0;
}