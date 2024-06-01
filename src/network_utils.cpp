#include "network_utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <array>
#include <memory>
#include <stdexcept>
#include <regex>

std::string executeCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    return result;
}

std::string getNetworkInterface() {
    std::string result = executeCommand("ifconfig");
    std::regex iface_regex(R"((\S+): flags)");
    std::smatch iface_match;
    if (std::regex_search(result, iface_match, iface_regex)) {
        return iface_match[1];
    }
    throw std::runtime_error("No network interface found");
}

std::string getNetworkMask(const std::string& interface) {
    std::string command = "ifconfig " + interface;
    std::string result = executeCommand(command);
    std::regex mask_regex(R"(netmask (\S+))");
    std::smatch mask_match;
    if (std::regex_search(result, mask_match, mask_regex)) {
        return mask_match[1];
    }
    throw std::runtime_error("No network mask found for interface " + interface);
}

std::string getIPAddress(const std::string& interface) {
    std::string command = "ifconfig " + interface;
    std::string result = executeCommand(command);
    std::regex ip_regex(R"(inet (\d+\.\d+\.\d+\.\d+))");
    std::smatch ip_match;
    if (std::regex_search(result, ip_match, ip_regex)) {
        return ip_match[1];
    }
    throw std::runtime_error("No IP address found for interface " + interface);
}
