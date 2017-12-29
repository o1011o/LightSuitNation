// /============== Tomas' NeoPixel




#include <Adafruit_NeoPixel.h>

#define PIN 10
#define NUM_LEDS 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

// === Main variables ===

//float strip_main[24][3]; //this is an array to hold rgb values for each pixel in the strip, created from the average of all other active strips.  This virtual strip is the one displayed.
unsigned long time_main = millis(); //this time measure is used as a previous time for the main refresher
int interval_main = 50;  //this interval determines the refresh rate of the actual, physical LEDs as they receive data from strip_main[] // control
unsigned long time_tick;
int interval_tick = 1000;
int tick = 0;
// ======================

// ========= Themes ===================================
// Themes are collections of RGB values that can be chosen from randomly by color functions.
// They are stored as 1-dimensional arrays, the first value of which (index 0) is the number of colors in the array, the following 3 values are the first RGB set, the 3 after them the second, etc.
// The length value stored in index 0 is for the number of RGB sets, not for the number of elements in the array.
int theme_primary[10] = {3,           //There are 3 rgb colors in this theme.  The length in brackets is the number of color sets + 1.  C++ may actually not need this value for arrays initiated like this.
                         255,  0,  0,  //The first color set as r, g, b,.  This is red.
                         0,  255,  0,  //The second color set, green.
                         0,    0,  255   //The third color set, blue.
                        };             //The end of the theme
int theme_warm[10] =    {3,
                         255,  0,  0,
                         255, 125,  0,
                         255,  0, 125
                        };
int theme_cool[10] =    {3,
                         0,    0, 255,
                         0,  125, 255,
                         125,  0, 255
                        };
int theme_black[4] =    {1,
                         0,    0,   0,
                        };
int theme_white[4] =    {1,
                         255, 255, 255
                        };
int theme_green[10] =   {3,
                         0, 255, 0,
                         85, 255, 0,
                         0, 255, 85
                        };
int theme_unicorn[10] = {3,
                         125, 255, 255,
                         255, 125, 255,
                         255, 255, 125
                        };
int theme_megalegs[7] =    {2,
                            32, 112, 104,
                            128, 0, 128
                           };
int theme_colleen[10] = {3,
                         0, 0, 255,
                         128, 0, 128,
                         0, 255, 0
                        };
// ======================================================

// === Color/Intensity Function Parameter Defaults ===============================
// | These are the default settings for all these color and intensity functions. |
// | Set them to whatever is most often useful to you here and then change them  |
// | for specific cases in the scheduler.                                        |
// ===============================================================================

// === sineWaves parameters ===
bool showSineWaves = false; //CONTROL: turns on or off display of this strip.
bool showSineWavesIntensity = false;  //CONTROL: turning this on makes this function like an intensity strip, affecting the brightness of colors underneath it.
float speed_sineWaves = 10; //CONTROL: 10 gives a fast-ish pulse.  Negative values move backwards.
float period_sineWaves = 24;
float stripBrightness_sineWaves = 1.0; //CONTROL: regulates the strips overall brightness/contribution to the final output.
int interval_sineWaves = 50; //CONTROL:  update interval.
//int rgbMin_sineWaves = 0; //CONTROL: minimum allowed rgb values for this strip. Might be superfluous.
//int rgbMax_sineWaves = 255; //CONTROL: maximum allowed rgb values for this strip. Might be superfluous.
float f_sineWaves = 0.0;
float strip_sineWaves[24][3];  //this one holds an individual functions strip state, acting as a virtual strip.
unsigned long time_sineWaves = millis();
int * theme_sineWaves;  //Themes are set in the setup() function below.
float currentColor_sineWaves[3] = {255, 0, 255};
// =============================

// === sineWaves2 parameters ===
bool showSineWaves2 = false; //CONTROL: turns on or off display of this strip.
bool showSineWaves2Intensity = false;  //CONTROL: turning this on makes this function like an intensity strip, affecting the brightness of colors underneath it.
float speed_sineWaves2 = -10; //CONTROL: speed of sine motion across strip, + values go one way, - values go the other.  10 is medium fast.  Higher values are faster.
float period_sineWaves2 = 24;
float f_sineWaves2 = 0.0;
float strip_sineWaves2[24][3];
float stripBrightness_sineWaves2 = 1.0; //CONTROL: regulates the strips overall brightness/contribution to the final output.
unsigned long time_sineWaves2 = millis();
int interval_sineWaves2 = 50; // control
int * theme_sineWaves2;  //Themes are set in the setup() function below.
float currentColor_sineWaves2[3] = {255, 0, 255};
//int rgbMin_sineWaves2 = 0; //CONTROL: minimum allowed rgb values for this strip. Might be superfluous.
//int rgbMax_sineWaves2 = 255; //CONTROL: maximum allowed rgb values for this strip. Might be superfluous.
// =============================

// === randomBlinkAndFade parameters ===
bool showRandomBlinkAndFade = false; //CONTROL: turns on or off the regular color display of this strip.
bool showRandomBlinkAndFadeIntensity = false;  //CONTROL: turning this on makes this function like an intensity strip, affecting the brightness of colors underneath it.
float strip_randomBlinkAndFade[24][3];
float stripBrightness_randomBlinkAndFade = 1.0; //CONTROL: regulates the strips overall brightness/contribution to the final output.
float decayRate_randomBlinkAndFade = 0.9;  //CONTROL: speed that pixels fade to black. Values between 0 and 1. Lower numbers are faster.  Could expand this to individual rgb for off-color fading
unsigned long time_randomBlinkAndFade = millis();
float interval_randomBlinkAndFade = 35; //CONTROL: speed.  values much higher than 50 have visible steps.  Lower is faster.
int * theme_randomBlinkAndFade;  //Themes are set in the setup() function below.
// =====================================

// === rainbowCycle parameters ===
//(this function modified from an adafruit example, credit to the author
bool showRainbowCycle = false;  //CONTROL: turns on or off display of this strip.
bool showRainbowCycleIntensity = false;
float strip_rainbowCycle[24][3];
float stripBrightness_rainbowCycle = 1.0;  //CONTROL: regulates the strips overall brightness/contribution to the final output.
unsigned long time_rainbowCycle = millis();
float interval_rainbowCycle = 3;  //CONTROL: regulates speed of color change.  Lower values are faster.
int i_rainbowCycle = 0;
int moveSpeed_rainbowCycle = 10;  //CONTROL: regulates movement of color along strip/width of color.  0 = no movement.  Adjust this and interval to control overall speed.
// ===============================

// === noise parameters ===
bool showNoise = false;  //CONTROL: turns on or off display of this strip.
bool showNoiseIntensity = false;
float strip_noise[24][3];
float stripBrightness_noise = 1.0;  //CONTROL: regulates the strips overall brightness/contribution to the final output.
unsigned long time_noise = millis();
float interval_noise = 10;  //CONTROL: regulates speed.  Lower values are faster.
int * theme_noiseON;  //Theme for pixels which are randomly 'on'.  Themes are set in the setup() function below.
int * theme_noiseOFF; //Theme for pixels which are randomly 'off'.  Themes are set in the setup() function below.
// ========================

//// === patternMover parameters ===
//bool showPatternMover = false;  //CONTROL: turns on or off display of this strip.
//float strip_patternMover[24][3];
//float stripBrightness_patternMover = 1.0;  //CONTROL: regulates the strips overall brightness/contribution to the final output.
//unsigned long time_patternMover = millis();
//float interval_patternMover = 250;  //CONTROL: regulates the update speed.
//float pattern_patternMover[24][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}}; //This is the actual pattern, one pixel after another.
//int step_patternMover = 1;  //CONTROL: controls how far the pattern moves per iteration.  Negative values go backwards.
//int i_patternMover = 0;
//// ================================

// === fire parameters ===
// (modified from another source, credit to the author)
bool showFire = false;
bool showFireIntensity = false;
float strip_fire[24][3];
float stripBrightness_fire = 1.0;
unsigned long time_fire = millis();
float interval_fire = 25;  //CONTROL: modifies speed of fire effect.  Lower values are faster.
// =======================

void setup() {
  strip.begin();
  strip.setBrightness(128);
  strip.show(); // Initialize all pixels to 'off'

  // === SET THEMES HERE ======================================
  theme_sineWaves = theme_primary;
  theme_sineWaves2 = theme_primary;
  theme_randomBlinkAndFade = theme_primary;
  theme_noiseON = theme_primary;
  theme_noiseOFF = theme_primary;
  // ===========================================================

}

void setPixel(uint8_t p, int r, int g, int b) {  //setPixel just gets rgb values in range and then pushes that value out to the physical pixel.
  if (p >= NUM_LEDS) return;
  if (r < 0) r = 0;
  if (g < 0) g = 0;
  if (b < 0) b = 0;
  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
  strip.setPixelColor(p, strip.Color(r, g, b));
  strip.show();
}

void refresh() {  //refresh() adds all active color strips together, then adds all active intensity strips together, then multiplies and outputs the value to physical LEDs.
  for (uint8_t p = 0; p < NUM_LEDS; ++p) {
    int r = 0;
    int g = 0;
    int b = 0;
    if (showSineWaves == true) {  //if sineWaves strip is turned on, add its values to the total.
      r = r + strip_sineWaves[p][0] * stripBrightness_sineWaves;
      g = g + strip_sineWaves[p][1] * stripBrightness_sineWaves;
      b = b + strip_sineWaves[p][2] * stripBrightness_sineWaves;
    }
    if (showSineWaves2 == true) {
      r = r + strip_sineWaves2[p][0] * stripBrightness_sineWaves2;
      g = g + strip_sineWaves2[p][1] * stripBrightness_sineWaves2;
      b = b + strip_sineWaves2[p][2] * stripBrightness_sineWaves2;
    }
    if (showRandomBlinkAndFade == true) {
      r = r + strip_randomBlinkAndFade[p][0] * stripBrightness_randomBlinkAndFade;
      g = g + strip_randomBlinkAndFade[p][1] * stripBrightness_randomBlinkAndFade;
      b = b + strip_randomBlinkAndFade[p][2] * stripBrightness_randomBlinkAndFade;
    }
    if (showRainbowCycle == true) {
      r = r + strip_rainbowCycle[p][0] * stripBrightness_rainbowCycle;
      g = g + strip_rainbowCycle[p][1] * stripBrightness_rainbowCycle;
      b = b + strip_rainbowCycle[p][2] * stripBrightness_rainbowCycle;
    }
    if (showNoise == true) {
      r = r + strip_noise[p][0] * stripBrightness_noise;
      g = g + strip_noise[p][1] * stripBrightness_noise;
      b = b + strip_noise[p][2] * stripBrightness_noise;
    }
    //    if (showPatternMover == true) {
    //      r = r + strip_patternMover[p][0] * stripBrightness_patternMover;
    //      g = g + strip_patternMover[p][1] * stripBrightness_patternMover;
    //      b = b + strip_patternMover[p][2] * stripBrightness_patternMover;
    //    }
    if (showFire == true) {
      r = r + strip_fire[p][0] * stripBrightness_fire;
      g = g + strip_fire[p][1] * stripBrightness_fire;
      b = b + strip_fire[p][2] * stripBrightness_fire;
    }
    float ri = 0.0;
    float gi = 0.0;
    float bi = 0.0;
    if (showSineWavesIntensity == true) {  //I'm borrowing every bit of the regular function here, only adjusting it to an intensity strip in the following lines.
      ri = ri + strip_sineWaves[p][0] / 255 * stripBrightness_sineWaves;
      gi = gi + strip_sineWaves[p][1] / 255 * stripBrightness_sineWaves;
      bi = bi + strip_sineWaves[p][2] / 255 * stripBrightness_sineWaves;
    }
    if (showSineWaves2Intensity == true) {
      ri = ri + strip_sineWaves2[p][0] / 255 * stripBrightness_sineWaves2;
      gi = gi + strip_sineWaves2[p][1] / 255 * stripBrightness_sineWaves2;
      bi = bi + strip_sineWaves2[p][2] / 255 * stripBrightness_sineWaves2;
    }
    if (showRainbowCycleIntensity == true) {
      ri = ri + strip_rainbowCycle[p][0] / 255 * stripBrightness_rainbowCycle;
      gi = gi + strip_rainbowCycle[p][1] / 255 * stripBrightness_rainbowCycle;
      bi = bi + strip_rainbowCycle[p][2] / 255 * stripBrightness_rainbowCycle;
    }
    if (showNoiseIntensity == true) {
      ri = ri + strip_noise[p][0] / 255 * stripBrightness_noise;
      gi = gi + strip_noise[p][1] / 255 * stripBrightness_noise;
      bi = bi + strip_noise[p][2] / 255 * stripBrightness_noise;
    }
    if (showFireIntensity == true) {
      ri = ri + strip_fire[p][0] / 255 * stripBrightness_fire;
      gi = gi + strip_fire[p][1] / 255 * stripBrightness_fire;
      bi = bi + strip_fire[p][2] / 255 * stripBrightness_fire;
    }
    if (showRandomBlinkAndFadeIntensity == true) {
      ri = ri + strip_randomBlinkAndFade[p][0] / 255 * stripBrightness_randomBlinkAndFade;
      gi = gi + strip_randomBlinkAndFade[p][1] / 255 * stripBrightness_randomBlinkAndFade;
      bi = bi + strip_randomBlinkAndFade[p][2] / 255 * stripBrightness_randomBlinkAndFade;
    }
    if (showSineWavesIntensity == false   // If there are no active intensity strips, set intensity values to 1 (default full bright).
        && showSineWaves2Intensity == false
        && showNoiseIntensity == false
        && showFireIntensity == false
        && showRainbowCycleIntensity == false
        && showRandomBlinkAndFadeIntensity == false) { // && showRainbowCycleIntensity == false) {
      ri = 1;
      gi = 1;
      bi = 1;
    }
    r = float(r) * ri;  //modify color by intensity
    g = float(g) * gi;
    b = float(b) * bi;
    setPixel(p, r, g, b);  //display on physical LEDs
  }
}

void sineWaves() {  //sineWaves - simple sine waves traveling through the strip, random color rotation after each pulse. color switches when enough pixels are dark - this isn't graceful but it does avoid eventual imprecision from truncated values for pi.
  if (strip_sineWaves[0][0] < 1.0 &&
      strip_sineWaves[0][1] < 1.0 &&
      strip_sineWaves[0][2] < 1.0 &&
      strip_sineWaves[11][0] < 1.0 &&
      strip_sineWaves[11][1] < 1.0 &&
      strip_sineWaves[11][2] < 1.0 &&
      strip_sineWaves[23][0] < 1.0 &&
      strip_sineWaves[23][1] < 1.0 &&
      strip_sineWaves[23][2] < 1.0) {
    int numColors = theme_sineWaves[0];
    int ran = random(0, numColors) * 3 + 1;
    float r = theme_sineWaves[ran];
    float g = theme_sineWaves[ran + 1];
    float b = theme_sineWaves[ran + 2];
    currentColor_sineWaves[0] = r;
    currentColor_sineWaves[1] = g;
    currentColor_sineWaves[2] = b;
  }
  for (int p = 0; p < NUM_LEDS; ++p) {
    float fp = p;
    float pixelBrightness = sin(fp / period_sineWaves * 3.14159 * 1 + f_sineWaves); //this is the pixel's brightness (-1 to 1), based on the sine of it's percentage along the strip times pi
    float nr = currentColor_sineWaves[0] * pixelBrightness;
    float ng = currentColor_sineWaves[1] * pixelBrightness;
    float nb = currentColor_sineWaves[2] * pixelBrightness;
    //    if (nr < rgbMin_sineWaves)  //This shit is probably irrelevant and certainly not a useful way to clog up limited flora memory.  May come back to it if a use comes up.
    //      nr = rgbMin_sineWaves;
    //    if (ng < rgbMin_sineWaves)
    //      ng = rgbMin_sineWaves;
    //    if (nb < rgbMin_sineWaves)
    //      nb = rgbMin_sineWaves;
    //    if (nr > rgbMax_sineWaves)
    //      nr = rgbMax_sineWaves;
    //    if (ng > rgbMax_sineWaves)
    //      ng = rgbMax_sineWaves;
    //    if (nb > rgbMax_sineWaves)
    //      nb = rgbMax_sineWaves;
    strip_sineWaves[p][0] = nr;
    strip_sineWaves[p][1] = ng;
    strip_sineWaves[p][2] = nb;
  }
  f_sineWaves = f_sineWaves + speed_sineWaves * 0.01;
}

void sineWaves2() {  // duplicate of sineWaves, so we can have more sineWaves in our sineWaves.
  if (strip_sineWaves2[0][0] < 1.0 &&
      strip_sineWaves2[0][1] < 1.0 &&
      strip_sineWaves2[0][2] < 1.0 &&
      strip_sineWaves2[11][0] < 1.0 &&
      strip_sineWaves2[11][1] < 1.0 &&
      strip_sineWaves2[11][2] < 1.0 &&
      strip_sineWaves2[23][0] < 1.0 &&
      strip_sineWaves2[23][1] < 1.0 &&
      strip_sineWaves2[23][2] < 1.0) {
    int numColors = theme_sineWaves2[0];
    int ran = random(0, numColors) * 3 + 1;
    int r = theme_sineWaves2[ran];
    int g = theme_sineWaves2[ran + 1];
    int b = theme_sineWaves2[ran + 2];
    currentColor_sineWaves2[0] = r;
    currentColor_sineWaves2[1] = g;
    currentColor_sineWaves2[2] = b;
  }
  for (int p = 0; p < NUM_LEDS; ++p) {
    float fp = p;
    float pixelBrightness = sin(fp / period_sineWaves2 * 3.14159 * 1 + f_sineWaves2); //this is the pixel's brightness (-1 to 1), based on the sine of it's percentage along the strip times pi
    float nr = currentColor_sineWaves2[0] * pixelBrightness;
    float ng = currentColor_sineWaves2[1] * pixelBrightness;
    float nb = currentColor_sineWaves2[2] * pixelBrightness;
    //    if (nr < rgbMin_sineWaves2)
    //      nr = rgbMin_sineWaves2;
    //    if (ng < rgbMin_sineWaves2)
    //      ng = rgbMin_sineWaves2;
    //    if (nb < rgbMin_sineWaves2)
    //      nb = rgbMin_sineWaves2;
    //    if (nr > rgbMax_sineWaves2)
    //      nr = rgbMax_sineWaves2;
    //    if (ng > rgbMax_sineWaves2)
    //      ng = rgbMax_sineWaves2;
    //    if (nb > rgbMax_sineWaves2)
    //      nb = rgbMax_sineWaves2;
    strip_sineWaves2[p][0] = nr;
    strip_sineWaves2[p][1] = ng;
    strip_sineWaves2[p][2] = nb;
  }
  f_sineWaves2 = f_sineWaves2 + speed_sineWaves2 * 0.01;
}

void randomBlinkAndFade() {  //  randomBlinkAndFade - random colors randomly placed fading to black
  int p = random(0, 24);
  int numColors = theme_randomBlinkAndFade[0];
  int ran = random(0, numColors) * 3 + 1;
  strip_randomBlinkAndFade[p][0] = theme_randomBlinkAndFade[ran];
  strip_randomBlinkAndFade[p][1] = theme_randomBlinkAndFade[ran + 1];
  strip_randomBlinkAndFade[p][2] = theme_randomBlinkAndFade[ran + 2];
  for (int p = 0; p < NUM_LEDS; p++) {  //  reduce each pixel's brightness by a bit
    strip_randomBlinkAndFade[p][0] = strip_randomBlinkAndFade[p][0] * decayRate_randomBlinkAndFade;  //MOD these values to change rate of decay
    strip_randomBlinkAndFade[p][1] = strip_randomBlinkAndFade[p][1] * decayRate_randomBlinkAndFade;
    strip_randomBlinkAndFade[p][2] = strip_randomBlinkAndFade[p][2] * decayRate_randomBlinkAndFade;
  }
}

// ========= rainbowCycle ===========
uint8_t wheelRed(uint8_t wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return (255 - wheelPos * 3);
  }
  else if (wheelPos < 170)
  {
    wheelPos -= 85;
    return 0;
  }
  else {
    wheelPos -= 170;
    return (wheelPos * 3);
  }
}

uint8_t wheelGreen(uint8_t wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return (0);
  }
  else if (wheelPos < 170)
  {
    wheelPos -= 85;
    return (wheelPos * 3);
  }
  else
  {
    wheelPos -= 170;
    return (255 - wheelPos * 3);
  }
}

uint8_t wheelBlue(uint8_t wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return (wheelPos * 3);
  }
  else if (wheelPos < 170)
  {
    wheelPos -= 85;
    return (255 - wheelPos * 3);
  }
  else
  {
    wheelPos -= 170;
    return (0);
  }
}

void rainbowCycle()
{
  for (uint8_t p = 0; p < NUM_LEDS; ++p)
  {
    //setPixel(p,255,255,0);
    //    strip.setPixelColor(p, Wheel(((p * 256 / strip.numPixels()) + i_rainbowCycle) & 255));
    //setPixel(p, wheelRed(i_rainbowCycle), wheelGreen(i_rainbowCycle), wheelBlue(i_rainbowCycle));
    strip_rainbowCycle[p][0] = wheelRed(p * moveSpeed_rainbowCycle + i_rainbowCycle);
    strip_rainbowCycle[p][1] = wheelGreen(p * moveSpeed_rainbowCycle + i_rainbowCycle);
    strip_rainbowCycle[p][2] = wheelBlue(p * moveSpeed_rainbowCycle + i_rainbowCycle);
  }
  i_rainbowCycle = i_rainbowCycle + 1;
  if (i_rainbowCycle > 255) i_rainbowCycle = 0;
}
// ============== END rainbowCycle ===============

void noise() {  // noise - randomly puts one of two themes on each pixel in the strip.
  for (int p = 0; p < NUM_LEDS; p++) {
    if (random(0, 2) == 1) {
      int numColors = theme_noiseON[0];
      int ran = random(0, numColors) * 3 + 1;
      strip_noise[p][0] = theme_noiseON[ran];
      strip_noise[p][1] = theme_noiseON[ran + 1];
      strip_noise[p][2] = theme_noiseON[ran + 2];
    } else {
      int numColors = theme_noiseOFF[0];
      int ran = random(0, numColors) * 3 + 1;
      strip_noise[p][0] = theme_noiseOFF[ran];
      strip_noise[p][1] = theme_noiseOFF[ran + 1];
      strip_noise[p][2] = theme_noiseOFF[ran + 2];
    }
  }
}

//void patternMover() {  // patternMover - moves a simple pattern defined in an array along the strip.
//TODO values of step over 1 causes something weird to happen
//  for (int p = 0; p < NUM_LEDS; p++) {
//    uint8_t pp = p + i_patternMover;
//    if (pp > 23) pp = pp - 24;
//    strip_patternMover[p][0] = pattern_patternMover[pp][0];
//    strip_patternMover[p][1] = pattern_patternMover[pp][1];
//    strip_patternMover[p][2] = pattern_patternMover[pp][2];
//  }
//  i_patternMover = i_patternMover + step_patternMover;
//  if (i_patternMover > NUM_LEDS) i_patternMover = 0;
//}

// ======= fire =================================
void fire(int Cooling, int Sparking) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little

  for ( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little

  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom

  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors

  for ( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  //showStrip();
  //delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {

  // Scale 'heat' down from 0-255 to 0-191

  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from

  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:

  if ( t192 > 0x80) {                    // hottest
    //setPixel(Pixel, 255, 255, heatramp);
    strip_fire[Pixel][0] = 255;
    strip_fire[Pixel][1] = 255;
    strip_fire[Pixel][2] = heatramp;
  } else if ( t192 > 0x40 ) {            // middle
    //setPixel(Pixel, 255, heatramp, 0);
    strip_fire[Pixel][0] = 255;
    strip_fire[Pixel][1] = heatramp;
    strip_fire[Pixel][2] = 0;
  } else {                               // coolest
    //setPixel(Pixel, heatramp, 0, 0);
    strip_fire[Pixel][0] = heatramp;
    strip_fire[Pixel][1] = 0;
    strip_fire[Pixel][2] = 0;
  }
}
// ======= end fire =====================

void stripsOFF() {  //Utility function to turn off any strips that have been turned on.  Use it in scheduler.
  showSineWaves = false;
  showSineWavesIntensity = false;
  showSineWaves2 = false;
  showSineWaves2Intensity = false;
  showRandomBlinkAndFade = false;
  showRandomBlinkAndFadeIntensity = false;
  showNoise = false;
  showNoiseIntensity = false;
  showRainbowCycle = false;
  showRainbowCycleIntensity = false;
  showFire = false;
  showFireIntensity = false;
}

// /================ SCHEDULER - schedule your parameter changes here yo ==================\
// | Scheduler is where you can schedule parameter changes for all your strips.  Turn them |
// | on or off here or change any of their editable variables.  Tick is a number of        |
// | seconds since last timer reset.  Once you're at the end of your scheduled loop, set   |
// | tick back to 0 to repeat.                                                             |
// \=======================================================================================/
void scheduler() {
  if (tick > 200) {
    tick = 0;
    return;
  }
  if (tick > 180) {  // warm sines vs. cool sines, shorter period on the warm ones and slower speed on the cool ones.
    stripsOFF();
    showSineWaves = true;
    showSineWaves2 = true;
    theme_sineWaves = theme_warm;
    theme_sineWaves2 = theme_cool;
    period_sineWaves = 6;
    speed_sineWaves = 30;
    period_sineWaves2 = 24;
    speed_sineWaves2 = -10;
    return;
  }
  if (tick > 160) {  // blink and fade, colleen's colors, visible through a short period sine wave
    showSineWavesIntensity = true;
    theme_sineWaves = theme_white;
    period_sineWaves = 6;
    speed_sineWaves = -20;
    showRandomBlinkAndFade = true;
    theme_randomBlinkAndFade = theme_colleen;
    return;
  }
  if (tick > 140) {  // warm sines going one way, cool sines going the other.  Semi-fast.  (intense, like it a lot, called 'warm worm cool worm fucking')
    stripsOFF();
    showSineWaves = true;
    showSineWaves2 = true;
    theme_sineWaves = theme_warm;
    theme_sineWaves2 = theme_cool;
    speed_sineWaves = -30;
    speed_sineWaves2 = 30;
    period_sineWaves = 12;
    period_sineWaves2 = 12;
    return;
  }
  if (tick > 120) {  //frantically blinking turquoise and purple noise visible through slow crawling short period sine waves (like it, called sparkle-unicorn)
    stripsOFF();
    showSineWavesIntensity = true;
    showNoise = true;
    theme_noiseON = theme_megalegs;
    theme_noiseOFF = theme_black;
    theme_sineWaves = theme_white;
    period_sineWaves = 6;
    speed_sineWaves = -10;
    return;
  }
  if (tick > 100) {  // fastish full length sine visibility layer over primary random blink and fade (fun, vivid colors)
    stripsOFF();
    showSineWavesIntensity = true;
    showRandomBlinkAndFade = true;
    theme_randomBlinkAndFade = theme_primary;
    theme_sineWaves = theme_white;
    speed_sineWaves = -30;
    return;
  }
  if (tick > 80) { //fire under a rainbow visibility layer.  Fucking sick.  10/10.  (love it, like a fire you're throwing chemicals in)
    stripsOFF();
    showRainbowCycleIntensity = true;
    showFire = true;
    return;
  }
  if (tick > 60) {  //fast long sine pulse showing unicorn blink and fade underneath.  kinda meh.  (fun, like it, but like it least)
    stripsOFF();
    showSineWavesIntensity = true;
    theme_sineWaves = theme_white;
    showRandomBlinkAndFade = true;
    theme_randomBlinkAndFade = theme_unicorn;
    stripBrightness_randomBlinkAndFade = 1.0;
    stripBrightness_sineWaves = 1.0;
    speed_sineWaves = -50;
    period_sineWaves = 48;
    return;
  }
  if (tick > 40) {  // rainbow visible through randomBlinkAndFade  (nice, kinda peaceful)maybe speed it up for rock and roll.
    stripsOFF();
    showRainbowCycle = true;
    showRandomBlinkAndFadeIntensity = true;
    theme_randomBlinkAndFade = theme_white;
    return;
  }
  if (tick > 40) {  //slightly noisy rainbow visible through short sine waves (like it)
    stripsOFF();
    showRainbowCycle = true;
    showNoise = true;
    stripBrightness_noise = 0.25;
    showSineWavesIntensity = true;
    theme_sineWaves = theme_white;
    period_sineWaves = 6;
    speed_sineWaves = -30;
    return;
  }
  if (tick > 0) {  //rainbowCycle (like it a lot)
    stripsOFF();
    // showRainbowCycle = true;
    showSineWavesIntensity = true;
    showRainbowCycle = true;
    theme_sineWaves = theme_white;
    period_sineWaves = 48;
    speed_sineWaves = 50;
    return;
  }
}
// =======================  END SCHEDULER  ===================================

void loop() {  // in loop we check all these references to millis() since program start to keep everything going when it ought.
  int now = millis();
  scheduler();
  if (now - time_tick > interval_tick) {
    time_tick = now;
    tick = tick + 1;
  }
  if (now - time_main > interval_main) {
    time_main = now;
    refresh();
  }
  if (now - time_sineWaves > interval_sineWaves) {
    time_sineWaves = now;
    if (showSineWaves == true || showSineWavesIntensity == true) {
      sineWaves();
    }
  }
  if (now - time_sineWaves2 > interval_sineWaves2) {
    time_sineWaves2 = now;
    if (showSineWaves2 == true || showSineWaves2Intensity == true) {
      sineWaves2();
    }
  }
  if (now - time_randomBlinkAndFade > interval_randomBlinkAndFade) {
    time_randomBlinkAndFade = now;
    if (showRandomBlinkAndFade == true || showRandomBlinkAndFadeIntensity == true) {
      randomBlinkAndFade();
    }
  }
  if (now - time_rainbowCycle > interval_rainbowCycle) {
    time_rainbowCycle = now;
    if (showRainbowCycle == true || showRainbowCycleIntensity == true) {
      rainbowCycle();
    }
  }
  if (now - time_noise > interval_noise) {
    time_noise = now;
    if (showNoise == true || showNoiseIntensity == true) {
      noise();
    }
  }
  //  if (now - time_patternMover > interval_patternMover) {
  //    time_patternMover = now;
  //    if (showPatternMover == true) {
  //      patternMover();
  //    }
  //  }
  if (now - time_fire > interval_fire) {
    time_fire = now;
    if (showFire == true || showFireIntensity == true) {
      fire(55, 120);
    }
  }
}

