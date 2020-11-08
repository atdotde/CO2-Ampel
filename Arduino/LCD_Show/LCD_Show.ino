#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <SoftwareSerial.h>

SoftwareSerial co2Serial(2, 3); // define MH-Z19 RX TX

void readSensor(int *ppm, int *temperature);
void setautocalibration();
void calibrate();

void plot_digit(char c, int xc, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background, int s)
{
  int x = xc * Font->Width * s;
  int y = (LCD_HEIGHT - Font->Height * s) / 2;
  int dx, dy, i, j;
  int color;
  uint32_t Char_Offset = (c - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
  const unsigned char *ptr = &Font->table[Char_Offset];
  const unsigned char *save;

  LCD_SetCursor(x, y, x + Font->Width * s - 1, y + Font->Height * s - 1);

  for ( dy = 0; dy < Font->Height; dy ++ ) {
    save = ptr;
    for (j = 0; j < s; j++) {
      ptr = save;
      for ( dx = 0; dx < Font->Width; dx ++ ) {
        if (pgm_read_byte(ptr) & (0x80 >> (dx % 8))) {
          color = Color_Foreground;
        } else {
          color = Color_Background;
        }
        for (i = 0; i < s; i++)
          LCD_WriteData_Word(color);
        if (dx % 8 == 7)
          ptr++;
      }
    }
    ptr++;
  }
}

void setup()
{
  Serial.begin(115200);

  co2Serial.begin(9600);

  char s[] = "Klasse 3a";
  Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(0);
  Paint_DrawString_EN(0, 0, "CO2 Konz.", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(0, 23, "R Helling", &Font24, BLUE, CYAN);


  //setautocalibration();

  char co2string[100];
  int x, y, color;

  LCD_SetCursor(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  for (x = 0; x < LCD_WIDTH; x++)
    for (y  = 0; y < LCD_HEIGHT; y++)
      LCD_WriteData_Word(RED);

  LCD_SetCursor(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  for (x = 0; x < LCD_WIDTH; x++)
    for (y  = 0; y < LCD_HEIGHT; y++)
      LCD_WriteData_Word(0x000f);

  while (1) {
    int ppm, temperature = 0;

    readSensor(&ppm, &temperature);
    //    Serial.print("PPM: ");
    Serial.println(ppm);
    //    Serial.print(" Temperature: ");
    //    Serial.println(temperature);

    sprintf(co2string, "%d PPM ", ppm);
    //Paint_DrawString_EN(0, 23, co2string, &Font24, BLUE, CYAN);


    color = heatcolor(0x3f * (ppm - 400) / 600);
    if (ppm < 750) {
      color = GREEN;
      //      Paint_DrawString_EN(0, 45, " OK ", &Font24, GREEN, BLUE);
    } else {
      if (ppm < 1000) {
        color = YELLOW;
        //      Paint_DrawString_EN(0, 45, "WARN", &Font24, YELLOW, BLUE);
      } else {
        //    Paint_DrawString_EN(0, 45, "HIGH", &Font24, RED, BLUE);
        color = RED;
      }
    }
    int rest = ppm;
    if (rest >= 1000)
      plot_digit('0' + rest / 1000, 0, &Font24, BLACK, color, 2);
    else
      plot_digit(' ', 0, &Font24, BLACK, color, 2);

    rest = rest % 1000;
    plot_digit('0' + rest / 100, 1, &Font24, BLACK, color, 2);
    rest = rest % 100;
    plot_digit('0' + rest / 10, 2, &Font24, BLACK, color, 2);
    rest = rest % 10;
    plot_digit('0' + rest, 3, &Font24, BLACK, color, 2);
    plot_digit(' ', 4, &Font24, BLACK, color, 2);

    delay(1000);
  }
}
void loop()
{

}

void setautocalibration() {
  byte cmd[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};

  co2Serial.write(cmd, 9);
}

void calibrate() {
  byte cmd[9] = {0xFF, 0x01, 0x87, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x78};

  co2Serial.write(cmd, 9);
}

// Die Funktion liest die CO2-Werte über die UART des
// Sensors ein und schreibt die ermittelten Werte mit
// Hilfe der übergebenen Pointer in die Variablen ppm
// und temperature.

void readSensor(int *ppm, int *temperature) {

  // Die Befehlskette zum Einlesen des PPM-Wertes laut Datenblatt

  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

  // Speicherplatzreservierung von 9 Byte für die Antwort des Sensors.
  // Alles Befehle und Antworten des Sensors haben eine Länge von
  // 9 Byte, wobei das letzte Byte eine Prüfsumme zur Kontrolle
  // der Übermittlung darstellt.

  byte response[9];

  // Befehl zum Auslesen schreiben

  co2Serial.write(cmd, 9);

  // Zuerst den Eingangsbuffer löschen (mit 0 füllen) und
  // danach in einer while-Schleife darauf warten, bis
  // die Funktion co2Serial.available() einen Wert ungleich 0
  // zurückgibt.

  memset(response, 0, 9);
  while (co2Serial.available() == 0) {
    delay(1000);
  }

  // Die Antwort wird in den Speicher eingelesen.

  co2Serial.readBytes(response, 9);

  // Die Prüfsumme mit Hilfe einer eigenen Funk-
  // tion errechnen, um zu klären, ob die
  // Übertragung fehlerfrei abgelaufen ist.

  byte check = getCheckSum(response);

  if (response[8] != check) {
    Serial.println("Fehler in der Übertragung!");
    return;
  }

  // PPM-Wert errechnen, sie finden sind
  // im 3. und 4. Byte der Antwort im Format
  // HIGH-Byte und LOW-Byte und müssen über die
  // folgende Formel zu einem Word (int) verkettet
  // werden.

  *ppm = 256 * (int)response[2] + response[3];

  // Temperaturwert wird als 5. Byte der Response
  // übermittelt (im Datenblatt nicht angegeben).
  // Damit auch negative Temperaturen übertragen
  // werden können, wurde der Wert 40 dazuaddiert,
  // der jetzt wieder entfernt werden muss.

  *temperature = response[4] - 40;

}

// Die Funktion errechnet eine Prüfsumme über die
// durch einen Zeiger übergebene Befehls- oder
// Antwortkette. Der Algorithmus zur
// Prüfsummenberechnung findet sich im
// Datenblatt.

byte getCheckSum(byte * packet) {
  byte i;
  byte checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += packet[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}

// 5 bit rot, 6 bit gruen, 5 bit blau
// max 31, 63, 31, daher fraction bis 63

UWORD heatcolor(int f) {
  UWORD c;

  if (f > 0x3f)
    f = 0x3f;

  c = ((RED * (0x1f - f) / 0x1f) & RED) | ((f * GREEN / 0x3d) & GREEN);
  // u  c = (0x1F - (f / 2)) << 11 + f << 5;
  //  Serial.println(f);
  //  Serial.println(c);
  return c;

}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
