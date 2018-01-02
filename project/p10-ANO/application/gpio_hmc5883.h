
#include <stdint.h> // u8 define in stm32f4xx.h for legacy
extern short magX, magY, magZ; 
extern float magX_off, magY_off, magZ_off; 
extern float magX_cc, magY_cc, magZ_cc;  
extern short magRange0,magRange1,magRange2,magRange3,magRange4,magRange5;

 void InitHMC5883(void);
 void hmc_get_off(float mx, float my, float mz);
	void hmc_correct(void);
 void updateHMC5883(void);
 