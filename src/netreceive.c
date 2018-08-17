/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** receive function
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <lsocket.h>
#include <stdbool.h>
#include <string.h>

#include "netfile.h"

static bool is_a_packet(int fd)
{
	uint16_t magic;

	if (read(fd, &magic, sizeof(magic)) != sizeof(magic))
		return (false);
	if (magic == 0xca0c)
		return (true);
	return (false);
}

static int rcloop(netfile_t *netf)
{
	char buffer[8192];
	ssize_t rd;
	ssize_t wr;

	if (is_a_packet(netf->socket.fd) == false) {
		dprintf(2, "Error: Incorrect packet type received\n");
		return (-1);
	}
	if (read(netf->socket.fd, &netf->filesize, sizeof(netf->filesize)) != sizeof(netf->filesize))
		return (-1);
	display_filesize(netf->filesize);
	while (netf->transfered < netf->filesize) {
		rd = read(netf->socket.fd, buffer, sizeof(buffer));
		if (rd == 0 || rd == -1) {
			dprintf(2, "Error: Connection aborted.\n");
			return (-1);
		}
		wr = write(netf->fd, buffer, rd);
		if (wr == 0 || wr == -1 || rd != wr) {
			dprintf(2, "Error: not enough place on disk.\n");
			return (-1);
		}
		netf->transfered += wr;
		netdisplay(netf);
	}
	printf("\n");
	return (0);
}

int netreceive(const char *filename, uint16_t port)
{
	lsocket_t server;
	netfile_t netf;

	memset(&netf, 0, sizeof(netf));
	if (lsocket_server(&server, port, 1) == -1) {
		dprintf(2, "Error: can't use port %u.\n", port);
		return (-1);
	}
	if (lsocket_accept(&server, &netf.socket) == -1) {
		dprintf(2, "Error: can't connect to client: %m.\n");
		return (-1);
	}
	netf.fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (netf.fd == -1) {
		dprintf(2, "Error: can't access \"%s\".\n", filename);
		return (-1);
	}
	if (gettimeofday(&netf.tv, NULL) == -1)
		return (-1);
	rcloop(&netf);
	close(netf.fd);
	lsocket_destroy(&server);
	lsocket_destroy(&netf.socket);
	return (0);
}
