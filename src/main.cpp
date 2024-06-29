#include <Arduino.h>
#include <SoftwareSerial.h>
//#include "UTFT.h"
#include "LCDWIKI_GUI.h" //Core graphics library
#include "LCDWIKI_KBV.h" //Hardware-specific library
#include <TinyGPSPlus.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint32_t delayMS;
// The serial connection to the GPS module
SoftwareSerial gpsSerial(51, 53); //RX = 51, TX = 53
extern uint8_t GroteskBold32x64[];
LCDWIKI_KBV mylcd(ILI9486, A3 , A2, A1, A0, A4); //model,cs,cd,wr,rd,reset
int screenWidth, screenHeight;
TinyGPSPlus gps;
//UTFT myGLCD(ILI9486, 38, 39, 40, 41);
//UTFT myGLCD(ILI9486, A2, A1, A3, A4);
//RS=38, WR=39, CS=40, RST=41
//RS=A2, WR=A1, CS=A3, RST=A4

const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

void setup()
{
  Serial.begin(115200);
  gpsSerial.begin(115200);

  mylcd.Init_LCD();
  Serial.println(mylcd.Read_ID(), HEX);
  mylcd.Fill_Screen(BLACK);
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(2);
  mylcd.Set_Rotation(1);

    while (*gpsStream)
    if (gps.encode(*gpsStream++))
    {
      Serial.println(gps.location.lat(), 6);
      Serial.println(gps.location.lng(), 6);
    }
}

void loop()
{
  screenWidth = mylcd.Get_Display_Width();
  screenHeight = mylcd.Get_Display_Height();
  mylcd.Set_Text_Size(2.5);

  
  while (gpsSerial.available() > 0)
  {
    gps.encode(gpsSerial.read());
    Serial.println(gps.location.lat(), 6);
    Serial.println(gps.location.lng(), 6);

    mylcd.Print_String("LAT: " + String(gps.location.lat()), 0, 80);
    mylcd.Print_String("LON: " + String(gps.location.lng()), 0, 160);
    delay(50);
  }
}