#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "DEV_Config.h"
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "GUI_Paint.h"
#include "image.h"
#include "DEV_Config.h"
#include <unistd.h>

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Program  End\r\n");
    DEV_ModuleExit();

    exit(0);
}


int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
	if(DEV_ModuleInit()==1)
		return 1;
	LCD_SetBackLight(500);
    printf("Jetson Nano Test program\r\n");
    printf("Longer refresh time\r\n");
	LCD_Init();
    printf("Please wait for the screen\r\n");
	LCD_Clear(0x0000);
	Paint_NewImage(LCD_WIDTH, LCD_HEIGHT,0,WHITE);
	Paint_Clear(WHITE);
    printf("Please wait for the screen\r\n");
	//Paint_SetRotate(180);
	Paint_DrawString_EN(0, 0, "123", &Font24, 0x000f, 0xfff0);
	Paint_DrawString_EN(0, 23, "ABC", &Font24, BLUE, CYAN);
	Paint_DrawString_CN(20,42, "Î¢Ñ©µç×Ó",  &Font24CN, WHITE, RED);
	Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
	Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
	Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);

	Paint_DrawImage(gImage_40X40,120, 0,40, 40); 
	DEV_Delay_ms(2000);
	Paint_DrawImage(gImage_160X80,0, 0, 160, 80); 
	while (1){
        sleep(1);
	}
	DEV_ModuleExit();
}

