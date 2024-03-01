#include "uart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32_board.h"

uint8_t RxData[MAX_RX_LEN];
volatile uint8_t RxFlag;
/**************************ʵ�ֺ���********************************************
����˵��������usart����

*******************************************************************************/ 
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.AFIO = 1;
	COM1_CLK(CKCUClock)  = 1;  //����ʱ��
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
  AFIO_GPxConfig(USART_GPIO_GROUP, USART_TX_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(USART_GPIO_GROUP, USART_RX_PIN, AFIO_FUN_USART_UART);

  /*
		�����ʣ� 115200
		���ȣ�   8bits
		ֹͣλ�� 1λ
	  У��λ�� ��			
	  ģʽ��   ����ģʽ
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(COM1_PORT, &USART_InitStructure);

  // ʹ�� COM1_PORT  ���ͺͽ��� 
  USART_TxCmd(COM1_PORT, ENABLE);
  USART_RxCmd(COM1_PORT, ENABLE);
	
	 //�ж�����    
   NVIC_EnableIRQ(COM1_IRQn);
	 
	 USART_IntConfig(COM1_PORT, USART_INT_RXDR , ENABLE);

   USART_IntConfig(COM1_PORT, USART_FLAG_RXDR , ENABLE);
	 USART_IntConfig(COM1_PORT, USART_FLAG_TXDE , ENABLE);
	 
	   /* ����FIFO���յȼ�                                                                                   */
  USART_RXTLConfig(COM1_PORT, USART_RXTL_04);
}


/**************************ʵ�ֺ���********************************************
����˵���������жϷ�����

*******************************************************************************/ 
void COM1_IRQHandler(void)
{
	static uint8_t RxConter=0;
	if(USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR))         //�����ж�
	{
		RxData[RxConter] = USART_ReceiveData(COM1_PORT);                         //��ȡ���յ�������
		if (RxData[RxConter] == '\n' ){//������յ������ַ�
			RxData[RxConter+1] = '\0';		
			RxConter=0;
			RxFlag=1;
		}else{
			RxConter++;
		}
		// printf("data = %c\n",data);                                  //���յ������ݷ��ͻص���		
	}
}
/**************************ʵ�ֺ���********************************************
����˵����FIFO

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


/**************************ʵ�ֺ���********************************************
����˵��������һ���ֽ�

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************ʵ�ֺ���********************************************
����˵������������

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
 /**************************ʵ�ֺ���********************************************
����˵���������ַ���

*******************************************************************************/ 

void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}


