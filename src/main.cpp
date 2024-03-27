// Example sketch which shows how to display some patterns
// on a 64x32 LED matrix

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another
 
//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
  if(pos < 85) {
    return dma_display->color565(pos * 3, 255 - pos * 3, 0);
  } else if(pos < 170) {
    pos -= 85;
    return dma_display->color565(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return dma_display->color565(0, pos * 3, 255 - pos * 3);
  }
}

void drawText(int colorWheelOffset)
{
  dma_display->fillScreen(myBLACK);
  // draw text with a rotating colour
  dma_display->setTextSize(1);     // size 1 == 8 pixels high
  dma_display->setTextWrap(false); // Don't wrap at end of line - will do ourselves

  dma_display->setCursor(0, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  const char *str = "HACKERSPACE";
  for (w=0; w<strlen(str); w++) {
    dma_display->setTextColor(colorWheel((w*32)+colorWheelOffset));
    dma_display->print(str[w]);
  }

  dma_display->println();
  dma_display->print(" ");
  for (w=9; w<18; w++) {
    dma_display->setTextColor(colorWheel((w*32)+colorWheelOffset));
    dma_display->print("*");
  }
  
  dma_display->println();

  dma_display->setTextColor(dma_display->color444(0,15,4));
  dma_display->println(" POMERANIA ");

  dma_display->fillCircle(32, 44, 18, dma_display->color444(0, 15, 4));

  dma_display->setTextColor(dma_display->color444(0,0,0));

  dma_display->setCursor(19, 39);
  dma_display->print("truj");
  dma_display->setCursor(20, 47);
  dma_display->print("zomb");
}

void setup() {
  // Serial.begin(115200);
  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );
  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;
  // mxconfig.setPixelColorDepthBits(32);
  mxconfig.double_buff = true;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(255); //0-255
  dma_display->clearScreen();
  /* power draw test */
  // dma_display->fillScreen(myWHITE); 
  // dma_display->flipDMABuffer();
  // delay(5000);
}

uint8_t wheelval = 0;
void loop() {
    // animate by going through the colour wheel for the first two lines
    drawText(wheelval);
    wheelval +=1;

    dma_display->flipDMABuffer();
    delay(20);  
}