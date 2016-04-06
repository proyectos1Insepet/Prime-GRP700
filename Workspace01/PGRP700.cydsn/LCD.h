/*
*********************************************************************************************************
*                                           GRP550M CODE
*
*                             (c) Copyright 2013; Sistemas Insepet LTDA
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                               GRP550M CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : LCD.h
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/

#ifndef LCD_H
#define LCD_H
#include <device.h>
void set_imagen(uint8 lcd, uint16 id);
void write_LCD(uint8 lcd, uint8 dato, uint8 pos);
void Hora_LCD(uint8 lcd);
void Fecha_LCD(uint8 lcd);
void Nombre_LCD(uint8 lcd);
void direccion_LCD(uint8 lcd);
void telefono_LCD(uint8 lcd);
void lema2_LCD(uint8 lcd);
void lema1_LCD(uint8 lcd);
void nit_LCD(uint8 lcd);
void writemini_LCD(uint8 lcd, uint8 dato, uint8 pos);
void Precio_LCD(uint8 lcd, uint8 y1, uint8 y2, uint16 eeprom);
void Grado_LCD(uint8 lcd, uint8 y1, uint8 y2, uint16 eeprom);
void Cuenta_LCD(uint8 lcd);
void Placa_LCD(uint8 lcd);

#endif

//[] END OF FILE
