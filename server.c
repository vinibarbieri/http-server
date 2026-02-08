#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

#define PORT 8080
// Tamanho da fila de conexões pendentes (backlog)
#define BACKLOG 10
// Tamanho máximo do buffer para ler a requisição do cliente
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in address;

    // 1. CRIAR O SOCKET
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // retorna um int que é o índice na tabela de arquivos abertos no processo
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Socket created with file descriptor: %d\n", server_fd);

    // 2. CONFIGURAR O ENDEREÇO (A estrutura "struct sockaddr_in)
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer IP da máquina (localhost, wifi, etc)


    address.sin_port = htons(PORT);

    // 3. VINCULAR O SOCKET À PORTA (BIND)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha no bind");
        close(server_fd); // Boa prática: fechar o socket em caso de erro
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Binded to port %d\n", PORT);

    // 4. ESCUTAR POR CONEXÕES (LISTEN)
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Falha no listen");
        close(server_fd); // Boa prática: fechar o socket em caso de erro
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] Server Listening for connections on port %d\n", PORT);

    // =============================================
    // 5. LOOP PRINCIPAL — ACEITAR E RESPONDER
    // =============================================

    while (1) {
        int client_fd;
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        // 5a. ACCEPT — Aceitar uma conexão da fila
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        if (client_fd < 0) {
            perror("Falha no accept");
            continue; // Não mata o servidor, apenas pula para a próxima iteração
        }
        printf("[DEBUG] Client connected! client_fd = %d\n", client_fd);

        // 5b. READ — Ler a requisição HTTP do cliente
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer)); // Limpa o buffer (mesmo motivo do memset anterior)

        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read < 0) {
            perror("Falha no read");
            close(client_fd);
            continue;
        }
        printf("[DEBUG] Received request (%zd bytes):\n%s\n", bytes_read, buffer);

        // 5c. Montar a resposta HTTP

        char *body = "<html><body><h1>Hello from C!</h1></body></html>";

        char response[BUFFER_SIZE];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(body), body);

        // 5d. WRITE — Enviar a resposta para o cliente
        write(client_fd, response, strlen(response));
        printf("[DEBUG] Response sent!\n");

        // 5e. Fechar a conexão com o CLIENTE (não com o servidor!)
        close(client_fd);
        printf("[DEBUG] Connection closed. Waiting for next client...\n\n");
    }

    close(server_fd);
    printf("[DEBUG] Server closed\n");
    return 0;
}