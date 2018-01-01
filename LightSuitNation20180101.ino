// /-------------- Light Suit Nation code for Adafruit Flora Neopixels -----------------\
// |------------------ TRIPLE TOP SECRET UNLESS YOU ARE DEAD SEXY ----------------------|
// | Here's some code shit that powers an Adafruit Flora and its Neopixels.  This code  |
// | operates on a paradigm of combining multiple virtual neopixel strips each driven   | 
// | by its own bit of programming.  This way we can layer effects very easily and try  |
// | to find weird emergent patterns.  Strip data can be interpreted as Color or as     |
// | Intensity.  If color, the RGB values for each pixel are added together with that   |
// | of every other color strip, and if intensity those values are converted to a       |
// | percentage before being added to every other intensity strip.  The sum of all the  |
// | color strips is then multiplied by the sum of all the intensity strips to get an   |
// | output that is sent to the physical LEDs.
// | 
// | This particular release is a super-slimmed down version I created to do some       |
// | troubleshooting, but I think it makes a good simple example/learning tool before   |
// | getting bogged down with tons of different LED pattern classes.  
// | illustrate this convention.  Themes are assigned at strip creation in Schedule.    |
// \------------------------------------------------------------------------------------/

#include <Adafruit_NeoPixel.h>

#define PIN 10
#define NUM_LEDS 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

// /-------------------------------------- Themes --------------------------------------\
// | Themes are color schemes from which colors are chosen randomly by many programs.   |
// | Each one is a simple 1d array, the first element of which is a number telling us   |
// | how many colors are in the theme, and the following elements being sets of 3 RGB   |
// | color values.  Note that the first number is number of colors, not number of RGB   |
// | values.  A theme of 3 colors will have 10 values total, the count and 9 RGB values |
// | making up 3 sets of 3.  The themes below are laid out on multiple lines to         |
// | illustrate this convention.  Themes are assigned at strip creation in Schedule.    |
// | (TODO get themes easily assignable, probably with pointers to these themes.)       |
// \------------------------------------------------------------------------------------/
int theme_primary[10] = {3,           //There are 3 rgb colors in this theme.  The length in brackets is the number of color sets + 1.  TODO check that C++ may actually not need this value for arrays initiated like this.
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
                        
// /---------------------------- Class VirtualStrip() ----------------------------------\
// | The parent class for all VirtualStrips, this contains variables that are shared    |
// | among all that follow.  In this class the array that holds color values is set up  |
// | along with timers and themes.  'isIntensity' is a value that tells us whether the  |
// | programming here should be treated as a color strip or as an intensity strip.  If  |
// | isIntensity == true the 0-255 RGB color values will be converted into 0.0 - 1.0    |
// | intensity values in the refresh() function.  TODO get themes as pointers so they   |
// | can be passed to the constructor and maybe a setter.                               |
// \------------------------------------------------------------------------------------/
class VirtualStrip {
  public:
    bool isIntensity = false;  //CONTROL:  This is either a color strip (isIntensity = false) or an intensity strip (isIntensity = true).
    float strip[NUM_LEDS][3];  //the virtual strip
    float brightness = 1.0;  //CONTROL:  strip brightness.  decimal values from 0-1.  When using many color functions simultaneously, turn the brightness down to avoid washing everything out.
    unsigned long timer = millis();  //used to measure time for update()
    int interval = 50;  //CONTROL:  update interval of this function in milliseconds.
    int * theme1;
    int * theme2;
    uint8_t theme1Index = 1;
    uint8_t theme2Index = 1;

    VirtualStrip() {
      theme1 = theme_primary;
      theme2 = theme_primary;
    }

    virtual void update(void) {}

    void setIsIntensity(bool x) {
      isIntensity = x;
    }
    void setStripBrightness(float x)  {
      brightness = x;  //setBrightness already defined?
    }
    void setInterval(int x) {
      interval = x;
    }
    void setTheme(int * x) {
      theme1 = x;  //TODO figure out how to assign an array to this
    }
    void setTheme2(int * x) {
      theme2 = x;
    }

    void setVPixel(int p, float r, float g, float b) {
      strip[p][0] = r;
      strip[p][1] = g;
      strip[p][2] = b;
    }

    void newRandomThemeIndices() {  //Picks an index from the theme at random
      int numColors = theme1[0];
      theme1Index = random(0, numColors) * 3 + 1;
      numColors = theme2[0];
      theme2Index = random(0, numColors) * 3 + 1;
    }
};

// /------------------------------Class SineWaves() ------------------------------------\
// | SineWaves is our first VirtualStrip LED program.  It inherits the functionality of |
// | its parent 'VirtualStrip' and adds the specific programming that dictates the      |
// | behaviour of LEDs in this VirtualStrip.  SineWaves creates a sine wave of custom   |
// | velocity, wavelength, and color theme.  The velocity variable determines speed and |
// | direction of travel (negative values traveling backwards), period determines the   |
// | wavelength, and the theme determines the color theme from which random colors will |
// | be chosen.  (color changes happen in an awkward way, will attempt to fix in future.|
// | TODO fix color changes and add themes to constructor by passing pointers.)         |
// \------------------------------------------------------------------------------------/
class SineWaves: public VirtualStrip {
  public:
    uint8_t period = NUM_LEDS;
    float f = 0;
    int velocity = 10;

    SineWaves() {}
    SineWaves(int vel, uint8_t per) {
      velocity = vel;
      period = per;
    }

    void update() {
      if (strip[0][0] < 1.0 && //We're checking if the strip is fully in a dark phase here, to see if a color change can happen without being noticed.
          strip[0][1] < 1.0 && //It might seem there should be a cleaner way to use math to find the dark part, but the truncation of pi and other
          strip[0][2] < 1.0 && //floating point calculations will lead to error after enough iterations.  This method works despite any of that.
          strip[11][0] < 1.0 &&
          strip[11][1] < 1.0 &&
          strip[11][2] < 1.0 &&
          strip[23][0] < 1.0 &&
          strip[23][1] < 1.0 &&
          strip[23][2] < 1.0) {
        newRandomThemeIndices();
      }
      for (uint8_t p = 0; p < NUM_LEDS; ++p) {
        float fp = p;
        float pixelBrightness = sin(fp / period * 3.14159 + f); //this is the pixel's brightness (-1 to 1), based on the sine of it's percentage along the strip times pi
        float nr = theme1[theme1Index] * pixelBrightness;
        float ng = theme1[theme1Index + 1] * pixelBrightness;
        float nb = theme1[theme1Index + 2] * pixelBrightness;
        setVPixel(p, nr, ng, nb);  //set the virtual pixel in the virtual strip equal to the value we've calculated.

      }
      f = f + velocity * 0.01;
    }
};

// --- Global variables. ---
VirtualStrip ** strips;  //This one is the pointer that keeps track of all current VirtualStrips.  
float strip_color[NUM_LEDS][3];  // the virtual strip that stores the collective (additive) color values of all the others
float strip_intensity[NUM_LEDS][3]; // the virtual strip that stores the collective (additive) intensity values of all the others
unsigned long time_main = millis(); //this time measure is used as a previous time for the main refresher
float interval_main = 50;  //this interval determines the refresh rate of the actual, physical LEDs as they receive data from strip_main[] // editable control
unsigned long time_tick = millis();
int interval_tick = 1000;
int tick = 0;
uint8_t numStrips = 1;  //used instead of constantly doing math to get size of strips.  Might change this.
uint8_t schedule = 0;  //this keeps track of where in the schedule we are
//------------------------------------------------------------------

// /---------------------------------- setup() -----------------------------------------\
// | In setup() we do some necessary preparation, the only thing of note as far as      |
// | values to modify being 'strip.setBrightness'.  This affects the overall final      |
// | brightness of the LEDs.  Set it to max (255) for eye-searing intensity or lower    |
// | for something more mellow.  In setup() we also create a VirtualStrip that will be  |
// | present on program start so we can avoid any errors.  The first schedule block     |
// | overrides this.                                                                    |
// \------------------------------------------------------------------------------------/
void setup() {
  strip.begin();
  strip.setBrightness(32);  //set overall brightness here.
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));  //initializes a new random seed so that random is different every boot.
  numStrips = 2;
  strips = new VirtualStrip * [2];
  VirtualStrip * s1 = new SineWaves(30, 24);  
  VirtualStrip * s2 = new SineWaves(10, 24);
  strips[0] = s1;
  strips[1] = s2;
}

// setPixel is just a utility function that ensures that rgb values are within a valid range
// and then sends those values out to the physical LEDs.
void setPixel(uint8_t p, int r, int g, int b) {
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

// /-------------------------------- refresh() -----------------------------------------\
// | refresh combines all virtual strip data into one output that goes to the physical  |
// | LEDs.  This output is an additive combination of every color strip multiplied by   |
// | an additive combination of every intensity strip.  If no intensity strips are      |
// | present default values of 1.0 are set for intensity.  Since color and intensity    |
// | strips are summed, if you're using a lot you can get extreme values that end up    |
// | washing out your final output.  Adjust brightness levels of individual strips when |
// | you add them in the scheduler to account for this.                                 |
// \------------------------------------------------------------------------------------/
void refresh() {
  for (uint8_t p = 0; p < NUM_LEDS; p++) { //for each pixel
    strip_color[p][0] = 0;
    strip_color[p][1] = 0;
    strip_color[p][2] = 0;
    strip_intensity[p][0] = 1.0;
    strip_intensity[p][1] = 1.0;
    strip_intensity[p][2] = 1.0;
    for (uint8_t i = 0; i < numStrips; i++) {  //for each virtual strip, color or intensity
      if (strips[i]->isIntensity == false) {  //if this strip is a color strip, add its values to the main virtual color strip
        strip_color[p][0] = strip_color[p][0] + strips[i]->strip[p][0] * strips[i]->brightness;
        strip_color[p][1] = strip_color[p][1] + strips[i]->strip[p][1] * strips[i]->brightness;
        strip_color[p][2] = strip_color[p][2] + strips[i]->strip[p][2] * strips[i]->brightness;
      }
      else {  //else it's an intensity strip, so add its values to the main virtual intensity strip
        strip_intensity[p][0] = strip_intensity[p][0] + strips[i]->strip[p][0] * strips[i]->brightness;
        strip_intensity[p][1] = strip_intensity[p][1] + strips[i]->strip[p][1] * strips[i]->brightness;
        strip_intensity[p][2] = strip_intensity[p][2] + strips[i]->strip[p][2] * strips[i]->brightness;
      }
    }
    float r = strip_color[p][0] * strip_intensity[p][0];
    float g = strip_color[p][1] * strip_intensity[p][1];
    float b = strip_color[p][2] * strip_intensity[p][2];
    setPixel(p, r, g, b); //assign the virtual strip pixel values to the actual physical LED strip
    //something is crashing before here
  }
}

// /------------------------------ scheduler() -----------------------------------------\
// | The scheduler lets you arrange program changes whenever you like, utilizing blocks |
// | of time in which one collection of strips will be active.  Look at the last block  |
// | in the function (which is the first block chronologically) for details of what is  |
// | happening.                                                                         |
// \------------------------------------------------------------------------------------/
void scheduler() {
  if (tick > 40) {  //This is the final schedule block, all we'll do is reset 'tick' and 'schedule' to 0 and it will loop.
    if (schedule >= 2) {
      tick = 0;
      schedule = 0;
    }
    return;
  }
  if (tick > 20) {  //This middle schedule block will run after 20 seconds.
    if (schedule == 1){
      for (uint8_t i = 0; i < numStrips; i++) {  //delete old VirtualStrips
        delete strips[i];
      }
      numStrips = 2;  //set to the number of new strips we're about to create
      strips = new VirtualStrip * [numStrips];  //set our global 'strips' equal to a new array to hold this shit
      VirtualStrip * s1 = new SineWaves(30, 24);  //create a new VirtualStrip
      VirtualStrip * s2 = new SineWaves(10, 24);  //and another
      strips[0] = s1;  //and add them to the array we made
      strips[1] = s2;  //this one too
      schedule = schedule + 1;  //iterate the schedule counter so this block doesn't execute again until after it's looped.
    }
    return;
  }
  if (tick > 0) {  //This schedule block will run if we're over 0 seconds from scheduler loop reset,
    if (schedule == 0) {  //given that it hasn't already been run, which is what the schedule variable tracks.
      // /--------------------- How to create a schedule block ---------------------------------\
      // | Each schedule block lasts for an amount of time determined by when the next call is. |
      // | In that block you must first delete all existing VirtualStrip objects by doing the   |
      // | for loop that immediately follows this text.  Don't change 'numStrips' before you do |
      // | this!  After deleting old strips, set numStrips equal to the number of new strips    |
      // | you're about to create, set strips equal to a new VirtualStrip array of a size equal |
      // | to numStrips, create the actual strips, and finally add them to the array.  We need  |
      // | to make sure this work only happens once, that's why the schedule variable is there. |
      // | Set it equal to the next schedule block's check and set that block's tick value to a |
      // | number of seconds you'd like to reach before activating it.  Tick counts up from 0   |
      // | through the entire schedule until it is reset to 0 in the final block.               |
      // \--------------------------------------------------------------------------------------/
      for (uint8_t i = 0; i < numStrips; i++) {  //delete old VirtualStrips
        delete strips[i];
      }
      numStrips = 2;  //set to the number of new strips we're about to create
      strips = new VirtualStrip * [numStrips];  //set our global 'strips' equal to a new array to hold this shit
      VirtualStrip * s1 = new SineWaves(-30, 24);  //create a new VirtualStrip using the constructor to put in custom values.
      VirtualStrip * s2 = new SineWaves(-10, 24);  //and another
      strips[0] = s1;  //and add them to the array we made
      strips[1] = s2;  //this one too
      schedule = schedule + 1;  //iterate the schedule counter so this block doesn't execute again until after it's looped.
    }
    return;  //return so we don't waste time checking older schedule blocks.
  }
}

// /------------------------------ loop() -----------------------------------\
// | loop only checks on the timers running to determine if anything is due. |
// | Because we're not using delays but instead checking intervals against a |
// | system clock there isn't any LED programming inside loop as is common   |
// | other examples of similar code.                                         |
// \-------------------------------------------------------------------------/
void loop() {
  int now = millis();
  if (now - time_tick > interval_tick) {  //if our scheduler doodad is due (i.e. 1 second has passed)
    scheduler();  //We'll call it
    time_tick = now;  //update its timer
    tick = tick + 1;  //and iterate 'tick', which measure seconds since the last schedule loop reset.
  }
  if (now - time_main > interval_main) {  //if the main refresh doodad is due
    time_main = now;  //update its timer
    refresh();  //and refresh.  Refresh compiles all virtual strips into an output that goes to the LEDs.
  }
  for (uint8_t i = 0; i < numStrips; i++) {  //for the number of strips we've declared
    if (now - strips[i]->timer > strips[i]->interval) {  //if they are due to update
      strips[i]->timer = now;  //we'll update their timer
      strips[i]->update();  //and call the update function for this strip.
    }
  }
}
