# LightSuitNation

Here's a paradigm for programming NeoPixels:  Multiple virtual strips interacting before producing a final output strip state.  We use multiple color strips whose values, pixel by pixel, are added together to create a master color strip, like so:

Strip1 = red,   red,   green, green, blue,  blue.  (6 pixels, each pixel having an rgb value that produces the color named.)
          +      +       +      +      +      +
Strip2 = black, blue,  black, green, white, green.
          =      =       =      =      =      =
master = red,   purple,green, green, white, teal.

This final strip, being the sum of the previous ones, can easily produce a lot of whites, such as in a case of rgb pixels adding together like this:

pixel1 = 255,   0, 255  (these are rgb values now)
          +    +    +
pixel2 = 0,   255,   0
          =    =    =
master = 255, 255, 255

In order to avoid washing things out too much you can adjust the brightness of each strip individually.

Once we've come up with some fun combination of color strips (or just a single one) we can then come up with some intensity strips using the same method.  Intensity strips are converted before processing from rgb 0-255 values into a percentage (0.0-1.0) and then multiplied by the value of the color strips.  This lets us put, say, a RainbowCycle color strip down that fills the entire strip with color all the time, but then combine it with a SineWave intensity strip that will put half of it in darkness at any given time.  Conceptualize it by imagining the intensity strips as a transparency layer through which you are seeing the color strips beneath.

Color and intensity strips use the same programming, we just denote at the time of creation what type they are.

That's the basic paradigm, check out the commented sections of the actual code for more details.
