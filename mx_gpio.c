/**
  ******************************************************************************
  * File Name          : mx_gpio.c
  * Date               : 09/03/2013 01:51:30
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT 2013 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_gpio.h"


/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/


void pinout_config(void) {
	/* Private typedef ---------------------------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;


	/** Configure pins as GPIO
		 PE6	 ------> GPIO_Output
		 PB1	 ------> GPIO_Analog
		 PE7	 ------> GPIO_Output
		 PE8	 ------> GPIO_Output
		 PE9	 ------> GPIO_Output
		 PE10	 ------> GPIO_Output
		 PE11	 ------> GPIO_Output
		 PE12	 ------> GPIO_Output
		 PE13	 ------> GPIO_Output
		 PE14	 ------> GPIO_Output
		 PE15	 ------> GPIO_Output
		 PD8	 ------> GPIO_Input
		 PD9	 ------> GPIO_Input
		 PD10	 ------> GPIO_Input
		 PD11	 ------> GPIO_Input
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}



/**
  * @brief  Returns the selected Button state.
  * @param  button: Specifies the button to be checked.
  * @retval The Button GPIO pin value.
  */
uint8_t PBGetState(uint16_t button)
{
  return GPIO_ReadInputDataBit(GPIOD, button);
}


/**
  * @brief  Turns selected LEDs On.
  * @param  Leds: Specifies the Leds to be set on.
  * @retval None
  */
void BG_Write(uint16_t Leds)
{
	GPIO_Write(GPIOE, Leds);
}

/**
  * @brief  Turns selected LEDs Off.
  * @param  Leds: Specifies the Leds to be set off.
  * @retval None
  */
/*
void BG_LEDsOff(uint16_t Leds)
{
	GPIO_Write(GPIOE, Leds);
}
*/
/**
  * @brief  Toggles the selected LEDs.
  * @param  Leds: Specifies the Leds to be toggled.
  * @retval None
  */
void BG_LEDsToggle(uint16_t Leds)
{
	GPIO_ToggleBits(GPIOE, Leds);
}


