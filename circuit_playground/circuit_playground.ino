#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>

#define greenScale 0
#define redScale 10
#define blueScale 12.75
#define brightness 200
#define offset 0.50

uint8_t pixeln = 0;

int red = 0;
int green = 0;
int blue = 0;

float x_prev = 0;
float y_prev = 0;
float z_prev = 0;


void setup() {
  // put your setup code here, to run once:
 // while (!Serial);
  
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  

}

void loop() {
  CircuitPlayground.clearPixels();
  float x = CircuitPlayground.motionX();
  float y = CircuitPlayground.motionY();
  float z = CircuitPlayground.motionZ();
  red = redScale*z;
  blue = blueScale*y;
  green = greenScale*x;
 

  

  delay(100);
  CircuitPlayground.setBrightness(0);

  if( (((x - x_prev) <= offset) && ((y - y_prev) <=offset) && ((z - z_prev) <=offset))) {
    /*Twinkle Twinkle little star...*/
    int led_to_light_1 = random(0, 10);
    //Serial.println(led_to_light);
    for(int i =0; i < brightness; i++) {
      CircuitPlayground.setBrightness(i);
      CircuitPlayground.setPixelColor(led_to_light_1, red, green, blue);
      delayMicroseconds(150);
    }

    for(int i = brightness; i > -1; i--) {
      CircuitPlayground.setBrightness(i);
      CircuitPlayground.setPixelColor(led_to_light_1, red, green, blue);
      delayMicroseconds(150);
    }
    
  } else {
    for (int j=0; j<brightness; j++) {
      CircuitPlayground.setBrightness(j);
     for(int i=0; i<10;i++) {
      CircuitPlayground.setPixelColor(i, red, green, blue);  
      }
      delayMicroseconds(150);
    }

    for (int j=brightness; j>0; j--) {
      CircuitPlayground.setBrightness(j);
     for(int i=0; i<10;i++) {
      CircuitPlayground.setPixelColor(i, red, green, blue);  
      }
      delayMicroseconds(150);
    }
  }
  
  

 /*  CircuitPlayground.setPixelColor(pixeln++, CircuitPlayground.colorWheel(25 * pixeln));
  if (pixeln == 11) {
    pixeln = 0;
    
  }*/

 /* Serial.print("X Axis: "); Serial.print(x);
  Serial.print(" Y Axis: "); Serial.print(y);
  Serial.print(" Z Axis: "); Serial.print(z);
  Serial.println("m/s^2");
  Serial.print("Red: "); Serial.print(red);
  Serial.print("Blue: "); Serial.print(blue);
  Serial.print("Green: "); Serial.print(green);
  Serial.println("");*/
  Serial.print("X Diff: "); Serial.print(x - x_prev);
  Serial.print(" Y Diff: "); Serial.print(y - y_prev);
  Serial.print(" Z Diff: "); Serial.print(z - z_prev);
  Serial.println("m/s^2");
  x_prev = x;
  y_prev = y;
  z_prev = z;
  
  
  delay(5);
}
