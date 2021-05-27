/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DFPLAYER_MINI.h"
#include "tcs_color_sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t debugStatus=0;
uint8_t state=1;
uint16_t r, g, b, c, colorTemp, lux;

int IR_LEFT   = 0;
int IR_CENTER = 0;
int IR_RIGHT  = 0;
uint8_t right_msg[10]   = "RIGHT";
uint8_t left_msg[10]    = "LEFT";
uint8_t forward_msg[10] = "CENTER";
uint8_t endline         = '\n';
uint8_t cr              = '\r';
int red_flag            = 0;
int green_flag          = 0;
int blue_flag           = 0;
uint8_t Speed           = 0x20;
uint8_t zero            = 0;
uint8_t ForwardBoth[]   = {0xC2,0xCA};
uint8_t Left[]          = {0xC1,0xCA};
uint8_t Right[]         = {0xC2,0xC9};
char SPI_data[3]        = {0};
int corners_counter     = 0;
int trun_delay          = 1480;
	
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void play_audio()
{
	  HAL_Delay(1000);
		getRawData_noDelay(&r, &g, &b, &c);
		uint32_t sum = c;

		// Avoid divide by zero errors ... if clear = 0 return black
		if (c == 0) {
			r = g = b = 0;
		}
		else
		{
			r = (float)r / sum * 255.0f;
			g = (float)g / sum * 255.0f;
			b = (float)b / sum * 255.0f;
		}
		if(r>100 && g<70 && b<100) //red
		{
				DF_Play(1);
		}
		else if(r<100 && g>100 && b<100) //green
		{
				DF_Play(2);
		}
		else if(r<100 && g<100 && b>100) //blue
		{
				DF_Play(3);
		}
		else
		{
				DF_Play(4);
		}
}
void detect_color()
{
		getRawData_noDelay(&r, &g, &b, &c);
		uint32_t sum = c;
		if (c == 0) 
			r = g = b = 0;
		else
		{
			r = (float)r / sum * 255.0f;
			g = (float)g / sum * 255.0f;
			b = (float)b / sum * 255.0f;
		}
		if(r>100 && g<70 && b<100) //red
				red_flag = 1; 
		else if(r<100 && g>100 && b<100) //green
				green_flag = 1;
		else if(r<100 && g<100 && b>100) //blue
				blue_flag = 1;
}
void follow_red()
{
	 detect_color();
	 while (!red_flag)
	 {
			IR_LEFT   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
			IR_CENTER = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
			IR_RIGHT  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
			
			if (IR_LEFT == 1 && IR_RIGHT == 0) //turn right conditions
			{
				HAL_UART_Transmit(&huart2 , &Right[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Right[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else if (IR_LEFT == 0 && IR_RIGHT == 1) //turn left conditions
			{
				HAL_UART_Transmit(&huart2 , &Left[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &Left[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay	
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else //forward
			{
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			}
			
			//re-read the color
			detect_color();
		}
		
		//reset the flag
		red_flag = 0;
		//stop when while loop is finished --> when red is reached
		HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		
		//play audio
		play_audio();
		HAL_Delay(5000);
}
void follow_green()
{
	 detect_color();
	 while (!green_flag)
	 {
			IR_LEFT   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
			IR_CENTER = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
			IR_RIGHT  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
			
			if (IR_LEFT == 1 && IR_RIGHT == 0) //turn right conditions
			{
				HAL_UART_Transmit(&huart2 , &Right[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Right[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else if (IR_LEFT == 0 && IR_RIGHT == 1) //turn left conditions
			{
				HAL_UART_Transmit(&huart2 , &Left[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &Left[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay	
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else //forward
			{
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			}
			
			//re-read the color
			detect_color();
		}
		
		//reset the flag
		green_flag = 0;
		//stop when while loop is finished --> when red is reached
		HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		
		//play audio
		play_audio();
		HAL_Delay(5000);
}
void follow_blue()
{
	 detect_color();
	 while (!blue_flag)
	 {
			IR_LEFT   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
			IR_CENTER = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
			IR_RIGHT  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
			
			if (IR_LEFT == 1 && IR_RIGHT == 0) //turn right conditions
			{
				HAL_UART_Transmit(&huart2 , &Right[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Right[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else if (IR_LEFT == 0 && IR_RIGHT == 1) //turn left conditions
			{
				HAL_UART_Transmit(&huart2 , &Left[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &Left[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(trun_delay); //90-degrees turn delay	
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
				HAL_Delay(500);
			}
			else //forward
			{
				HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
				HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			}
			
			//re-read the color
			detect_color();
		}
		
		//reset the flag
		blue_flag = 0;
		//stop when while loop is finished --> when red is reached
		HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		
		//play audio
		play_audio();
		HAL_Delay(5000);
}
void general_tour()
{
	while (corners_counter < 4)
	{
		IR_LEFT   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
		IR_CENTER = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
		IR_RIGHT  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
		
		if (IR_LEFT == 1 && IR_RIGHT == 0) //turn right conditions
		{
			HAL_UART_Transmit(&huart2 , &Right[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Right[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			
			HAL_Delay(trun_delay); //90-degrees turn delay
			
			//play record 
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
			play_audio();
			corners_counter++; 
			HAL_Delay(5000);
			
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			HAL_Delay(500);
		}
		if (IR_LEFT == 0 && IR_RIGHT == 1) //turn left conditions
		{
			HAL_UART_Transmit(&huart2 , &Left[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &Left[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			
			HAL_Delay(trun_delay); //90-degrees turn delay
			
			//play record 
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
			play_audio();
			corners_counter++;
			HAL_Delay(5000);
			
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
			HAL_Delay(500);			
		}
		else //forward
		{
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &Speed ,1 ,HAL_MAX_DELAY);
		}
	}
	
	//reset the counter
	corners_counter = 0;
	//stop when the general tour is finished 
	HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
	DF_Init(40);
	if (tcs_init()) {
      debugStatus=0x55; //Found sensor
  } else {
      debugStatus=0xAA;    //No TCS34725 found ... check your connections
    while (1);
  }
  // Set persistence filter to generate an interrupt for every RGB Cycle, regardless of the integration limits
  write8(0x0C, 0);
  setInterrupt(1);
	
	//DF_PlayFromStart();
	
	HAL_Delay(3000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		/////////////////////////////start SPI slave part///////////////////////////////////////
		while (HAL_SPI_Receive(&hspi3, (uint8_t*)SPI_data, sizeof(SPI_data), 100) != HAL_OK) ;
		
		//HAL_SPI_Receive(&hspi3, (uint8_t*)SPI_data, sizeof(SPI_data), 100);
		if (SPI_data[0] == 'R')
		{ 
			//follow the line until you find Green --> then stop
			follow_red(); 
		}
		else if (SPI_data[0] == 'G')
		{
			//follow the line until you find Green --> then stop
			follow_green();
		}
		else if (SPI_data[0] == 'B')
		{
			//follow the line until you find Blue --> then stop
			follow_blue();
		}
		else if (SPI_data[0] == 'T')
		{
			//give the general tour of all exhibits
			general_tour();
		}
		else 
		{
			//stop
			HAL_UART_Transmit(&huart2 , &ForwardBoth[0] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);		
			HAL_UART_Transmit(&huart2 , &ForwardBoth[1] ,1 ,HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2 , &zero ,1 ,HAL_MAX_DELAY);
		}
		/////////////////////////////end SPI slave part///////////////////////////////////////

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA4 PA7 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
