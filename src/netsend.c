/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** netsend
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <lsocket.h>

static int send_header(lsocket_t *socket, struct stat *st)
{
	uint16_t magic = 0xca0c;
	uint64_t filesize = (uint64_t)st->st_size;

	if (write(socket->fd, &magic, sizeof(magic)) != sizeof(magic) ||
		write(socket->fd, &filesize, sizeof(filesize)) != sizeof (filesize)) {
			dprintf(2, "Error: connection lost.\n");
			return (-1);
		}
	return (0);
}

static int snloop(lsocket_t *socket, int fd, uint64_t filesize)
{
	uint64_t done = 0;
	char buffer[8192];
	ssize_t rd;
	ssize_t wr;

	printf("Sending file of size %lu\n", filesize);
	while (done < filesize) {
		rd = read(fd, buffer, sizeof(buffer));
		if (rd == 0 || rd == -1)
			return (-1);
		wr = write(socket->fd, buffer, rd);
		if (wr == 0 || wr == -1 || rd != wr)
			return (-1);
		done += wr;
		printf("\rTransfert: [%03d%%]\n", (int)((double)done / (double)filesize * 100));
	}
	return (0);
}

int netsend(const char *filename, const char *ipaddr, uint16_t port)
{
	lsocket_t socket;
	struct stat st;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1 || stat(filename, &st) == -1) {
		dprintf(2, "Can't access file \"%s\".\n", filename);
		return (-1);
	}
	if (lsocket_connect(&socket, ipaddr, port) == -1) {
		dprintf(2, "Error: Could not connect to \"%s\" on port %u.\n", ipaddr, port);
		return (-1);
	}
	if (send_header(&socket, &st) == -1)
		return (-1);
	if (snloop(&socket, fd, (uint64_t)st.st_size) == -1)
		dprintf(2, "Error: connection aborted\n");
	close(fd);
	lsocket_destroy(&socket);
	return (0);
}