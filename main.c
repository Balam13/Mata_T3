#include <stdio.h>
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_pit.h"


#include "rgb_leds.h"

uint8_t g_ButtonPress = 0;



gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
	};


#define PIN06        6u  //switch2 PortC
#define PIN04        4u  //switch3 PortA

void PORTA_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOA, 1u<<PIN04);
	blanco();
	SDK_ISR_EXIT_BARRIER;
}

void PORTC_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOC, 1u<<PIN06);
	verde();
	SDK_ISR_EXIT_BARRIER;
}

int main(void) {

	const port_pin_config_t porta_pin_config ={
			kPORT_PullUp,
			kPORT_FastSlewRate,
			kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable,
			kPORT_HighDriveStrength,
			kPORT_MuxAsGpio,
			kPORT_UnlockRegister

	};

	uint8_t inicio = 0;

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortE);

	PORT_SetPinConfig(PORTA, PIN04, &porta_pin_config);
	PORT_SetPinConfig(PORTC, PIN06, &porta_pin_config);

	PORT_SetPinInterruptConfig(PORTA, PIN04, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, PIN06, kPORT_InterruptFallingEdge);

	PORT_SetPinMux(PORTB, PIN22, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, PIN21, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, PIN26, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, PIN06, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTA, PIN04, kPORT_MuxAsGpio);

	GPIO_PinInit(GPIOB, PIN22, &led_config);
	GPIO_PinInit(GPIOB, PIN21, &led_config);
	GPIO_PinInit(GPIOE, PIN26, &led_config);
	GPIO_PinInit(GPIOC, PIN06, &sw_config);
	GPIO_PinInit(GPIOA, PIN04, &sw_config);

	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn, 3);

	NVIC_EnableIRQ(PORTC_IRQn);
	NVIC_SetPriority(PORTC_IRQn, 3);




	 while(1) {

		limpiar();


		switch(inicio){
		case 0:  //Secuencia uno
			secuencia_one();

				//presionar switch 2
				if (!GPIO_PinRead(GPIOC, PIN06))
				{
					inicio = 1;   //Secuencia dos();
					//break;
				}
				else
					inicio =0;

				break;


		case 1:  //Secuencia dos
				secuencia_two();
				//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);

				//presionar switch 3
				if (!GPIO_PinRead(GPIOA, PIN04))
				{ inicio = 2; //secuencia tres
				//break;
				}

				else inicio = 1;
				break;


		case 2:  //Secuencia tres
			secuencia_three();
				//SDK_DelayAtLeastUs(DELAY, CORE_FREQ);

				if(!GPIO_PinRead(GPIOA, PIN04)) //presionar switch 3
				{
					//GPIO_PortSet(GPIOB, 1u << PIN21);
					inicio = 3;   //secuencia cuatro
					//break;
				}

				else inicio=2;
				break;

		case 3:   //secuencia cuatro
				secuencia_four();

				if (!GPIO_PinRead(GPIOC, PIN06)) //presionar switch 2
					{
					inicio = 0;  //Secuencia uno;
						//break;
					}
				else inicio = 3;
				break;
			}

    }
    return 0 ;
}

