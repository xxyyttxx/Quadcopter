#ifndef __GPIO_HMC5883_H
#define __GPIO_HMC5883_H

#include <stdint.h>

uint8_t InitHMC5883(void);
void hmc_correct(short mag_mid[3]);
void updateHMC5883(short mag[3]);

#endif /* __GPIO_HMC5883_H */
