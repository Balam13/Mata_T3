/*
 * rgb_leds.h
 *
 *  Created on: 9 jun. 2021
 *      Author: cmata
 */

#include <stdio.h>
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_pit.h"

#define PIN26        26u //led verde
#define PIN22        22u //led rojo
#define PIN21        21u //led azul

#define PIT_BASE_1        PIT
#define PIT_CHANNEL       kPIT_Chnl_0
#define PIT_INTERRUPTION  PIT0_IRQHandler
#define PIT_IRQ           PIT0_IRQn
#define PIT_SOURCE_CLOCK  CLOCK_GetFreq(kCLOCK_BusClk)



gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        1,
};


volatile bool pitIsrFlag = false;

void PIT_INTERRUPTION (void)
{
	PIT_ClearStatusFlags(PIT_BASE_1,PIT_CHANNEL, kPIT_TimerFlag);
	pitIsrFlag = true;
	//_DSB();
}


pit_config_t pitConfig;



void PIT_CONFIG (void)
{

	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	PIT_GetDefaultConfig(&pitConfig);

	PIT_Init(PIT_BASE_1, &pitConfig);

	PIT_SetTimerPeriod(PIT_BASE_1,PIT_CHANNEL,USEC_TO_COUNT(150000U, PIT_SOURCE_CLOCK));

	PIT_EnableInterrupts(PIT_BASE_1,PIT_CHANNEL, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT0_IRQn);

	PIT_StartTimer(PIT_BASE_1,PIT_CHANNEL);

}



void limpiar(void);
void verde (void);
void azul (void);
void morado (void);
void rojo (void);
void amarillo (void);
void blanco(void);
void secuencia_one(void);
void secuencia_two(void);
void secuencia_three(void);
void secuencia_four(void);

void limpiar(void){
	GPIO_PortSet(GPIOB, 1u << PIN21);
	GPIO_PortSet(GPIOE, 1u << PIN26);
	GPIO_PortSet(GPIOB, 1u << PIN22);
}
//
void azul (void){
	limpiar();
	GPIO_PortClear(GPIOB, 1u << PIN21);
}
//
void morado (void){
	limpiar();
	GPIO_PortClear(GPIOB, 1u << PIN22);
	GPIO_PortClear(GPIOB, 1u << PIN21);
}
//
void rojo (void){
	limpiar();
	GPIO_PortClear(GPIOB, 1u << PIN22);
}

//
void amarillo (void){
	limpiar();
	GPIO_PortClear(GPIOB, 1u << PIN22);
	GPIO_PortClear(GPIOE, 1u << PIN26);
}
//
void verde (void){
	limpiar();
	GPIO_PortClear(GPIOE, 1u << PIN26);
}
//

void blanco(void){
	limpiar();
	GPIO_PortClear(GPIOB, 1u << PIN22);
	GPIO_PortClear(GPIOE, 1u << PIN26);
	GPIO_PortClear(GPIOB, 1u << PIN21);
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
}

void secuencia_one(void){
	//while(true){
		//if(true == pitIsrFlag){
	//PIT_INTERRUPTION();
	amarillo();
	pitIsrFlag = true;
		//pitIsrFlag = true;
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);

	//PIT_INTERRUPTION();
	//	if (true == pitIsrFlag){
	//PIT_INTERRUPTION();
	rojo();
	pitIsrFlag = true;
			//pitIsrFlag = true;

	//PIT_INTERRUPTION();
	//	if (true == pitIsrFlag){
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	//PIT_INTERRUPTION();
	//PIT_INTERRUPTION();
	morado();
	pitIsrFlag = true;
			//pitIsrFlag = true;
	//PIT_CONFIG();
	//PIT_INTERRUPTION();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	//PIT_INTERRUPTION();
}

void secuencia_two(void){
	verde();
	//PIT_INTERRUPTION();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	if (true == pitIsrFlag){
	azul();
		if (true == pitIsrFlag){
			rojo();
	}
	}
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	//PIT_INTERRUPTION();
	//rojo();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	//PIT_INTERRUPTION();
}

void secuencia_three(void){
	azul();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
	blanco();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
	rojo();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
}

void secuencia_four(void){
	verde();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
	morado();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
	amarillo();
	//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
	PIT_INTERRUPTION();
}

