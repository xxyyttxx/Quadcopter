#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H

#include <stdint.h>

/// @brief GPIO初始化
void IIC_Init(void);


/// @brief 产生IIC起始信号
void IIC_Start(void);
/// @brief 产生IIC停止信号
void IIC_Stop(void);


/** 等待应答信号到来
    @retval int(_bool会不会更好)
            1 接收应答失败
            0 接收应答成功
*/
uint8_t IIC_Wait_Ack(void);
/// @brief 产生ACK应答
void IIC_Ack(void);
/// @brief 不产生ACK应答
void IIC_NAck(void);


/** @brief IIC发送一个字节.
    @param 要发送的字节数据
    @notes 不等待应答信号（没有调用IIC_Wait_Ack）
*/
void IIC_Send_Byte(uint8_t txd);


/** @brief 读1个字节，并发送ack/nack
    @param ack=1时，发送ACK，ack=0，发送nACK
*/
uint8_t IIC_Read_Byte(uint8_t ack);


#endif /* __GPIO_IIC_H */
