#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>

std::string getNetworkInterface();
std::string getNetworkMask(const std::string& interface);
std::string getIPAddress(const std::string& interface);  // Yeni fonksiyon deklarasyonu

#endif
