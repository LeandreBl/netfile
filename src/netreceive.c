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

static bool is_a_packet(int fd)
{
	uint16_t magic;

	if (read(fd, &magic, sizeof(magic)) != sizeof(magic))
		return (false);
	if (magic == 0xca0c)
		return (true);
	return (false);
}

static int rcloop(lsocket_t *client, int fd)
{
	uint64_t done = 0;
	uint64_t filesize;
	char buffer[8192];
	ssize_t rd;
	ssize_t wr;

	if (is_a_packet(client->fd) == false) {
		dprintf(2, "Error: Incorrect packet type received\n");
		return (-1);
	}
	if (read(client->fd, &filesize, sizeof(filesize)) != sizeof(filesize))
		return (-1);
	printf("Downloading file of size %lu:\n", (long unsigned int)filesize);
	while (done < filesize) {
		rd = read(client->fd, buffer, sizeof(buffer));
		if (rd == 0 || rd == -1)
			return (-1);
		wr = write(fd, buffer, rd);
		if (wr == 0 || wr == -1 || rd != wr)
			return (-1);
		done += wr;
		printf("\rTransfert: [%03d%%]\n", (int)((double)done / (double)filesize * 100));
	}
	return (0);
}

int netreceive(const char *filename, uint16_t port)
{
	lsocket_t server;
	lsocket_t client;
	int fd;

	if (lsocket_server(&server, port, 1) == -1) {
		dprintf(2, "Error: can't use port %u.\n", port);
		return (-1);
	}
	if (lsocket_accept(&server, &client) == -1) {
		dprintf(2, "Error: can't connect to client: %m.\n");
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd == -1) {
		dprintf(2, "Error: can't access \"%s\".\n", filename);
		return (-1);
	}
	if (rcloop(&client, fd) == -1)
		dprintf(2, "Error: connection aborted\n");
	close(fd);
	lsocket_destroy(&server);
	lsocket_destroy(&client);
	return (0);
}
