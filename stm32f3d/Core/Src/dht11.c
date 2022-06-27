#include "<stm32f303VCTx.h>"                  // Device header
#include "dht11.h"
/*******************************************************************************
* Название функции: DHT11_Init ()
 * Функция: инициализация DHT11
 * Контакт: PA4
 * Выход: возврат 0: успешная инициализация, 1: сбой
*******************************************************************************/
u8 DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin=DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
	 GPIO_SetBits (GPIO_DHT11, DHT11); // Подъехать

	DHT11_Rst();
	return DHT11_Check();
}
/*******************************************************************************
 * Название функции: DHT11_Rst (), DHT11_Check ()
 * Функциональная функция: сброс DHT11, обнаружение DHT11
 * Контакт: PA4
 * Выход: // return 1: существование DHT11 не обнаружено, return 0: существование
*******************************************************************************/
void DHT11_Rst()
{
		 DHT11_IO_OUT (); // Настраиваем порт io как режим вывода
         DHT11_DQ_OUT = 0; // Опускаем DQ вниз
         delay_ms (20); // Опускаем минимум на 18 мс
    DHT11_DQ_OUT=1; 		//DQ=1
		 delay_us (30); // Хост подтягивается на 20 ~ 40 мкс
}
u8 DHT11_Check()
{
	u8 retry=0;
	 DHT11_IO_IN (); // Настраиваем порт io как режим ввода
         while (DHT11_DQ_IN && retry <100) // Цикл высокого уровня, выход на низкий уровень
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;
	else retry=0;
         while (! DHT11_DQ_IN && retry <100) // После того, как DHT11 будет опущен, он будет снова вытянут на 40 ~ 50 мкс.
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;
	return 0;
}


/*******************************************************************************
 * Название функции: DHT11_Read_Bit (void)
 * Функция: читать бит из DHT11
 * Контакт: PA4
 * Выход: возвращаемое значение: 1/0
*******************************************************************************/
u8 DHT11_Read_Bit(void)
{
 	u8 retry=0;
	 while (DHT11_DQ_IN && retry <100) // Ждем начала низкого уровня 12-14us
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	 while (! DHT11_DQ_IN && retry <100) // Ожидание высокого уровня 26-28us означает 0, 116-118us означает 1
	{
		retry++;
		delay_us(1);
	}
	 delay_us (40); // ждем 40us
	if(DHT11_DQ_IN)return 1;
	else return 0;
}
/*******************************************************************************
 * Название функции: DHT11_Read_Byte (void)
 * Функция: читать байт из DHT11
 * Контакт: PA4
 * Вывод: возвращаемое значение: прочитанные данные
*******************************************************************************/
u8 DHT11_Read_Byte(void)
{
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++)
	{
   		dat<<=1;
	    dat|=DHT11_Read_Bit();
    }
    return dat;
}
/*******************************************************************************
 * Название функции: DHT11_Read_Data (u8 * temp, u8 * humi)
 * Функциональная функция: чтение данных из DHT11 один раз
 * Контакт: PA4
 * Выход: возвращаемое значение: 0, нормально; 1, чтение не удалось
*******************************************************************************/
u8 DHT11_Read_Data(u8 *temp,u8 *humi)
{
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		 for (i = 0; i <5; i ++) // читаем 40-битные данные
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}

	}
		else
		return 1;
		return 0;
}
 // Конфигурация режима вывода DHT11
void DHT11_IO_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Двухтактный вывод
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
}

 // Конфигурация режима ввода DHT11
void DHT11_IO_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Режим ввода Pull up
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
}
