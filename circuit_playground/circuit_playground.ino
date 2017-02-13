#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>

/*
 * Still playing with. Will update with better comment
 */

#define brightness_white 200
#define delay_white 150

/*
 * Still playing with. Will update with better comment
 */
#define brightness_color 200
#define delay_color 150

/*
 * Number of times the leds pulse
 */
#define pulse_number 1

/*
 * Stores how much movement is required to switch motion.
 */
#define offset 0.50

/*
 * Number of pixels on the current String
 */
#define numpixels 2

/*
 * Data pin the pixels are connected to.
 */
#define pixelpin 6

uint8_t pixeln = 0;

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

int pulseStyle = 1; //1 for white pulsing, 2 for color pulsing

/*
 * This is for defining the neopixels
 */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, 6, NEO_GRB + NEO_KHZ800);

/*
 * This is the function to make a randomly selected led on a string to
 * blink. This creates a twinkling effect. 
 */
void twinkle_twinkle( ){
  int q = 0;
  int led_to_light_1 = random(0, (numpixels +1));
  
  for (int j=0; j < numpixels; j++) {
       pixels.setPixelColor(j, 0, 0, 0);
      }
  pixels.show();
  
  for(int i =0; i < 255; i++) {
    pixels.setPixelColor(led_to_light_1, i, i, i);
    delayMicroseconds(delay_white);
    pixels.show();
  }

  for(int i = 255; i > -1; i--) {
    pixels.setPixelColor(led_to_light_1, i, i, i);
    delayMicroseconds(delay_white);
    pixels.show();
  }
  
}


/*
 * This function will pulse each LED a different color with a breathing effect.
 */
void pulse() {
  int q = 0;
  for (; q <pulse_number; q++) {
    int r = 0;
  for(; r < numpixels; r++) {
    red[r] = random(0, 150);
    green[r] = 0;
    blue[r] = random(0, 150);
  }
     
    /*
     * When the toggle is set to only choose white LEDs, the values default to only pulsing white LEDs.
     * When the toggle is set to color, the pixels will choose the color defined in the color array to choose
     * one of the subset colors we have chosen for this input. Each color is randomly generated on each
     * run of the code.
     */
     switch(pulseStyle) {
      case 1:
        for ( int i=0; i < brightness_white; i++) {
          for (int j=0; j < numpixels; j++) {
            pixels.setPixelColor(j, i, i, i);
          }
          pixels.show();
          delay(5);
        }

        for ( int i=brightness_white; i > 0; i--) {
          for (int j=0; j < numpixels; j++) {
            pixels.setPixelColor(j, i, i, i);
          }
        pixels.show();
        delay(5);
        }
      case 2:
        for (int j=0; j<brightness_color; j++) {
          for(int i=0; i<numpixels;i++) {
            pixels.setPixelColor(i, red[i], green[i], blue[i]);
          }
        pixels.setBrightness(j);
        pixels.show();
        delay(5);
        }
        
        for (int j=brightness_color; j>0; j--) {
          for(int i=0; i<numpixels;i++) {
            pixels.setPixelColor(i, red[i], green[i], blue[i]);
          }
        pixels.setBrightness(j);
        pixels.show();
        delay(5);
        }
     }
    

  }
}


void setup() {

  /*
   * Initialize Serial, CircuitPlayground, accelerometer, and our pixel strip.
   */
 // Serial.begin(9600); // Debugging serial, leave commented unless debugging accelerometer
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
  

  /*
   * This is a conditional that will check to see whether the dancer is moving or completely still before choosing what flashing style to use. Twinkle or pulse.
   * The offset variable is at the top of the file and can be tweaked as needed.
   * The higher the value, the more motion required in order to make the lights switch to pulse.
   */
  if( (((x - x_prev) <= offset) && ((y - y_prev) <=offset) && ((z - z_prev) <=offset))) {
    twinkle_twinkle();
  } else {
    pulse();
  }
  

  x_prev = x;
  y_prev = y;
  z_prev = z;


  delay(5);
}
