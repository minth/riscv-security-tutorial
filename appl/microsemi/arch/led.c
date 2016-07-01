
#include "shared.h"
gpio_instance_t g_gpio0;
int led_state;

void initLeds(void){
	  GPIO_init(&g_gpio0, COREGPIO_IN_BASE_ADDR, GPIO_APB_32_BITS_BUS);

	  // DIP Switches
	  GPIO_config(&g_gpio0, GPIO_0, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_1, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_2, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_3, GPIO_INPUT_MODE);

	  // Buttons
	  GPIO_config(&g_gpio0, GPIO_4, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_5, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_6, GPIO_INPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_7, GPIO_INPUT_MODE);

	  // LEDs
	  GPIO_config(&g_gpio0, GPIO_8, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_9, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_10, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_11, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_12, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_13, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_14, GPIO_OUTPUT_MODE);
	  GPIO_config(&g_gpio0, GPIO_15, GPIO_OUTPUT_MODE);

	  GPIO_set_outputs(&g_gpio0, 0x0000);
	  led_state = 0;
}

void setLed(int index, int state){
    if(state == 1){
    	led_state |= 1 << index;
    }else{
    	led_state &= ~(1 << index);
    }
}

void writeLeds(void){
	GPIO_set_outputs(&g_gpio0,led_state << 8);
	dbprintf("writeLeds %x\n", led_state);
}