/* Copyright (C) 
* 2014 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file cpu_occupancy_rate_curve.c
* @brief CPU occupancy rate curve.
* Note, those program is copy from the book <beauty of programming>, and also
* can not run the correct result in my linux machine.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-08
*/
#include <unistd.h>
#include <math.h>

/**
* @brief CPU occupancy rate 50% curve.
*/
void cpu_occupancy_rate_50percent_curve() {
	int i = 0;	
	for ( ; ; ) {
		for (i = 0; i < 9600000; i ++) { }
		usleep(10);
	}
}

/**
* @brief CPU occupancy rate sine curve.
*/
void cpu_occupancy_rate_sine_curve() {

	#define SAMPLE_COUNT 200
	#define PI 3.14159265354
	#define HEIGHT 300
	const double HALF_HEIGHT = HEIGHT / 2.0;

	double busyspan[SAMPLE_COUNT] = {0};
	double increment_radian = 2.0 / (double) SAMPLE_COUNT, radian = 0.0;
	int i = 0, j = 0;
	for (i = 0; i < SAMPLE_COUNT; i ++, radian += increment_radian) {
		busyspan[i] = sin(radian * PI) * HALF_HEIGHT + HALF_HEIGHT;
	}

	for (i = 0; ; i = (i+1)%SAMPLE_COUNT) {
		for (j = 0; j < busyspan[i]; j ++) { }
		usleep(HEIGHT - busyspan[i]);
	}
}

int main(int argc, const char *argv[])
{
	// cpu_occupancy_rate_50percent_curve();
	cpu_occupancy_rate_sine_curve();

	return 0;
}
