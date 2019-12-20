/*
 * chestScan.cpp
 *
 * This file is part of the sim-ctl distribution (https://github.com/OpenVetSimDevelopers/sim-ctl).
 * 
 * Copyright (c) 2019 VetSim, Cornell University College of Veterinary Medicine Ithaca, NY
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <string>
#include <unistd.h>
#include "cprI2C.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <termios.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>

#include "../comm/simCtlComm.h"
#include "../comm/simUtil.h"
#include "../comm/shmData.h"

using namespace std;

struct shmData *shmData;

char msgbuf[2048];

int debug = 0;
#define Z_IDLE		16000
#define Z_COMPRESS	19000
#define Z_RELEASE	5000
#define X_Y_LIMIT	7000
#define CPR_HOLD	20

int main(int argc, char *argv[]){
	printf("Start!");
	int sts;
	int newData;
	int lastZ, diffZ;
	int lastX, lastY;
	int cummZ;
	int count = 0;
	int compressed = 0;
	int loop = 0;
	cprI2C cprSense(0 );
	newData = cprSense.readSensor();
	diffZ = cprSense.readingZ;
	lastX = cprSense.readingX;
	lastY = cprSense.readingY;
	lastZ = cprSense.readingZ;
	cummZ = 0;
	printf("%05d\t%05d\t%05d\t%05d  %d\n", loop, lastX, lastY, lastZ, compressed );
	if ( ! debug )
	{
		printf("No debug!");
		daemonize();
	}
	else
	{
		printf("Debug!");
		catchFaults();
	}
	sts = initSHM(0 );
	if ( sts )
	{	
		printf("Error!");
		printf(msgbuf, "SHM Failed (%d) - Exiting", sts );
		//sprintf(msgbuf, "SHM Failed (%d) - Exiting", sts );
		log_message("", msgbuf );
		exit ( -1 );
	}
	
	//cprI2C cprSense(0 );
	if ( cprSense.present == 0 )
	{
		
		//log_message("","No cprSense Found on bus - Waiting" );
		printf("No cprSense Found on bus - Waiting");
		while ( 1 )
		{
			// We loop here to keep the deamon open. Makes for a cleaner shutdown.
			sleep(60);
		}
		//log_message("","cprSense Found Sensor" );
		printf("cprSense Found Sensor" );

	}
	
	
	// shmData->present = cprSense.present;
	newData = cprSense.readSensor();
	diffZ = cprSense.readingZ;
	lastX = cprSense.readingX;

	lastY = cprSense.readingY;
	lastZ = cprSense.readingZ;
	cummZ = 0;
	
	//printf("%3d:\t%05d:\t%05d\t%05d\t: %05d  %d\n", count, loop, lastZ, diffZ, cummZ, compressed );
	printf("All set now!");
	printf("%05d\t%05d\t%05d\t%05d  %d\n", loop, lastX, lastY, lastZ, compressed );
	usleep(10000);
	while ( 1 )
	{
		while ( ! ( newData = cprSense.readSensor() ) )
		{
			usleep(5000);
		}
		if ( newData )
		{
			loop++;
			diffZ = cprSense.readingZ - lastZ;
			lastZ = cprSense.readingZ;
			lastX = cprSense.readingX;
			lastY = cprSense.readingY;
			cummZ += diffZ;

			if ( compressed )
			{
				printf("compressed!!!/n");
				count++;
				if ( count > CPR_HOLD )
				{
					printf("Over hold!");
					compressed = 0;
					shmData->cpr.compression = 0;
					shmData->cpr.release = 0;
				}
				/*
				if ( lastZ < Z_RELEASE )
				{
					// released
					compressed = 0;
					shmData->cpr.compression = 0;
					shmData->cpr.release = 100;
				} */
			}
			else

			{
				/*
				if ( ( abs(lastX ) > X_Y_LIMIT ) || ( abs(lastY ) > X_Y_LIMIT ) )
				{
					// Large X or Y displacement indication moving the mannequin rather than possible compression
				}
				else if ( abs(lastZ) > Z_COMPRESS  )
					*/
				printf("Compiled!");
				if ( ( abs(lastX ) > X_Y_LIMIT ) || ( abs(lastY ) > X_Y_LIMIT ) ||  abs(lastZ) > Z_COMPRESS )
				{
					compressed = 1;
					shmData->cpr.compression = 1;
					shmData->cpr.release = 0;
					count = 0;
				}
				else
				{
					// If we are short of the Z_COMPRESS threshold, limit the compression to 200 ms.
					count++;
					if ( count > CPR_HOLD )
					{
						compressed = 0;
						shmData->cpr.compression = 0;
						shmData->cpr.release = 50;
					}
				}
			}
			if (  debug &&  ( compressed || ( abs(diffZ) > 1000 ) ) )
			{
				//printf("%3d:\t%05d:\t%05d\t%05d\t: %05d  %d\n", count, loop, lastZ, diffZ, cummZ, compressed );
				printf("%05d\t%05d\t%05d\t%05d  %d\n", loop, lastX, lastY, lastZ, compressed );
			}
			shmData->cpr.x = lastX;
			shmData->cpr.y = lastY;
			shmData->cpr.z = lastZ;
		}
		usleep(20000);
	}

	return 0;
}
