/**********************TH32 µãµÆ**********************
Author:JCML
Brif:Please use KEIL V536
Date:2023-2-29
Version: 2023-03-01				ÒÆÖ²RT-Thread
******************************************************/

#include "ht32.h"
#include "ht32_board.h"
#include "led.h"
#include "delay.h"
#include "uart.h"
#include "rtthread.h"


static void led1_thread_entry(void* parameter)
{

    for (;;)
    {
			rt_kprintf("Test Thread 1\n");
			LED1_ON();
			LED2_ON();
			rt_thread_mdelay(100);
			LED1_OFF();
			LED2_OFF();
			rt_thread_mdelay(100);
    }
}
static void led2_thread_entry(void* parameter)
{

    for (;;)
    {
			rt_kprintf("Test Thread 2\n");
			LED1_ON();
			rt_thread_mdelay(100);
			LED1_OFF();
			rt_thread_mdelay(100);
    }
}
void led_thread_init(void)
{
  	static rt_uint8_t s_led1_stack[256];
  	static rt_uint8_t s_led2_stack[256];
  	static struct rt_thread led1_thread;
  	static struct rt_thread led2_thread;  
	rt_err_t result;

    result = rt_thread_init(&led1_thread,"led1",led1_thread_entry,RT_NULL,(rt_uint8_t*)&s_led1_stack[0],sizeof(s_led1_stack),7,5);
		result = rt_thread_init(&led2_thread,"led2",led2_thread_entry,RT_NULL,(rt_uint8_t*)&s_led2_stack[0],sizeof(s_led2_stack),7,4);
		
    if (result == RT_EOK)
    {
        rt_thread_startup(&led1_thread);
				rt_thread_startup(&led2_thread);
    }
}

int main()
{
	Led_Init();
	led_thread_init();
}
