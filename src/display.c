/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** display
*/

#include <stdint.h>
#include <stdio.h>

void netdisplay(uint64_t total, uint64_t done)
{
	int perc;

	perc = (double)done / (double)total * 100;
	printf("Transfert: % 3d\n", perc);
	fflush(stdin);
}
