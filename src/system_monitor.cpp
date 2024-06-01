#include "system_monitor.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <sys/stat.h>

// Sistem kaynaklarını izleyen ve raporlayan fonksiyon
void monitorSystemResources() {
    // log dizini mevcut değilse oluşturulur
    struct stat info;
    if (stat("log", &info) != 0) {
        if (mkdir("log", 0777) == -1) {
            std::cerr << "Error creating log directory." << std::endl;
            exit(1);
        }
    } else if (info.st_mode & S_IFDIR) {
        // log dizini mevcut
    } else {
        std::cerr << "log is not a directory." << std::endl;
        exit(1);
    }

    // log/system_report.txt dosyasını açar
    std::ofstream logFile("log/system_report.txt", std::ios::out);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }

    // CPU kullanımı
    double load[3];
    if (getloadavg(load, 3) != -1) {
        logFile << "CPU Load Averages: 1min: " << load[0] << ", 5min: " << load[1] << ", 15min: " << load[2] << std::endl;
    } else {
        logFile << "Failed to get CPU load averages." << std::endl;
    }

    // Bellek kullanımı
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long totalPhysMem = memInfo.totalram;
    totalPhysMem *= memInfo.mem_unit;
    long long freePhysMem = memInfo.freeram;
    freePhysMem *= memInfo.mem_unit;
    long long usedPhysMem = totalPhysMem - freePhysMem;
    logFile << "Memory Usage: Total: " << totalPhysMem / (1024 * 1024) << " MB, Used: " << usedPhysMem / (1024 * 1024) << " MB, Free: " << freePhysMem / (1024 * 1024) << " MB" << std::endl;

    // Disk kullanımı
    struct statvfs diskInfo;
    if (statvfs("/", &diskInfo) == 0) {
        unsigned long long totalDisk = diskInfo.f_blocks * diskInfo.f_frsize;
        unsigned long long freeDisk = diskInfo.f_bfree * diskInfo.f_frsize;
        unsigned long long usedDisk = totalDisk - freeDisk;
        logFile << "Disk Usage: Total: " << totalDisk / (1024 * 1024) << " MB, Used: " << usedDisk / (1024 * 1024) << " MB, Free: " << freeDisk / (1024 * 1024) << " MB" << std::endl;
    } else {
        logFile << "Failed to get disk usage." << std::endl;
    }

    // Ağ kullanımı
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        logFile << "Failed to get network interfaces." << std::endl;
    } else {
        logFile << "Network Interfaces:" << std::endl;
        for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;
            int family = ifa->ifa_addr->sa_family;
            if (family == AF_PACKET) {
                logFile << "Interface: " << ifa->ifa_name << std::endl;
            }
        }
        freeifaddrs(ifaddr);
    }

    logFile.close();
    std::cout << "System resource report written to log/system_report.txt" << std::endl;
}
