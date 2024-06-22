## TD5Fuel
In car fuel consumption guage in Litres per 100 K -  STM32, OLED Display (SSD1306, SSD1331, SSD1351)

This project is Work In Progress.

### Requirements
* Adafruit_GFX
* Adafruit library matching your OLED
* L9637D or ECU Emulator wired up appropriately - will function without either
* supported microcontroller, see below.

### Can be used with
* Maple Mini (or clone)
* Blue Pill
* Black Pill
* Black Pill F303

### Display types supported:
    *SSD1306 - Monochrome 128 * 96 OLED
    *SSD1331 - Full colour 96 * 64 OLED
    *SSD1351 - Full colour 128 * 128 OLED
    Adding additional displays (which have Adafruit libraries) should be as simple as adding a section to Macros.h and an ifdef section in LRDuinoTD5.ino


## License

      The source code of this project are released under "THE BEER-WARE" license.

I would, however, consider it a great courtesy if you could email me and tell me about your project and how this code was used, just for my own continued personal gratification :)

## Contribution

        Contributions in all forms(including documentation) are welcomed. If you would like to contribute to this project, then just fork it in github and send a pull request.

## References

*STM32Duino - http ://www.stm32duino.com
* Adafruint Graphics Library - https : //github.com/adafruit/Adafruit-GFX-Library

## Limitations

* Many
