#include "ssl_client.h"
#include "ssl_common.h"
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

#define PORT 4443

// Sunucudan dosya alan fonksiyon
void receiveFile(SSL *ssl) {
    std::ofstream file("received_nmap_scan.txt", std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to create file." << std::endl;
        return;
    }

    char buffer[1024];
    int bytes;
    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer))) > 0) {
        file.write(buffer, bytes);
    }

    file.close();
    std::cout << "nmap_scan.txt dosyası alındı." << std::endl;
}

// SSL istemcisi başlatan fonksiyon
void startClient() {
    int client_fd;
    struct sockaddr_in addr;

    // OpenSSL kitaplığını başlatır
    initializeSSL();
    SSL_CTX *ctx = createContext(SSLv23_client_method());

    // İstemci soketi oluşturur
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Sunucuya bağlanır
    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    // SSL bağlantısını başlatır
    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        receiveFile(ssl); // Dosyayı alır
    }

    // SSL bağlantısını kapatır ve kaynakları serbest bırakır
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
    SSL_CTX_free(ctx);
    cleanupSSL();
}
