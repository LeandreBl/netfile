/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** header
*/

#ifndef NETFILE_H_
# define NETFILE_H_

#include <stdint.h>
#include <stdbool.h>

int netreceive(const char *filename, uint16_t port);
int netsend(const char *filename, const char *ipaddr, uint16_t port);

bool is_a_packet(int fd);

#endif /* !NETFILE_H_ */
