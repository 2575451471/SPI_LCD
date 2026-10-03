#include "debug.h"
#include "epd_port.h"
#include "epd.h"
#include "image.h"


int main(void)
{
    SystemCoreClockUpdate();

    Delay_Init();

    USART_Printf_Init(115200);


    /*
     * Hardware
     */
    EPD_GPIO_Init();

    EPD_SPI_Init();


    /*
     * SSD1608
     */
    EPD_Init();


    /*
     * œ‘ æ200x200Õº∆¨
     */
    EPD_Display(
        Image_Test
    );


    while(1)
    {
        Delay_Ms(1000);
    }
}