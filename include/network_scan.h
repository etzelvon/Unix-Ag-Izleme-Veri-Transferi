#ifndef NETWORK_SCAN_H
#define NETWORK_SCAN_H

#include <vector>
#include <string>

struct Device {
    std::string ip;
    std::string status;
    std::string os;
    std::vector<std::string> open_ports;
};

void createLogDirectory();
void scanNetwork();
std::vector<Device> parseResults();
void displayDevices(const std::vector<Device>& devices);

#endif
