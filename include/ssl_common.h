#ifndef SSL_COMMON_H
#define SSL_COMMON_H

#include <openssl/ssl.h>

void initializeSSL();
void cleanupSSL();
SSL_CTX* createContext(const SSL_METHOD *method);

#endif
