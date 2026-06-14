#ifndef NL_H
#define NL_H

int init_socket();
int close_socket(int fd);

int show(int fd);
void set_iffup(int fd, int ifindex, int up);
#endif 