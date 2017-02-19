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
#define offset 0.80

/*
 * Number of pixels on the current String
 */
#define numpixels 12

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


/*
 * These arrays store the color values that are to be randomly
 * assigned to the LEDs. This allows for them all to be indexeds individually,
 * and lit with a different color.
 * The size of the array is directly related to the number of pixels + 1
 */
int red[numpixels];
int green[numpixels];
int blue[numpixels];

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
    pixels.setPixelColor(led_to_light_1, 1, 1, 1);
    pixels.setBrightness(i);
    pixels.show();
    delayMicroseconds(delay_white);
  }

  for(int i = brightness_white; i > -1; i--) {
    pixels.setPixelColor(led_to_light_1, 255, 255, 255);
    pixels.setBrightness(i);
    pixels.show();
    delayMicroseconds(delay_white);
  }
  
}


/*
 * This function will pulse each LED a different color with a breathing effect.
 */
void pulse() {
  int q = 0;
  //for (; q < pulse_number; q++) {
  int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(0, 10);
    green[r] = 0;
    blue[r] = random(0, 10);
  }
     
    /*
     * When the toggle is set to only choose white LEDs, the values default to only pulsing white LEDs.
     * When the toggle is set to color, the pixels will choose the color defined in the color array to choose
     * one of the subset colors we have chosen for this input. Each color is randomly generated on each
     * run of the code.
     */
        for (int j=0; j<brightness_color; j++) {
          for(int i=0; i<numpixels;i++) {
            if (state == true) {
             pixels.setPixelColor(i, red[i], green[i], blue[i]);
             state = !state; 
            } else {
              pixels.setPixelColor(i, 0, 0, 0);
              state = !state;
            }
          }
        pixels.setBrightness(j);
        pixels.show();
        delayMicroseconds(delayval);
        }
        
        for (int j=brightness_color; j>-1; j--) {
          for(int i=0; i<numpixels;i++) {
            if (state == true) {
             pixels.setPixelColor(i, red[i], green[i], blue[i]); 
             state = !state;
            } else {
              pixels.setPixelColor(i, 0, 0, 0);
              state = !state;
            }
          }
        pixels.setBrightness(j);
        pixels.show();
        delayMicroseconds(delayval);
        }
       state = !state;
       //}
}

void changeColor() {
  int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(10, 255);
    green[r] = 0;
    blue[r] = random(10, 255);
  }
  switch(color) {
    case 0:
      color = 1;
      for(int i =0; i < numpixels; i++) {
        pixels.setPixelColor(i, red[i], green[i], blue[i]);
        pixels.show();
      }
      break;
    case 1:
      color = 2;
      for(int i =0; i < numpixels; i++) {
        pixels.setPixelColor(i, red[i], green[i], blue[i]);
        pixels.show();
      }
      break;
    case 2:
      color = 0;
      for(int i =0; i < numpixels; i++) {
        pixels.setPixelColor(i, red[i], green[i], blue[i]);
        pixels.show();
      }
      break;
  }

  pixels.show();
}

void modBright(int average) {
  uint8_t brightness = average * 10;
  int difference = brightness - storeBrightness;
  Serial.print("Difference: "); Serial.print(difference); Serial.println("");
  if (difference > 15 || difference < -15) {
    if (difference < 0) {
      Serial.println("Negative!");
      for (int j=storeBrightness; j > brightness; j--) {
        Serial.print("J: "); Serial.print(j); Serial.println("");
        if(j > 0) {
          pixels.setBrightness(j);
        }
        delayMicroseconds(750);
        pixels.show();
      
      }
    } else {
      Serial.println("Positive!");
      for (int j=storeBrightness; j<=brightness; j++) {
        Serial.print("J: "); Serial.print(j); Serial.println("");
        if(j > 0) {
          pixels.setBrightness(j);
        }
        delayMicroseconds(750);
        pixels.show();
      
      }
    }
  }
  
  storeBrightness = brightness;
  Serial.print("Brightness: "); Serial.print(storeBrightness); Serial.println("");

}

void shimmer() {
  int shimoff = random(-20, 20);
  delay(20);
  for(int i =0; i < numpixels; i++) {
     pixels.setPixelColor(i, red[i] + shimoff, green[i], blue[i] + shimoff);
     pixels.show();
  }
}

void fadeWithBrightness() {
  for (int j=brightness_color; j>-1; j--) {
    for(int i=0; i<numpixels;i++) {
      if (state == true) {
        pixels.setPixelColor(i, red[i], green[i], blue[i]); 
        state = !state;
      } else {
         pixels.setPixelColor(i, 0, 0, 0);
         state = !state;
      }
    }
    pixels.setBrightness(j);
    pixels.show();
    delayMicroseconds(delayval);
  }
  
}

void fadeWithColor() {
  for (int j=brightness_color; j>-1; j--) {
    for(int i=0; i<numpixels;i++) {
      if (state == true) {
        pixels.setPixelColor(i, red[i], green[i], blue[i]); 
        state = !state;
      } else {
        pixels.setPixelColor(i, 0, 0, 0);
        state = !state;
      }
    }
    pixels.setBrightness(j);
    pixels.show();
    delayMicroseconds(delayval);
  }
  
}


void setup() {
  int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(10, 255);
    green[r] = 0;
    blue[r] = random(10, 255);
  }

  /*
   * Initialize Serial, CircuitPlayground, accelerometer, and our pixel strip.
   */
  Serial.begin(9600); // Debugging serial, leave commented unless debugging accelerometer
  CircuitPlayground.begin(); //Make sure this is before the pixels.begin() call or the leds will not start properly.
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  pixels.begin();
  for(int i =0; i < numpixels; i++) {
    pixels.setPixelColor(i, red[i], green[i], blue[i]);
   
  }
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
  //delayval = colorDelayRatio * average;
  

  /*
   * This is a conditional that will check to see whether the dancer is moving or completely still before choosing what flashing style to use. Twinkle or pulse.
   * The offset variable is at the top of the file and can be tweaked as needed.
   * The higher the value, the more motion required in order to make the lights switch to pulse.
   */
  /*if( (((x - x_prev) <= offset) && ((y - y_prev) <=offset) && ((z - z_prev) <=offset))) {
    twinkle_twinkle();
  } else {
    pulse();    
  }*/
  if(average > 15) {
    Serial.println("Triggered");
    changeColor(); 
  } else {
    modBright(average);
    //shimmer();
  }

  //fadeWithBrightness();
  //fadewithColor();

  

  /*Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" Z: "); Serial.print(z);
  Serial.print("Average: "); Serial.print(average);
  Serial.print("Color: "); Serial.print(color);
  Serial.println("m/s^2");*/
  

  x_prev = x;
  y_prev = y;
  z_prev = z;


  delay(20);
}
