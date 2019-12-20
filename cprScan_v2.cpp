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
	double X,Y,Z;
	int cc;
    	cprI2C cprSense(0 );;
    	newData = cprSense.readSensor();
	lastX = cprSense.readingX;
	lastY = cprSense.readingY;
	lastZ = cprSense.readingZ;
	X=lastX/1300.0;
	Y=lastY/1300.0;
	Z=lastZ/1300.0;
	//printf("%3d:\t%05d:\t%05d\t%05d\t: %05d  %d\n", count, loop, lastZ, diffZ, cummZ, compressed );
	printf("%05d\t%05f\t%05f\t%05f \n", loop, X, Y, Z);
	usleep(10000);
	while ( 1 )
	{
		printf("%05d\t%05f\t%05f\t%05f \n", loop, X, Y, Z);
		while ( ! ( newData = cprSense.readSensor() ) )
		{
			usleep(5000);
		}
		if ( newData )
		{
			loop++;
			lastZ = cprSense.readingZ;
			lastX = cprSense.readingX;
            		lastY = cprSense.readingY;
			X=lastX/1300.0;
			Y=lastY/1300.0;
			Z=lastZ/1300.0;


        }
		
    }
	return 0;
}
