#ifndef __GPIO_HMC5883_H
#define __GPIO_HMC5883_H

#include <stdint.h>

uint8_t InitHMC5883(void);
void updateHMC5883(short mag[3]);

#endif /* __GPIO_HMC5883_H */
