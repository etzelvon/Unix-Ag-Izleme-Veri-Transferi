#ifndef ANOMALY_DETECTION_H
#define ANOMALY_DETECTION_H

#include <string>

struct PacketData {
    std::string srcIP;
    std::string destIP;
    uint16_t srcPort;
    uint16_t destPort;
};

PacketData extractPacketData(const u_char *packet);
void logAnomaly(const std::string &message);
void detectAnomalies();

#endif 