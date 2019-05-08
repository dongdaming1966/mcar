#ifndef CONFIG_H
#define CONFIG_H

//**************************//
//         function         //
//**************************//
//#define		TIMECHECK	
//#define		DATARECORD
//need define DATARECORD before define PLOT
//#define		PLOT
#define		MONITOR
#define		REALTIME
#define		MOTOR

//**************************//
//        controller        //
//**************************//
// must choose one controller.
// if more than one controller was chosen, every
// chosen controller will running, but only the 
// last one's output will be implemented.
#define		PID
//#define		LINEARPID
//#define		SLIDINGMODE	
//#define		SLIDINGMODEPID	
//#define		LINEARLQR
//#define		MPC

//**************************//
//         setting          //
//**************************//
#define         SPI0_PATH "/dev/spidev0.0"
#define         SPI1_PATH "/dev/spidev0.1"
#define		I2C_PATH "/dev/i2c-1"

#define		STARTPERIOD 0.5

#ifdef MPC
#define		SAMPLETIME 0.01
#else
#define		SAMPLETIME 0.002
#endif

#define		PARAMETERNUMBER 27

#endif //CONFIG_H
