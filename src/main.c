/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** main
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netfile.h"

static void usage(const char *binary_name)
{
  dprintf(2,
          "USAGE:\n"
          "\t%s [filename] [ip address] [port] to send a file.\n"
          "\t%s [filename] [port] to receive a file.\n",
          binary_name, binary_name);
}

int main(int ac, char **av)
{
  if (ac != 3 && ac != 4) {
    usage(av[0]);
    return (1);
  }
  if (ac == 3)
    return (netreceive(av[1], atoi(av[2])));
  return (netsend(av[1], av[2], atoi(av[3])));
}
