#include "ssl_server.h"
#include "ssl_common.h"
#include "anomaly_detection.h" // Anomaly detection başlığı
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <thread>

#define PORT 4443

// SSL sunucusu için SSL_CTX yapılandırma fonksiyonu
void configureServerContext(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// İstemciye dosya gönderme fonksiyonu
void sendFile(SSL *ssl) {
    std::ifstream file("log/nmap_scan.txt", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        SSL_write(ssl, buffer, file.gcount());
    }

    file.close();
}

// Sunucunun ana döngüsü
void serverMainLoop(int server_fd, SSL_CTX *ctx) {
    while (1) {
        struct sockaddr_in client_addr;
        uint len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) {
            perror("Unable to accept");
            continue;
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            sendFile(ssl);
            std::cout << "nmap_scan.txt dosyası gönderildi." << std::endl;
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
    }
}

// Sunucuyu başlatan fonksiyon
void startServer() {
    int server_fd;
    struct sockaddr_in addr;

    initializeSSL();
    SSL_CTX *ctx = createContext(SSLv23_server_method());
    configureServerContext(ctx);

    // Sunucu soketi oluşturulur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Sunucu soketi belirtilen portta bağlanır
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    // Sunucu belirtilen portta dinlemeye başlar
    if (listen(server_fd, 1) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Anomali tespitini ayrı bir iş parçacığında başlatır
    std::thread anomalyThread(detectAnomalies);

    // Ana sunucu döngüsü
    serverMainLoop(server_fd, ctx);

    // Anomali tespit iş parçacığının bitmesini bekler
    anomalyThread.join();

    // Kaynakları serbest bırakır
    close(server_fd);
    SSL_CTX_free(ctx);
    cleanupSSL();
}
