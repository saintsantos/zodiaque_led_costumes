#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>

/*
 * Still playing with. Will update with better comment
 */

#define brightness_white 255 
#define delay_white 150

/*
 * Still playing with. Will update with better comment
 */
#define brightness_color 255
#define delay_color 150

/*
 * Number of times the leds pulse
 */
#define pulse_number 1

/*
 * Stores how much movement is required to switch motion.
 */
#define offset 1.0

/*
 * Number of pixels on the current String
 */
#define numpixels 2

/*
 * Data pin the pixels are connected to.
 */
#define pixelpin 6

/*
 * These variables store the previous values form the accelerometers, 
 * which alows us to detect whether the dancer is moving or not.
 */
float x_prev = 0;
float y_prev = 0;
float z_prev = 0;
int avg_offset = 0;


/*
 * These arrays store the color values that are to be randomly
 * assigned to the LEDs. This allows for them all to be indexeds individually,
 * and lit with a different color.
 * The size of the array is directly related to the number of pixels + 1
 */
int red[numpixels];
int green[numpixels];
int blue[numpixels];

/**
 * This array is the modified array. All of our changes to brightness go here
 */
int modred[numpixels];
int modgreen[numpixels];
int modblue[numpixels];

/**
 * This array will store whether an LED is on or not.
 */
int ledstate[numpixels];

/*
 * 1 = pulse white
 * 2 = pulse color
 */
int pulseStyle = 1;

int colorDelayRatio = 1000;

int delayval;

bool state;

int color = 0;

uint8_t storeBrightness = 1;

/*
 * This is for defining the neopixels. DO NOT TOUCH UNLESS YOU KNOW WHAT YOU'RE DOING!
 */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, pixelpin, NEO_GRB + NEO_KHZ800);

/*
 * This is the function to make a randomly selected led on a string to
 * blink. This creates a twinkling effect. 
 */
void twinkle_twinkle( ){
  int q = 0;
  int led_to_light_1 = random(0, (numpixels +1));
  
  for(int i =0; i < brightness_white; i++) {
    pixels.setPixelColor(led_to_light_1, i, i, i);
    pixels.show();
    delayMicroseconds(delay_white);
  }

  for(int i = brightness_white; i > -1; i--) {
    pixels.setPixelColor(led_to_light_1, i, i, i);
    pixels.show();
    delayMicroseconds(delay_white);
  }
  
}

bool checkOff() {
  for (int i=0; i < numpixels; i++) {
      /*Serial.print("modred: "); Serial.print(modred[i]); Serial.println("");
      Serial.print("modblue: "); Serial.print(modblue[i]); Serial.println("");
      Serial.print("red: "); Serial.print(red[i]); Serial.println("");
      Serial.print("blue: "); Serial.print(blue[i]); Serial.println("");
      delay(1000);*/
    if (modred[i] != 0 || modblue[i] != 0) {
      Serial.println("False!");
      return false; 
    }
  }

  return true;
  
}

bool checkOn() {
  for (int i=0; i < numpixels; i++) {
    if(modred[i] != red[i] || modblue[i] != blue[i]) {
      return false;
    }
  }

  return true;
  
}

/*
 * This initializes the LEDs to their initial color. Used for debugging
 */
void turnOn() {
  for(int i = 0; i < numpixels; i++) {
    pixels.setPixelColor(i, red[i], green[i], blue[i]);
  }
  pixels.show();
}

void fadeOut() {
  int j = 0;
  bool off = checkOff();
  while(!off) {
    Serial.print("off: "); Serial.print(off);Serial.println("");
    for(int i = 0; i < numpixels; i++) {
      if(modred[i] > 0) {
        modred[i]--;
      } 

      if (modblue[i] > 0) {
        modblue[i]--;
      }
      /*Serial.print("modred: "); Serial.print(modred[i]); Serial.println("");
      Serial.print("modblue: "); Serial.print(modblue[i]); Serial.println("");
      Serial.print("red: "); Serial.print(red[i]); Serial.println("");
      Serial.print("blue: "); Serial.print(blue[i]); Serial.println("");
      delay(2000);*/
      pixels.setPixelColor(i, modred[i], modgreen[i], modblue[i]);
    }
    pixels.show();
    delay(75);
    j++;
    off = checkOff();
  }
  /*Serial.println("Leaving!");
  delay(5000);*/
}

void fadeIn() {
  int j = 0;
  bool on = checkOn();
  while(!on) {
    //Serial.print("on: "); Serial.print(on);Serial.println("");
    for(int i = 0; i < numpixels; i++) {
      if(modred[i] < red[i]) {
        modred[i]++;
      } 

      if (modblue[i] < blue[i]) {
        modblue[i]++;
      }
      /*Serial.print("modred: "); Serial.print(modred[i]); Serial.println("");
      Serial.print("modblue: "); Serial.print(modblue[i]); Serial.println("");
      Serial.print("red: "); Serial.print(red[i]); Serial.println("");
      Serial.print("blue: "); Serial.print(blue[i]); Serial.println("");
      delay(1000);*/
      pixels.setPixelColor(i, modred[i], modgreen[i], modblue[i]);
    }
    pixels.show();
    delay(75);
    j++;
    on = checkOn();
  }
}

void changeColor() {
  int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(10, 255);
    green[r] = 0;
    blue[r] = random(10, 255);
  }
  r = 0;
  for(; r < numpixels; r++) {
    modred[r] = red[r];
    modgreen[r] = 0;
    modblue[r] = blue[r];
  }

  for(int i =0; i < numpixels; i++) {
    pixels.setPixelColor(i, red[i], green[i], blue[i]);
    pixels.show();
  }
  pixels.show();
}

void modBright(int average) {
  uint8_t brightness = average * 5;
  int difference = brightness - storeBrightness;
  Serial.print("Difference: "); Serial.print(difference); Serial.println("");
  if (difference > 15 || difference < -15) {
    if (difference < 0) {
      Serial.println("Negative!");
      for (int j=storeBrightness; j > brightness; j--) {
        Serial.print("J: "); Serial.print(j); Serial.println("");
        for(int i = 0; i < numpixels; i++) {
          if (modred[i] != 0 && modblue[i] != 0) {
            Serial.println("Not Dim Enough");
            modred[i] = modred[i] - 1;
            modblue[i] = modblue - 1;
          } else if(modred[i] == 0) {
            modred[i] = modred[i] + 1;
          } else {
            modblue[i] = modblue + 1;
          }

          pixels.setPixelColor(i, modred[i], modgreen[i], modblue[i]);
        }
        delay(5);
        pixels.show();
      
      }
    } else {
      Serial.println("Positive!");
      for (int j=storeBrightness; j<=brightness; j++) {
        Serial.print("J: "); Serial.print(j); Serial.println("");
        for(int i = 0; i < numpixels; i++) {
          if (modred[i] != 255 && modblue[i] != 255) {
            Serial.println("Not bright enough");
            modred[i] = modred[i] + 1;
            modblue[i] = modblue[i] + 1;
          } else if (modred[i] == 255){
            modred[i] = modred[i] - 1;
            
          } else {
            modblue[i] = modblue - 1;
          }
          pixels.setPixelColor(i, modred[i], modgreen[i], modblue[i]);
        }
        delay(5);
        pixels.show();
      
      }
    }
  }
  
  storeBrightness = brightness;
  Serial.print("Brightness: "); Serial.print(storeBrightness); Serial.println("");

}





void setup() {
  int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(1, 10);
    green[r] = 0;
    blue[r] = random(1, 10);
    //blue[r] = 0;
  }

  r = 0;
  for(; r < numpixels; r++) {
    int q = random(0, 1);
    ledstate[r] = q;
  }

  r = 0;
  for(; r < numpixels; r++) {
    modred[r] = 0;
    modgreen[r] = 0;
    modblue[r] = 0;
  }

  /*
   * Initialize Serial, CircuitPlayground, accelerometer, and our pixel strip.
   */
  Serial.begin(9600); // Debugging serial, leave commented unless debugging accelerometer
  CircuitPlayground.begin(); //Make sure this is before the pixels.begin() call or the leds will not start properly.
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  pixels.begin();
  pixels.show();

}

void loop() {
  //CircuitPlayground.clearPixels();
  float x = CircuitPlayground.motionX();
  float y = CircuitPlayground.motionY();
  float z = CircuitPlayground.motionZ();
  if(x < 0) {
    x = x * (-1);
  }

  if(y < 0) {
    y = y * (-1);
  }

  if (z < 0) {
    z = z * (-1);
  }

  int average = (x + y + z) / 3;
  

  /*
   * This is a conditional that will check to see whether the dancer is moving or completely still before choosing what flashing style to use. Twinkle or pulse.
   * The offset variable is at the top of the file and can be tweaked as needed.
   * The higher the value, the more motion required in order to make the lights switch to pulse.
   */
  if( (((x - x_prev) <= offset) && ((y - y_prev) <=offset) && ((z - z_prev) <=offset))) {
    bool on = checkOn();
    if(on) {
      fadeOut(); 
    }
    delay(100);
    twinkle_twinkle();
  } else {
    turnOn();
    if(average > 9) {
      Serial.println("Changing Color");
      //changeColor();
    }
    bool on = checkOn();
    if (!on) {
      fadeIn();
    }
    turnOn();
    //delay(1000);
    //modBright(average);
  }

  

  /*Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" Z: "); Serial.print(z);
  Serial.print("Average: "); Serial.print(average);
  Serial.print("Color: "); Serial.print(color);
  Serial.println("m/s^2");*/
  

  x_prev = x;
  y_prev = y;
  z_prev = z;
  //avg_offset = average;


  delay(5);
}

