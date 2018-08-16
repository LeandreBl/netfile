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
	static int prev = -1;
	int perc;

	perc = (double)done / (double)total * 100;
	if (perc != prev) {
		printf("Transfert: % 3d\n", perc);
		fflush(stdin);
	}
}
