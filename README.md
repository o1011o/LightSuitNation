# LightSuitNation

This readme sucks, but it will get better.

Here's a paradigm for programming NeoPixels:  Multiple virtual color strips added together viewed through a visibility/intensity layer.  Like so:

color strips are 2d arrays, length equal to number of pixels, width equal to 3 (r,g,b) values from 0-255 (usually*).
intensity strips are the same size, with decimal values from 0-1.

(colorStrip1 + colorStrip2) * (intensityStrip1 + intensityStrip2) = output

color strips have a ton of variables that can hopefully be attached to physical controls at some point, but in the meantime they can still be set programmatically.  Brightness, speed, color themes, etc.  If adding a lot of color strips, reduce each one's brightness so the final result isn't washed out.  Same with intensity strips.

Color and intensity strips are the same thing used differently -- if showSineWaves == true that array is treated like a color array and left as 0-255 color values.  if instead showSineWavesIntensity == true then those values are translated to 0-1 and it's calculated as intensity.  

Themes are 1d arrays of rgb color values that can be used by a function to randomly choose from.
Default parameters are set right after themes.  If those values are changed in the scheduler they'll have to be changed back manually until next runtime.
Scheduler is a place to program in changes to variables, making different strips active, shit like that.

I made an object-oriented version of this but it had too much overhead for poor little adafruit flora...I may still go back to that paradigm if I can make it fit.
