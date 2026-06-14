#include <stdio.h>
#include "nl.h"
#include <net/if.h>
#include <string.h>


void print_usage(const char *prog_name) {
    printf("usage:\n");
    printf("  %s show               - show ip and ifaces\n", prog_name);
    printf("  %s show stats         - show ips and ifaces with stats\n", prog_name);
    printf("  %s set <ifname> up    - set iface up/down\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage("link");
        return 0;
    }

    int fd = init_socket();
    if (strcmp(argv[1], "show") == 0) {
        show(fd);
    } 
    
    else if (strcmp(argv[1], "set") == 0) {
        if (argc < 4) {
            fprintf(stderr, "set requires ifname and action\n");
            print_usage(argv[0]);
            return 1;
        }

        char *ifname = argv[2];
        unsigned int ifi = if_nametoindex(ifname);

        char *action = argv[3]; 
        if (strcmp(action, "up") == 0) {
            set_iffup(fd, ifi, 1);
        } else if (strcmp(action, "down") == 0) {
            set_iffup(fd, ifi, 0);
        } else {
            fprintf(stderr, "invalid action: %s > use up or down\n", action);
            return 1;
        }
    } 
    
    else {
        fprintf(stderr, "invalid command '%s'\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }
    close_socket(fd);

    return 0;
}