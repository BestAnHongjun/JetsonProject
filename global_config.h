#ifndef __GLOBAL_CONFIG_H__
#define __GLOBAL_CONFIG_H__

/*********** Edit Here ***********/


/********** Module Set ***********/
// UDP
#define UDP_PACK_SIZE   512
#define UDP_QUE_SIZE    1000
#define UDP_TIMEOUT     3

/*********** For Debug ***********/
#define TIMER_SWITCH    1
#define UDP_SWITCH      0

#define ERR_INFO __FILE__ << ":" << __LINE__

#endif  // __GLOBAL_CONFIG_H__
