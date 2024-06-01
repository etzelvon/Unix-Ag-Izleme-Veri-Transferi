#include <iostream>
#include <ncurses.h>
#include <pcap.h>
#include "network_scan.h"
#include "traffic_monitor.h"
#include "ssl_server.h"
#include "ssl_client.h"
#include "system_monitor.h"

void displayMenu() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    const char *choices[] = {
        "Aga bagli cihazlari tara",
        "Ag trafigini izle",
        "Sunucu ol",
        "İstemci ol",
        "Sistem Kaynaklarini İzle ve Raporla",
        "Cikis"
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int choice;
    int highlight = 0;

    while (1) {
        clear();
        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i, 0, "%s", choices[i]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        choice = getch();

        switch (choice) {
            case KEY_UP:
                if (highlight > 0) {
                    --highlight;
                }
                break;
            case KEY_DOWN:
                if (highlight < n_choices - 1) {
                    ++highlight;
                }
                break;
            case 10: // Enter key
                if (highlight == n_choices - 1) {
                    clear();
                    endwin();
                    return;
                }
                clear();
                endwin();
                switch (highlight) {
                    case 0:
                        std::cout << "Ağ taraması yapılıyor..." << std::endl;
                        scanNetwork();
                        std::cout << "Tespit edilen cihazlar:" << std::endl;
                        displayDevices(parseResults());
                        break;
                    case 1:
                        {
                            char errbuf[PCAP_ERRBUF_SIZE];
                            pcap_if_t *alldevs;
                            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                                std::cerr << "pcap_findalldevs() failed: " << errbuf << std::endl;
                                return;
                            }
                            if (alldevs == nullptr) {
                                std::cerr << "No devices found." << std::endl;
                                return;
                            }
                            char* dev = alldevs->name;
                            std::cout << "Cihaz: " << dev << " üzerinde trafik izleniyor..." << std::endl;
                            monitorTraffic(dev);
                            pcap_freealldevs(alldevs);
                        }
                        break;
                    case 2:
                        startServer();
                        break;
                    case 3:
                        startClient();
                        break;
                    case 4:
                        monitorSystemResources();
                        break;
                }
                displayMenu();
        }
    }
}

int main() {
    displayMenu();
    return 0;
}
