# HTTP Server em C — Roadmap

Projeto para aprender C construindo um servidor HTTP do zero.

---

## Fase 1 — Fundamentos

### 1. [x] Servidor básico
- Criar socket, bind, listen, accept
- Responder com HTML fixo ("Hello from C!")
- **Conceitos:** sockets, file descriptors, structs, `memset`, `htons`, system calls

### 2. [ ] Routing básico
- Parsear a URL da requisição (`GET /about HTTP/1.1`)
- Responder diferente para cada rota (`/`, `/about`)
- Retornar 404 para rotas desconhecidas
- **Conceitos:** `strstr()`, `strncmp()`, parsing manual de strings

### 3. [ ] Servir arquivos estáticos
- Ler arquivos `.html`, `.css`, `.js` do disco e enviar como resposta
- Detectar o `Content-Type` correto baseado na extensão
- Tratar arquivo não encontrado (404)
- **Conceitos:** `fopen()`, `fread()`, `fclose()`, `stat()`, `malloc()`, `free()`

### 4. [ ] Tratar Ctrl+C (sinais)
- Capturar `SIGINT` para fechar o servidor de forma limpa
- Fechar o socket antes de sair
- **Conceitos:** `signal()` / `sigaction()`, variáveis `volatile`, ciclo de vida de processos

---

## Fase 2 — Intermediário

### 5. [ ] Argumentos de linha de comando
- Configurar porta e diretório via `./server --port 3000 --dir ./public`
- Valores padrão quando não informados
- **Conceitos:** `argc` / `argv`, `getopt()`, `atoi()`

### 6. [ ] Parsear headers HTTP
- Extrair headers como `Content-Type`, `User-Agent`, `Host`
- Criar uma struct para representar a requisição parseada
- **Conceitos:** `strtok()`, aritmética de ponteiros, structs compostas

### 7. [ ] Suportar método POST
- Receber dados do cliente (JSON, form data)
- Ler o `Content-Length` e fazer múltiplos `read()` até receber tudo
- **Conceitos:** protocolo HTTP completo, buffers dinâmicos

---

## Fase 3 — Avançado

### 8. [ ] Logging com timestamps
- Registrar cada requisição em um arquivo de log
- Formato: `[2026-02-07 14:30:00] GET /about 200`
- **Conceitos:** `time()`, `localtime()`, `strftime()`, `fprintf()` em arquivo

### 9. [ ] Concorrência — Multi-thread ou Fork
- Atender múltiplos clientes ao mesmo tempo
- Opção A: threads com `pthread_create()`
- Opção B: processos com `fork()`
- **Conceitos:** threads vs processos, race conditions, `mutex`, zombie processes

### 10. [ ] I/O Multiplexing
- Atender múltiplos clientes com uma única thread (modelo event-driven)
- **Conceitos:** `poll()` / `select()`, non-blocking sockets, `fcntl()`

---

## Fase 4 — Projeto completo

### 11. [ ] Mini API REST
- Endpoints: `GET /users`, `POST /users`, `DELETE /users/:id`
- Dados armazenados em memória (array/linked list)
- **Conceitos:** CRUD, estruturas de dados em C

### 12. [ ] Keep-Alive
- Reusar conexões TCP em vez de abrir/fechar para cada requisição
- **Conceitos:** HTTP/1.1 persistent connections, timeouts

### 13. [ ] CGI simples
- Executar scripts externos (Python, Bash) e retornar a saída como resposta
- **Conceitos:** `execvp()`, `pipe()`, `dup2()`, redirecionamento de I/O

### 14. [ ] HTTPS (TLS)
- Adicionar criptografia com OpenSSL
- **Conceitos:** linking com libs externas (`-lssl`), certificados, handshake TLS
