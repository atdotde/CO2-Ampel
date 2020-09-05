#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

void setup()
{
  char s[] = "Klasse 3a";
  Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT,0,WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(180);
  Paint_DrawString_EN(0, 0, "Robert", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(0, 23, "Helling", &Font24, BLUE, CYAN);
  Paint_DrawString_EN(0,45, "Klasse 3a",  &Font24, WHITE, RED);
  //Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
  //Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
  //Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);

  Paint_DrawImage(gImage_40X40,120, 0,40, 40);
  while(1) {
    for (s[8] = 'a'; s[8] <= 'z'; s[8]++) {
      delay(200);
      Paint_DrawString_EN(0,45, s,  &Font24, RED, GREEN);
      delay(200);
      Paint_DrawString_EN(0,45, s,  &Font24, GREEN, BLUE);
      delay(200);
      Paint_DrawString_EN(0,45, s,  &Font24, BLUE, RED);
    }
  }
}
void loop()
{

}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
