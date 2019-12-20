/*
 * chestI2C.h
 * Definition of a class to interface with the Simulator Chest Sensors
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

#ifndef CPRI2C_H_
#define CPRI2C_H_
#define CPR_I2C_BUFFER 0x80

#define MAX_BUS 64
#define CPR_BASE_ADDR		0x18
#define CPR_MAX_ADDR		0x19

class cprI2C {

private:
	int I2CBus;;
	char I2CdataBuffer[CPR_I2C_BUFFER];
	char I2Cnamebuf[MAX_BUS];
	int I2Cfile;
	int I2CAddr;
public:
	cprI2C(int dummy);
	int scanForSensor(void );
	int readRegister(int reg );
	int readRegister16(int reg );
	int writeRegister(int reg, unsigned char val );
	int readSensor(void );
	int present;

	unsigned int count;
	int readingX;
	int readingY;
	int readingZ;
	int lastCompression;
	int lastRelease;
	int duration;
	int averageCompForDuration;
	int averageReleaseForDuration;
	int averageCompForRecent;
	int averageReleaseForRecent;
	
	//float temperature;
	
	virtual ~cprI2C();
};

// Definitions for LIS3DH Chip
#define STATUS_REG_AUX		0x07	// Status for ADCs
#define SRA_321OR			0x80	// Overrun Occured
#define SRA_3OR				0x40
#define SRA_2OR				0x20
#define SRA_1OR				0x10
#define SRA_321DA			0x08	// Data Available
#define SRA_3DA				0x04
#define SRA_2DA				0x02
#define SRA_1DA				0x01

#define OUT_ADC1_L			0x08	// Accelleration Data
#define OUT_ADC1_H			0x09
#define OUT_ADC2_L			0x0A
#define OUT_ADC2_H			0x0B
#define OUT_ADC3_L			0x0C
#define OUT_ADC3_H			0x0D

#define INT_COUNTER_REG		0x0E
#define WHO_AM_I			0x0F	// ID Register: Returns 0x33
#define TEMP_CFG_REG		0x1F	// Temperature Config
#define TEMP_ADC_PD			0x80	// ADC Enabled
#define TEMP_TEMP_EN		0x40	// Temperature Probe Enabled

#define CTRL_REG1			0x20
#define CR1_ODR_BITS		0xF0	// Data Rate Selected bits
#define CR1_LPEN			0x08	// Low Power Enable
#define CR1_ZEN				0x04	// Z Enable
#define CR1_YEN				0x02	// Y Enable
#define CR1_XEN				0x01	// X Enable
#define	CR1_ODR_PD			0x0	//  Power Down
#define	CR1_ODR_1Hz			0x1	//  Normal/Low power 1 Hz
#define	CR1_ODR_10Hz		0x2	//  Normal/Low power 10 Hz
#define	CR1_ODR_25Hz		0x3	//  Normal/Low power 25 Hz
#define	CR1_ODR_50Hz		0x4	//  Normal/Low power 50 Hz
#define	CR1_ODR_100Hz		0x5	//  Normal/Low power 100 Hz
#define	CR1_ODR_200Hz		0x6	//  Normal/Low power 200 Hz
#define	CR1_ODR_400Hz		0x7	//  Normal/Low power 400 Hz
#define CR1_ODR_LP_1_6KHz	0x8 //	Low Power 1.6 Khz
	// Note: Yes, setting 0x9 has different speed based on the power mode.
#define CR1_ODR_NP_1_25KHz	0x9	//  Normal power 1.25 KHz 
#define CR1_ODR_LP_5KHz		0x9	//  Low power 5 KHz

#define CTRL_REG2			0x21
#define CR2_HPM_BITS		0xC0	// High-pass filter Mode
#define CR2_HPFC_BITS		0x30	// High-pass filter Cut off freq.
#define CR2_FDS				0x08	// Filter Data Select. 0 - bypass, 1 - filtered
#define CR2_HPCLICK			0x04	// High Pass filter enabled for CLICK function
#define CR2_HPIS2			0x02	// High Pass filter enabled for AOI function on Int 2
#define CR2_HPIS1			0x01	// High Pass filter enabled for AOI function on Int 1
#define CR2_HPM_NORM_0		0x0		// Normal
#define CR2_HPM_REF			0x1		// Reference signal for Filtering
#define CR2_HPM_NORM_1		0x2		// Normal
#define CR2_HPM_AUTORESET	0x3		// Autoreset on Interrupt Event

#define CTRL_REG3			0x22
#define CR3_I1_CLICK		0x80	// CLICK interrupt on INT1
#define CR3_I1_AOI1			0x40
#define CR3_I1_AOI2			0x20
#define CR3_I1_DRDY1		0x10
#define CR3_I1_DRDY2		0x08
#define CR3_I1_WTM			0x04
#define CR3_I1_OVERRUN		0x02

#define CTRL_REG4			0x23
#define CR4_BDU				0x80	// Block Data Update
#define CR4_BLE				0x40	// Big/little endian 0-DataLSB@lower address
#define CR4_FS_BITS			0x30	// Full Scale Selection
#define CR4_FS_2G			0x0
#define CR4_FS_4G			0x10
#define CR4_FS_8G			0x20
#define CR4_FS_16G			0x30
#define CR4_HR				0x08	// High Resolution Mode
#define CR4_ST_BITS			0x06	// Self test Enable (00 - Normal, 01- Self Test 0, 02- Self test 1)
#define CR4_SIM				0x01	// SPI Mode Select (0 - 4 Wire, 1 - 3 Wire)

#define CTRL_REG5			0x24
#define CR5_BOOT			0x80	// Reboot Memory Content
#define CR5_FIFO_EN			0x40
#define CR5_LIR_INT1		0x08	// Latch Int Request on INT1_SRC
#define CR5_D4D_INT1		0x04	// 4D Enable

#define CTRL_REG6			0x25
#define CR6_I2_CLICKEN		0x80
#define CR6_I2_INT1			0x40
#define CR6_BOOT_I1			0x10
#define CR6_H_LACTIVE		0x02

#define REFERENCE			0x26	// Reference/DataCapture

#define STATUS_REG			0x27	// Status for Axis
#define SR_ZYXOR			0x80	// Overrun Occured
#define SR_ZOR				0x40
#define SR_YOR				0x20
#define SR_XOR				0x10
#define SR_ZYXDA			0x08	// Data Available
#define SR_ZDA				0x04
#define SR_YDA				0x02
#define SR_XDA				0x01

#define	OUT_X_L				0x28
#define	OUT_X_H				0x29
#define	OUT_Y_L				0x2A
#define	OUT_Y_H				0x2B
#define	OUT_Z_L				0x2C
#define	OUT_Z_H				0x2D

#define FIFO_CTRL_REG		0x2E
#define FCR_FM_BITS			0xC0	// FIFO Mode Select (00- Bypass, 01- FIFO, 10- Stream, 11- Trigger)
#define FCR_TR				0x20	// Trigger 0- INT1, 1-INT2
#define FCR_FTH_BITS		0x1F	// 

#define FIFO_SRC_REG		0x2F
#define FSR_WTM				0x80
#define FSR_OVRN_FIFO		0x40
#define FSR_EMPTY			0x20
#define FSR_FSS_BITS		0x1F

#define INT1_CFG			0x30
#define INTC_AOI			0x80
#define INTC_6D				0x40
#define INTC_ZHIE_ZUPE		0x20	// Enable interrupt generation on Z high event or on Direction recognition. Default
									// value: 0 (0: disable interrupt request;1: enable interrupt request)
#define INTC_ZLIE_ZDOWNE	0x10	// Enable interrupt generation on Z low event or on Direction recognition. Default
									// value: 0 (0: disable interrupt request;1: enable interrupt request)
#define INTC_YHIE_YUPE		0x08	// Enable interrupt generation on Y high event or on Direction recognition. Default
									// value: 0 (0: disable interrupt request; 1: enable interrupt request.)
#define INTC_YLIE_YDOWNE	0x04	// Enable interrupt generation on Y low event or on Direction recognition. Default
									// value: 0 (0: disable interrupt request; 1: enable interrupt request.)
#define INTC_XHIE_XUPE		0x02	// Enable interrupt generation on X high event or on Direction recognition. Default
									//	value: 0 (0: disable interrupt request; 1: enable interrupt request.)
#define INTC_XLIE_XDOWNE 	0x01	// Enable interrupt generation on X low event or on Direction recognition. Default
									// value: 0 (0: disable interrupt request; 1: enable interrupt request.)

#define INT1_SOURCE			0x31	// Source
#define INT1S_IA			0x40	// Interrupt Active
#define INT1S_ZH			0x20	// High Event
#define INT1S_ZL			0x10	// Low Event
#define INT1S_YH			0x08
#define INT1S_YL			0x04
#define INT1S_XH			0x02
#define INT1S_XL			0x01

#define INT1_THS			0x32	// Interrupt Threshold (7-biut value)
#define INT1_DURATION		0x33	// Duration for Int condition based on ODR
#define CLICK_CFG			0x38
#define CC_ZD				0x20	// Double Click on Z
#define CC_ZS				0x10	// Single Click on Z
#define CC_YD				0x08	// Double Click on Y
#define CC_YS				0x04	// Single Click on Y
#define CC_XD				0x02	// Double Click on X
#define CC_XS				0x01	// Single Click on X

#define CLICK_SRC			0x39
#define CS_IA				0x40	// Interrupt Active
#define CS_DCICK			0x20	// Double Click Enable
#define CS_SCLICK			0x10	// Single Click Enable
#define CS_SIGN				0x08	// CLICK-CLICK Sign detection 0-Positive, 1-Negative
#define CS_Z				0x04	// Z CLICK-CLICK 
#define CS_Y				0x02	// Y CLICK-CLICK 
#define CS_X				0x01	// X CLICK-CLICK 

#define CLICK_THS			0x3A	// CLICK-CLICK  Threshold
#define	TIME_LIMIT			0x3B	// CLICK-CLICK Time Limit
#define TIME_LATENCY		0x3C	// CLICK-CLICK Time Latency
#define TIME_WINDOW			0x3D	// CLICK-CLICK Time Window

#endif /* CPRI2C_H_ */
