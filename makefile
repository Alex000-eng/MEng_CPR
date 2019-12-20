#
# This file is part of the sim-ctl distribution (https://github.com/OpenVetSimDevelopers/sim-ctl).
# 
# Copyright (c) 2019 VetSim, Cornell University College of Veterinary Medicine Ithaca, NY
# 
# This program is free software: you can redistribute it and/or modify  
# it under the terms of the GNU General Public License as published by  
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program. If not, see <http://www.gnu.org/licenses/>.

installTargets=cprScan 
targets=$(installTargets)
CFLAGS=-pthread -Wall -g -ggdb
LDFLAGS=-lrt

default:	$(targets)

all: $(targets)

cprScan: cprScan.cpp  cprI2C.o cprI2C.h ../comm/simUtil.o ../comm/simUtil.h
	g++ cprScan.cpp  $(CFLAGS) $(LDFLAGS) cprI2C.o ../comm/simUtil.o -lrt -o cprScan
	
cprI2C.o: cprI2C.cpp cprI2C.h ../comm/simUtil.h ../comm/shmData.h
	g++   $(CFLAGS) -c -o cprI2C.o cprI2C.cpp
	
install: $(installTargets) .FORCE
	sudo cp -u $(installTargets) /usr/local/bin

clean: .FORCE
	rm -f $(targets) *.o *.cgi
	
.FORCE: