/*
 * cprScan.cpp
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

#ifndef CPRSCAN_H_
#define CPRSCAN_H_

#define SHM_NAME	"cprSense"


struct shmData 
{
	unsigned int present;
	unsigned int count;
	unsigned int reading;
	unsigned int lastCompression;
	unsigned int lastRelease;
	unsigned int duration;
	unsigned int averageCompForDuration;
	unsigned int averageReleaseForDuration;
	unsigned int averageCompForRecent;
	unsigned int averageReleaseForRecent;
};

#endif /* CPRSCAN_H_ */