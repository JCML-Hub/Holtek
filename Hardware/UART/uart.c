#include "uart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32_board.h"

uint8_t RxData[MAX_RX_LEN];
volatile uint8_t RxFlag;
/**************************实现函数********************************************
函数说明：配置usart串口

*******************************************************************************/ 
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.AFIO = 1;
	COM1_CLK(CKCUClock)  = 1;  //开启时钟
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
  AFIO_GPxConfig(USART_GPIO_GROUP, USART_TX_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(USART_GPIO_GROUP, USART_RX_PIN, AFIO_FUN_USART_UART);

  /*
		波特率： 115200
		长度：   8bits
		停止位： 1位
	  校验位： 无			
	  模式：   正常模式
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(COM1_PORT, &USART_InitStructure);

  // 使能 COM1_PORT  发送和接收 
  USART_TxCmd(COM1_PORT, ENABLE);
  USART_RxCmd(COM1_PORT, ENABLE);
	
	 //中断设置    
   NVIC_EnableIRQ(COM1_IRQn);
	 
	 USART_IntConfig(COM1_PORT, USART_INT_RXDR , ENABLE);

   USART_IntConfig(COM1_PORT, USART_FLAG_RXDR , ENABLE);
	 USART_IntConfig(COM1_PORT, USART_FLAG_TXDE , ENABLE);
	 
	   /* 设置FIFO接收等级                                                                                   */
  USART_RXTLConfig(COM1_PORT, USART_RXTL_04);
}


/**************************实现函数********************************************
函数说明：接收中断服务函数

*******************************************************************************/ 
void COM1_IRQHandler(void)
{
	static uint8_t RxConter=0;
	if(USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR))         //接收中断
	{
		RxData[RxConter] = USART_ReceiveData(COM1_PORT);                         //读取接收到的数据
		if (RxData[RxConter] == '\n' ){//如果接收到结束字符
			RxData[RxConter+1] = '\0';		
			RxConter=0;
			RxFlag=1;
		}else{
			RxConter++;
		}
		// printf("data = %c\n",data);                                  //把收到的数据发送回电脑		
	}
}
/**************************实现函数********************************************
函数说明：FIFO

*******************************************************************************/ 
void USART_Tx(const char* TxBuffer, u32 length)
{
  int i;

  for (i = 0; i < length; i++)
  {
    while (!USART_GetFlagStatus(COM1_PORT, USART_FLAG_TXC));
    USART_SendData(COM1_PORT, TxBuffer[i]);
  }
}


/**************************实现函数********************************************
函数说明：发送一个字节

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************实现函数********************************************
函数说明：发送数组

*******************************************************************************/ 
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart_Sendbyte(USARTx,*array);
		array++;
	}
}
 /**************************实现函数********************************************
函数说明：发送字符串

*******************************************************************************/ 

void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}


