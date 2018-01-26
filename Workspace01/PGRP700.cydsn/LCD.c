/**
* @file LCD.c
* @Author Insepet LTDA
* @date 28/2/2016
* @brief Archivo que contiene las funciones de control de los LCD
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

//#include <device.h>
#include "VariablesG.h"
#include "I2C.h"

/**
* set_imagen
* @brief envía identificador de imagen a la pantalla para que se muestre
* @param lcd pantalla que se va a usar
* @param id número de imágen a mostrar
*
*/
void set_imagen(uint8 lcd, uint16 id){
    if(lcd == 1){
        LCD_1_PutChar(0xAA);
        LCD_1_PutChar(0x70);
        LCD_1_PutChar(((id>>8)&0xFF));
        LCD_1_PutChar((id&0xFF));
        LCD_1_PutChar(0xCC);
        LCD_1_PutChar(0x33);
        LCD_1_PutChar(0xC3);
        LCD_1_PutChar(0x3C);
    }
    if(lcd == 2){
       	LCD_2_PutChar(0xAA);
        LCD_2_PutChar(0x70);
        LCD_2_PutChar(((id>>8)&0xFF));
        LCD_2_PutChar((id&0xFF));
        LCD_2_PutChar(0xCC);
        LCD_2_PutChar(0x33);
        LCD_2_PutChar(0xC3);
        LCD_2_PutChar(0x3C);   		 
    }
}

/**
* write_LCD
* @brief función para escribir caracteres en pantalla, escribe byte a byte
* @param lcd pantalla que se va a usar
* @param dato caracter a mostrar
* @param pos poscición en pantalla 
*
*/
void write_LCD(uint8 lcd, uint8 dato, uint8 pos){
    if(lcd==1){
        LCD_1_PutChar(0xAA);
        LCD_1_PutChar(0x98);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar((0x3F)+(0x19*pos));
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0x23);
        LCD_1_PutChar(0x24);
        LCD_1_PutChar(0xC5);
        LCD_1_PutChar(0x02);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0xFF);
        LCD_1_PutChar(0xFF);
        LCD_1_PutChar(dato);
        LCD_1_PutChar(0xCC);
        LCD_1_PutChar(0x33);
        LCD_1_PutChar(0xC3);
        LCD_1_PutChar(0x3C);
    }
    else{
        LCD_2_PutChar(0xAA);
        LCD_2_PutChar(0x98);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar((0x3F)+(0x19*pos));
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0x23);
        LCD_2_PutChar(0x24);
        LCD_2_PutChar(0xC5);
        LCD_2_PutChar(0x02);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0xFF);
        LCD_2_PutChar(0xFF);
        LCD_2_PutChar(dato);
        LCD_2_PutChar(0xCC);
        LCD_2_PutChar(0x33);
        LCD_2_PutChar(0xC3);
        LCD_2_PutChar(0x3C);    
    }
}

/**
* Hora_LCD
* @brief escribe la hora actual en la pantalla
* @param lcd pantalla que se va a usar
*
*/
void Hora_LCD(uint8 lcd){
	uint8 hora[24]={0xAA,0x98,0x00,0xd0,0x00,0xbb,0x24,0xC5,0x02,0x00,0x00,0xFF,0xFF,0X30,0x30,':',0x30,0x30,0x41,'m',0xCC,0x33,0xC3,0x3C};	
	hora[13]=(((rventa.hora[1]&0x10)>>4)+48);
	hora[14]=((rventa.hora[1]&0x0F)+48);
	hora[16]=(((rventa.hora[0]&0xF0)>>4)+48);
	hora[17]=((rventa.hora[0]&0x0F)+48);
	if((rventa.hora[1]&0x20)==0x20){
		hora[18]='p';
	}
	else{
		hora[18]='a';						
	}	
    if(lcd==1){
		for(x=0;x<=23;x++){
        	LCD_1_PutChar(hora[x]);
		}
    }
    else{
		for(x=0;x<=23;x++){
	        LCD_2_PutChar(hora[x]);
		}
    }
}

/**
* Fecha_LCD
* @brief escribe la fecha actual en la pantalla
* @param lcd pantalla que se va a usar
*
*/
void Fecha_LCD(uint8 lcd){
	uint8 fecha[25]={0xAA,0x98,0x00,0xcb,0x01,0x27,0x24,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x30,0x30,0x2f,0x30,0x30,0x2f,0x30,0x30,0xCC,0x33,0xC3,0x3C};	
	fecha[13]=(((rventa.fecha[0]&0x30)>>4)+48);
	fecha[14]=((rventa.fecha[0]&0x0F)+48);
	fecha[16]=(((rventa.fecha[1]&0x10)>>4)+48);
	fecha[17]=((rventa.fecha[1]&0x0F)+48);
	fecha[19]=(((rventa.fecha[2]&0xF0)>>4)+48);
	fecha[20]=((rventa.fecha[2]&0x0F)+48);	
    if(lcd==1){
		for(x=0;x<=24;x++){
        	LCD_1_PutChar(fecha[x]);
		}
    }
    else{
		for(x=0;x<=24;x++){
	        LCD_2_PutChar(fecha[x]);
		}
    }
}

/**
* Nombre_LCD
* @brief escribe nombre de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void Nombre_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x00,0x7E,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;(x<=rventa.nombre[0]&&x<=25);x++){
			buffer[13]=rventa.nombre[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;x<=rventa.nombre[0];x++){
			buffer[13]=rventa.nombre[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* Precio_LCD
* @brief escribe precios del producto en pantalla
* @param lcd pantalla que se va a usar
*
*/
void Precio_LCD(uint8 lcd, uint8 y1, uint8 y2, uint16 eeprom){
	uint8 hora[23]={0xAA,0x98,0x01,0x80,0x01,0x60,0x24,0xC5,0x02,0x00,0x00,0xFF,0xFF,'$',0X20,0x20,0X20,0X20,0X20,0xCC,0x33,0xC3,0x3C};	
	hora[4]=y1;
	hora[5]=y2;
	if(leer_eeprom(eeprom,6)==1){
		if((buffer_i2c[0]<=0x05)&&(buffer_i2c[0]>=0x00)){
			for(x=1;x<=buffer_i2c[0];x++){
				hora[x+13]=buffer_i2c[x];
			}
		}
	    if(lcd==1){
			for(x=0;x<=22;x++){
	        	LCD_1_PutChar(hora[x]);
			}
	    }
	    else{
			for(x=0;x<=22;x++){
		        LCD_2_PutChar(hora[x]);
			}
	    }
	}
}

/**
* Grado_LCD
* @brief escribe número de manguera en pantalla
* @param lcd pantalla que se va a usar
* @param y1 posición x
* @param y1 posición y
* @param eeprom posición en eeprom en que se encuentra el dato de grado
*
*/
void Grado_LCD(uint8 lcd, uint8 y1, uint8 y2, uint16 eeprom){
	uint8 hora[18]={0xAA,0x98,0x02,0x20,0x01,0x60,0x24,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
	hora[4]=y1;
	hora[5]=y2;
	if(leer_eeprom(eeprom,2)==1){
		if(buffer_i2c[0]==0x01){
			for(x=1;x<=buffer_i2c[0];x++){
				hora[x+12]=buffer_i2c[x];
			}
		}
	    if(lcd==1){
			for(x=0;x<=22;x++){
	        	LCD_1_PutChar(hora[x]);
			}
	    }
	    else{
			for(x=0;x<=22;x++){
		        LCD_2_PutChar(hora[x]);
			}
	    }
	}
}

/**
* direccion_LCD
* @brief escribe dirección de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void direccion_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x00,0xB7,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;x<=rventa.direccion[0];x++){
			buffer[13]=rventa.direccion[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;x<=rventa.direccion[0];x++){
			buffer[13]=rventa.direccion[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* lema1_LCD
* @brief escribe lema 1 de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void lema1_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x01,0x27,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;x<=rventa.lema1[0];x++){
			buffer[13]=rventa.lema1[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;x<=rventa.lema1[0];x++){
			buffer[13]=rventa.lema1[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* lema2_LCD
* @brief escribe lema 2 de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void lema2_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x01,0x5F,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;x<=rventa.lema2[0];x++){
			buffer[13]=rventa.lema2[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;x<=rventa.lema2[0];x++){
			buffer[13]=rventa.lema2[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* telefono_LCD
* @brief escribe el teléfono de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void telefono_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x00,0xEE,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;x<=rventa.telefono[0];x++){
			buffer[13]=rventa.telefono[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;x<=rventa.telefono[0];x++){
			buffer[13]=rventa.telefono[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* nit_LCD
* @brief escribe el NIT de la estación en pantalla
* @param lcd pantalla que se va a usar
*
*/
void nit_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x01,0x99,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;(x<=rventa.nit[0]&&x<=19);x++){
			buffer[13]=rventa.nit[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }
    else{
		for(x=1;(x<=rventa.nit[0]&&x<=19);x++){
			buffer[13]=rventa.nit[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}
			if(buffer[3]==0xFF){
				buffer[2]=0x01;
				buffer[3]=0x00;
			}			
			buffer[3]+=15;
		}
    }	
}

/**
* Cuenta_LCD
* @brief escribe la cuenta del cliente en pantalla
* @param lcd pantalla que se va a usar
*
*/
void Cuenta_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x01,0x32,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;(x<=Buffer_LCD1.cuenta[0]&&x<=25);x++){
			buffer[3]=((0xB8)+(0x0F*x))&0xFF;
			buffer[2]=((0xB8)+(0x0F*x))>>8;			
			buffer[13]=Buffer_LCD1.cuenta[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}	
		}
    }
    else{
		for(x=1;x<=Buffer_LCD2.cuenta[0];x++){
			buffer[3]=((0xB8)+(0x0F*x))&0xFF;
			buffer[2]=((0xB8)+(0x0F*x))>>8;				
			buffer[13]=Buffer_LCD2.cuenta[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}	
		}
    }	
}

/**
* Placa_LCD
* @brief escribe la placa del cliente en pantalla
* @param lcd pantalla que se va a usar
*
*/
void Placa_LCD(uint8 lcd){
	uint8 buffer[18]={0xAA,0x98,0x00,0xA5,0x00,0xC1,0x23,0xC5,0x02,0x00,0x00,0xFF,0xFF,0x20,0xCC,0x33,0xC3,0x3C};	
    if(lcd==1){
		for(x=1;(x<=Buffer_LCD1.placa[0]&&x<=20);x++){
			buffer[3]=((0xB8)+(0x0F*x))&0xFF;
			buffer[2]=((0xB8)+(0x0F*x))>>8;			
			buffer[13]=Buffer_LCD1.placa[x];			
			for(y=0;y<=17;y++){
        		LCD_1_PutChar(buffer[y]);
			}	
		}
    }
    else{
		for(x=1;(x<=Buffer_LCD2.placa[0]&&x<=20);x++){
			buffer[3]=((0xB8)+(0x0F*x))&0xFF;
			buffer[2]=((0xB8)+(0x0F*x))>>8;				
			buffer[13]=Buffer_LCD2.placa[x];			
			for(y=0;y<=17;y++){
        		LCD_2_PutChar(buffer[y]);
			}	
		}
    }	
}

/**
* writemini_LCD
* @brief escribe datos en pantalla, los datos son de menor tamaño a los usuales
* @param lcd pantalla que se va a usar
* @param dato dato que se va a escribir
* @param pos posición en eje x del dato a mostrar a partir de una posición inicial
*
*/
void writemini_LCD(uint8 lcd, uint8 dato, uint16 pos){
    if(lcd==1){
        LCD_1_PutChar(0xAA);
        LCD_1_PutChar(0x98);
        LCD_1_PutChar(((0x30)+(0x0F*pos))>>8);
        LCD_1_PutChar(((0x30)+(0x0F*pos))&0xFF);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0x30);
        LCD_1_PutChar(0x23);
        LCD_1_PutChar(0xC5);
        LCD_1_PutChar(0x02);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0x00);
        LCD_1_PutChar(0xFF);
        LCD_1_PutChar(0xFF);
        LCD_1_PutChar(dato);
        LCD_1_PutChar(0xCC);
        LCD_1_PutChar(0x33);
        LCD_1_PutChar(0xC3);
        LCD_1_PutChar(0x3C);
    }
    else{
        LCD_2_PutChar(0xAA);
        LCD_2_PutChar(0x98);
        LCD_2_PutChar(((0x30)+(0x0F*pos))>>8);
        LCD_2_PutChar(((0x30)+(0x0F*pos))&0xFF);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0x30);
        LCD_2_PutChar(0x23);
        LCD_2_PutChar(0xC5);
        LCD_2_PutChar(0x02);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0x00);
        LCD_2_PutChar(0xFF);
        LCD_2_PutChar(0xFF);
        LCD_2_PutChar(dato);
        LCD_2_PutChar(0xCC);
        LCD_2_PutChar(0x33);
        LCD_2_PutChar(0xC3);
        LCD_2_PutChar(0x3C);   
    }
}
/* [] END OF FILE */
