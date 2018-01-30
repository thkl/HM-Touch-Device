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

#include "GfxUi.h"

static const uint8_t isinTable8[] = { 
  0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 
  49, 53, 57, 62, 66, 70, 75, 79, 83, 87, 
  91, 96, 100, 104, 108, 112, 116, 120, 124, 128, 
  131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 
  167, 171, 174, 177, 180, 183, 186, 190, 192, 195, 
  198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 
  223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 
  241, 243, 244, 245, 246, 247, 248, 249, 250, 251, 
  252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 
}; 

GfxUi::GfxUi(Adafruit_ILI9341 *tft) {
  _tft = tft;
  _width = _tft->width();
  _height = _tft->height();
}

void GfxUi::drawString(int x, int y, char *text) {
  int16_t x1, y1;
  uint16_t w, h;
  _tft->setTextWrap(false);
  _tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  switch (_alignment) {
    case LEFT:
      x1 = x;
      break;
    case CENTER:
      x1 = x - w / 2;
      break;
    case RIGHT:
      x1 = x - w;
      break;
  }
  if (_textColor != _backgroundColor) {
    _tft->fillRect(x1, y - h -1, w + 1, h + 1, _backgroundColor);
  }
  _tft->setCursor(x1, y);
  _tft->print(text);
}

void GfxUi::drawString(int x, int y, String text) {
  char buf[text.length()+2];
  text.toCharArray(buf, text.length() + 1);
  drawString(x, y, buf);
}

void GfxUi::setTextColor(uint16_t c) {
  setTextColor(c, c);
}
void GfxUi::setTextColor(uint16_t c, uint16_t bg) {
  _textColor = c;
  _backgroundColor = bg;
  _tft->setTextColor(_textColor, _backgroundColor);
}

void GfxUi::setTextAlignment(TextAlignment alignment) {
  _alignment = alignment;
}

void GfxUi::drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor) {
  if (percentage == 0) {
    _tft->fillRoundRect(x0, y0, w, h, 3, _backgroundColor);
  }
  uint8_t margin = 2;
  uint16_t barHeight = h - 2 * margin;
  uint16_t barWidth = w - 2 * margin;
  _tft->drawRoundRect(x0, y0, w, h, 3, frameColor);
  _tft->fillRect(x0 + margin, y0 + margin, barWidth * percentage / 100.0, barHeight, barColor);
}


// Code mostly from 
// https://github.com/adafruit/Adafruit_ILI9341/blob/master/examples/spitftbitmap/spitftbitmap.ino
void GfxUi::drawBmp(String filename, uint16_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col, x2, y2, bx1, by1;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= _tft->width()) || (y >= _tft->height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SPIFFS.open(filename, "r")) == NULL) {
    Serial.println(F("File not found"));
    Serial.println("Here comes the directory");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      Serial.print(dir.fileName());
    }
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        x2 = x + bmpWidth  - 1; // Lower-right corner
        y2 = y + bmpHeight - 1;
        if((x2 >= 0) && (y2 >= 0)) { // On screen?
          w = bmpWidth; // Width/height of section to load/display
          h = bmpHeight;
          bx1 = by1 = 0; // UL coordinate in BMP file
          if(x < 0) { // Clip left
            bx1 = -x;
            x   = 0;
            w   = x2 + 1;
          }
          if(y < 0) { // Clip top
            by1 = -y;
            y   = 0;
            h   = y2 + 1;
          }
          if(x2 >= _tft->width())  w = _tft->width()  - x; // Clip right
          if(y2 >= _tft->height()) h = _tft->height() - y; // Clip bottom

          // Set TFT address window to clipped image bounds
          _tft->startWrite(); // Requires start/end transaction now
          _tft->setAddrWindow(x, y, w, h);

          for (row=0; row<h; row++) { // For each scanline...
  
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
              pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else     // Bitmap is stored top-to-bottom
              pos = bmpImageoffset + row * rowSize;
            if(bmpFile.position() != pos) { // Need seek?
              _tft->endWrite(); // End TFT transaction
              bmpFile.seek(pos, SeekSet);
              buffidx = sizeof(sdbuffer); // Force buffer reload
              _tft->startWrite(); // Start new TFT transaction
            }
  
            for (col=0; col<w; col++) { // For each pixel...
              // Time to read more pixel data?
              if (buffidx >= sizeof(sdbuffer)) { // Indeed
                _tft->endWrite(); // End TFT transaction
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0; // Set index to beginning
                _tft->startWrite(); // Start new TFT transaction
              }
  
              // Convert pixel from BMP to TFT format, push to display
              b = sdbuffer[buffidx++];
              g = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              _tft->writePixel(_tft->color565(r,g,b));
             yield();
            } // end pixel
          } // end scanline
          Serial.print(F("Loaded in "));
          Serial.print(millis() - startTime);
          Serial.println(" ms");
        } // end onscreen
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}




// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t GfxUi::read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t GfxUi::read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

int GfxUi::i_sin(int x){
  boolean pos = true;  // positive - keeps an eye on the sign.
  uint8_t idx;
   //remove next 6 lines for fastestl!
/*     if (x < 0) {
       x = -x;
       pos = !pos;  
     }  
    if (x >= 360) x %= 360;   */ 
  if (x > 180) 
  {
    idx = x - 180;
    pos = !pos;
  }
  else idx = x;
  if (idx > 90) idx = 180 - idx;
  if (pos) return isinTable8[idx]/2 ;
  return -(isinTable8[idx]/2);
}

int GfxUi::i_cos(int x){
  return i_sin(x+90);
}



void GfxUi::drawArc(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re, uint16_t color) {
  Serial.print("DrawArc");
  int16_t l,i,w;//int16_t
  int16_t x1,y1,x2,y2;
  unsigned short dw;
  if (re > rs)
    dw = re-rs;
  else
    dw = 256-rs+re;
    
  if (dw == 0) dw = 256;
  l = (uint8_t)(((((unsigned short)r * dw) >> 7) * (unsigned short)201)>>8);
  //l = (uint8_t)(((((uint16_t)r * dw) >> 7) * (uint16_t)201)>>7);
  //l = (uint8_t)(((((uint16_t)r * dw) >> 7) * (uint16_t)256)>>7);
  x1 = x + (((int16_t)r * (int16_t)i_cos(rs)) >> 7);
  y1 = y + (((int16_t)r * (int16_t)i_sin(rs)) >> 7);
  for (i = 1;i <= l;i++){
    w = ((unsigned short)dw*(unsigned short)i) / (unsigned short)l + rs;
  //w = ((uint16_t)dw * (uint16_t)i) / (uint16_t)l + rs;
    x2 = x + (((int16_t)r * (int16_t)i_cos(w)) >> 7);
    y2 = y + (((int16_t)r * (int16_t)i_sin(w)) >> 7);
  if ((x1 < _width && x2 < _width) && (y1 < _height && y2 < _height)) _tft->drawLine(x1,y1,x2,y2,color);
    x1 = x2;
    y1 = y2;
  }
}



void GfxUi::ellipse(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  long a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1; /* values of diameter */
  long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
  long err = dx + dy + b1 * a * a, e2; /* error of 1.step */

  if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
  if (y0 > y1) y0 = y1; /* .. exchange them */
  y0 += (b + 1) / 2; /* starting pixel */
  y1 = y0 - b1;
  a *= 8 * a;
  b1 = 8 * b * b;

  do {
    _tft->drawPixel(x1, y0, color); /*   I. Quadrant */
    _tft->drawPixel(x0, y0, color); /*  II. Quadrant */
    _tft->drawPixel(x0, y1, color); /* III. Quadrant */
    _tft->drawPixel(x1, y1, color); /*  IV. Quadrant */
    e2 = 2 * err;
    if (e2 >= dx) { x0++; x1--; err += dx += b1; } /* x step */
    if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */
  } while (x0 <= x1);

  while (y0 - y1 < b) {  /* too early stop of flat ellipses a=1 */
    _tft->drawPixel(x0 - 1, ++y0, color); /* -> complete tip of ellipse */
    _tft->drawPixel(x0 - 1, --y1, color);
  }
}

void GfxUi::drawBezier(int x0, int y0, int x1, int y1, int x2, int y2,uint16_t color){
  int sx = x0 < x2 ? 1 : -1, sy = y0 < y2 ? 1 : -1; /* step direction */
  int cur = sx * sy * ((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); /* curvature */
  int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2, xy = 2 * x * y * sx * sy;
                                /* compute error increments of P0 */
  long dx = (1 - 2 * abs(x0 - x1)) * y * y + abs(y0 - y1) * xy - 2 * cur * abs(y0 - y2);
  long dy = (1 - 2 * abs(y0 - y1)) * x * x + abs(x0 - x1) * xy + 2 * cur * abs(x0 - x2);
                                /* compute error increments of P2 */
  long ex = (1 - 2 * abs(x2 - x1)) * y * y + abs(y2 - y1) * xy + 2 * cur * abs(y0 - y2);
  long ey = (1 - 2 * abs(y2 - y1)) * x * x + abs(x2 - x1) * xy - 2 * cur * abs(x0 - x2);

  if (cur == 0) { _tft->drawLine(x0, y0, x2, y2, color); return; } /* straight line */

  x *= 2 * x; y *= 2 * y;
  if (cur < 0) { /* negated curvature */
    x = -x; dx = -dx; ex = -ex; xy = -xy;
    y = -y; dy = -dy; ey = -ey;
  }
  /* algorithm fails for almost straight line, check error values */
  if (dx >= -y || dy <= -x || ex <= -y || ey >= -x) {
    _tft->drawLine(x0, y0, x1, y1, color); /* simple approximation */
    _tft->drawLine(x1, y1, x2, y2, color);
    return;
  }
  dx -= xy; ex = dx+dy; dy -= xy; /* error of 1.step */

  for(;;) { /* plot curve */
    _tft->drawPixel(y0, x0, color);
    ey = 2 * ex - dy; /* save value for test of y step */
    if (2 * ex >= dx) { /* x step */
      if (x0 == x2) break;
      x0 += sx; dy -= xy; ex += dx += y;
    }
    if (ey <= 0) { /* y step */
      if (y0 == y2) break;
      y0 += sy; dx -= xy; ex += dy += x;
    }
  }
}


