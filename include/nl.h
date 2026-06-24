#pragma once

#include <stdint.h>

int init_socket();
int close_socket(int fd);

int show(int fd);
int set_iffup(int fd, unsigned ifindex, int up);
int set_if_mtu(int fd, uint32_t ifindex, int mtu);

int listen_sk(int fd);
int set_tc(int fd, uint32_t ifindex, uint32_t speed);
int del_qdisc(int fd, uint32_t ifindex);