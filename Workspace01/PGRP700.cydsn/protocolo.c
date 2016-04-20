/*
*********************************************************************************************************
*                                           GRP550/700 CODE
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
*                                             GRP550/700 CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : Protocolo.c
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "VariablesG.h"
#include "I2C.h"
#include "Print.h"
#include <string.h>

/*
*********************************************************************************************************
*                                            	MENSAJES
*********************************************************************************************************
*/

uint8 msn_tc[6]="TOTAL";
uint8 msn_lecact[15]="Valor actual: ";
uint8 msn_lecaan[17]="Valor anterior:  ";
uint8 msn_tventas[18]="Dinero total: $   ";
uint8 msn_totalcero[15]="No toma totales";


/*
*********************************************************************************************************
*                                         ver_pos( void )
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 ver_pos (void){
uint8 x;
    a.dir = 0xff;
    b.dir = 0xff;
    c.dir = 0xff;
    d.dir = 0xff;
     for(x=0;x<=15;x++){
        Surtidor_PutChar(x);
        CyDelay(100);
        if((Surtidor_GetRxBufferSize()>=1)&&(a.dir==0xff)){
           a.dir=x;	
           Surtidor_ClearRxBuffer();
        }
        if((Surtidor_GetRxBufferSize()>=1)&&(x!=a.dir)&&(c.dir==0xff)){
           c.dir=x;
           Surtidor_ClearRxBuffer();
        }
        if((Surtidor_GetRxBufferSize()>=1)&&(x!=a.dir)&&(x!=b.dir)&&(b.dir==0xff)){
           b.dir=x;
           Surtidor_ClearRxBuffer();
        }
        if((Surtidor_GetRxBufferSize()>=1)&&(x!=a.dir)&&(x!=b.dir)&&(x!=c.dir)&&(d.dir==0xff)){
           d.dir=x;
           Surtidor_ClearRxBuffer();
        }
    }
    if((a.dir!=0xff)&&(b.dir!=0xff)&&(d.dir!=0xff)&&(c.dir!=0xff)){        
        return 2;
    }
    else{
        if((a.dir!=0xff)||(b.dir!=0xff)||(c.dir!=0xff)||(d.dir!=0xff)){
            return 1;
        }
        else{
            return 0;
        }
    }
        
   
    return 0;
}

/*
*********************************************************************************************************
*                                         ver_estado( void )
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/

uint8 get_estado(uint8 dir){
    uint8 estado;
    Surtidor_PutChar(dir);
    CyDelay(50);
    if(Surtidor_GetRxBufferSize()>=1){
       estado=(Surtidor_ReadRxData()&0xF0)>>4;
       Surtidor_ClearRxBuffer();
       return estado;
    }
    else{
        return 0;
    }
}

//*
//*********************************************************************************************************
//*                                         void ver_error(uint8 val)
//*
//* Description : 
//*               
//*
//* Argument(s) : none
//*
//* Return(s)   : none
//*
//* Caller(s)   : 
//*
//* Note(s)     : none.
//*********************************************************************************************************
//*/
//void ver_error(uint8 val){
//    uint8 estado;
//    Surtidor_PutChar(val);
//    CyDelay(200);
//    if(Surtidor_GetRxBufferSize()>=1){
//       estado=(Surtidor_ReadRxData()&0xF0)>>4;
//       Surtidor_ClearRxBuffer();
//       if(estado==0){
//
//       }
//    }    
//}

/*
*********************************************************************************************************
*                                         estado_ex(uint8 val)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 estado_ex(uint8 val){
    uint8 rx_extend[19],x,comand_exten[9]={0xFF,0xE9,0xFE,0xE0,0xE1,0xE0,0xFB,0xEE,0xF0};
    Surtidor_PutChar(0x20|val);
    producto=0xff;
    CyDelay(100);
    if(Surtidor_GetRxBufferSize()>=1){
        if(Surtidor_ReadRxData()==(0xD0|val)){
            Surtidor_ClearRxBuffer();
            for(x=0;x<=8;x++){
               Surtidor_PutChar(comand_exten[x]); 
            }
            CyDelay(200);
            if(Surtidor_GetRxBufferSize()>=19){
                for(x=0;x<=18;x++){
                   rx_extend[x]=Surtidor_ReadRxData(); 
                }
                Surtidor_ClearRxBuffer();
                if((rx_extend[0]==0xBA)&&(rx_extend[17]==0x8D)&&(rx_extend[18]==0x8A)&&(rx_extend[12]==0xB1)){
                    switch(rx_extend[14]){
                        case 0xB1:
                            return 1;
                            break;
                        case 0xB2:
                            return 2;
                            break;
                        case 0xB3:
                            return 3;
                            break;
                        default:
                            return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

///*
//*********************************************************************************************************
//*                                         void detener(uint8 val)
//*
//* Description : 
//*               
//*
//* Argument(s) : none
//*
//* Return(s)   : none
//*
//* Caller(s)   : 
//*
//* Note(s)     : none.
//*********************************************************************************************************
//*/
//void detener(uint8 val){
//    Surtidor_PutChar(0x30|val);
//    CyDelay(200);
//}

/*
*********************************************************************************************************
*                                         void venta(uint8 val)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 venta(uint8 val){
    uint8 rx_venta[39],x=0,pos,y; 
    Surtidor_PutChar(0x40|val);
    CyDelay(300);
    if(Surtidor_GetRxBufferSize()<=39){
        while(Surtidor_GetRxBufferSize()>0){
            rx_venta[x]=Surtidor_ReadRxData();  
            x++;
        }
        Surtidor_ClearRxBuffer();
        if(val==0){
            pos=0x10;
            rventa.posicion=pos;
        }
        else{
            pos=val;
            rventa.posicion=pos;
        }
        if((rx_venta[0]==0xFF)&&(rx_venta[2]==0xF8)&&((rx_venta[4]&0x0F)==(pos-1))){
            x=0;
            while((rx_venta[x]!=0xF6)||(x>=39)){
                x++;
            }
            x++;
            rventa.producto=(rx_venta[x]&0x0F)+1;
            while((rx_venta[x]!=0xF7)||(x>=39)){
                x++;
            }
            x++;
            y=1;
            while((rx_venta[x]!=0xF9)||(x>=39)){    
                rventa.ppu[y]=(rx_venta[x]&0x0F);
                x++;
                y++;
            }
			rventa.ppu[0]=y-1;
            x++;
            y=1;
            while((rx_venta[x]!=0xFA)||(x>=39)){
                rventa.volumen[y]=(rx_venta[x]&0x0F);
                x++;
                y++;    
            }
			rventa.volumen[0]=y-1;
            x++;
            y=1;
            while((rx_venta[x]!=0xFB)||(x>=39)){
                rventa.dinero[y]=(rx_venta[x]&0x0F);
                x++;
                y++;    
            }
			rventa.dinero[0]=y-1;
            x+=2;
            if((rx_venta[x]==0xF0)&&(x<=38)){								
				if(val==a.dir){
					write_eeprom(457,rventa.dinero);
					write_eeprom(466,rventa.volumen);
					write_eeprom(475,rventa.ppu);
				}				
				else{
					write_eeprom(482,rventa.dinero);
					write_eeprom(500,rventa.ppu);
					write_eeprom(491,rventa.volumen);				
				}
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

/*
*********************************************************************************************************
*                                         uint8 get_totales(uint8 dir)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 get_totales(uint8 dir,uint8 val){ 
	uint8 corte[25], minuendo[14],sustraendo[14]; 
    uint8 tam;    
    Surtidor_PutChar(0x50|dir);    
    CyDelay(1000);
    tam = Surtidor_GetRxBufferSize();
	switch(tam){
		case 34:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[33]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				rventa.manguera=(Surtidor_rxBuffer[2]&0x0F)+1;			
				for(x=0;x<=7;x++){												
					rventa.tvolumen[x]=(Surtidor_rxBuffer[x+4]&0x0F);
				}
				for(x=0;x<=7;x++){
					rventa.tdinero[x]=(Surtidor_rxBuffer[x+13]&0x0F);
				}
				for(z=0;z<=7;z++){
					corte[z+1]=rventa.tvolumen[z];
				}
				for(z=9;z<=16;z++){
					corte[z]=rventa.tdinero[z-9];
				}
				corte[0]=16;				
				if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
					for(x=0;x<=5;x++){									
						write_psoc1(val,msn_tc[x]);
					}
					write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					}
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(8,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(192,25);
					}
					else{
						leer_eeprom(281,25);
					}
					print_totales(val);
					for(z=1;z<=8;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+8];
					}
					minuendo[0]=8;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);
					if(dir==a.dir){
						write_eeprom(192,corte);	
						write_eeprom(512,resultado);
					}
					else{
						write_eeprom(281,corte);
						write_eeprom(551,resultado);
					}
				}
				if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
					for(x=0;x<=5;x++){									
						write_psoc1(val,msn_tc[x]);
					}
					write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						write_psoc1(val,producto2n[x]);
					}                    
					write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(8,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(217,25);
					}
					else{
						leer_eeprom(320,25);
					}
					print_totales(val);
					for(z=1;z<=8;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+8];
					}
					minuendo[0]=8;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);				
					if(dir==a.dir){
						write_eeprom(217,corte);
						write_eeprom(538,resultado);
					}
					else{
						write_eeprom(320,corte);
						write_eeprom(576,resultado);
					}				
				}
				if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
					for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(8,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(256,25);
					}
					else{
						leer_eeprom(345,25);
					}
					print_totales(val);
					for(z=1;z<=8;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+8];
					}
					minuendo[0]=8;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);							
					if(dir==a.dir){
						write_eeprom(256,corte);
						write_eeprom(525,resultado);
					}
					else{
						write_eeprom(345,corte);
						write_eeprom(589,resultado);
					}					
				}
                
                if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))){					//TOTALES producto4
					for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(8,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(1096,25);
					}
					else{
						leer_eeprom(1054,25);
					}
					print_totales(val);
					for(z=1;z<=8;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+8];
					}
					minuendo[0]=8;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);							
					if(dir==a.dir){
						write_eeprom(1096,corte);
						write_eeprom(1038,resultado);
					}
					else{
						write_eeprom(1054,corte);
						write_eeprom(1081,resultado);
					}					
				}
				
				Surtidor_ClearRxBuffer();
				return 1;
			}
			else{
				return 0;
			}
		break;

		case 64:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[63]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				for(y=0;y<=30;y+=30){
					rventa.manguera=(Surtidor_rxBuffer[y+2]&0x0F)+1;
					for(x=0;x<=7;x++){
						rventa.tvolumen[x]=(Surtidor_rxBuffer[y+4+x]&0x0F);
					}
					for(x=0;x<=7;x++){
						rventa.tdinero[x]=(Surtidor_rxBuffer[y+13+x]&0x0F);
                        
					}
					for(z=0;z<=7;z++){
						corte[z+1]=rventa.tvolumen[z];
					}
					for(z=9;z<=16;z++){
						corte[z]=rventa.tdinero[z-9];
					}
					corte[0]=16;							
					if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
						for(x=0;x<=5;x++){									
							write_psoc1(val,msn_tc[x]);
						}
						write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(192,25);
						}
						else{
							leer_eeprom(281,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);
						if(dir==a.dir){
							write_eeprom(192,corte);	
							write_eeprom(512,resultado);
						}
						else{
							write_eeprom(281,corte);
							write_eeprom(551,resultado);
						}
					}
					if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto2n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(217,25);
						}
						else{
							leer_eeprom(320,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);				
						if(dir==a.dir){
							write_eeprom(217,corte);
							write_eeprom(538,resultado);
						}
						else{
							write_eeprom(320,corte);
							write_eeprom(576,resultado);
						}				
					}
					if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(256,25);
						}
						else{
							leer_eeprom(345,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
							write_eeprom(256,corte);
							write_eeprom(525,resultado);
						}
						else{
							write_eeprom(345,corte);
							write_eeprom(589,resultado);
						}					
					}	
                    
                    if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(1096,25);
						}
						else{
							leer_eeprom(1054,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
						    write_eeprom(1096,corte);
						    write_eeprom(1038,resultado);
					    }
					    else{
						    write_eeprom(1054,corte);
						    write_eeprom(1081,resultado);
						}					
					}
				}
				Surtidor_ClearRxBuffer();
				return 2;					
			}			
			else{
				return 0;
			}
		break;
		
		case 94:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[93]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				for(y=0;y<=60;y+=30){
					rventa.manguera=(Surtidor_rxBuffer[y+2]&0x0F)+1;
					for(x=0;x<=7;x++){
						rventa.tvolumen[x]=(Surtidor_rxBuffer[y+4+x]&0x0F);						
					}
					for(x=0;x<=7;x++){
						rventa.tdinero[x]=(Surtidor_rxBuffer[y+13+x]&0x0F);					
					}
					for(z=0;z<=7;z++){
						corte[z+1]=rventa.tvolumen[z];
					}
					for(z=9;z<=16;z++){
						corte[z]=rventa.tdinero[z-9];
					}
					corte[0]=16;		
					if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
						for(x=0;x<=5;x++){									
							write_psoc1(val,msn_tc[x]);
						}
						write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(192,25);
						}
						else{
							leer_eeprom(281,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);
						if(dir==a.dir){
							write_eeprom(192,corte);	
							write_eeprom(512,resultado);
						}
						else{
							write_eeprom(281,corte);
							write_eeprom(551,resultado);
						}
					}
					if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto2n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(217,25);
						}
						else{
							leer_eeprom(320,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);				
						if(dir==a.dir){
							write_eeprom(217,corte);
							write_eeprom(538,resultado);
						}
						else{
							write_eeprom(320,corte);
							write_eeprom(576,resultado);
						}				
					}
					if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(256,25);
						}
						else{
							leer_eeprom(345,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
							write_eeprom(256,corte);
							write_eeprom(525,resultado);
						}
						else{
							write_eeprom(345,corte);
							write_eeprom(589,resultado);
						}					
					}
                    
                    if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(8,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(1096,25);
						}
						else{
							leer_eeprom(1054,25);
						}
						print_totales(val);
						for(z=1;z<=8;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+8];
						}
						minuendo[0]=8;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
						    write_eeprom(1096,corte);
						    write_eeprom(1038,resultado);
					    }
					    else{
						    write_eeprom(1054,corte);
						    write_eeprom(1081,resultado);
						}					
					}
                    
				}
				Surtidor_ClearRxBuffer();
				return 3;					
			}			
			else{
				return 0;
			}			
		break;

		case 46:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[45]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				rventa.manguera=(Surtidor_rxBuffer[2]&0x0F)+ 1;
				for(x=0;x<=11;x++){
					rventa.tvolumen[x]=(Surtidor_rxBuffer[x+4]&0x0F);
				}
				for(x=0;x<=11;x++){
					rventa.tdinero[x]=(Surtidor_rxBuffer[x+17]&0x0F);
				}
				for(z=0;z<=11;z++){
					corte[z+1]=rventa.tvolumen[z];
				}
				for(z=13;z<=24;z++){
					corte[z]=rventa.tdinero[z-13];
				}
				corte[0]=24;				
				if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1d&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
					for(x=0;x<=5;x++){									
						write_psoc1(val,msn_tc[x]);
					}
					write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					}
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(12,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(192,25);
					}
                    if(dir==b.dir){
						leer_eeprom(1192,25);
					}
                    if(dir==d.dir){
						leer_eeprom(1292,25);
					}
					else{
						leer_eeprom(281,25);
					}
					print_totales(val);
					for(z=1;z<=12;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+12];
					}
					minuendo[0]=12;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);
					if(dir==a.dir){
						write_eeprom(192,corte);	
						write_eeprom(512,resultado);
					}
                    if(dir==b.dir){
						write_eeprom(1192,corte);	
						write_eeprom(1392,resultado);
					}
                    if(dir==d.dir){
						write_eeprom(1292,corte);	
						write_eeprom(1405,resultado);
					}
					else{
						write_eeprom(281,corte);
						write_eeprom(551,resultado);
					}
				}
				if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2d&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2c&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
					for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						    write_psoc1(val,producto2n[x]);
					    }
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(12,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(217,25);
					}
                    if(dir==b.dir){
						leer_eeprom(1217,25);
					}
                    if(dir==d.dir){
						leer_eeprom(1317,25);
					}
					else{
						leer_eeprom(320,25);
					}
					print_totales(val);
					for(z=1;z<=12;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+12];
					}
					minuendo[0]=12;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);				
					if(dir==a.dir){
						write_eeprom(217,corte);
						write_eeprom(538,resultado);
					}
                    if(dir==b.dir){
						write_eeprom(1217,corte);
						write_eeprom(1418,resultado);
					}
                    if(dir==d.dir){
						write_eeprom(1317,corte);
						write_eeprom(1431,resultado);
					}
					else{
						write_eeprom(320,corte);
						write_eeprom(576,resultado);
					}				
				}
				if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto3c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto3d&0x0F))&&(dir==d.dir))){					//TOTALES producto3
					for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(12,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(256,25);
					}
                    if(dir==b.dir){
						leer_eeprom(1342,25);
					}
                    if(dir==d.dir){
						leer_eeprom(1242,25);
					}
					else{
						leer_eeprom(345,25);
					}
					print_totales(val);
					for(z=1;z<=12;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+12];
					}
					minuendo[0]=12;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);							
					if(dir==a.dir){
						write_eeprom(256,corte);
						write_eeprom(525,resultado);
					}
                    if(dir==b.dir){
						write_eeprom(1342,corte);
						write_eeprom(1443,resultado);
					}
                    if(dir==d.dir){
						write_eeprom(1242,corte);
						write_eeprom(1456,resultado);
					}
					else{
						write_eeprom(345,corte);
						write_eeprom(589,resultado);
					}					
				}
                
                if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto4c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto4d&0x0F))&&(dir==d.dir))){					//TOTALES producto3
					for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                    for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                    write_psoc1(val,10);
					for(x=0;x<=14;x++){									
						write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
					}
					write_psoc1(val,10);
					print_totalesac(12,val);	
					for(x=0;x<=16;x++){												
						write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
					}
					write_psoc1(val,10);
					if(dir==a.dir){
						leer_eeprom(1096,25);
					}
                    if(dir==b.dir){
						leer_eeprom(1267,25);
					}
                    if(dir==d.dir){
						leer_eeprom(1367,25);
					}
					else{
						leer_eeprom(1054,25);
					}
					print_totales(val);
					for(z=1;z<=12;z++){
						minuendo[z]=rventa.tdinero[z-1];
						sustraendo[z]=buffer_i2c[z+12];
					}
					minuendo[0]=12;
					operacion(2,minuendo,sustraendo);
					for(x=0;x<=17;x++){									
						write_psoc1(val,msn_tventas[x]);				//Total de ventas
					}
					print_valor(resultado,val);							
					if(dir==a.dir){
						write_eeprom(1096,corte);
						write_eeprom(1038,resultado);
					}
                    if(dir==b.dir){
						write_eeprom(1267,corte);
						write_eeprom(1470,resultado);
					}
                    if(dir==d.dir){
						write_eeprom(1367,corte);
						write_eeprom(1483,resultado);
					}
					else{
						write_eeprom(1054,corte);
						write_eeprom(1081,resultado);
				    }						
				}
				Surtidor_ClearRxBuffer();
				return 1;
			}
			else{
				return 0;
			}			
		break;
		
		case 88:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[87]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				for(y=0;y<=42;y+=42){
					rventa.manguera=(Surtidor_rxBuffer[y+2]&0x0F)+1;
					for(x=0;x<=11;x++){
						rventa.tvolumen[x]=(Surtidor_rxBuffer[y+4+x]&0x0F);
					}
					for(x=0;x<=11;x++){
						rventa.tdinero[x]=(Surtidor_rxBuffer[y+17+x]&0x0F);
					}
					for(z=0;z<=11;z++){
						corte[z+1]=rventa.tvolumen[z];
					}
					for(z=13;z<=24;z++){
						corte[z]=rventa.tdinero[z-13];
					}
					corte[0]=24;				
					if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1d&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
						for(x=0;x<=5;x++){									
							write_psoc1(val,msn_tc[x]);
						}
                        write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					    }
                        write_psoc1(val,10);						
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(192,25);
						}
						else{
							leer_eeprom(281,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);
						if(dir==a.dir){
							write_eeprom(192,corte);	
							write_eeprom(512,resultado);
						}
						else{
							write_eeprom(281,corte);
							write_eeprom(551,resultado);
						}
					}
					if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2d&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2c&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto2n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(217,25);
						}
						else{
							leer_eeprom(320,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);				
						if(dir==a.dir){
							write_eeprom(217,corte);
							write_eeprom(538,resultado);
						}
						else{
							write_eeprom(320,corte);
							write_eeprom(576,resultado);
						}				
					}
					if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto3c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto3d&0x0F))&&(dir==d.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(256,25);
						}
						else{
							leer_eeprom(345,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
							write_eeprom(256,corte);
							write_eeprom(525,resultado);
						}
						else{
							write_eeprom(345,corte);
							write_eeprom(589,resultado);
						}					
					}
                    
                    
                    if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto4c&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto4d&0x0F))&&(dir==c.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(1096,25);
						}
						else{
							leer_eeprom(1054,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
						    write_eeprom(1096,corte);
						    write_eeprom(1038,resultado);
					    }
					    else{
						    write_eeprom(1054,corte);
						    write_eeprom(1081,resultado);
				        }					
					}
				}
				Surtidor_ClearRxBuffer();
				return 2;					
			}			
			else{
				return 0;
			}			
		break;
		
		case 130:
			if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[129]==0xF0)&&(Surtidor_rxBuffer[3]==0xF9)){
				for(y=0;y<=84;y+=42){
					rventa.manguera=(Surtidor_rxBuffer[y+2]&0x0F)+1;
					for(x=0;x<=11;x++){
						rventa.tvolumen[x]=(Surtidor_rxBuffer[y+4+x]&0x0F);
					}
					for(x=0;x<=11;x++){
						rventa.tdinero[x]=(Surtidor_rxBuffer[y+17+x]&0x0F);
					}
					for(z=0;z<=11;z++){
						corte[z+1]=rventa.tvolumen[z];
					}
					for(z=13;z<=24;z++){
						corte[z]=rventa.tdinero[z-13];
					}
					corte[0]=24;				
					if(((rventa.manguera==(producto1&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto1&0x0F))&&(dir==c.dir))||((rventa.manguera==(producto1b&0x0F))&&(dir==d.dir))){				//TOTAL PRODUCTO1
						for(x=0;x<=5;x++){									
							write_psoc1(val,msn_tc[x]);
						}
						write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						write_psoc1(val,producto1n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(192,25);
						}
						else{
							leer_eeprom(281,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);
						if(dir==a.dir){
							write_eeprom(192,corte);	
							write_eeprom(512,resultado);
						}
						else{
							write_eeprom(281,corte);
							write_eeprom(551,resultado);
						}
					}
					if(((rventa.manguera==(producto2&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==b.dir))||((rventa.manguera==(producto2b&0x0F))&&(dir==d.dir))||((rventa.manguera==(producto2&0x0F))&&(dir==c.dir))){		//TOTALES PRODUCTO 2
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto2n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(217,25);
						}
						else{
							leer_eeprom(320,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);				
						if(dir==a.dir){
							write_eeprom(217,corte);
							write_eeprom(538,resultado);
						}
						else{
							write_eeprom(320,corte);
							write_eeprom(576,resultado);
						}				
					}
					if(((rventa.manguera==(producto3&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto3b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto3n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(256,25);
						}
						else{
							leer_eeprom(345,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
							write_eeprom(256,corte);
							write_eeprom(525,resultado);
						}
						else{
							write_eeprom(345,corte);
							write_eeprom(589,resultado);
						}					
					}
                    
                    if(((rventa.manguera==(producto4&0x0F))&&(dir==a.dir))||((rventa.manguera==(producto4b&0x0F))&&(dir==b.dir))){					//TOTALES producto3
						for(x=0;x<=5;x++){									
						    write_psoc1(val,msn_tc[x]);
					    }
					    write_psoc1(val,10);
                        for(x=0;x<=10;x++){									
						    write_psoc1(val,producto4n[x]);
					    }
                        write_psoc1(val,10);
						for(x=0;x<=14;x++){									
							write_psoc1(val,msn_lecact[x]);				//LECTURA ACTUAL
						}
						write_psoc1(val,10);
						print_totalesac(12,val);	
						for(x=0;x<=16;x++){												
							write_psoc1(val,msn_lecaan[x]);				//LECTURA ANTERIOR
						}
						write_psoc1(val,10);
						if(dir==a.dir){
							leer_eeprom(1096,25);
						}
						else{
							leer_eeprom(1054,25);
						}
						print_totales(val);
						for(z=1;z<=12;z++){
							minuendo[z]=rventa.tdinero[z-1];
							sustraendo[z]=buffer_i2c[z+12];
						}
						minuendo[0]=12;
						operacion(2,minuendo,sustraendo);
						for(x=0;x<=17;x++){									
							write_psoc1(val,msn_tventas[x]);				//Total de ventas
						}
						print_valor(resultado,val);							
						if(dir==a.dir){
						    write_eeprom(1096,corte);
						    write_eeprom(1038,resultado);
					    }
					    else{
						    write_eeprom(1054,corte);
						    write_eeprom(1081,resultado);
				        }					
					}
				}
				Surtidor_ClearRxBuffer();
				return 3;					
			}			
			else{
				return 0;
			}			
		break;
			
		default:
            for(x=0;x<15;x++){									
				write_psoc1(val,msn_totalcero[x]);				//Total de ventas
			}
			return 0;	
		break;	
	}		
}

/*
*********************************************************************************************************
*                                         uint8 programar(uint8 dir)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 programar(uint8 dir, uint8 grado, uint8 *valor, uint8 preset){
	uint8 buffer[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},size,temp,i,j,decimal=0;
	temp=4;
	if(((versurt==5)||(versurt==6))&&(preset==1)){
		temp=3;
	}
	if((versurt==5)&&(preset==2)){
		temp=5;
	}
	if((versurt==7)&&(preset==1)){
		temp=0;
	}
	if((versurt==7)&&(preset==2)){
		temp=2;
	}	
	decimal=0;
	for(i=1;i<=valor[0];i++){
		if(decimal>=1){
			decimal++;
			valor[i]=valor[i+1];
		}				
		if(valor[i]==','){
			decimal++;
			valor[i]=valor[i+1];
		}
	}
	if(decimal>=1){
		decimal=decimal-1;
		valor[0]=valor[0]-1;
	}	
	if(decimal>=4){
		return 0;
	}	
	if(versurt!=7){		
		buffer[0]=0xFF;
		buffer[1]=(0xE0|temp);
		buffer[2]=(0xF0|preset);
		buffer[3]=0xF4;
		buffer[4]=0xF8;	
		for(i=5;i<((5+decimalD)-decimal);i++){
			buffer[i]=0xE0;
		}
		for(j=i;j<(i+valor[0]);j++){
			buffer[j]=(0xE0|(valor[valor[0]-(j-i)]&0x0F));	
		}
		for(i=j;i<=10;i++){
			buffer[i]=0xE0;
		}
		buffer[11]=0xFB;
		buffer[12]=0;
		for(i=0;i<=11;i++){
			buffer[12]+=(buffer[i]&0x0F);	
		}
		buffer[12]=(((~buffer[12])+1)&0x0F)|0xE0;
		buffer[13]=0xF0;
		size=13;
		if(versurt==5){
			buffer[10]=0xFB;
			buffer[11]=0;
			for(i=0;i<=10;i++){
				buffer[11]+=(buffer[i]&0x0F);	
			}
			buffer[11]=(((~buffer[11])+1)&0x0F)|0xE0;
			buffer[12]=0xF0;
			size=12;		
		}
		if(preset==1){
			if(decimal==3){
				valor[0]=valor[0]-1;
				decimal=decimal-1;
			}
			buffer[4]=0xF6;
			buffer[5]=(0xE0|(grado-1));
			buffer[6]=0xF8;
			for(i=7;i<((7+(decimalV-1))-decimal);i++){
				buffer[i]=0xE0;
			}
			for(j=i;j<(i+valor[0]);j++){
				buffer[j]=(0xE0|(valor[valor[0]-(j-i)]&0x0F));	
			}
			for(i=j;i<=11;i++){
				buffer[i]=0xE0;
			}			
			buffer[12]=0xFB;
			buffer[13]=0;
			for(i=0;i<=12;i++){
				buffer[13]+=(buffer[i]&0x0F);	
			}
			buffer[13]=(((~buffer[13])+1)&0x0F)|0xE0;			
			buffer[14]=0xF0;
			size=14;
		}	
	}	
	if(versurt==7){
		buffer[0]=0xFF;
		buffer[1]=(0xE0|temp);
		buffer[2]=(0xF0|preset);
		buffer[3]=0xF4;
		buffer[4]=0xF8;	
		for(i=5;i<((5+decimalD)-decimal);i++){
			buffer[i]=0xE0;
		}
		for(j=i;j<(i+valor[0]);j++){
			buffer[j]=(0xE0|(valor[valor[0]-(j-i)]&0x0F));	
		}
		for(i=j;i<=12;i++){
			buffer[i]=0xE0;
		}
		buffer[13]=0xFB;
		buffer[14]=0;
		for(i=0;i<=13;i++){
			buffer[14]+=(buffer[i]&0x0F);	
		}
		buffer[14]=(((~buffer[14])+1)&0x0F)|0xE0;
		buffer[15]=0xF0;
		size=15;
		if(preset==1){
			buffer[4]=0xF6;
			buffer[5]=(0xE0|(grado-1));
			buffer[6]=0xF8;
			for(i=7;i<((7+decimalV)-decimal);i++){
				buffer[i]=0xE0;
			}
			for(j=i;j<(i+valor[0]);j++){
				buffer[j]=(0xE0|(valor[valor[0]-(j-i)]&0x0F));	
			}
			for(i=j;i<=14;i++){
				buffer[i]=0xE0;
			}			
			buffer[15]=0xFB;
			buffer[16]=0;
			for(i=0;i<=15;i++){
				buffer[16]+=(buffer[i]&0x0F);	
			}
			buffer[16]=(((~buffer[16])+1)&0x0F)|0xE0;			
			buffer[17]=0xF0;
			size=17;
		}		
	}	
	Surtidor_PutChar(0x20|dir);	
    CyDelay(50);
    if(Surtidor_GetRxBufferSize()>=1){
		if(Surtidor_ReadRxData()==(0xD0|dir)){
            Surtidor_ClearRxBuffer();
            for(i=0;i<=size;i++){
               	Surtidor_PutChar(buffer[i]);
            }
			CyDelay(50);
			if(get_estado(dir)==0){
				return 0;
			}
			else{
				return 1;
			}
		}
		else{
			return 0;
		}
	}
	else{
		return 0;	
	}
}
/*
*********************************************************************************************************
*                                         uint8 cambiar precio(uint8 dir)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 cambiar_precio(uint8 dir){
	uint8 buffer[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},size;
	if(versurt!=7){
		buffer[0]=0xFF;
		buffer[1]=0xE5;
		buffer[2]=0xF4;
		buffer[3]=0xF6;
		buffer[4]=(0xE0|((rventa.producto&0x0f)-1));
		buffer[5]=0xF7;	
		if(ppux10==0){
			for(x=6;x<(6+teclas1);x++){
				buffer[x]=(0xE0|(Buffer_LCD1.valor[teclas1-(x-6)]&0x0F));
			}
		}
		else{
			teclas1=teclas1-1;
			for(x=6;x<(6+teclas1);x++){
				buffer[x]=(0xE0|(Buffer_LCD1.valor[teclas1-(x-6)]&0x0F));
			}	
		}
		for(y=x;y<=9;y++){
			buffer[y]=0xE0;	
		}
		buffer[10]=0xFB;
		for(x=0;x<=10;x++){
			buffer[11]+=(buffer[x]&0x0F);	
		}
		buffer[11]=(((~buffer[11])+1)&0x0F)|0xE0;
		buffer[12]=0xF0;
		size=12;
	}
	if(versurt==7){
		buffer[0]=0xFF;
		buffer[1]=0xE3;
		buffer[2]=0xF4;
		buffer[3]=0xF6;
		buffer[4]=(0xE0|((rventa.producto&0x0f)-1));
		buffer[5]=0xF7;		
		for(x=6;x<(6+teclas1);x++){
			buffer[x]=(0xE0|(Buffer_LCD1.valor[teclas1-(x-6)]&0x0F));
		}	
		for(y=x;y<=11;y++){
			buffer[y]=0xE0;	
		}		
		buffer[12]=0xFB;
		for(x=0;x<=12;x++){
			buffer[13]+=(buffer[x]&0x0F);	
		}
		buffer[13]=(((~buffer[13])+1)&0x0F)|0xE0;
		buffer[14]=0xF0;
		size=14;
	}	
	Surtidor_PutChar(0x20|dir);	
    CyDelay(100);
    if(Surtidor_GetRxBufferSize()>=1){
		if(Surtidor_ReadRxData()==(0xD0|dir)){
            Surtidor_ClearRxBuffer();
            for(x=0;x<=size;x++){
               	Surtidor_PutChar(buffer[x]); 	
            }
			CyDelay(50);
			if(get_estado(dir)==0){
				return 0;
			}
			else{
				return 1;
			}
		}
		else{
			return 0;
		}
	}
	else{
		return 0;	
	}
}

/*
*********************************************************************************************************
*                                         uint8 totales(uint8 dir)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8 totales (uint8 addr){
    uint8 x;
    Surtidor_ClearRxBuffer();
    Surtidor_PutChar(0x50|addr);
    CyDelay(100);
	if(Surtidor_GetRxBufferSize()>=1){
        if(addr==(a.dir)){
            x=Surtidor_GetRxBufferSize();
            if((x==46)||(x==88)||(x==130)){				
					if((Surtidor_rxBuffer[0]==0xFF)&&(Surtidor_rxBuffer[1]==0xF6)&&(Surtidor_rxBuffer[3]==0xF9)){
                    
                    }
            }
        }
    }
    
    return 0;
}




/* [] END OF FILE */
