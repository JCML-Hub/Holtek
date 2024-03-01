#ifndef __UART_H__
#define __UART_H__ 			   
#include "ht32f5xxxx_usart.h"

#define USART_GPIO_GROUP             (GPIO_PA)
#define USART_TX_PIN                 (GPIO_PIN_4)
#define USART_RX_PIN                 (GPIO_PIN_5)
#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //模式设置，默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
#define MAX_RX_LEN                   (256)  

extern uint8_t RxData[];
extern volatile uint8_t RxFlag;

void USART_Configuration(void);
void COM1_IRQHandler(void);
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data);
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);
void USART_Tx(const char* TxBuffer, u32 length);
void USART_Rx(const char* RxBuffer, u32 length);
#endif

