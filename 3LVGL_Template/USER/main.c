
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "stdlib.h"

#include "../lvgl.h"
#include "lv_port_disp_template.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint8_t USART_Buffer[USART_BUFFER_SIZE];
extern uint16_t USART_Cnt;
extern uint8_t USART_NOTIFY;

/* Private function prototypes -----------------------------------------------*/
void HardWare_Init(void);
void Screen_test(void);
void LV_Init(void);

int main(void)
{
	HardWare_Init();
	LV_Init();
	/* Infinite loop */
	Screen_test();
	while (1)
	{
		lv_task_handler();
		if(lv_tick_get()%1000 == 0)
			LED1_TOGGLE;
	}
}



/* Private functions ---------------------------------------------------------*/
void HardWare_Init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Delay_Init();
	LED_Init();
	KEY_Config();
	USART_Config();
	Timer_Config();
	LCD_Init();
	printf("hardware initialize successfully!\r\n");
}
void LV_Init(void){
	lv_init();
	lv_port_disp_init();
}
void Screen_test(void){
	
	lv_obj_t * obj1 = lv_label_create(lv_scr_act());
	lv_obj_set_size(obj1, 50, 50);
	lv_obj_align(obj1, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_bg_color(obj1, lv_color_hex(0x98F5FF), 0);
	lv_obj_set_style_opa(obj1, LV_OPA_70, 0);
	lv_obj_t * sw1 = lv_switch_create(lv_scr_act());
}

