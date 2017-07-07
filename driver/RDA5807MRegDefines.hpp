/**************************************************
 * RDA5807MRegDefines.hpp
 * Author: Ben Sherman
 *************************************************/

#ifndef RDA5807MREGDEFINES_HPP
#define RDA5807MREGDEFINES_HPP

/*
 * Defines for register masks
 * Taken from V1.1 of the datasheet
 */
 
// Register 0x00
#define CHIP_ID     0xFF00

// Register 0x02
#define DHIZ        0x8000
#define DMUTE       0x4000
#define DMONO       0x2000
#define DBASS       0x1000
#define RCLK_NCM    0x0800
#define RCLK_DIM    0x0400
#define SEEKUP      0x0200
#define SEEK        0x0100
#define SKMODE      0x0080
#define CLK_MODE    0x0070
#define RDS_EN      0x0008
#define NEW_METHOD  0x0004
#define SOFT_RESET  0x0002
#define ENABLE      0x0001

// Register 0x03
#define CHAN        0xFFC0
#define DIRECT_MODE 0x0020
#define TUNE        0x0010
#define BAND        0x000C
#define SPACE       0x0003

// Register 0x04
#define RSVD_04_0   0xF000
#define DE          0x0800
#define RSVD_04_1   0x0400
#define SOFTMUTE_EN 0x0200
#define AFCD        0x0100

// Register 0x05
#define INT_MODE    0x8000
#define RSVD_05_0   0x7000
#define SEEKTH      0x0F00
#define RSVD_05_1   0x0030
#define VOLUME      0x000F

// Register 0x06
#define RSVD_06_0   0x8000
#define OPEN_MODE   0x6000

// Register 0x07
#define RSVD_07_0       0x8000
#define TH_SOFRBLEND    0x7C00
#define R_65M_50M_MODE  0x0200 // The leading R_ is to avoid starting the define with a number
#define RSVD_07_1       0x0100
#define SEEK_TH_OLD     0x00FC
#define SOFTBLEND_EN    0x0002
#define FREQ_MODE       0x0001

// Register 0x0A
#define RDSR        0x8000
#define STC         0x4000
#define SF          0x2000
#define RDSS        0x1000
#define BLK_E       0x0800
#define ST          0x0400
#define READCHAN    0x03FF

// Register 0x0B
#define RSSI        0xFE00
#define FM_TRUE     0x0100
#define FM_READY    0x0080
#define RSVD_0B_0   0x0060
#define ABCD_E      0x0010
#define BLERA       0x000C
#define BLERB       0x0003

// Register 0x0C
#define RDSA        0xFFFF

// Register Ox0D
#define RDSB        0xFFFF

// Register 0x0E
#define RDSC        0xFFFF

// Register 0X0F
#define RDSD        0xFFFF

/**
 * RDS/RBDS Common Mappings
 */
// Block 1/A
#define PI_CODE     0xFFFF

// Block 2/B
#define GROUP_TYPE      0xF000
#define VERSION_CODE    0x0800
#define TRAFFIC_PROGRAM 0x0400
#define PROGRAM_TYPE    0x03E0



#endif // ifndef RDA5807MREGDEFINES_HPP
