//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

#include "Ass-03.h"

//
// PLACE YOUR COMMON CODE HERE
//

// STEPIEN: Added two touch panel functions to make the interface more
//          consistent with the LCD BSP.

uint8_t BSP_TP_Init(void)
{
  // Initialise the interface and calibrate
  TP_Init(); // This is an empty function since done by STM32CubeMX
  TouchPanel_Calibrate();

  return 0;
}

uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay)
{
  Coordinate *pScreen;

  pScreen = Read_Ads7846();
  if (pScreen == NULL)
  {
    return 1; // Error reading the touch panel
  }
  if (getDisplayPoint(pDisplay, pScreen, &matrix ) == DISABLE)
  {
    return 1; // Error in LCD
  }
  return 0;
}


//Button Structure initialisation
const button_s button[] = {


		{(uint8_t *)"DISPLAY",	{110, 	310, 	13,		181},	LCD_COLOR_WHITE, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"1",		{110,	151,	197,	227},	LCD_COLOR_WHITE, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"2",		{163,	204,	197,	227},	LCD_COLOR_WHITE, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"3",		{216,	257,	197,	227},	LCD_COLOR_WHITE, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"4",		{269,	310,	197,	227},	LCD_COLOR_WHITE, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},

		{(uint8_t *)"START",	{10, 	100,	13,		43},	LCD_COLOR_GREEN, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"STOP",		{10, 	100,	59,		89},	LCD_COLOR_RED, 		&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},
		{(uint8_t *)"STORE",	{10, 	100,	105,	135},	LCD_COLOR_YELLOW, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12}, 	&doNothing},
		{(uint8_t *)"LOAD",		{10, 	100,	151,	181},	LCD_COLOR_YELLOW, 	&Font20, LCD_COLOR_BLACK,	CENTER_MODE,	{0, 12},	&doNothing},

		{(uint8_t *)"DEBUG",	{10, 	100,	197,	227}, 	LCD_COLOR_CYAN, 	&Font20, LCD_COLOR_WHITE,	CENTER_MODE,	{0, 12},	&doNothing},

		//End of Structure
		{NULL,	{0,		0,		0,		0},		(int32_t)NULL,  (int32_t)NULL,	(int32_t)NULL,	(Line_ModeTypdef)NULL,	{0, 0}, NULL}
};

void doNothing (uint8_t ArgNum, uint8_t *ArgStrings[])
{
	return;
}

void ButtonDisplay(uint8_t buttonNum, uint8_t indent)
{

	//Fill Rectangle
	if(indent){
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	}
	else{
		BSP_LCD_SetTextColor(button[buttonNum].colour);
	}
	BSP_LCD_FillRect(button[buttonNum].co_ord[0],button[buttonNum].co_ord[2],
			button[buttonNum].co_ord[1]-button[buttonNum].co_ord[0],button[buttonNum].co_ord[3]-button[buttonNum].co_ord[2]);

	//Draw outlines
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect(button[buttonNum].co_ord[0],button[buttonNum].co_ord[2],
			button[buttonNum].co_ord[1]-button[buttonNum].co_ord[0],button[buttonNum].co_ord[3]-button[buttonNum].co_ord[2]);

	//Insert String
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	if(indent){
		BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	}
	else{
		BSP_LCD_SetBackColor(button[buttonNum].colour);
	}

	BSP_LCD_SetFont(button[buttonNum].fontSize);
	BSP_LCD_DisplayStringAt((((button[buttonNum].co_ord[1]-button[buttonNum].co_ord[0])/2)+button[buttonNum].co_ord[0]),
			button[buttonNum].co_ord[2]+(0.3*(button[buttonNum].co_ord[3]-button[buttonNum].co_ord[2])),
			button[buttonNum].display, CENTER_MODE);

}
