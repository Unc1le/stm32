#ifndef _dht11_h
#define _dht11_h

#include "system.h"
#include "SysTick.h"




#define DHT11 (GPIO_Pin_4) //PA4
#define GPIO_DHT11 GPIOA
 #define DHT11_DQ_IN PAin (4) // ввод
 #define DHT11_DQ_OUT PAout (4) // вывод
void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
u8 DHT11_Init(void);
void DHT11_Rst(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);

#endif
