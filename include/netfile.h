/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** header
*/

#ifndef NETFILE_H_
# define NETFILE_H_

# include <stdint.h>
# include <lsocket.h>
# include <sys/time.h>

typedef struct netfile_s
{
	int fd;
	uint64_t filesize;
	uint64_t transfered;
	int percentage;
	double speed;
	uint64_t lastsize;
	struct timeval tv;
	lsocket_t socket;
} netfile_t;

int netreceive(const char *filename, uint16_t port);
int netsend(const char *filename, const char *ipaddr, uint16_t port);

uint8_t index_unit(unsigned long int value);
void netdisplay(netfile_t *netf);
void display_filesize(unsigned long int value);

#endif /* !NETFILE_H_ */
