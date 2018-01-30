/**The MIT License (MIT)
Copyright (c) 2015 by Daniel Eichhorn
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
See more at http://blog.squix.ch
*/

#include <Adafruit_ILI9341.h>
#include <FS.h>

#ifndef _GFX_UI_H
#define _GFX_UI_H


#define BUFFPIXEL 30

enum TextAlignment {
  LEFT, CENTER, RIGHT
};

class GfxUi {
  public:
    GfxUi(Adafruit_ILI9341 * tft);
    void drawString(int x, int y, char *text);
    void drawString(int x, int y, String text);
    void setTextAlignment(TextAlignment alignment);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void drawBmp(String filename, uint16_t x, uint16_t y);
    void drawProgressBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor);
    void drawArc(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re, uint16_t color);//new
    void ellipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);//new
    void drawBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);//new
 //   void arcPosition(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re,int16_t rc);
  private:
    Adafruit_ILI9341 * _tft;
    TextAlignment _alignment = LEFT;
    uint16_t _textColor;
    uint16_t _backgroundColor;
    uint16_t read16(File &f);
    uint32_t read32(File &f);
    int i_sin(int x);
    int i_cos(int x);
    int16_t _width, _height;
};

#endif


