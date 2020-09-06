#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <SoftwareSerial.h>

SoftwareSerial co2Serial(2, 3); // define MH-Z19 RX TX

void readSensor(int *ppm, int *temperature);

void setup()
{
  Serial.begin(9600);

  co2Serial.begin(9600);

  char s[] = "Klasse 3a";
  Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(180);
  Paint_DrawString_EN(0, 0, "CO2 Konz.", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(0, 23, "Helling", &Font24, BLUE, CYAN);
  //Paint_DrawString_EN(0,45, "Klasse 3a",  &Font24, WHITE, RED);
  //Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
  //Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
  //Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);

  //  Paint_DrawImage(gImage_40X40,120, 0,40, 40);
  char co2string[100];
  while (1) {
    int ppm, temperature = 0;

    readSensor(&ppm, &temperature);
    Serial.print("PPM: ");
    Serial.print(ppm);
    Serial.print(" Temperature: ");
    Serial.println(temperature);

    //Paint_Clear(WHITE);
    sprintf(co2string, "%d PPM", ppm);
    Paint_DrawString_EN(0, 23, co2string, &Font24, BLUE, CYAN);
    //Paint_DrawRectangle(0, 45, 10, 59, WHITE,DRAW_FILL_EMPTY, DOT_PIXEL_1X1 );
    //Paint_DrawString_EN(0,45, "XXXX",  &Font24, BLACK, BLACK);

    if (ppm < 750)
      Paint_DrawString_EN(0, 45, " OK ",  &Font24, GREEN, BLUE);
    else
      Paint_DrawString_EN(0, 45, "HIGH",  &Font24, RED, BLUE);

    delay(1000);
    //    for (s[8] = 'a'; s[8] <= 'd'; s[8]++) {
    //      delay(200);
    //      Paint_DrawString_EN(0,45, s,  &Font24, RED, GREEN);
    //      delay(200);
    //      Paint_DrawString_EN(0,45, s,  &Font24, GREEN, BLUE);
    //      delay(200);
    //      Paint_DrawString_EN(0,45, s,  &Font24, BLUE, RED);
    //    }
  }
}
void loop()
{

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

byte getCheckSum(byte *packet) {
  byte i;
  byte checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += packet[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
