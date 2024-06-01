#include "ssl_common.h"
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>

// OpenSSL kitaplığını başlatır
void initializeSSL() {
    SSL_load_error_strings();    // Hata mesajlarını yükler
    OpenSSL_add_ssl_algorithms(); // SSL algoritmalarını ekler
}

// OpenSSL kitaplığını temizler ve kaynakları serbest bırakır
void cleanupSSL() {
    EVP_cleanup();
}

// Belirtilen SSL yöntemine göre SSL bağlamı oluşturur
SSL_CTX* createContext(const SSL_METHOD *method) {
    SSL_CTX *ctx;

    // Yeni bir SSL bağlamı oluşturur
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        ERR_print_errors_fp(stderr); // Hata mesajlarını yazdırır
        exit(EXIT_FAILURE);          // Programı sonlandırır
    }

    return ctx;
}
