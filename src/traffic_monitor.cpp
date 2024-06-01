#include "traffic_monitor.h"
#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

// Paket yakalama fonksiyonu
void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    // Eğer paket IP paketi ise, başlığı al
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        const struct ip* ipHeader;
        ipHeader = (struct ip*)(packet + sizeof(struct ether_header));
        std::cout << "IP Packet from: " << inet_ntoa(ipHeader->ip_src) << " to " << inet_ntoa(ipHeader->ip_dst) << std::endl;
        // Eğer paket TCP ise, TCP başlığını al
        if (ipHeader->ip_p == IPPROTO_TCP) {
            const struct tcphdr* tcpHeader;
            tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            std::cout << "TCP Packet from port: " << ntohs(tcpHeader->source) << " to port: " << ntohs(tcpHeader->dest) << std::endl;
        }
    }
}
// Ağ tarfiğini izleme fonksiyonu
void monitorTraffic(const char* dev) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    // Canlı trafiği yakalama 
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "pcap_open_live() failed: " << errbuf << std::endl;
        return;
    }
    // Paketleri yakalar ve packethandler fonksiyonuna iletir.
    if (pcap_loop(handle, 0, packetHandler, nullptr) < 0) {
        std::cerr << "pcap_loop() failed: " << pcap_geterr(handle) << std::endl;
        return;
    }
    // kaynakları serbest bırak.
    pcap_close(handle);
}
