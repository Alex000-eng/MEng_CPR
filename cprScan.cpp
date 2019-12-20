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
int main(int argc, char *argv[])
{
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
	
	//printf("%3d:\t%05d:\t%05d\t%05d\t: %05d  %d\n", count, loop, lastZ, diffZ, cummZ, compressed );
	//printf("%05d\t%05d\t%05d\t%05d  %d\n", loop, lastX, lastY, lastZ, compressed );
	usleep(10000);
	while ( 1 )
	{
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
#if 0
			if ( compressed )
			{
				count++;
				if ( count > CPR_HOLD )
				{
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
#endif
			{
				/*
				if ( ( abs(lastX ) > X_Y_LIMIT ) || ( abs(lastY ) > X_Y_LIMIT ) )
				{
					// Large X or Y displacement indication moving the mannequin rather than possible compression
				}
				else if ( abs(lastZ) > Z_COMPRESS  )
					*/
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
}
