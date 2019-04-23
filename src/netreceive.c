/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** receive function
*/

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <lsocket.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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
  char *buffer = malloc(BUFFER_SIZE);
  ssize_t rd;
  ssize_t wr;

  if (buffer == NULL) {
    dprintf(2, "Error: not enough memory\n");
    return (-1);
  }
  if (is_a_packet(netf->socket.fd) == false) {
    dprintf(2, "Error: Incorrect packet type received\n");
    free(buffer);
    return (-1);
  }
  if (read(netf->socket.fd, &netf->filesize, sizeof(netf->filesize)) != sizeof(netf->filesize)) {
    free(buffer);
    return (-1);
  }
  display_filesize(netf->filesize);
  while (netf->transfered < netf->filesize) {
    rd = read(netf->socket.fd, buffer, BUFFER_SIZE);
    if (rd == 0 || rd == -1) {
      dprintf(2, "Error: Connection aborted.\n");
      free(buffer);
      return (-1);
    }
    wr = write(netf->fd, buffer, rd);
    if (wr == 0 || wr == -1 || rd != wr) {
      dprintf(2, "Error: not enough place on disk.\n");
      free(buffer);
      return (-1);
    }
    netf->transfered += wr;
    netdisplay(netf);
  }
  printf("\n");
  free(buffer);
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
  netf.fd = open(filename, O_WRONLY | O_APPEND | O_CREAT | O_LARGEFILE, 0666);
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
