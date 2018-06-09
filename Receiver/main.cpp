/*
 *
 * Author Be-P, Gabrio Tognozzi
 * Pervasive systems 2018
 *
 * Credits:
 * Copyright (c) 2017 Helmut Tschemernjak
 * 30826 Garbsen (Hannover) Germany
 *
 * Licensed under the Apache License, Version 2.0);
 */
 #include "main.h"

DigitalOut myled(LED1);
BufferedSerial *ser;

int main() {
    SystemClock_Config();
    ser = new BufferedSerial(USBTX, USBRX);
    ser->baud(115200*2);
    ser->format(8);
    ser->printf("Hello World\n\r");
    myled = 1;
    
    Receiver();
}




void SystemClock_Config(void)
{
#ifdef B_L072Z_LRWAN1_LORA
    /* 
     * The L072Z_LRWAN1_LORA clock setup is somewhat differnt from the Nucleo board.
     * It has no LSE.
     */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable HSE Oscillator and Activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLLMUL_6;
    RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLLDIV_3;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        // Error_Handler();
    }

    /* Set Voltage scale1 as MCU will run at 32MHz */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
    while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        // Error_Handler();
    }
#endif
}

void dump(const char *title, const void *data, int len, bool dwords)
{
    dprintf("dump(\"%s\", 0x%x, %d bytes)", title, data, len);

    int i, j, cnt;
    unsigned char *u;
    const int width = 16;
    const int seppos = 7;

    cnt = 0;
    u = (unsigned char *)data;
    while (len > 0) {
        ser->printf("%08x: ", (unsigned int)data + cnt);
        if (dwords) {
            unsigned int *ip = ( unsigned int *)u;
            ser->printf(" 0x%08x\r\n", *ip);
            u+= 4;
            len -= 4;
            cnt += 4;
            continue;
        }
        cnt += width;
        j = len < width ? len : width;
        for (i = 0; i < j; i++) {
            ser->printf("%2.2x ", *(u + i));
            if (i == seppos)
                ser->putc(' ');
        }
        ser->putc(' ');
        if (j < width) {
            i = width - j;
            if (i > seppos + 1)
                ser->putc(' ');
            while (i--) {
                printf("%s", "   ");
            }
        }
        for (i = 0; i < j; i++) {
            int c = *(u + i);
            if (c >= ' ' && c <= '~')
                ser->putc(c);
            else
                ser->putc('.');
            if (i == seppos)
                ser->putc(' ');
        }
        len -= width;
        u += width;
        ser->printf("\r\n");
    }
    ser->printf("--\r\n");
}
