/*
** EPITECH PROJECT, 2018
** netfile
** File description:
** display
*/

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "netfile.h"

static const char *units[] = {
	"B",
	"KB",
	"MB",
	"GB",
	"TB",
};

uint8_t index_unit(unsigned long int value)
{
	uint8_t val;

	for (val = -1; value > 0; ++val) {
		value /= 1000;
	}
	return (val);
}

static void clear_line(void)
{
	write(1, "\033[2K", 4);
}

static double cut_with(unsigned long int value, uint8_t val)
{
	int coef = pow(1000, val);

	return ((double)value / coef);
}

void display_filesize(unsigned long int value)
{
	uint8_t val = index_unit(value);
	double to_disp = cut_with(value, val);

	printf("Transfert file of size: %.3f%s\n", to_disp, units[val]);
}

void netdisplay(netfile_t *netf)
{
	int perc;
	struct timeval tv;
	time_t elapsed;
	uint64_t transfered = netf->transfered - netf->lastsize;
	uint8_t i;

	gettimeofday(&tv, NULL);
	elapsed = (tv.tv_sec - netf->tv.tv_sec);
	i = index_unit(transfered);
	if (elapsed >= 1) {
		netf->lastsize = netf->transfered;
		netf->speed = cut_with(transfered, i);
		netf->tv = tv;
	}
	perc = (double)netf->transfered / (double)netf->filesize * 100;
	if (perc > netf->percentage) {
		clear_line();
		printf("\rTransfert: [%d%%] at [%.3f %s/s]", perc, netf->speed, units[i]);
		fflush(stdout);
		netf->percentage = perc;
	}
}
