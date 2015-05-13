
#include <SPI.h>
#include <SD.h>
#include "ColorLCDShield.h"
#include "Keyes_SJoys.h"
#include <inttypes.h>

#define SD_CS   10  // Chip select line for SD card
#define BKL   45 

LCDShield lcd;
KeyesSjoys KeSj;
char prcl;
volatile int encoder_div;
void setup() {
  Serial.begin(9600);

  lcd.init(PHILLIPS);
  pinMode(BKL, OUTPUT);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");
  KeSj.init(&encoder_div);
  encoder_div = 0;
  attachInterrupt(PIN_A, irp_encoder, FALLING);

}
void loop() {
	COM();
	KeSj.task();
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

 // if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
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
        w = bmpWidth;
        h = bmpHeight;
       // if((x+w-1) >= tft.width())  w = tft.width()  - x;
       // if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        //tft.setAddrWindow(x, y, x+w-1, y+h-1);
		lcd.start_refresh();
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
          if(bmpFile.position() != pos) // Need seek?
		  { 
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
			print_lcd(sdbuffer[buffidx], sdbuffer[buffidx + 1], sdbuffer[buffidx + 2]);
			buffidx += 3;
         //   tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
	uint32_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read();
	((uint8_t *)&result)[2] = f.read();
	((uint8_t *)&result)[3] = f.read(); // MSB
	return result;
}


void print_lcd(uint8_t r, uint8_t g, uint8_t b) {
	static bool load = false;
	uint8_t RGBRGB[6];

	if (load)
	{
		RGBRGB[3] = b;
		RGBRGB[4] = g;
		RGBRGB[5] = r;
		load = false;
		lcd.print_two_pix_deep_color_8_to_4(RGBRGB[0], RGBRGB[1], RGBRGB[2], RGBRGB[3], RGBRGB[4], RGBRGB[5]);
	}
	else
	{
		RGBRGB[0] = b;
		RGBRGB[1] = g;
		RGBRGB[2] = r;
		load = true;
	}
}
char* change_str()
{
	static uint8_t index = 0;
	static char str[7];
	if (index > 26) index = 0;
	str[1] = swch(index % 10);
	str[0] = swch(index / 10);
	index++;
	str[2] = '.';
	str[3] = 'b';
	str[4] = 'm';
	str[5] = 'p';
	str[6] = '\0';

	KeSj.glob_str = str;

	return str;
	
}
char swch(uint8_t indx)
{
	switch (indx)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default:
		return 0;
		break;
	}
}



void irp_encoder(void)
{
	static  uint32_t prevMillis = 0;
	if ((millis() - prevMillis)>100)
	{
		prevMillis = millis();
		if (digitalRead(PIN_B)) {
			encoder_div--;
		}
		else {
			encoder_div++;
		}
	}
	//Serial.println((encoder_div));
}

void COM(void)
{

	if (Serial.available())
	{
		prcl = Serial.read() - 48;
		switch (prcl){
		case 0:  //"0" - Clear
			bmpDraw(change_str(), 0, 0);
			break;

		default:
			Serial.println("Invalid");
		}
		Serial.flush();
	}
}