#include "anomaly_detection.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <sys/stat.h>
#include <arpa/inet.h>

// Ham paketten paket verilerini çıkaran fonksiyon
PacketData extractPacketData(const u_char *packet) {
    PacketData data;

    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        const struct ip *ipHeader;
        ipHeader = (struct ip *)(packet + sizeof(struct ether_header));
        data.srcIP = inet_ntoa(ipHeader->ip_src);
        data.destIP = inet_ntoa(ipHeader->ip_dst);

        if (ipHeader->ip_p == IPPROTO_TCP) {
            const struct tcphdr *tcpHeader;
            tcpHeader = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            data.srcPort = ntohs(tcpHeader->source);
            data.destPort = ntohs(tcpHeader->dest);
        }
    }

    return data;
}

// Anomalileri bir dosyaya kaydeden fonksiyon
void logAnomaly(const std::string &message) {
    struct stat info;
    if (stat("log", &info) != 0) {
        if (mkdir("log", 0777) == -1) {
            std::cerr << "Log dizini oluşturulurken hata oluştu." << std::endl;
            return;
        }
    } else if (info.st_mode & S_IFDIR) {
        // log dizini mevcut
    } else {
        std::cerr << "log bir dizin değil." << std::endl;
        return;
    }

    std::ofstream logFile("log/anomaly_log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Log dosyası açılamadı." << std::endl;
        return;
    }

    logFile << message << std::endl;
    logFile.close();
}

// Ağ trafiğindeki anomalileri tespit eden fonksiyon
void detectAnomalies() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *d;
    pcap_t *handle;

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "pcap_findalldevs() başarısız: " << errbuf << std::endl;
        return;
    }

    if (alldevs == nullptr) {
        std::cerr << "Hiçbir cihaz bulunamadı." << std::endl;
        return;
    }

    // İlk cihazı kullan
    d = alldevs;
    handle = pcap_open_live(d->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "pcap_open_live() başarısız: " << errbuf << std::endl;
        return;
    }

    std::map<std::string, int> ipCount;
    std::map<std::string, int> portCount;
    const int threshold = 100; // Anomali tespiti için örnek eşik değeri

    while (true) {
        struct pcap_pkthdr header;
        const u_char *packet = pcap_next(handle, &header);
        if (packet == nullptr) continue;

        PacketData data = extractPacketData(packet);
        if (data.srcIP.empty() || data.destIP.empty()) continue;

        // Kaynak IP ve hedef portların sayısını artır
        ipCount[data.srcIP]++;
        portCount[std::to_string(data.destPort)]++;

        // Eşik değerine göre anomalileri kontrol et
        if (ipCount[data.srcIP] > threshold) {
            std::string message = "Anomali tespit edildi: IP " + data.srcIP + " adresinden çok sayıda paket.";
            logAnomaly(message);
            std::cout << message << std::endl;
        }

        if (portCount[std::to_string(data.destPort)] > threshold) {
            std::string message = "Anomali tespit edildi: " + std::to_string(data.destPort) + " portuna çok sayıda paket.";
            logAnomaly(message);
            std::cout << message << std::endl;
        }
    }

    pcap_close(handle);
    pcap_freealldevs(alldevs);
}
