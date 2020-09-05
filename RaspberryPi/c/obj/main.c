#include "DEV_Config.h"
#include "LCD_0in96.h"
#include "GUI_Paint.h"
#include "image.h"
#include <stdlib.h>		//exit()
#include "fonts.h"

int main(int argc, char **argv)
{
	//Module Init
	if(DEV_ModuleInit() != 0){
        DEV_ModuleExit();
        exit(0); 
    } 
	// LCD  Init
    printf("0.96inch LCD Module code\r\n");
	LCD_Init();
	LCD_Clear(WHITE);
    UWORD *BlackImage;
    UDOUBLE Imagesize = LCD_HEIGHT*LCD_WIDTH*2;
    printf("Imagesize = %d\r\n", Imagesize);
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    
	Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
    Paint_Clear(WHITE);
	// Paint_SetRotate(180);
    printf("drawing...\r\n");
	Paint_DrawString_EN(0, 0, "123", &Font24, 0x000f, 0xfff0);
	Paint_DrawString_EN(0, 23, "ABC", &Font24, BLUE, CYAN);
	Paint_DrawString_CN(20,42, "Î¢Ñ©µç×Ó",  &Font24CN, BLUE, WHITE);
	Paint_DrawRectangle(70, 10, 100, 40, RED, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
	Paint_DrawLine(70, 10, 100, 40, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(100, 10, 70, 40, MAGENTA, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	
	Paint_DrawImage(gImage_40X40,120, 0,40, 40); 
    LCD_Display(BlackImage);
    DEV_Delay_ms(2000);
	Paint_DrawImage(gImage_160X80,0, 0, 160, 80); 
    LCD_Display(BlackImage);
  /*  Module Exit  */
	DEV_ModuleExit();
	return 0;	
}
