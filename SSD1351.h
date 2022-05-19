/**
 * @file SSD1351.h
 * @author Gustavo Villaescusa Escudero (@gutzchi)
 * @brief This is a simple library for driving efficiently the SSD1351 under the mbed OS.
 * 
 * Part of the code is taken from the Adafruit SSD1351 library.
 * 
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SSD1351_GUTZCHI_
#define _SSD1351_GUTZCHI_

#include <mbed.h>

/* some 16bit RGB color definitions  */
typedef enum {                                       
 Black           = 0x0000,      /*   0,   0,   0 */
 Navy            = 0x000F,      /*   0,   0, 128 */
 DarkGreen       = 0x03E0,      /*   0, 128,   0 */
 DarkCyan        = 0x03EF,      /*   0, 128, 128 */
 Maroon          = 0x7800,      /* 128,   0,   0 */
 Purple          = 0x780F,      /* 128,   0, 128 */
 Olive           = 0x7BE0,      /* 128, 128,   0 */
 LightGrey       = 0xC618,      /* 192, 192, 192 */
 DarkGrey        = 0x7BEF,      /* 128, 128, 128 */
 Blue            = 0x001F,      /*   0,   0, 255 */
 Green           = 0x07E0,      /*   0, 255,   0 */
 Cyan            = 0x07FF,      /*   0, 255, 255 */
 Red             = 0xF800,      /* 255,   0,   0 */
 Magenta         = 0xF81F,      /* 255,   0, 255 */
 Yellow          = 0xFFE0,      /* 255, 255,   0 */
 White           = 0xFFFF,      /* 255, 255, 255 */
 Orange          = 0xFD20,      /* 255, 165,   0 */
 GreenYellow     = 0xAFE5,      /* 173, 255,  47 */
 Pink            = 0xF81F
} Color_t;    

/* Union definition for pixel transfer*/
typedef union{
    uint16_t i;
    char c[2];
} Pixel_t;

// SSD1351 Commands
#define SSD1351_CMD_SETCOLUMN 		0x15
#define SSD1351_CMD_SETROW    		0x75
#define SSD1351_CMD_WRITERAM   		0x5C
#define SSD1351_CMD_READRAM   		0x5D
#define SSD1351_CMD_SETREMAP 		0xA0
#define SSD1351_CMD_STARTLINE 		0xA1  //Set display start line 
#define SSD1351_CMD_DISPLAYOFFSET 	0xA2
#define SSD1351_CMD_DISPLAYALLOFF 	0xA4
#define SSD1351_CMD_DISPLAYALLON  	0xA5
#define SSD1351_CMD_NORMALDISPLAY 	0xA6
#define SSD1351_CMD_INVERTDISPLAY 	0xA7
#define SSD1351_CMD_FUNCTIONSELECT 	0xAB
#define SSD1351_CMD_DISPLAYOFF 		0xAE
#define SSD1351_CMD_DISPLAYON     	0xAF
#define SSD1351_CMD_PRECHARGE 		0xB1
#define SSD1351_CMD_DISPLAYENHANCE	0xB2
#define SSD1351_CMD_CLOCKDIV 		0xB3
#define SSD1351_CMD_SETVSL 			0xB4
#define SSD1351_CMD_SETGPIO 		0xB5
#define SSD1351_CMD_PRECHARGE2 		0xB6
#define SSD1351_CMD_SETGRAY 		0xB8
#define SSD1351_CMD_USELUT 		    0xB9
#define SSD1351_CMD_PRECHARGELEVEL 	0xBB
#define SSD1351_CMD_VCOMH 			0xBE
#define SSD1351_CMD_CONTRASTABC		0xC1
#define SSD1351_CMD_CONTRASTMASTER	0xC7
#define SSD1351_CMD_MUXRATIO            0xCA
#define SSD1351_CMD_COMMANDLOCK         0xFD
#define SSD1351_CMD_HORIZSCROLL         0x96
#define SSD1351_CMD_STOPSCROLL          0x9E
#define SSD1351_CMD_STARTSCROLL         0x9F

// Constants
constexpr char SSD1351_DEFAULT_WIDTH = 128;
constexpr char SSD1351_DEFAULT_HEIGHT = 96;

class SSD1351{
public:
    /**
     * @brief Display constructor with SPI interface
     * 
     * @param spi_device a pointer to the SPI interface
     * @param CS The Chip Select pin, has to be connected to a GPIO.
     * @param DC The Data/Command pin, has to be connected to a GPIO. 
     * @param RST The display reset pin, has to be connected to a GPIO.
     * @param POWER The power pin, has to be connected to a GPIO.
     *              If no power pin is used, leave unconnected however this
     *              will affect the power down policy.
     * @param powerDownState the power down state of the power pin
     */
    SSD1351(SPI * spi_device, PinName CS, PinName DC, PinName RST, PinName POWER = NC, int powerDownState = 0);

    /**
     * @brief Initialize the display.
     * Has to be called before trying to write to the screen
     */
    void init();

    /**
     * @brief Deinitialization function. Used to 
     * reduce power consumption.
     * 
     */
    void deinit();

    /**
     * @brief Send one byte of command instructions
     * 
     * @param command the byte to send
     * @return int 0 on success*/
    void writeCommand(const char command);

    /**
     * @brief Send one byte of data instructions
     * 
     * @param command the byte to send
     */
    void writeData(const char data);

    /**
     * @brief Send a list of data instructions
     * 
     * @param command the list to send
     * @param size the size of the list
     */
    void writeData(char * data, const int size);

    /**
     * @brief Set the size of the display. 
     * The display size is initialized to SSD1351_DEFAULT_WIDTH
     * and SSD1351_DEFAULT_HEIGHT
     * 
     * @param width the width of the display
     * @param height the height of the display
     */
    void setDisplaySize(char width, char height){ _width = width; _height = height;}

    /**
     * @brief Sets the rectangle to write directly to the screen.
     * This function is called internally.
     * 
     * @param x x coordinate of the top right corner of the rectangle
     * @param y y coordinate of the top right corner of the rectangle
     * @param width width of the rectangle
     * @param height height of the rectangle
     */
    void setBorders(char x, char y, char width, char height);

    /**
     * @brief Fills the display screen with the specified color.
     * 
     * @param color the background color of the screen.
     */
    void fillScreen(uint16_t color);

    /**
     * @brief Locate the cursor in which to write 
     *        It is used to give the feel of a command window
     *        when calling several putString operations
     * @param x the x coordinate
     * @param y the y coordinate
     * @return int 0 on success
     */
    void locateCursor(char x, char y){ _x = x; _y = y; }

    /**
     * @brief Write a string to the display
     *        New lines are allowed and the cursor moves along.
     * @param str the string to write
     * @return int 0 on success
     */
    int writeString(const std::string& str, uint16_t color);

private:
    SPI * _spi;
    DigitalOut _cs;
    DigitalOut _dc;
    DigitalOut _rst;
    DigitalOut _power;    

    char _width, _height;
    char _x = 0, _y = 0;

    uint16_t _bgColor = 0x0000;
};

#endif