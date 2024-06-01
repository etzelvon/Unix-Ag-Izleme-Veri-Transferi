#include "network_scan.h"
#include "network_utils.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>

// Log dizinini oluşturan fonksiyon
void createLogDirectory() {
    struct stat info;
    if (stat("log", &info) != 0) {
        // log dizini mevcut değilse oluştur
        if (mkdir("log", 0777) == -1) {
            std::cerr << "Log dizini olusturulurken hata olustu." << std::endl;
            exit(1);
        }
    } else if (info.st_mode & S_IFDIR) {
        // log dizini mevcut
    } else {
        std::cerr << "log bir dizin degil." << std::endl;
        exit(1);
    }
}

// IP ve ağ maskesinden ağ adresini hesaplayan fonksiyon
std::string calculateNetwork(const std::string& ip, const std::string& netmask) {
    struct in_addr ip_addr, mask_addr, network_addr;
    inet_pton(AF_INET, ip.c_str(), &ip_addr);
    inet_pton(AF_INET, netmask.c_str(), &mask_addr);
    network_addr.s_addr = ip_addr.s_addr & mask_addr.s_addr;

    char network_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &network_addr, network_str, INET_ADDRSTRLEN);

    int prefix_len = 0;
    uint32_t mask = ntohl(mask_addr.s_addr);
    while (mask) {
        prefix_len += mask & 1;
        mask >>= 1;
    }

    return std::string(network_str) + "/" + std::to_string(prefix_len);
}

// Ağı tarayan fonksiyon
void scanNetwork() {
    createLogDirectory();
    std::string interface = getNetworkInterface();
    std::string ip = getIPAddress(interface);
    std::string netmask = getNetworkMask(interface);
    std::string network = calculateNetwork(ip, netmask);

    // Ekrana tarama yapılacak IP ve ağ maskesini yazdır
    std::cout << "Tarama yapilacak IP ve ag maskesi: " << network << std::endl;

    std::string command = "nmap -A " + network + " -oG log/nmap_scan.txt";
    std::system(command.c_str());
}

// Tarama sonuçlarını ayrıştıran fonksiyon
std::vector<Device> parseResults() {
    std::ifstream file("log/nmap_scan.txt");
    std::string line;
    std::vector<Device> devices;
    Device current_device;
    bool in_host_section = false;

    while (std::getline(file, line)) {
        if (line.find("Host:") != std::string::npos) {
            if (in_host_section) {
                devices.push_back(current_device);
                current_device = Device();
            }
            in_host_section = true;
            size_t ipStart = line.find("Host: ") + 6;
            size_t ipEnd = line.find(" ", ipStart);
            current_device.ip = line.substr(ipStart, ipEnd - ipStart);

            size_t statusStart = line.find("Status: ") + 8;
            size_t statusEnd = line.find(" ", statusStart);
            current_device.status = line.substr(statusStart, statusEnd - statusStart);
        }

        if (line.find("OS details:") != std::string::npos) {
            size_t osStart = line.find("OS details: ") + 12;
            current_device.os = line.substr(osStart);
        }

        if (line.find("Ports:") != std::string::npos) {
            size_t portStart = line.find("Ports: ") + 7;
            std::string ports = line.substr(portStart);
            size_t pos = 0;
            std::string delimiter = ", ";
            while ((pos = ports.find(delimiter)) != std::string::npos) {
                current_device.open_ports.push_back(ports.substr(0, pos));
                ports.erase(0, pos + delimiter.length());
            }
            current_device.open_ports.push_back(ports);
        }
    }
    if (in_host_section) {
        devices.push_back(current_device);
    }
    file.close();
    return devices;
}

// Tarama sonuçlarını ekrana yazdıran fonksiyon
void displayDevices(const std::vector<Device>& devices) {
    for (const auto& device : devices) {
        std::cout << "IP: " << device.ip << " Durum: " << device.status << std::endl;
        if (!device.os.empty()) {
            std::cout << "İsletim Sistemi: " << device.os << std::endl;
        }
        if (!device.open_ports.empty()) {
            std::cout << "Acik Portlar: ";
            for (const auto& port : device.open_ports) {
                std::cout << port << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
