
//**************************//
//         function         //
//**************************//
//#define		TIMECHECK	
#define		DATARECORD
//need define DATARECORD before define PLOT
//#define		PLOT
//#define		MONITOR
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
//#define	LINEARPID

//**************************//
//         setting          //
//**************************//
#define         SPI0_PATH "/dev/spidev0.0"
#define         SPI1_PATH "/dev/spidev0.1"

#define		STARTPERIOD 0.5
#define		SAMPLETIME 0.002
