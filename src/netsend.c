/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** netsend
*/

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <lsocket.h>
#include <string.h>

#include "netfile.h"

static int send_header(lsocket_t *socket, uint64_t filesize)
{
	uint16_t magic = 0xca0c;

	if (write(socket->fd, &magic, sizeof(magic)) != sizeof(magic) ||
		write(socket->fd, &filesize, sizeof(filesize)) != sizeof (filesize)) {
			dprintf(2, "Error: connection lost.\n");
			return (-1);
		}
	return (0);
}

static int snloop(netfile_t *netf)
{
	char buffer[8192];
	ssize_t rd;
	ssize_t wr;

	netf->transfered = 0;
	netf->lastsize = 0;
	display_filesize(netf->filesize);
	while (netf->transfered < netf->filesize) {
		rd = read(netf->fd, buffer, sizeof(buffer));
		if (rd == 0 || rd == -1)
			return (-1);
		wr = write(netf->socket.fd, buffer, rd);
		if (wr == 0 || wr == -1 || rd != wr)
			return (-1);
		netf->transfered += wr;
		netdisplay(netf);
	}
	printf("\n");
	return (0);
}

int netsend(const char *filename, const char *ipaddr, uint16_t port)
{
	netfile_t netf;

	memset(&netf, 0, sizeof(netf));
	netf.fd = open(filename, O_RDONLY | O_LARGEFILE);
	if (netf.fd == -1) {
		dprintf(2, "Can't access file \"%s\".\n", filename);
		return (-1);
	}
#ifdef _WIN64
	netf.filesize = _lseeki64(netf.fd, 0, SEEK_END);
	_lseeki64(netf.fd, 0, SEEK_SET);
#endif
#ifdef __linux__
	netf.filesize = lseek64(netf.fd, 0, SEEK_END);
	lseek64(netf.fd, 0, SEEK_SET);
#else
#error undefined architecture
#endif
	if (lsocket_connect(&netf.socket, ipaddr, port) == -1) {
		dprintf(2, "Error: Could not connect to \"%s\" on port %u.\n", ipaddr, port);
		return (-1);
	}
	if (gettimeofday(&netf.tv, NULL) == -1 || send_header(&netf.socket, netf.filesize) == -1)
		return (-1);
	if (snloop(&netf) == -1)
		dprintf(2, "Error: connection aborted\n");
	close(netf.fd);
	lsocket_destroy(&netf.socket);
	return (0);
}
