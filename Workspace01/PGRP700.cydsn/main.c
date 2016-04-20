/*
*********************************************************************************************************
*                                           GRP550/700 CODE
*
*                             (c) Copyright 2016; Sistemas Insepet LTDA
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
*                                           GRP 700 Prime CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : main.c
* Version       : 1.0.0
* Programmer(s) : 
                  
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/


#include "Protocolo.h"
#include "VariablesG.h"
#include "Print.h"
#include "LCD.h"
#include "ibutton.h"
#include "I2C.h"
#include <project.h>
#include <device.h>




/*
*********************************************************************************************************
*                                             DECLARACION DE FUNCIONES
*********************************************************************************************************
*/
uint8 letras[26]={0x25,0x42,0x31,0x27,0x1D,0x28,0x29,0x2A,0x22,0x2B,0x2C,0x2D,0x35,0x34,0x23,0x24,0x1B,0x1E,0x26,0x1F,0x21,0x32,0x1C,0x30,0x20,0x2F};
uint8 test[18]="Test de Impresora";
uint8 puk[8]="18928005";
uint8 serial[17]="0FFFFF8FEEBEB2DC0";
uint8 pasword_corte[5]={4,'1','2','3','4'};

/*
*********************************************************************************************************
*                                         init( void )
*
* Description : Verifica el serial, inicia los periféricos, la versión y los datos de la estación.
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : main()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void init(void){
	/**** Inicio de periféricos ****/
    
    CyGlobalIntEnable;
    Surtidor_Start();
    Impresora_Start();
    LCD_1_Start();
    LCD_2_Start();
    Surtidor_EnableRxInt();    
    I2C_1_Start();           
    CyDelay(5);	
    a_copias = 0;
    
    /**** Lectura de los campos de la eeprom para inicializar variables ****/
    
    leer_eeprom(0,32);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.nombre[x]=buffer_i2c[x];                  //Nombre EDS 
	}
	leer_eeprom(32,15);
	for(x=0;x<=buffer_i2c[0];x++){                       //Nit
		rventa.nit[x]=buffer_i2c[x];
	}
    leer_eeprom(47,12);
	for(x=0;x<=buffer_i2c[0];x++){                       //Teléfono
		rventa.telefono[x]=buffer_i2c[x];
	}
	leer_eeprom(64,32);
	for(x=0;x<=buffer_i2c[0];x++){                       //Dirección
		rventa.direccion[x]=buffer_i2c[x];
	}
    
    leer_eeprom(96,9);                                   //Password
	if((buffer_i2c[0]<=0x09)&&(buffer_i2c[0]>=0x01)){     
		for(x=0;x<=7;x++){
			rventa.password[x+1]=buffer_i2c[x+1];
		}
		rventa.password[0]=buffer_i2c[0];
	}else{
		for(x=1;x<=8;x++){
			rventa.password[x]=x;
		}	
	}
    
	leer_eeprom(128,32);
	for(x=0;x<=buffer_i2c[0];x++){                        //Lema 1
		rventa.lema1[x]=buffer_i2c[x];
	}	
	leer_eeprom(160,32);                                  //Lema 2
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.lema2[x]=buffer_i2c[x];
	}	
    
    leer_eeprom(442,2);                                // Version del equipo
	if(buffer_i2c[0]==5){
		 versurt=(buffer_i2c[1]&0x0f);
	}
    
	leer_eeprom(451,2);                     
	if(buffer_i2c[0]==1){                               //Grados de los productos
		producto1=buffer_i2c[1];
	}
    leer_eeprom(453,2);
	if(buffer_i2c[0]==1){
		producto2=buffer_i2c[1];
	}
	leer_eeprom(449,2);
	if(buffer_i2c[0]==1){
		producto3=buffer_i2c[1];
	}
	
    leer_eeprom(1006,2);
	if(buffer_i2c[0]==1){
		producto4=buffer_i2c[1];                        //Grados de los productos
	}			    
    
    leer_eeprom(636,2);                                //Grados productos 2do lado
	if(buffer_i2c[0]==1){
		producto1b=buffer_i2c[1];
	}
	leer_eeprom(634,2);
	if(buffer_i2c[0]==1){
		producto3b = buffer_i2c[1];
	}
	leer_eeprom(638,2);
	if(buffer_i2c[0]==1){
	    producto2b=buffer_i2c[1];
	}
    leer_eeprom(1008,2);
	if(buffer_i2c[0]==1){
		producto4b=buffer_i2c[1];                      //Grados productos 2do lado
	}
    
    leer_eeprom(1176,2);
	if(buffer_i2c[0]==1){
		producto1c=buffer_i2c[1];                      //Grados productos 3er lado
	}
    
    leer_eeprom(1178,2);
	if(buffer_i2c[0]==1){
		producto2c=buffer_i2c[1];                      //Grados productos 3er lado
	}
    leer_eeprom(1180,2);
	if(buffer_i2c[0]==1){
		producto3c=buffer_i2c[1];                      //Grados productos 3er lado
	}
    leer_eeprom(1182,2);
	if(buffer_i2c[0]==1){
		producto4c=buffer_i2c[1];                      //Grados productos 3er lado
	}
    leer_eeprom(1184,2);
	if(buffer_i2c[0]==1){
		producto1d=buffer_i2c[1];                      //Grados productos 4to lado
	}
    leer_eeprom(1186,2);
	if(buffer_i2c[0]==1){
		producto2d=buffer_i2c[1];                      //Grados productos 4to lado
	}
    leer_eeprom(1188,2);
	if(buffer_i2c[0]==1){
		producto3d=buffer_i2c[1];                      //Grados productos 4to lado
	}
    leer_eeprom(1190,2);
	if(buffer_i2c[0]==1){
		producto4d=buffer_i2c[1];                      //Grados productos 4to lado
	}
    leer_eeprom(671,5);
	if(buffer_i2c[0]==4){
		for(x=1;x<=4;x++){                             //Password cierre de turno
			pasword_corte[x]=buffer_i2c[x];
		}
		pasword_corte[0]=buffer_i2c[0];
	}
    
    leer_eeprom(1130,10);
    for(x=0;x<=buffer_i2c[0];x++){                      //Carga nombre producto 1
		producto1n[x]=buffer_i2c[x]; 
	}
    leer_eeprom(1141,10);
    for(x=0;x<=buffer_i2c[0];x++){                      //Carga nombre producto 2
		producto2n[x]=buffer_i2c[x];
	}
    leer_eeprom(1152,10);
    for(x=0;x<=buffer_i2c[0];x++){                     //Carga nombre producto 3
		producto3n[x]=buffer_i2c[x];
	}
    leer_eeprom(1163,10);
    for(x=0;x<=buffer_i2c[0];x++){                     //Carga nombre producto 4
		producto4n[x]=buffer_i2c[x];
	}
    leer_eeprom(1174,2);
    for(x=0;x<=buffer_i2c[0];x++){                    //Carga cantidad de copias
		n_copias[x]=buffer_i2c[x];
	}
		     
	leer_eeprom(602,6);
	if(buffer_i2c[0]==5){
		 decimalD=(buffer_i2c[3]&0x0f);               //Si hay o no decimales en volumen y precio
		 decimalV=(buffer_i2c[5]&0x0f);
	}	
	leer_eeprom(455,2);
    if(buffer_i2c[0]==1){
        bandera[0]=buffer_i2c[0];                       // Carga bandera
	    bandera[1]=(buffer_i2c[1]&0x0f);
    }  
	leer_eeprom(444,2);                                 // PPU x1 o x10   
	if(buffer_i2c[0]==1){
		 ppux10=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(611,2);                                 //Impresoras
	if(buffer_i2c[0]==1){
		 print1[1]=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(613,2);                                 //Impresora lado 2
	if(buffer_i2c[0]==1){
		 print2[1]=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(668,3);                                 //Id de corte
	if(buffer_i2c[0]==2){                               
		 id_corte=((buffer_i2c[2]<<8)|buffer_i2c[2]);
	}
	else{
		 id_corte=1;
	}	
    leer_eeprom(984,2);										//Copia de Recibo
	if(buffer_i2c[0]==1){
		 copia_recibo[1]=buffer_i2c[1];
	}
	else{
		 copia_recibo[0]=1;
		 copia_recibo[1]=1;
		 write_eeprom(984,copia_recibo);
	}
    
    leer_eeprom(1167,2);	
    if(buffer_i2c[0]==1){
		 copia_recibo2[1]=buffer_i2c[1];                   //Copia recibo lcd2
	}
	else{
		 copia_recibo2[0]=1;
		 copia_recibo2[1]=1;
		 write_eeprom(1167,copia_recibo2);
	}
    leer_eeprom(978,6);										//Id venta
	if(buffer_i2c[0]==5){
		for(x=0;x<=buffer_i2c[0];x++){
			id_venta[x]=buffer_i2c[x]; 
		}
	}          
	else{
		id_venta[0]=5;	
		id_venta[1]='0';
		id_venta[2]='0';
		id_venta[3]='0';
		id_venta[4]='0';
		id_venta[5]='0';
		write_eeprom(978,id_venta);
	}
	leer_eeprom(1012,2);										//Tipo impresora
	if(buffer_i2c[0]==1){
		for(x=0;x<=buffer_i2c[0];x++){
			tipo_imp[x]=buffer_i2c[x]; 
		}
	}
	no_venta=((id_venta[5]&0x0F)*10000)+((id_venta[4]&0x0F)*1000)+((id_venta[3]&0x0F)*100)+((id_venta[2]&0x0F)*10)+((id_venta[1]&0x0F));

    
}
/*
*********************************************************************************************************
*                                         init_surt( void )
*
* Description : Busca las posiciones del surtidor y las graba en lado.a.dir y lado.b.dir
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : main()
*
* Note(s)     : Falta generar codigo para los casos 1 y 2
*********************************************************************************************************
*/
void init_surt(void){
	uint8 seguir=0;
	while(seguir==0){
		switch(ver_pos()){
			case 0:
                CyDelay(100);
			    set_imagen(1,79);						//mostrar error de comunicacion
                seguir = 0;
			break;
			
			case 1:
			CyDelay(100);
			if(get_estado(a.dir)==6 && get_estado(b.dir)==6 ){	
				seguir = 1;
                set_imagen(1,60); 
                set_imagen(2,60); 
			}else{
                set_imagen(1,85);
                set_imagen(2,85);
            }
			break;
			
			case 2:
			CyDelay(100);
			if(get_estado(a.dir)==6 && get_estado(b.dir)==6 && get_estado(c.dir)==6 && get_estado(d.dir)==6 ){	
				seguir = 2;
                set_imagen(1,74);
                set_imagen(2,74);
			}else{
                set_imagen(1,85);
                set_imagen(2,85);
            }
            
			break;
		}
	}		
}
/*
*********************************************************************************************************
*                                         init_pantallas( void )
*
* Description : Inicio de las pantallas por primera vez, las deja listas para iniciar venta y reinicia 
*               flujo de los LCD
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : main()
*
* Note(s)     : Falta generar codigo para los casos 1 y 2
*********************************************************************************************************
*/
void init_pantallas(void){
    flujo_LCD  = 0;
    flujo_LCD2 = 0;    
    CyDelay(500);
    set_imagen(1,46);
    CyDelay(500);
    set_imagen(2,46);    
}



/*
*********************************************************************************************************
*                                         void polling_pos1(void)
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

void polling_pos1(void){    
    switch(flujo_LCD){ 
        case 0:            
        if(venta_activa == 1){
            flujo_LCD = flujo_pos;            
        }
        if(venta_activa2 == 1){
            flujo_LCD2 = flujo_pos2;
            seleccion_pos = 2;
        }
		    if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){                    
                    switch(LCD_1_rxBuffer[3]){
				        case 0x56:						        //POS A                                                                                                                       
                                if(venta_activa == 1){
                                    flujo_LCD = flujo_pos;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    set_imagen(1,99);           //Inicio opciones GRP 700 POS A
                                    seleccion_pos = 1;
                                    flujo_LCD  = 1;
                                }
                                if (venta_activa2 == 1){
                                    flujo_LCD2 = flujo_pos2;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    flujo_LCD2 = 0;                            
                                }
					    break;
                            		       
					    case 0x57:  				            //POS B                                                                 
                                if(venta_activa == 1){
                                    flujo_LCD = flujo_pos;
                                }else{
                                    flujo_LCD = 0;
                                } 
                                if (venta_activa2 == 1){                                    
                                    flujo_LCD2 = flujo_pos2;
                                }else{
                                    set_imagen(1,99);           //Inicio opciones GRP 700
							        seleccion_pos = 2;
                                    flujo_LCD2 = 1;                            
                                }                           
					    break;
                                                   
				    }                                                            
			    }
                CyDelay(100);
			    LCD_1_ClearRxBuffer();
		    }
                                
        
        break;
            
        case 1:
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    switch(LCD_1_rxBuffer[3]){
                        case 0x5C:
                            set_imagen(1,73);        //Seleccion de tipo de venta
                            flujo_LCD = 2;
                        break;
                        
                        case 0x45:		  
					        set_imagen(1,37);	
                            teclas1=0; 						    //Otras opciones
                            flujo_LCD=15;   
                        break; 

                        case 0x59:		 
                            set_imagen(1,37);
					        teclas1=0;								//Corte
                            flujo_LCD = 39;   
                        break;
                            
                        case 0x7E:
                            set_imagen(1,46); //Regreso a pantalla de selección de posición
                            flujo_LCD = 0;
                        break;
                            
                        case 0x94:
                            set_imagen(1,46); //Regreso a pantalla de selección de posición
                            flujo_LCD = 0;
                        break;                            
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
        }
        break;
        
        case 2:     
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x5F:							//Sin ID	                                                                              
                      set_imagen(1,11);                 //Pasa a pantalla de solicitud de recibo 
                      flujo_LCD=10; 
                    break;
                    
                    case 0x5E:  						//Con ID                                         
                      set_imagen(1,29);
                      flujo_LCD=11;
                    break;	

                    case 0x7E:						    //ir a menu
					  set_imagen(1,46);	
                      flujo_LCD=0;     
                    break;	
                    
                    case 0x94:
                        set_imagen(1,99); //Regreso a pantalla de selección de posición
                        flujo_LCD = 1;
                    break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
        }        
        break;
        
        case 4:   
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				    Buffer_LCD1.preset&=0xFC;
                    switch(LCD_1_rxBuffer[3]){
                        case 0x0F:		   					 	 //Preset por dinero	                               
                            set_imagen(1,6);                   
                            teclas1=0;                           //Inicia el contador de teclas                                         
                            write_LCD(1,'$',0);
					        Buffer_LCD1.preset|=2;
                            flujo_LCD=5;   
                        break;
                    
                        case 0x10:  	                         //Inicia el contador de teclas
                            set_imagen(1,13);              
                            teclas1=0;       
                            comas1=0;
                            write_LCD(1,'G',0);
					        Buffer_LCD1.preset|=1;				 // Preset por volumen
                            flujo_LCD=5;   
                        break;
                    
                        case 0x43:  	                                     
                            set_imagen(1,7);					 //Full
                            flujo_LCD = 6;    
                        break;
                    
                        case 0x3B:                        	    //Cancel                     					  
		            	    set_imagen(1,46);			 						  
					        flujo_LCD=0;  
                        break; 
					
                        case 0x7E:									//ir a menu
					        set_imagen(1,46);
                            flujo_LCD=0;     
                        break;	
                        
                        case 0x94:
                            set_imagen(1,11); //Regreso a pantalla de selección de posición
                            flujo_LCD = 10;
                        break;
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 5:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=(versurt-1)){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
                    if(LCD_1_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas1>=1 && comas1==0){
                            teclas1++;
                            Buffer_LCD1.valor[teclas1]=0x2C;
                            write_LCD(1,0x2C,teclas1);
                            comas1=1;
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,5);
                        flujo_LCD=4;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=1 && a_copias ==0){
                        set_imagen(1,7);
						Buffer_LCD1.valor[0]=teclas1;
                        flujo_LCD=6;                                                 				
                    }
                    if(teclas1>=1 && a_copias == 1){ 
                        set_imagen(1,112);
                        n_copias[0]=Buffer_LCD1.valor[1];                       
                        write_eeprom(1174,n_copias);
                        a_copias = 0;
                        flujo_LCD = 14;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }          
        break;     
            
            
        case 6:		 
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    if(LCD_1_rxBuffer[3]==0x7E){			//ir a menu
					    set_imagen(1,46);					//Cancel
					    flujo_LCD = 0;
                    }												
			    }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
			break;
		    }
            if(get_estado(a.dir)==7){													//Espera a que este en listo el equipo				
			CyDelay(50);
            grado1=estado_ex(a.dir);
			if((Buffer_LCD1.preset&0x02)==0x02||(Buffer_LCD1.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(a.dir,grado1,Buffer_LCD1.valor,(Buffer_LCD1.preset&0x03))==0){
					set_imagen(1,46);
					flujo_LCD=0;
					break;
				}					
			}			
			grado1=estado_ex(a.dir);		//Manejo de grados?, como	
			Surtidor_PutChar(0x10|a.dir);									//Autoriza el surtidor
			set_imagen(1,8);            
            flujo_LCD=7;
            flujo_pos = flujo_LCD;
            venta_activa = 1;
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(a.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(a.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);                     
                    venta_activa = 0;                    
                    CyDelay(20);
                    flujo_LCD = 13;                   
				}
                if(venta(a.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    venta_activa = 0;                    
                    CyDelay(500);
                    set_imagen(1,46);                    
                    flujo_LCD=0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(a.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);
                    venta_activa = 0;                    
                    CyDelay(20);               //Finaliza venta con impresión de recibo                      
                    flujo_LCD=13;
				}
                if(venta(a.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    venta_activa = 0;                    
                    CyDelay(500);                    
                    set_imagen(1,46);
                    flujo_LCD = 0;
				}
			break;

	        case 0x06: 
                set_imagen(1,46); //No hizo venta
                venta_activa = 0;
                Buffer_LCD1.preset = 0x00;
				flujo_LCD=0;
			break;
                
            case 0x09:
                CyDelay(50);
                set_imagen(1,46);
                flujo_LCD = 0;                
            break;    
             			 	
         }		
        break;
        
        case 9:
          if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){				
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.placa[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);                        
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas1++;                        
                        Buffer_LCD1.placa[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);                        
                    }  
                    if(LCD_1_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas1++;                        
                        Buffer_LCD1.placa[teclas1]='-';
                        write_LCD(1,'-',teclas1);                        
                    } 					
                    if(LCD_1_rxBuffer[3]>=0x1B && LCD_1_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_1_rxBuffer[3]==letras[x]){
                                teclas1++;                            
                                Buffer_LCD1.placa[teclas1]=x+0x41;
                                write_LCD(1,(x+0x41),teclas1);                            
                            }
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas1==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(1,46);                        
                        Buffer_LCD1.placa[0]=0;						
                        flujo_LCD=0;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);                        
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){                                        //Enter pasa a venta y guarda los datos de recibo
                    if(teclas1>=1){ 
                        set_imagen(1,5);
                        Buffer_LCD1.placa[0]=teclas1; 
                        Buffer_LCD1.posventa=1;
                        flujo_LCD=4;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }    
		
        break;
        
        case 10:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x39:                          //Si pide impresión                         
                        set_imagen(1,10);               //Pasa a pedir placa
                        teclas1 = 0;                                              
                        no_imprime = 0;
                        flujo_LCD = 9;                       
                    break; 
                    
                    case 0x38:                          //Pide venta sin impresión
                        set_imagen(1,5);	            //Pasa a preset de venta
                        no_imprime = 1;
                        flujo_LCD = 4;                    
                    break;  
                    
                    case 0x7E:						   //ir a menu de selección de pos
					    set_imagen(1,46);	
                        flujo_LCD = 0;     
                    break;
                        
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 		 	
        break;
        
        case 11:		
		if(read_memory_ibutton(1,1)>=6){
			CyDelay(10);
			if(read_memory_ibutton(1,0x21)>=6){
		        if(touch_present(1)==1){
		            LCD_1_PutChar(0x33);
		            if(touch_write(1,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD1.id[z]=touch_read_byte(1);
		                }				
						set_imagen(1,19);  //Id correctamente reconocida
						Buffer_LCD1.preset|=0x04;
						CyDelay(500);
						set_imagen(1,14); 
						teclas1=0;
						Buffer_LCD1.km[0]=0; //Pedir Kilometraje
						flujo_LCD = 12;
					}
				}	
			}
		}
	    if(LCD_1_GetRxBufferSize()==8){
	        if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
	            switch(LCD_1_rxBuffer[3]){
	                case 0x3B:
	                 set_imagen(1,73); //Kilometraje
	                 flujo_LCD = 2;	                 
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_1_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=5){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;                    
                        Buffer_LCD1.km[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas1++;                    
                        Buffer_LCD1.km[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas1==0){
                        set_imagen(1,5);
						flujo_LCD = 4;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){				//Enter
                    if(teclas1>=1 && Buffer_LCD1.km[1]!=0x30){                       
                        set_imagen(1,5);
                        Buffer_LCD1.km[0]=teclas1;   
                        flujo_LCD = 4;                                           
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print1[1], grado1,0,a.dir);  //Imprime recibo sin copia
			if(copia_recibo[1]==0){
				set_imagen(1,12);
				CyDelay(500);
                Buffer_LCD1.preset = 0x00;
                set_imagen(1,46);
                flujo_LCD = 0;
			}
			else{
				set_imagen(1,49);
				teclas1=0;				
				flujo_LCD = 33;					
			}
        break;  
        
        case 14:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x58:								 	 //Configurar Fecha y Hora	 
                        teclas1=0;                            	  
                        set_imagen(1,62); 
					    if(leer_hora()==1){
						    Hora_LCD(1);
					    } 
					    if(leer_fecha()==1){
						    Fecha_LCD(1);						
					    }		
                        flujo_LCD = 16;  
                    break;
                    
                    case 0x5D:  							     //Cambiar Precio
                                        
                      set_imagen(1,92);
                      Precio_LCD(1,0x00,0xD6,429);               // primer producto   segundo campo en el lcd
					  Precio_LCD(1,0x00,0x89,423);               // segundo producto 			  
                      Precio_LCD(1,0x01,0x24,435);               // segundo producto 
                      Precio_LCD(1,0x01,0x78,1000);              //cuarto producto 
                      flujo_LCD = 19; 
                      
                    break;
                    
                   case 0x5A:  	
                        set_imagen(1,96);                       //Datos de Recibo
					    Nombre_LCD(1);
					    direccion_LCD(1);
					    telefono_LCD(1);
					    nit_LCD(1);
					    lema1_LCD(1);
					    lema2_LCD(1);							 
                        flujo_LCD = 18;                    
                    break;
                    
                    case 0x5B:         
                        set_imagen(1,47); 
					    teclas1=0;               			     //Cambiar pasword                     
                        flujo_LCD = 17;                    
                    break;   					
					
                    case 0x96:     
                        set_imagen(1,95); 
					    teclas1=0;                   			 //Configurar Surtidor                   
                        flujo_LCD = 27;                    
                    break; 
					
                    case 0x65: 
                        set_imagen(1,77);                 	     //Test                     
                        flujo_LCD = 20;
                    break;
					
                    case 0x93:
                        set_imagen(1,110);                       //Grabar Ibutton
					    if(read_memory_ibutton(1,1)>=6){
						    CyDelay(10);
						    Placa_LCD(1);
						    if(read_memory_ibutton(1,0x21)>=6){						
							    Cuenta_LCD(1);
						    }
					    }			                  
                        flujo_LCD = 35;                                                    
                    break;
                    
                    case 0x70:         
                        set_imagen(1,137);                     //Tipo de impresora 
                        flujo_LCD = 41;                         				
                    break;
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,46);
                      flujo_LCD=0;     
                    break;
                    
                    case 0x94:									//Volver a menu de opciones
					  set_imagen(1,46);
                      flujo_LCD=0;     
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x7E){
                    set_imagen(1,49);
                    flujo_LCD = 0;
                }else if(LCD_1_rxBuffer[3] == 0x94){
                    set_imagen(1,99);
                    flujo_LCD = 1;
                }
                if(teclas1<=7){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,'*',teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,'*',teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						set_imagen(1,57);
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD = 0;
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas1>=1){
						y=0;
						for(x=0;x<teclas1;x++){						
							if(rventa.password[x+1]==(Buffer_LCD1.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(1,38);
							CyDelay(500);	                        							
						    set_imagen(1,112);							
                            flujo_LCD = 14;
						}
						else{
							y=0;
							for(x=0;x<teclas1;x++){						
								if(puk[x]==Buffer_LCD1.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(1,38);
								CyDelay(500);																							    set_imagen(1,93);                     																
								set_imagen(1,112);										
                                flujo_LCD=14;
							}	
							else{		
								set_imagen(1,39);								
								CyDelay(500);
                                set_imagen(1,46);
                                flujo_LCD = 0;
							}		
						}
                    }					
                }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
            } 
        }
        break;
        
        case 17:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x94){
                        set_imagen(1,112);
                        flujo_LCD = 14; 
                }
                if(teclas1<=7){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						set_imagen(1,85);               //Error de operacion
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD = 0;
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			   
					if(teclas1>=6){
						set_imagen(1,57);		                    						
						rventa.password[0]=teclas1;
						for(x=0;x<teclas1;x++){
							rventa.password[x+1]=(Buffer_LCD1.password[x]-0x30);
						}
						if(write_eeprom(96,rventa.password)==0){							//Guarda el nombre en la eeprom
							set_imagen(1,85);               //Error de operacion
                            CyDelay(500);
                            set_imagen(1,46);
                            flujo_LCD = 0;
						}
                        set_imagen(1,38);
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD=0;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }   
        break;
        
        case 16:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
					case 0x3F:								//Hora
                        set_imagen(1,64);
						teclas1=0;
						comas1=0;												
						write_LCD(1,'a',6);						
						write_LCD(1,'m',7);
                        flujo_LCD = 21;
					break;
					
					case 0x3E:								//Fecha
						set_imagen(1,63);
                        teclas1=0;
						comas1=0;
						flujo_LCD = 22;
											
					break;
						
                    case 0x7E:								//ir a menu
					  set_imagen(1,46);	
                      flujo_LCD = 0;     
                    break;	
                    
                    case 0x94:									//Volver a menu de opciones
					  set_imagen(1,112);
                      flujo_LCD = 14;     
                    break;
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 18:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x76:								 	 //Cambiar Nombre	             
                      set_imagen(1,48);
					  teclado=0;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD = 24;
                    break;
					
                    case 0x82:								 	 //Cambiar Lema1	
                      set_imagen(1,121);                       
					  teclado=2;
                      teclas1=0;                            	 //Inicia el contador de teclas  
                      nombreproducto = 0;
                      flujo_LCD = 24;  					
                    break;
					
                    case 0x83:								 	 //Cambiar Lema2	             
                      set_imagen(1,122);
					  teclado=3;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD = 24;
                    break;					
                    
                    case 0x77:  								//Cambiar Direccion                      
					  set_imagen(1,120);
                      teclado=1;                      
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD = 24;
                    break;
                    
                    case 0x78:  								//Cambiar telefono
                      set_imagen(1,101);
					  teclado=1;                      
                      teclas1=0;                            	 //Inicia el contador de teclas
                      flujo_LCD = 23;	
                    break;
                    
                    case 0x7A:                         			//Cambiar NIT                     
                      set_imagen(1,50);
					  teclado=0;					 
                      teclas1=0;                            	 //Inicia el contador de teclas
                      flujo_LCD = 23;
                    break;   

                    case 0x95:                         			//Cambiar Bandera 	
                        set_imagen(1,102);
	                    flujo_LCD = 32;
						
                    break; 
					
                    case 0x7E:						//ir a menu
					  set_imagen(1,46);            //Volver a otras opciones Maicol
                      flujo_LCD = 0;     
                    break;
                    
                    case 0x94:						//ir a menu
					  set_imagen(1,112);            //Volver a otras opciones Maicol
                      flujo_LCD = 14;     
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }	
        break;
        
        case 19:
        if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:		
                      set_imagen(1,6);   
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto1;	
					  write_LCD(1,'$',teclas1);							 	 //Producto 1	                               
                      flujo_LCD = 26; 
                    break;
                    
                    case 0x80:  	
                      set_imagen(1,6);		
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto2;
					  write_LCD(1,'$',teclas1);						 //Producto 2
                      flujo_LCD = 26;              
                    break;
                    
                    case 0x81:  
                      set_imagen(1,6);			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto3;
					  write_LCD(1,'$',teclas1);						//Producto 3
                      flujo_LCD = 26;	
                    break;
                    
                    case 0x82:  	
                      set_imagen(1,6); 			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto4;
					  write_LCD(1,'$',teclas1);					//Otro producto
                      flujo_LCD=26;		
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(1,46);            //Otras Opciones
                      flujo_LCD = 0;     
                    break;
                    
                    case 0x94:
                        set_imagen(1,112);
                        flujo_LCD = 14;
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;
        
        case 20:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(LCD_1_rxBuffer[3]){
	                case 0x62:								 	 //Comunicacion Surtidores
						set_imagen(1,57);
						CyDelay(100);
						if(get_estado(a.dir)==6){	
							if(get_estado(b.dir)==6){
								set_imagen(1,74);
								CyDelay(500);				  								
							}
							else{
								set_imagen(1,85);               //Error de operacion
                                CyDelay(500);
                                set_imagen(1,46);
                                flujo_LCD = 0;
							}
						}	
						else{
							set_imagen(1,85);                  //Error de operacion
                            CyDelay(500);
                            set_imagen(1,46);
                            flujo_LCD = 0;
						}
	                break;
	                
	                case 0x63:  								 //Comunicacion Impresora
						set_imagen(1,55);
                        if(tipo_imp[1] == 1){                    //Impresora Kiosko
						    for(x=0;x<=16;x++){
			        		    write_psoc1(1,test[x]);			
						    }
					        write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(2,test[x]);			
						    }
					        write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);						
                        }
                        if (tipo_imp[1] == 2){                    //Impresora panel
                            for(x=0;x<=16;x++){
			        		    write_psoc1(3,test[x]);			
						    }
					        write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(4,test[x]);			
						    }
					        write_psoc1(4,10);
						    write_psoc1(4,10);
						    write_psoc1(4,10);
					    	write_psoc1(4,10);
                        }    
						set_imagen(1,107);
						flujo_LCD = 37;
	                break;
	                
	                case 0x64:  								 //Comunicacion Ibutton
					if(touch_present(1)==1){
						LCD_1_PutChar(0x33);
						if(touch_write(1,0x33)){
							for(z=0;z<=7;z++){
							    Buffer_LCD1.id[z]=touch_read_byte(1);
							}
							crc_total=0;
							for(z=0;z<7;z++){
							    crc_total=crc_check(crc_total,Buffer_LCD1.id[z]);
							}	
							if(crc_total==Buffer_LCD1.id[7]){
								set_imagen(1,75);
								CyDelay(1000);				  							   	
							}
							else{
								set_imagen(1,85);     //Error de operacion
                                CyDelay(500);
                                set_imagen(1,46);
                                flujo_LCD = 0;
							}
						}
					}
					else{
						set_imagen(1,85);             //Error de operacion
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD = 0;
					}
	                break;
	               										
	                case 0x7E:									//ir a menu
					  set_imagen(1,46);
	                  flujo_LCD = 0;     
	                break;	
                    
                    case 0x94:									//ir a menu
					  set_imagen(1,112);
	                  flujo_LCD = 14;     
	                break;
				}
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }                    
        break;
        
        case 21:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(teclas1){
	                case 0:										//Decenas de la Hora
	                    if(LCD_1_rxBuffer[3]<=1)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[1]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        rventa.hora[1]=0;
	                        write_LCD(1,0x30,teclas1);
	                    }                     
	                break;
						
	                case 1:											//Unidades de la Hora
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[1]|=LCD_1_rxBuffer[3];						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
	                    }                     
	                break;
						
	                case 2:											//Decenas de la Minutos					
	                    if(LCD_1_rxBuffer[3]<=5)					
						{
							teclas1++;					
							write_LCD(1,':',teclas1);							
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[0]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
							teclas1++;					
							write_LCD(1,':',teclas1);							
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
							rventa.hora[0]=0;
	                    } 
	                break;
						
	                case 4:											//Unidades de la Minutos					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[0]|=LCD_1_rxBuffer[3];						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
	                    } 
	                break;
				}
                if(LCD_1_rxBuffer[3]==0x40){            		//Comando de Am/Pm
                    if(comas1==0){
                        comas1=1;
                        write_LCD(1,'p',6);
                    }
					else{
                        comas1=0;
                        write_LCD(1,'a',6);						
					}
					write_LCD(1,'m',7);
                } 				
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior						                        
						set_imagen(1,112);
                        flujo_LCD = 14;						
						
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==5){
						rventa.hora[1]|=((comas1|2)<<5);
						if(write_hora()==1){
	                        set_imagen(1,57);
                            CyDelay(500);
	                        set_imagen(1,112);
							flujo_LCD = 14; 
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }        
        break;
        
        case 22:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(teclas1){
	                case 0:										//Decenas de dia
	                    if(LCD_1_rxBuffer[3]<=3)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    teclas1++;							
		                    rventa.fecha[0]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        rventa.fecha[0]=0;
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    }                     
	                break;
						
	                case 1:											//Unidades de dia
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[0]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    }                     
	                break;
						
	                case 2:											//Decenas de mes					
	                    if(LCD_1_rxBuffer[3]<=1)					
						{					
							write_LCD(1,'/',teclas1);
							teclas1++;							
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[1]=LCD_1_rxBuffer[3]<<4;
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(1,'/',teclas1);
							teclas1++;							
	                        write_LCD(1,0x30,teclas1);
							rventa.fecha[1]=0;
	                        teclas1++;							
	                    } 
	                break;
						
	                case 4:											//Unidades de mes					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[1]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    } 
	                break;
						
	                case 5:											//Decenas de año					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{					
							write_LCD(1,'/',teclas1);
							teclas1++;							
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[2]=LCD_1_rxBuffer[3]<<4;
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(1,'/',teclas1);
							teclas1++;							
	                        write_LCD(1,0x30,teclas1);
							rventa.fecha[2]=0;
	                        teclas1++;							
	                    } 
	                break;
						
	                case 7:											//Unidades de año					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[2]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    } 
	                break;						
				}				
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior						
                        set_imagen(1,112);
						flujo_LCD = 14;						
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==8){
						if(write_fecha()==1){
                            set_imagen(1,57);
                            CyDelay(500);
	                        set_imagen(1,112);
							flujo_LCD = 14;                         
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			
		break;
        
        case 23:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x7E){
                    set_imagen(1,46);
                    flujo_LCD = 0;
                }
                if(teclas1<=12){
                    if(LCD_1_rxBuffer[3]<=9){ 
                        teclas1++;						
						switch(teclado){
							case 0:
								rventa.nit[teclas1]=LCD_1_rxBuffer[3]+0x30;
							break;
							
							case 1:
								rventa.telefono[teclas1]=LCD_1_rxBuffer[3]+0x30;
							break;
						}
                        writemini_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1); 						
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            //Comando de 0    
                        teclas1++;						
						switch(teclado){
							case 0:
								rventa.nit[teclas1]=0x30;
							break;
							
							case 1:
								rventa.telefono[teclas1]=0x30;
							break;
						}						
                        writemini_LCD(1,0x30,teclas1); 						
                    }  
                    if(LCD_1_rxBuffer[3]==0x51){            //Comando de Coma
                        if(teclas1>=1){ 
                        	teclas1++;							
							switch(teclado){
								case 0:
									rventa.nit[teclas1]='.';
								break;
								
								case 1:
									rventa.telefono[teclas1]='.';
								break;
							}							
                            writemini_LCD(1,'.',teclas1);
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){				//Cancel
                    if(teclas1==0){
                        flujo_LCD = 14;						
						set_imagen(1,112);						
						leer_eeprom(47,12);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.telefono[x]=buffer_i2c[x];
						}
						leer_eeprom(32,15);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nit[x]=buffer_i2c[x];
						}						
                    }
                    else{					
                        writemini_LCD(1,0x20,teclas1);
                        teclas1--;							
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){				//Enter
                    if(teclas1>=0){
                        flujo_LCD=14;
    					set_imagen(1,112);
	
						switch(teclado){
							case 0:
		                        rventa.nit[0]=teclas1;  
								if(write_eeprom(32,rventa.nit)==0){
										set_imagen(1,85);
										flujo_LCD=100;
								}
							break;	
							case 1:
		                        rventa.telefono[0]=teclas1;  							
								if(write_eeprom(47,rventa.telefono)==0){
										set_imagen(1,85);
                                        CyDelay(500);
                                        set_imagen(1,46);
										flujo_LCD=0;
								}
							break;								
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 24:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=23){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
						switch(teclado){
							case 0:                        		
                                if(nombreproducto == 1){
                                    producto1n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.nombre[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;
							
							case 1:                       			
                                if(nombreproducto == 1){
                                    producto2n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.direccion[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;

							case 2:                       			
                                if(nombreproducto == 1){
                                    producto3n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema1[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;								

							case 3:                       			
                                if(nombreproducto == 1){
                                    producto4n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema2[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;									
							break;								
									
						}
                        writemini_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);                        
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]=0x30;
							break;
							
							case 1:
                       			rventa.direccion[teclas1]=0x30;									
							break;
								
							case 2:
                       			rventa.lema1[teclas1]=0x30;									
							break;								

							case 3:
                       			rventa.lema2[teclas1]=0x30;									
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=0x30;									
							break;								
						}						
                        writemini_LCD(1,0x30,teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x67){                                        //Comando de #
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='#';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='#';									
							break;
								
							case 2:
                       			rventa.lema1[teclas1]='#';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='#';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='#';									
							break;								
						}						
                        writemini_LCD(1,'#',teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='-';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='-';									
							break;

							case 2:
                       			rventa.lema1[teclas1]='-';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='-';									
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='-';									
							break;								
						}						
                        writemini_LCD(1,'-',teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x66){                                        //Comando de @
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='@';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='@';									
							break;

							case 2:
                       			rventa.lema1[teclas1]='@';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='@';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='@';									
							break;								
						}						
                        writemini_LCD(1,'@',teclas1);                        
                    } 					
                    if(LCD_1_rxBuffer[3]==0x69){                                        //Comando de .
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='.';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='.';									
							break;
							
							case 2:
                       			rventa.lema1[teclas1]='.';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='.';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='.';									
							break;								
						}						
                        writemini_LCD(1,'.',teclas1);                        
                    }	
                    if(LCD_1_rxBuffer[3]==0x68){                                        //Comando de ESPACIO
                        teclas1++;                        
						switch(teclado){
							case 0:
		                        if(nombreproducto == 1){
                                    producto1n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.nombre[teclas1]=' ';
                                }
                            break;
							
							case 1:
                                if(nombreproducto == 1){
                                    producto2n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.direccion[teclas1]=' ';
                                }
                       												
							break;

							case 2:
                                if(nombreproducto == 1){
                                    producto3n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema1[teclas1]=' ';
                                }
                       												
							break;								

							case 3:
                                if(nombreproducto == 1){
                                    producto4n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema2[teclas1]=' ';
                                }
                       												
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=' ';									
							break;								
						}						
                        writemini_LCD(1,' ',teclas1);                        
                    }					
                    if(LCD_1_rxBuffer[3]>=0x1B && LCD_1_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_1_rxBuffer[3]==letras[x]){
                                teclas1++;                            
								switch(teclado){
									case 0:
		                        		if(nombreproducto == 1){
                                            producto1n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.nombre[teclas1]=x+0x41;
                                        }
                                    break;
									
									case 1:                       			
                                        if(nombreproducto == 1){
                                         producto2n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.direccion[teclas1]=x+0x41;
                                        }
							        break;

									case 2:                       			
                                        if(nombreproducto == 1){
                                            producto3n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.lema1[teclas1]=x+0x41;
                                        }
							        break;								

									case 3:                       			
                                        if(nombreproducto == 1){
                                            producto4n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.lema2[teclas1]=x+0x41;
                                        }
							        break;		
										
										
									case 4:
		                       			Buffer_LCD1.valor[teclas1]=x+0x41;									
									break;										
								}                                                                                     		                                						
                                writemini_LCD(1,(x+0x41),teclas1);                            
                            }
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){                                        			//Borrar - Cancelar
                    if(teclas1==0){                                                 			//Si no tiene nada pasa a pedir impresion                        						
                        set_imagen(1,112);						
						leer_eeprom(64,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.direccion[x]=buffer_i2c[x];
						}
						leer_eeprom(128,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema1[x]=buffer_i2c[x];
						}	
						leer_eeprom(160,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema2[x]=buffer_i2c[x];
						}
						leer_eeprom(0,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nombre[x]=buffer_i2c[x];
						}
                        flujo_LCD = 14;
                    }
                    else{
                        writemini_LCD(1,0x20,teclas1);                        
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){                                        			//Enter para guardar
                    if(teclas1>=0){
                        set_imagen(1,57);
                        CyDelay(500);
                        set_imagen(1,112);						
                        flujo_LCD = 14;
						switch(teclado){
							case 0:
                                if(nombreproducto == 1){
                                    producto1n[0]=teclas1;                                    
                                    write_eeprom(1130,producto1n);	                //Guarda el nombre en la eeprom
									    
								    }
                                
                                if(nombreproducto == 0){
                                    rventa.nombre[0]=teclas1;
                                    if(write_eeprom(0,rventa.nombre)==0){							//Guarda el nombre en la eeprom
									set_imagen(1,85);
                                    CyDelay(500);
                                    set_imagen(1,46);
									flujo_LCD = 0;
								}
                                }
								
							break;
								
							case 1:
                                if(nombreproducto == 1){
                                    producto2n[0]=teclas1;                                    
                                    write_eeprom(1141,producto2n);	//Guarda el nombre en la eeprom
                                }
								if(nombreproducto == 0){    
								rventa.direccion[0]=teclas1;
								if(write_eeprom(64,rventa.direccion)==0){						//Guarda la direccion en la eeprom
									set_imagen(1,85);
                                    CyDelay(500);
                                    set_imagen(1,46);
									flujo_LCD = 0;
								}
                                }
							break;	
								
							case 2:
                                if(nombreproducto == 1){
                                    producto3n[0]=teclas1;                                    
                                    write_eeprom(1152,producto3n);	     //Guarda el nombre en la eeprom
                                }
                                if(nombreproducto == 0){ 
								rventa.lema1[0]=teclas1;
								if(write_eeprom(128,rventa.lema1)==0){   //Guarda el lema 1 en la eeprom
									set_imagen(1,85);
                                    CyDelay(500);
                                    set_imagen(1,46);
									flujo_LCD = 0;
								}
                                }
							break;	
								
							case 3:
                                if(nombreproducto == 1){
                                    producto4n[0]=teclas1;                                    
                                    write_eeprom(1163,producto4n);	//Guarda el nombre en la eeprom
                                }
                                if(nombreproducto == 0){
								rventa.lema2[0]=teclas1;
								if(write_eeprom(160,rventa.lema2)==0){							//Guarda el lema 2 en la eeprom
									set_imagen(1,85);
                                    CyDelay(500);
                                    set_imagen(1,46);
									flujo_LCD = 0;
								}
                                }
							break;	
								
							case 4:
								Buffer_LCD1.valor[0]=teclas1;
								set_imagen(1,57);
								flujo_LCD = 36;
							break;									
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
        
        break;
        
        case 26:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                               
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior                        
						set_imagen(1,112);
						flujo_LCD = 14;						
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=4){
                        flujo_LCD = 6;                         
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;					
						if(cambiar_precio(a.dir)!=0){ 	
							if(rventa.producto==producto1){
								write_eeprom(423,Buffer_LCD1.valor);	//429						
							}
                            if(rventa.producto==producto3){
								write_eeprom(435,Buffer_LCD1.valor); //423							
							}
							if(rventa.producto==producto2){
								write_eeprom(429,Buffer_LCD1.valor);	//435						
							}
                            
							
                            if(rventa.producto==producto4){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(1,60);	
							CyDelay(500);														
							set_imagen(1,112);
							flujo_LCD = 14;							
						}
						else{
                            set_imagen(1,85);   //Error de operacion
                            CyDelay(500);
                            set_imagen(1,46);
                            flujo_LCD = 0;
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 27:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                      
                      set_imagen(1,88); 
					  Grado_LCD(1,0x00,0x73,451); // primer producto 449
					  Grado_LCD(1,0x00,0xCF,453); // segundo producto			  
                      Grado_LCD(1,0x01,0x29,449); // tercer producto
                      Grado_LCD(1,0x01,0x7B,1006); //cuarto producto
                      flujo_LCD = 28;   
                    break;
                    
                    case 0x80:  								 //Version de Digitos                      
                      set_imagen(1,6); 	
					  teclas1=0;
                      flujo_LCD = 29;
                    break;
                    
                    case 0x81:  								 //PPU                      
                      set_imagen(1,109); 	
                      flujo_LCD = 30;	
                    break;
                    
                    case 0x82:  								 //Nombre de productos                      
                      set_imagen(1,138); 	
                      flujo_LCD = 42;	
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,46);
                      flujo_LCD = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(1,112);
                      flujo_LCD = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;
        
        case 28:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Producto1	                                                         
                      set_imagen(1,6); 
					  teclas1=0; 
					  rventa.producto=1;
                      flujo_LCD = 31;
                    break;
                    
                    case 0x80:  								 //Producto2                      
                      set_imagen(1,6); 	 
					  teclas1=0;
					  rventa.producto=2;
                      flujo_LCD = 31;
                    break;
                    
                    case 0x81:  								 //Producto3                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=3;
                      flujo_LCD = 31;
                    break;
                    
                    case 0x82:  								 //Producto4                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=4;
                      flujo_LCD = 31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,46);
                      flujo_LCD = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(1,112);
                      flujo_LCD = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;	
        
        case 29:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3]==0x94){
                    set_imagen(1,112);
                    flujo_LCD = 14;
                }
                if(teclas1<=4){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
					
                    if(LCD_1_rxBuffer[3]==0x51){            	//Comando de coma
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=',';
                        write_LCD(1,',',teclas1);
                    }					
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior                        												
						set_imagen(1,112);
                        flujo_LCD=14;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==5 && (Buffer_LCD1.valor[1]==0x35||Buffer_LCD1.valor[1]==0x36||Buffer_LCD1.valor[1]==0x37)){                        
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;
						if(write_eeprom(442,Buffer_LCD1.valor)==1){	
							versurt=(Buffer_LCD1.valor[1]&0x0F);
						}
						if(write_eeprom(602,Buffer_LCD1.valor)==1){	
							decimalD=(Buffer_LCD1.valor[3]&0x0F);
							decimalV=(Buffer_LCD1.valor[5]&0x0F);
							set_imagen(1,60);
							CyDelay(500);														
							set_imagen(1,112);
                            flujo_LCD = 14;											
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 30:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x8F:								 	 //PPUx1                              
						rventa.configppu[0]=0;
						rventa.configppu[1]=0;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=0;
	                        set_imagen(1,60);	
							CyDelay(500);							
						    set_imagen(1,112);																				
                            flujo_LCD = 14;
						}
						else{
							set_imagen(1,85);        //Error de operacion
                            CyDelay(500);
                            set_imagen(1,46);
                            flujo_LCD = 0;
						}
                    break;
                    
                    case 0x90:  								 //PPUx10
						rventa.configppu[0]=1;
						rventa.configppu[1]=1;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=1;
	                        set_imagen(1,60);	
							CyDelay(500);							
							flujo_LCD=14;
                            set_imagen(1,112);																				
						}
						else{
							set_imagen(1,85);   //Error de operacion
                            CyDelay(500);
                            set_imagen(1,46);
                            flujo_LCD = 0;
						}
                    break;               
                   										
                    case 0x7E:									 
					  set_imagen(1,46);             //ir a menu principal
                      flujo_LCD = 0;         
                    break;
                    
                    case 0x94:									
					  set_imagen(1,112);
                      flujo_LCD = 14;     //Volver a otras opciones
                    break;	
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			
		break;
        
        case 31:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=0){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,0);
                        flujo_LCD=0;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==1){                        
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;
						if(rventa.producto==1){
							producto3=Buffer_LCD1.valor[1];
							write_eeprom(449,Buffer_LCD1.valor);							
						}
						if(rventa.producto==2){
							producto1=Buffer_LCD1.valor[1];
							write_eeprom(451,Buffer_LCD1.valor);							
						}
						if(rventa.producto==3){
							producto2=Buffer_LCD1.valor[1];
							write_eeprom(453,Buffer_LCD1.valor);							
						}
                        if(rventa.producto==4){
							producto4=Buffer_LCD1.valor[1];
							write_eeprom(1006,Buffer_LCD1.valor);							
						}
						CyDelay(500);						
					    set_imagen(1,112);
                        flujo_LCD=14;												
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 32:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    
                    case 0x89:                         			//Texaco                     
					  bandera[0]=1;
					  bandera[1]=1;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);
						flujo_LCD = 14;						  
					  }                      
                    break;
                    
                    case 0x86:  								//Terpel
					  bandera[0]=1;
					  bandera[1]=2;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);				
                        flujo_LCD = 14;
					  }
                    break;
                    
                    case 0xA1:                         			//Exito               
					  bandera[0]=1;
					  bandera[1]=3;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  	
					    set_imagen(1,112);
						flujo_LCD=14;					  							  
					  }
                    break;
                    
                     case 0x88:  								//ESSO
					  bandera[0]=1;
					  bandera[1]=4;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  	
					    set_imagen(1,112);
                        flujo_LCD=14;					  													  
					  }
                    break;
                    
                    case 0x8A:                         			//Mobil                    
					  bandera[0]=1;
					  bandera[1]=5;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
					    set_imagen(1,112);
                        flujo_LCD=14;												  
					  }
                    break;
                    
                    case 0x99:                         			//Zeus                
					  bandera[0]=1;
					  bandera[1]=6;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
                        set_imagen(1,112);
					  	flujo_LCD=14;					  																			  
					  }
                    break; 	
                    
                     case 0x97:                         			//Cencosur               
					  bandera[0]=1;
					  bandera[1]=7;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  	
						set_imagen(1,112);
						flujo_LCD=14;						  
					  }
                    break;
                    
                    case 0xA2:                         			//Mineroil               
					  bandera[0]=1;
					  bandera[1]=8;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);
						flujo_LCD=14;						  
					  }
                    break;
                    
                    case 0x87:                         			//Gulf                   
					  bandera[0]=1;
					  bandera[1]=9;
					  write_eeprom(455,bandera);                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  					  
						set_imagen(1,112);
						flujo_LCD=14;						  
					  
                    break; 
                        
                    case 0xA0:                         			//Ecospetrol                
					  bandera[0]=1;
					  bandera[1]=10;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);
						flujo_LCD=14;						  
					  }
                    break;
                    
                     case 0x98:                         			//Petromil                 
					  bandera[0]=1;
					  bandera[1]=11;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  	
						set_imagen(1,112);
                        flujo_LCD=14;					  													  
					  }
                    break; 
                    
                     case 0xA3:                         			//Plus               
					  bandera[0]=1;
					  bandera[1]=12;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);
						flujo_LCD=14;						  
					  }
                    break;	
                    
                    case 0x8B:                         			//Petrobras                    
					  bandera[0]=1;
					  bandera[1]=13;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
						set_imagen(1,112);
                        flujo_LCD=14;												  
					  }
                    break; 
                    
                     case 0x85:                         			//Brio                    
					  bandera[0]=1;
					  bandera[1]=14;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  					  	
						set_imagen(1,112);											  
                        flujo_LCD=14;	
					  }
                    break;
                    
                    case 0x84:								 	//Biomax	
					  bandera[0]=1;
					  bandera[1]=15;                        
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  						  	
					    set_imagen(1,112);
                        flujo_LCD = 14;
												  
					  }
                    break;
                                                                                                     
                    case 0x94:                         			//Sin Bandera               
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);					  	
						set_imagen(1,112);
						flujo_LCD=14;						  
					  }
                    break;					
					
                    case 0x7E:									//ir a menu
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	                         
							set_imagen(1,112);
						flujo_LCD=14;						  
					  }	
					  else{
						set_imagen(1,46);
	                	flujo_LCD = 0;
					  }
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
            
		break;	
        
        case 33:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x0A)
                {
                    for(z=1;z<=(n_copias[0]&0x0F);z++){
                        imprimir(print1[1],grado1,1,a.dir);   //Impresión de número de copias por configuración
                        CyDelay(500);
                    }
                    set_imagen(1,12);					
				    CyDelay(500);
                    Buffer_LCD1.preset = 0x00;
                    set_imagen(1,46);
                    flujo_LCD = 0;
                }
                else{
                    set_imagen(1,12);
					CyDelay(500);               //Confirman NO, muestra imagen y sale
                    Buffer_LCD1.preset = 0x00;
                    set_imagen(1,46);
                    flujo_LCD = 0;
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }	
		
        
		break;
        
        case 34:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x38:								//No Corte                      
					    set_imagen(1,46);	
                        flujo_LCD=0;     
                    break; 

                    case 0x39:								//Si Corte                                         
                        set_imagen(1,59);
					    imprimir_corte(print1[1]);	
					    write_psoc1(print1[1],10);
					    write_psoc1(print1[1],10);
					    write_psoc1(print1[1],10);
					    write_psoc1(print1[1],0x1D);
					    write_psoc1(print1[1],0x56);
					    write_psoc1(print1[1],0x31);
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD=0;					   						
                    break;
					
                    case 0x94:								//cambiar clave corte
					    set_imagen(1,47);
					    teclas1=0;
                        flujo_LCD = 39;     
                    break;				
					
                    case 0x7E:								//ir a menu
					    set_imagen(1,46);	
                        flujo_LCD = 0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
        
        case 35:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x91:								//Placa                          
					    set_imagen(1,10);	
					    pos_ibutton = 1;                    //Grabado de ibutton
					    teclado=4;
					    teclas1=0;	
                        flujo_LCD = 24; 
                    break; 

                    case 0x92:								//Cuenta                         
					    set_imagen(1,111);
					    pos_ibutton=0x21;
					    teclado=4;
					    teclas1=0;					
                        flujo_LCD = 24;  
                    break;					
					
                    case 0x7E:								//ir a menu
					    set_imagen(1,46);	
                        flujo_LCD = 0;     
                    break;	
                    case 0x94:						        //ir a menu anterior
					    set_imagen(1,112);
	                    flujo_LCD = 14;     
	                break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
        
         case 36:  
			if(touch_write_memoria(1,Buffer_LCD1.valor[0],pos_ibutton,Buffer_LCD1.valor)==1){
				set_imagen(1,110);
				flujo_LCD = 35;
				if(read_memory_ibutton(1,1)>=6){
					CyDelay(10);
					Placa_LCD(1);
					if(read_memory_ibutton(1,0x21)>=6){						
						Cuenta_LCD(1);                          //Caso para enviar datos al ibutton
					}
				}	
				else{
					set_imagen(1,85);   //Error de operacion
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD = 0;
				}	
			} 
			else{
				set_imagen(1,85);      //Error de operacion
                CyDelay(500);
                set_imagen(1,46);
                flujo_LCD = 0;
			}
        break;
            
        case 37:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x39:								//SI
					  if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=4;
                        }
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,104); 
						  	CyDelay(500);
						  	set_imagen(1,112);							
                            flujo_LCD = 14;						  									
						}	
					  }
                    break; 

                    case 0x38:								//NO
                      flujo_LCD = 38;     
					  set_imagen(1,103);					
                    break;					
					
                    case 0x7E:								//ir a menu
                      if(copia_recibo[1]==1){
						copia_recibo[1]=0;
					  }	
					  else{
						copia_recibo[1]=1;
					  }
					  write_eeprom(986,copia_recibo);
					  set_imagen(1,60);
					  CyDelay(500);
					  set_imagen(1,46);	
                      flujo_LCD = 0;     
                    break;	
                    case 0x94:						        //ir a menu anterior
					    set_imagen(1,112);
	                    flujo_LCD = 14;     
	                break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
        
        case 38:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x8C:								//La impresora 1
                        if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=2;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=4;
					        print2[0]=1;
					        print2[1]=4;
                        }
                        
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,106); 
						  	CyDelay(500);
						  	flujo_LCD = 14;						  	
							set_imagen(1,112);							
						}	
					  }
                    break; 

                    case 0x8D:								//La impresora 2
                    
                        if(tipo_imp[1] ==1){   //Kiosko
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=1;					
                        }
                        if (tipo_imp[1] ==2){ //Panel
                            print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=3;
                        }
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,105); 
						  	CyDelay(500);						    
							set_imagen(1,112);							
					        flujo_LCD = 14;
						}	
					  }					
                    break;
					
                    case 0x8E:								//Ninguna
					  print1[0]=1;
					  print1[1]=0;
					  print2[0]=1;
					  print2[1]=0;					
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,108); 
						  	CyDelay(500);						  							  	
							set_imagen(1,112);
                            flujo_LCD = 14;
						}	
					  }				
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(1,46);	
                      flujo_LCD = 0;     
                    break;	
                    
                    case 0x94:						        //ir a menu anterior
					    set_imagen(1,112);
	                    flujo_LCD = 14;     
	                break;
                    
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
        
        case 39: 
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=3){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,'*',teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,'*',teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						set_imagen(1,85);                       //Error de operacion
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD = 0;
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas1>=1){
						y=0;
						for(x=0;x<teclas1;x++){						
							if(pasword_corte[x+1]==Buffer_LCD1.password[x]){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==pasword_corte[0]){								
	                        set_imagen(1,38);           //Verificacion de password corte
							CyDelay(500);
	                        flujo_LCD = 34;
							set_imagen(1,113);							
						}
						else{
							y=0;
							for(x=0;x<teclas1;x++){						
								if(puk[x]==Buffer_LCD1.password[x]){
									y++;
								}
							}
							if(y==4){								
								set_imagen(1,38);
								CyDelay(500);
								flujo_LCD = 34;
								set_imagen(1,113);							
							}	
							else{		
								set_imagen(1,39); //Error de password								   
                                CyDelay(500);
                                set_imagen(1,46);
                                flujo_LCD = 0;
							}		
						}
                    }
					else{
                        set_imagen(1,39); //Error de password								   
                        CyDelay(700);
                        set_imagen(1,46);
                        flujo_LCD = 0;					
					}					
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
        break;
        
        case 41:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){ 
                    
                    case 0xBC:								//Impresora Kiosko
                        tipo_imp[0] = 1;
                        tipo_imp[1] = 1;                                                
                        write_eeprom(1012,tipo_imp);
                        set_imagen(1,57);
                        CyDelay(700);
                        set_imagen(1,112);					                            	
                        flujo_LCD = 14;
                    break; 

                    case 0xBD:								//Impresora panel                        
                        tipo_imp[0] = 1;
                        tipo_imp[1] = 2; 
                        write_eeprom(1012,tipo_imp);         
                        set_imagen(1,57);
                        CyDelay(700);
                        set_imagen(1,112);                                                               	
                        flujo_LCD = 14;
                    break;
                        
                    case 0xBE:
                        set_imagen(1,6);  // Configuración número de copias
                        a_copias = 1;
                        teclas1=0;       
                        comas1=0;
                        flujo_LCD = 5;                                                
                    break;
                        
                    case 0x7E:									//ir a menu
					  set_imagen(1,112);	
                      flujo_LCD=14;     
                    break;    
                        
                     
                    }
                }
            
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }    
        break;
        
        case 42:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x76:								 	 //Cambiar Nombre producto 1	             
                      set_imagen(1,48);
					  teclado=0;
                      teclas1=0;                            	 //Inicia el contador de teclas                         
                      nombreproducto = 1;
                      flujo_LCD = 24;
                    break;
					
                    case 0x82:								 	 //Cambiar Nombre producto 2	
                      set_imagen(1,48);                       
					  teclado=1;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 1;
                      flujo_LCD = 24;  					
                    break;
					
                    case 0x83:								 	 //Cambiar Nombre producto 3	             
                      set_imagen(1,48);
					  teclado=2;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 1;
                      flujo_LCD = 24;
                    break;					
                    
                    case 0x77:  								//Cambiar Nombre producto 4                     
					  set_imagen(1,48);
					  teclado=3;
                      teclas1=0;                            	 //Inicia el contador de teclas
                      nombreproducto = 1;
                      flujo_LCD = 24;
                    break;
                    case 0x7E:
                      set_imagen(1,95); 					    
                      flujo_LCD = 27;                        
                    break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();   
        }
        break;

        
    }
}


/*
*********************************************************************************************************
*                                         void polling_pos2(void)
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

void polling_pos2(void){
    
    switch(flujo_LCD2){ 
        case 0:   
        if(venta_activa2 == 1){
            flujo_LCD2 = flujo_pos2;            
        }
        if(venta_activa == 1){
            flujo_LCD = flujo_pos;
            seleccion_pos = 1;
        }
		    if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    switch(LCD_1_rxBuffer[3]){
				        case 0x56:						//POS A                                                                                                                       
                                if(venta_activa == 1){
                                    flujo_LCD = flujo_pos;
                                }else{
                                    set_imagen(1,99);           //Inicio opciones GRP 700
                                    seleccion_pos = 1;
                                    flujo_LCD  = 1;
                                }
                                if (venta_activa2 == 1){
                                    flujo_LCD2 = flujo_pos2;
                                }else{
                                    flujo_LCD2 = 0;                            
                                }
					    break;
                            		       
					    case 0x57:  				    //POS B                                 
                                set_imagen(1,99);           //Inicio opciones GRP 700
							    seleccion_pos = 2;
                                if(venta_activa == 1){
                                    flujo_LCD = flujo_pos;
                                }else{
                                    flujo_LCD = 0;
                                } 
                                if (venta_activa2 == 1){
                                    flujo_LCD2 = flujo_pos2;
                                }else{
                                    set_imagen(1,99);           //Inicio opciones GRP 700
							        seleccion_pos = 2;
                                    flujo_LCD2 = 1;                            
                                }
					    break;
                                                   
				    }
			    }
                CyDelay(100);
			    LCD_1_ClearRxBuffer();
		    }	                    
        break;
            
        case 1:
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    switch(LCD_1_rxBuffer[3]){
                        case 0x5C:
                            set_imagen(1,73);  //Seleccion de tipo de venta
                            flujo_LCD2 = 2;
                        break;
                        
                        case 0x45:		  
					        set_imagen(1,37);	
                            teclas1=0; 						    //Otras opciones
                            flujo_LCD2=15;    
                        break; 

                        case 0x59:		 
                            set_imagen(1,43);                     //Cancelado por PC
                            CyDelay(700);
                            set_imagen(1,46);
                            flujo_LCD2 = 0;   
                        break;    
                            
                        case 0x7E:
                            set_imagen(1,46); //Regreso a pantalla de selección de posición
                            flujo_LCD2 = 0;
                        break;
                        
                        case 0x94:
                            set_imagen(1,46); //Regreso a pantalla de selección de posición
                            flujo_LCD2 = 0;
                        break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
        }
        break;
        
        case 2:     
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x5F:								 	 //Sin ID	                                                                              
                      set_imagen(1,11);                       
                      flujo_LCD2 = 10; 
                    break;
                    
                    case 0x5E:  								//Con ID                                         
                      set_imagen(1,29);
                      flujo_LCD2 = 11;
                    break;	
                    
                    case 0x7E:									//ir a menu
					  set_imagen(1,46);	
                      flujo_LCD2 = 0;     
                    break;	
                    
                    case 0x94:
                        set_imagen(1,99); //Regreso a pantalla de selección de posición
                        flujo_LCD2 = 1;
                    break;
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
        }        
        break;
        
        case 4:   
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				    Buffer_LCD1.preset&=0xFC;
                    switch(LCD_1_rxBuffer[3]){
                        case 0x0F:		   					 	 //Preset por dinero	                               
                            set_imagen(1,6);                   
                            teclas1=0;                           //Inicia el contador de teclas                                         
                            write_LCD(1,'$',0);
					        Buffer_LCD2.preset|=2;
                            flujo_LCD2 = 5;   
                        break;
                    
                        case 0x10:  	                         //Inicia el contador de teclas
                            set_imagen(1,13);              
                            teclas1=0;       
                            comas1=0;
                            write_LCD(1,'G',0);
					        Buffer_LCD2.preset|=1;				 // Preset por volumen
                            flujo_LCD2 = 5;   
                        break;
                    
                        case 0x43:  	                                     
                            set_imagen(1,7);					 //Full
                            flujo_LCD2 = 6;    
                        break;
                    
                        case 0x3B:                        	    //Cancel                     					  
		            	    set_imagen(1,46);			 						  
					        flujo_LCD2 = 0;  
                        break; 
					
                        case 0x7E:									//ir a menu
					        set_imagen(1,46);
                            flujo_LCD2 = 0;     
                        break;
                            
                        case 0x94:
                            set_imagen(1,11); //Regreso a pantalla de selección de posición
                            flujo_LCD2 = 10;
                        break;
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;                                
            
        case 5:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=(versurt-1)){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD2.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD2.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
                    if(LCD_1_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas1>=1 && comas1==0){
                            teclas1++;
                            Buffer_LCD2.valor[teclas1]=0x2C;
                            write_LCD(1,0x2C,teclas1);
                            comas1=1;
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,5);
                        flujo_LCD2=4;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=1){
                        set_imagen(1,7);
						Buffer_LCD2.valor[0]=teclas1;
                        flujo_LCD2=6;                                                 				
                    }
                    
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }          
        break;    
            
        case 6:		 
            if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    if(LCD_1_rxBuffer[3]==0x7E){			//ir a menu
					    set_imagen(1,46);					//Cancel
					    flujo_LCD2 = 0;
                    }												
			    }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
			break;
		    }
            if(get_estado(b.dir)==7){													//Espera a que este en listo el equipo				
			CyDelay(50);
            grado2=estado_ex(b.dir);
			if((Buffer_LCD2.preset&0x02)==0x02||(Buffer_LCD2.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(b.dir,grado2,Buffer_LCD2.valor,(Buffer_LCD2.preset&0x03))==0){
					set_imagen(1,46);
					flujo_LCD2 = 0;
					break;
				}					
			}			
			grado2=estado_ex(b.dir);			
			Surtidor_PutChar(0x10|b.dir);									//Autoriza el surtidor
			set_imagen(1,8);
            flujo_LCD2 = 7;	
            flujo_pos2 = flujo_LCD2;
            venta_activa2 = 1;
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(b.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(b.dir)==1 && no_imprime2 == 0){	
		            set_imagen(1,57);
                    venta_activa2 = 0;                    
                    CyDelay(20);
                    flujo_LCD2 = 13;
				}
                if(venta(b.dir)==1 && no_imprime2 == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    venta_activa2 = 0;                    
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD2 = 0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(b.dir)==1 && no_imprime2 == 0){	
		            set_imagen(1,57);
                    venta_activa2 = 0;                    
                    CyDelay(20);               //Finaliza venta con impresión de recibo
                    flujo_LCD2 = 13;
				}
                if(venta(b.dir)==1 && no_imprime2 == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    venta_activa2 = 0;                    
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD2 = 0;
				}
			break;

	        case 0x06: 
                set_imagen(1,46); //No hizo venta
                venta_activa2 = 0;
                Buffer_LCD2.preset = 0x00;
				flujo_LCD2 = 0;
			break;		
            
            case 0x09:
                CyDelay(50);
                set_imagen(1,46);  //posición ocupada, regresa al inicio para retomar consulta
                flujo_LCD2 = 0;                
            break;
             			 	
         }		
        break;
        
        case 9:
          if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){				
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD2.placa[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);                        
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas1++;                        
                        Buffer_LCD2.placa[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);                        
                    }  
                    if(LCD_1_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas1++;                        
                        Buffer_LCD2.placa[teclas1]='-';
                        write_LCD(1,'-',teclas1);                        
                    } 					
                    if(LCD_1_rxBuffer[3]>=0x1B && LCD_1_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_1_rxBuffer[3]==letras[x]){
                                teclas1++;                            
                                Buffer_LCD2.placa[teclas1]=x+0x41;
                                write_LCD(1,(x+0x41),teclas1);                            
                            }
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas1==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(1,46);                        
                        Buffer_LCD2.placa[0]=0;						
                        flujo_LCD2 = 0;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);                        
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){                                        //Enter pasa a imprimir
                    if(teclas1>=1){ 
                        set_imagen(1,5);
                        Buffer_LCD2.placa[0]=teclas1;    
                        Buffer_LCD2.posventa=1;
                        flujo_LCD2 = 4;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }    
		
        break;
        
        case 10:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x39:                          //Si pide impresión                         
                        set_imagen(1,10);               //Pasa a pedir placa
                        teclas1 = 0;                                              
                        no_imprime2 = 0;
                        flujo_LCD2 = 9;                       
                    break; 
                    
                    case 0x38:                          //Pide venta sin impresión
                        set_imagen(1,5);	            //Pasa a preset de venta
                        no_imprime2 = 1;
                        flujo_LCD2 = 4;                    
                    break;  
                    
                    case 0x7E:						   //ir a menu de selección de pos
					    set_imagen(1,46);	
                        flujo_LCD2 = 0;     
                    break;
                        
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 		 	
        break; 
        
        case 11:		
		if(read_memory_ibutton(3,1)>=6){
			CyDelay(10);
			if(read_memory_ibutton(3,0x21)>=6){
		        if(touch_present(3)==1){
		            LCD_1_PutChar(0x33);
		            if(touch_write(1,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD2.id[z]=touch_read_byte(1);
		                }				
						set_imagen(1,19);  //Id correctamente reconocida
						Buffer_LCD2.preset|=0x04;
						CyDelay(500);
						set_imagen(1,14); 
						teclas1=0;
						Buffer_LCD2.km[0]=0; //Pedir Kilometraje
						flujo_LCD2 = 12;
					}
				}	
			}
		}
	    if(LCD_1_GetRxBufferSize()==8){
	        if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
	            switch(LCD_1_rxBuffer[3]){
	                case 0x3B:
	                 set_imagen(1,73); //Kilometraje
	                 flujo_LCD2 = 2;	                 
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_1_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=5){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;                    
                        Buffer_LCD2.km[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas1++;                    
                        Buffer_LCD2.km[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas1==0){
                        set_imagen(1,5);
						flujo_LCD2 = 4;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){				//Enter
                    if(teclas1>=1 && Buffer_LCD2.km[1]!=0x30){                       
                        set_imagen(1,5);
                        Buffer_LCD2.km[0]=teclas1;   
                        flujo_LCD2 = 4;                                           
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print1[1], grado2,0,b.dir);
			if(copia_recibo[1]==0){
				set_imagen(1,12);			//Imprime venta y finaliza
				CyDelay(700);
                Buffer_LCD2.preset = 0x00;
                set_imagen(1,46);
                flujo_LCD2 = 0;
			}
			else{
				set_imagen(1,49);					
				teclas2=0;					
                flujo_LCD2=33;
			}
        break; 
        
        case 14:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x58:								  //Configurar Fecha y Hora	 
                        set_imagen(1,43);                     //Cancelado por PC
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;  
                    break;
                    
                    case 0x5D:  							//Cambiar Precio                                        
                        set_imagen(1,92);
                        Precio_LCD(1,0x00,0xD6,429); // segundo campo en lcd  
					    Precio_LCD(1,0x00,0x89,423); // primer campo en el lcd			  
                        Precio_LCD(1,0x01,0x24,435); // tercer producto 
                        Precio_LCD(1,0x01,0x78,1000); //cuarto producto 
                        flujo_LCD2 = 19;                       
                    break;
                    
                   case 0x5A:  	
                        set_imagen(1,43);                     //Cancelado por PC
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;                   
                    break;
                    
                    case 0x5B:         
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;                   
                    break;   					
					
                    case 0x96:     
                        set_imagen(1,95); 
					    teclas1=0;                   			//Configurar Surtidor                   
                        flujo_LCD2 = 27;                    
                    break; 
					
                    case 0x65: 
                        set_imagen(1,43);                     //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;
                    break;
					
                    case 0x93:
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;                                                    
                    break;
                    
                    case 0x70:         
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;                         				
                    break;
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,46);
                      flujo_LCD2 = 0;     
                    break;
                    
                    case 0x94:									//ir a menu
					  set_imagen(1,46);
                      flujo_LCD2 = 0;     
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x7E){
                    set_imagen(1,49);
                    flujo_LCD2 = 0;
                }else if(LCD_1_rxBuffer[3] == 0x94){
                    set_imagen(1,99);
                    flujo_LCD2 = 1;
                }
                if(teclas1<=7){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD2.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,'*',teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD2.password[teclas1]=0x30;
                        write_LCD(1,'*',teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						set_imagen(1,57);
                        CyDelay(500);
                        set_imagen(1,46);
                        flujo_LCD2 = 0;
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas1>=1){
						y=0;
						for(x=0;x<teclas1;x++){						
							if(rventa.password[x+1]==(Buffer_LCD2.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(1,38);
							CyDelay(500);	                        							
						    set_imagen(1,112);
							}
                            flujo_LCD2 = 14;
						}
						else{
							y=0;
							for(x=0;x<teclas1;x++){						
								if(puk[x]==Buffer_LCD2.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(1,38);
								CyDelay(500);																							    set_imagen(1,93);                     																
								set_imagen(1,112);										
                                flujo_LCD2 = 14;
							}	
							else{		
								set_imagen(1,39);								
								CyDelay(500);
                                set_imagen(1,46);
                                flujo_LCD2 = 0;
							}		
						}
                    }					
                }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
            }                     		  
        break; 
        
        case 19:
        if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:		
                      set_imagen(1,6);   
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto1b;	
					  write_LCD(1,'$',teclas1);					 //Producto 1	                               
                      flujo_LCD2 = 26; 
                    break;
                    
                    case 0x80:  	
                      set_imagen(1,6);		
                      teclas1=0;                            	  //Inicia el contador de teclas 
					  rventa.producto=producto2b;
					  write_LCD(1,'$',teclas1);					  //Producto 2
                      flujo_LCD2 = 26;              
                    break;
                    
                    case 0x81:  
                      set_imagen(1,6);			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto3b;
					  write_LCD(1,'$',teclas1);						//Producto 3
                      flujo_LCD2 = 26;	
                    break;
                    
                    case 0x82:  	
                      set_imagen(1,6); 			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto4b;
					  write_LCD(1,'$',teclas1);					//Otro producto
                      flujo_LCD2 = 26;		
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(1,46);                         //Otras Opciones
                      flujo_LCD2 = 0;     
                    break;
                    
                    case 0x94:
                        set_imagen(1,112);
                        flujo_LCD2 = 14;
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;
                       
        case 26:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                               
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior                        
						set_imagen(1,112);
						flujo_LCD2 = 14;						
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=4){
                        flujo_LCD2 = 6;                         
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;					
						if(cambiar_precio(b.dir)!=0){ 	
							if(rventa.producto==producto1b){
								write_eeprom(423,Buffer_LCD1.valor);	//429						
							}
                            if(rventa.producto==producto3b){
								write_eeprom(435,Buffer_LCD1.valor); //423							
							}
							if(rventa.producto==producto2b){
								write_eeprom(429,Buffer_LCD1.valor);	//435						
							}
                            
							
                            if(rventa.producto==producto4b){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(1,60);	
							CyDelay(500);														
							set_imagen(1,112);
							flujo_LCD2 = 14;							
						}
						else{
                            set_imagen(1,85);   //Error de operacion
                            CyDelay(700);
                            set_imagen(1,46);
                            flujo_LCD2 = 0;
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 27:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                      
                      set_imagen(1,88); 
					  Grado_LCD(1,0x00,0x73,636); // primer producto 
					  Grado_LCD(1,0x00,0xCF,638); // segundo producto			  
                      Grado_LCD(1,0x01,0x29,634); // tercer producto
                      Grado_LCD(1,0x01,0x7B,1008); //cuarto producto
                      flujo_LCD2 = 28;   
                    break;
                    
                    case 0x80: 
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;                      
                    break;
                    
                    case 0x81:  								 //PPU                      
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;	
                    break;
                    
                    case 0x82:  								                      
                        set_imagen(1,43);                      //Cancelado por PC 
                        CyDelay(700);
                        set_imagen(1,112);
                        flujo_LCD2 = 14;  
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,46);
                      flujo_LCD2 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(1,112);
                      flujo_LCD2 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;
        
        case 28:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Producto1	                                                         
                      set_imagen(1,6); 
					  teclas1=0; 
					  rventa.producto=1;
                      flujo_LCD2 = 31;
                    break;
                    
                    case 0x80:  								 //Producto2                      
                      set_imagen(1,6); 	 
					  teclas1=0;
					  rventa.producto=2;
                      flujo_LCD2 = 31;
                    break;
                    
                    case 0x81:  								 //Producto3                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=3;
                      flujo_LCD2 = 31;
                    break;
                    
                    case 0x82:  								 //Producto4                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=4;
                      flujo_LCD2 = 31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,46);
                      flujo_LCD2 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(1,112);
                      flujo_LCD2 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;	
        
        case 31:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=0){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD2.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD2.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,0);
                        flujo_LCD2 = 0;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD2.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==1){                        
                        set_imagen(1,57);	
						Buffer_LCD2.valor[0]=teclas1;
						if(rventa.producto==2){
							producto1b=Buffer_LCD2.valor[1];
							write_eeprom(636,Buffer_LCD2.valor);							
						}
						if(rventa.producto==1){
							producto3b=Buffer_LCD2.valor[1];
							write_eeprom(634,Buffer_LCD2.valor);							
						}
						if(rventa.producto==3){
							producto2b=Buffer_LCD2.valor[1];
							write_eeprom(638,Buffer_LCD2.valor);							
						}
                        if(rventa.producto==4){
							producto4b=Buffer_LCD2.valor[1];
							write_eeprom(1008,Buffer_LCD2.valor);							
						}
						CyDelay(500);						
					    set_imagen(1,112);
                        flujo_LCD2 = 14;												
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
        
        case 33:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x0A)
                {
                    for(z=1;z<=(n_copias[0]&0x0F);z++){
                        imprimir(print1[1],grado2,1,b.dir);
                        CyDelay(500);
                    }                    
                    set_imagen(1,12);
                    CyDelay(700);
                    Buffer_LCD1.preset = 0x00;
                    set_imagen(1,46);                    
                    flujo_LCD2=0;
                }
                else{
					set_imagen(1,12);					      // Si presionan NO o cualquier otro touch
				    CyDelay(700);
                    set_imagen(1,46);
                    flujo_LCD2=0;
                }                             
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }	
			 
		break;
       
        
        
    }
}


/*
*********************************************************************************************************
*                                         void polling_pos3(void)
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

void polling_pos3(void){       
    switch(flujo_LCD3){ 
        case 0:            
        if(venta_activa3 == 1){
            flujo_LCD3 = flujo_pos3;            
        }
        if(venta_activa4 == 1){
            flujo_LCD4 = flujo_pos4;
            seleccion_pos2 = 4;
        }
		    if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){                    
                    switch(LCD_2_rxBuffer[3]){
				        case 0x56:						        //POS A                                                                                                                       
                                if(venta_activa3 == 1){
                                    flujo_LCD3 = flujo_pos3;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    set_imagen(2,99);           //Inicio opciones GRP 700 POS A
                                    seleccion_pos2 = 3;
                                    flujo_LCD3  = 1;
                                }
                                if (venta_activa4 == 1){
                                    flujo_LCD4 = flujo_pos4;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    flujo_LCD4 = 0;                            
                                }
					    break;
                            		       
					    case 0x57:  				            //POS B                                                                 
                                if(venta_activa == 1){
                                    flujo_LCD3 = flujo_pos3;
                                }else{
                                    flujo_LCD3 = 0;
                                } 
                                if (venta_activa4 == 1){                                    
                                    flujo_LCD4 = flujo_pos4;
                                }else{
                                    set_imagen(2,99);           //Inicio opciones GRP 700
							        seleccion_pos2 = 4;
                                    flujo_LCD4 = 1;                            
                                }                           
					    break;
                                                   
				    }                                                            
			    }
                CyDelay(100);
			    LCD_2_ClearRxBuffer();
		    }
                                
        
        break;
            
        case 1:
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                    switch(LCD_2_rxBuffer[3]){
                        case 0x5C:
                            set_imagen(2,73);        //Seleccion de tipo de venta
                            flujo_LCD3 = 2;
                        break;
                        
                        case 0x45:		  
					        set_imagen(2,37);	
                            teclas2=0; 						    //Otras opciones
                            flujo_LCD3 = 15;   
                        break; 

                        case 0x59:		 
                            set_imagen(2,43);                     //Cancelado por PC
                            CyDelay(500);
                            set_imagen(2,46);
                            flujo_LCD3 = 0; 
                        break;
                            
                        case 0x7E:
                            set_imagen(2,46); //Regreso a pantalla de selección de posición
                            flujo_LCD3 = 0;
                        break;
                            
                        case 0x94:
                            set_imagen(2,46); //Regreso a pantalla de selección de posición
                            flujo_LCD3 = 0;
                        break;                            
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
        }
        break;
        
        case 2:     
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x5F:							//Sin ID	                                                                              
                      set_imagen(2,11);                 //Pasa a pantalla de solicitud de recibo 
                      flujo_LCD3 = 10; 
                    break;
                    
                    case 0x5E:  						//Con ID                                         
                      set_imagen(2,29);
                      flujo_LCD3=11;
                    break;	

                    case 0x7E:						    //ir a menu
					  set_imagen(2,46);	
                      flujo_LCD3=0;     
                    break;	
                    
                    case 0x94:
                        set_imagen(2,99); //Regreso a pantalla de selección de posición
                        flujo_LCD3 = 1;
                    break;
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
        }        
        break;
        
        case 4:   
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				    Buffer_LCD3.preset&=0xFC;
                    switch(LCD_2_rxBuffer[3]){
                        case 0x0F:		   					 	 //Preset por dinero	                               
                            set_imagen(2,6);                   
                            teclas2=0;                           //Inicia el contador de teclas                                         
                            write_LCD(2,'$',0);
					        Buffer_LCD3.preset|=2;
                            flujo_LCD3 = 5;   
                        break;
                    
                        case 0x10:  	                         //Inicia el contador de teclas
                            set_imagen(2,13);              
                            teclas2=0;       
                            comas2=0;
                            write_LCD(2,'G',0);
					        Buffer_LCD3.preset|=1;				 // Preset por volumen
                            flujo_LCD3=5;   
                        break;
                    
                        case 0x43:  	                                     
                            set_imagen(2,7);					 //Full
                            flujo_LCD3 = 6;    
                        break;
                    
                        case 0x3B:                        	    //Cancel                     					  
		            	    set_imagen(2,46);			 						  
					        flujo_LCD3=0;  
                        break; 
					
                        case 0x7E:									//ir a menu
					        set_imagen(2,46);
                            flujo_LCD3=0;     
                        break;	
                        
                        case 0x94:
                            set_imagen(2,11); //Regreso a pantalla de selección de posición
                            flujo_LCD3 = 10;
                        break;
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 5:   
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=(versurt-1)){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD3.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD3.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }  
                    if(LCD_2_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas2>=1 && comas2==0){
                            teclas2++;
                            Buffer_LCD3.valor[teclas2]=0x2C;
                            write_LCD(2,0x2C,teclas2);
                            comas2=1;
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,5);
                        flujo_LCD3=4;
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD3.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2>=1 && a_copias ==0){
                        set_imagen(2,7);
						Buffer_LCD3.valor[0]=teclas2;
                        flujo_LCD3 = 6;                                                 				
                    }
                    if(teclas2>=1 && a_copias == 1){ 
                        set_imagen(2,112);
                        n_copias[0]=Buffer_LCD3.valor[1];                       
                        write_eeprom(1174,n_copias);
                        a_copias = 0;
                        flujo_LCD3 = 14;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }          
        break;     
            
            
        case 6:		 
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                    if(LCD_2_rxBuffer[3]==0x7E){			//ir a menu
					    set_imagen(2,46);					//Cancel
					    flujo_LCD3 = 0;
                    }												
			    }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
			break;
		    }
            if(get_estado(c.dir)==7){													//Espera a que este en listo el equipo				
			CyDelay(50);
            grado3=estado_ex(c.dir);
			if((Buffer_LCD3.preset&0x02)==0x02||(Buffer_LCD3.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(c.dir,grado3,Buffer_LCD3.valor,(Buffer_LCD3.preset&0x03))==0){
					set_imagen(2,46);
					flujo_LCD3 = 0;
					break;
				}					
			}			
			grado3 = estado_ex(c.dir);		//Manejo de grados?, como	
			Surtidor_PutChar(0x10|c.dir);									//Autoriza el surtidor
			set_imagen(2,8);            
            flujo_LCD3 = 7;
            flujo_pos3 = flujo_LCD3;
            venta_activa3 = 1;
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(c.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(c.dir)==1 && no_imprime == 0){	
		            set_imagen(2,57);                     
                    venta_activa3 = 0;                    
                    CyDelay(20);
                    flujo_LCD3 = 13;                   
				}
                if(venta(c.dir)==1 && no_imprime == 1){	
		            set_imagen(2,12);         //Finaliza venta sin impresión de recibo
                    venta_activa3 = 0;                    
                    CyDelay(500);
                    set_imagen(2,46);                    
                    flujo_LCD3 = 0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(c.dir)==1 && no_imprime == 0){	
		            set_imagen(2,57);
                    venta_activa3 = 0;                    
                    CyDelay(20);               //Finaliza venta con impresión de recibo                      
                    flujo_LCD3 = 13;
				}
                if(venta(c.dir)==1 && no_imprime == 1){	
		            set_imagen(2,12);         //Finaliza venta sin impresión de recibo
                    venta_activa3 = 0;                    
                    CyDelay(500);                    
                    set_imagen(2,46);
                    flujo_LCD3 = 0;
				}
			break;

	        case 0x06: 
                set_imagen(2,46); //No hizo venta
                venta_activa3 = 0;
                Buffer_LCD3.preset = 0x00;
				flujo_LCD3=0;
			break;
                
            case 0x09:
                CyDelay(50);
                set_imagen(2,46);
                flujo_LCD3 = 0;                
            break;    
             			 	
         }		
        break;
        
        case 9:
          if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){				
                if(teclas2<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD3.placa[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);                        
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas2++;                        
                        Buffer_LCD3.placa[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);                        
                    }  
                    if(LCD_2_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas2++;                        
                        Buffer_LCD3.placa[teclas2]='-';
                        write_LCD(2,'-',teclas2);                        
                    } 					
                    if(LCD_2_rxBuffer[3]>=0x1B && LCD_2_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_2_rxBuffer[3]==letras[x]){
                                teclas2++;                            
                                Buffer_LCD3.placa[teclas2]=x+0x41;
                                write_LCD(2,(x+0x41),teclas2);                            
                            }
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas2==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(2,46);                        
                        Buffer_LCD3.placa[0]=0;						
                        flujo_LCD3 = 0;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);                        
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){                                        //Enter pasa a venta y guarda los datos de recibo
                    if(teclas2>=1){ 
                        set_imagen(2,5);
                        Buffer_LCD3.placa[0]=teclas2; 
                        Buffer_LCD3.posventa=1;
                        flujo_LCD3 = 4;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }    
		
        break;
        
        case 10:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x39:                          //Si pide impresión                         
                        set_imagen(2,10);               //Pasa a pedir placa
                        teclas2 = 0;                                              
                        no_imprime = 0;
                        flujo_LCD3 = 9;                       
                    break; 
                    
                    case 0x38:                          //Pide venta sin impresión
                        set_imagen(2,5);	            //Pasa a preset de venta
                        no_imprime = 1;
                        flujo_LCD3 = 4;                    
                    break;  
                    
                    case 0x7E:						   //ir a menu de selección de pos
					    set_imagen(2,46);	
                        flujo_LCD3 = 0;     
                    break;
                        
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 		 	
        break;
        
        case 11:		
		if(read_memory_ibutton(2,1)>=6){
			CyDelay(10);
			if(read_memory_ibutton(2,0x21)>=6){
		        if(touch_present(2)==1){
		            LCD_2_PutChar(0x33);
		            if(touch_write(2,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD3.id[z]=touch_read_byte(2);
		                }				
						set_imagen(2,19);  //Id correctamente reconocida
						Buffer_LCD3.preset|=0x04;
						CyDelay(500);
						set_imagen(2,14); 
						teclas2=0;
						Buffer_LCD3.km[0]=0; //Pedir Kilometraje
						flujo_LCD3 = 12;
					}
				}	
			}
		}
	    if(LCD_2_GetRxBufferSize()==8){
	        if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
	            switch(LCD_2_rxBuffer[3]){
	                case 0x3B:
	                 set_imagen(2,73); //Kilometraje
	                 flujo_LCD3 = 2;	                 
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_2_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=5){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;                    
                        Buffer_LCD3.km[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas2++;                    
                        Buffer_LCD3.km[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas2==0){
                        set_imagen(2,5);
						flujo_LCD3 = 4;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){				//Enter
                    if(teclas2>=1 && Buffer_LCD3.km[1]!=0x30){                       
                        set_imagen(2,5);
                        Buffer_LCD3.km[0]=teclas2;   
                        flujo_LCD3 = 4;                                           
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print2[1], grado3,0,c.dir);  //Imprime recibo sin copia
			if(copia_recibo2[1]==0){
				set_imagen(2,12);
				CyDelay(600);
                Buffer_LCD3.preset = 0x00;
                set_imagen(2,46);
                flujo_LCD3 = 0;
			}
			else{
				set_imagen(2,49);
				teclas2=0;				
				flujo_LCD3 = 33;					
			}
        break;  
        
        case 14:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x58:								 	 //Configurar Fecha y Hora	 
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;   
                    break;
                    
                    case 0x5D:  							     //Cambiar Precio
                                        
                      set_imagen(2,92);
                      Precio_LCD(2,0x00,0xD6,429);               // primer producto   segundo campo en el lcd
					  Precio_LCD(2,0x00,0x89,423);               // segundo producto 			  
                      Precio_LCD(2,0x01,0x24,435);               // segundo producto 
                      Precio_LCD(2,0x01,0x78,1000);              //cuarto producto 
                      flujo_LCD3 = 19; 
                      
                    break;
                    
                   case 0x5A:  	
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;                     
                    break;
                    
                    case 0x5B:         
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;                     
                    break;   					
					
                    case 0x96:     
                        set_imagen(2,95); 
					    teclas2=0;                   			 //Configurar Surtidor                   
                        flujo_LCD3 = 27;                    
                    break; 
					
                    case 0x65: 
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14; 
                    break;
					
                    case 0x93:
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;                                                     
                    break;
                    
                    case 0x70:         
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;                     				
                    break;
					
                    case 0x7E:									//ir a menu
					  set_imagen(2,46);
                      flujo_LCD3 = 0;     
                    break;
                    
                    case 0x94:									//Volver a menu de opciones
					  set_imagen(2,46);
                      flujo_LCD3 = 0;     
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3] == 0x7E){
                    set_imagen(2,49);
                    flujo_LCD3 = 0;
                }else if(LCD_2_rxBuffer[3] == 0x94){
                    set_imagen(2,99);
                    flujo_LCD3 = 1;
                }
                if(teclas2<=7){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD3.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,'*',teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD3.password[teclas2]=0x30;
                        write_LCD(2,'*',teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						set_imagen(2,57);
                        CyDelay(500);
                        set_imagen(2,46);
                        flujo_LCD3 = 0;
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas2>=1){
						y=0;
						for(x=0;x<teclas2;x++){						
							if(rventa.password[x+1]==(Buffer_LCD3.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(2,38);
							CyDelay(500);	                        							
						    set_imagen(2,112);							
                            flujo_LCD3 = 14;
						}
						else{
							y=0;
							for(x=0;x<teclas2;x++){						
								if(puk[x]==Buffer_LCD3.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(2,38);
								CyDelay(500);																							    set_imagen(1,93);                     																
								set_imagen(2,112);										
                                flujo_LCD=14;
							}	
							else{		
								set_imagen(2,39);								
								CyDelay(500);
                                set_imagen(2,46);
                                flujo_LCD3 = 0;
							}		
						}
                    }					
                }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
            } 
        }
        break;                                              
        
        case 19:
        if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:		
                      set_imagen(2,6);   
                      teclas2=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto1c;	
					  write_LCD(2,'$',teclas2);							 	 //Producto 1	                               
                      flujo_LCD3 = 26; 
                    break;
                    
                    case 0x80:  	
                      set_imagen(2,6);		
                      teclas2=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto2c;
					  write_LCD(2,'$',teclas2);						 //Producto 2
                      flujo_LCD3 = 26;              
                    break;
                    
                    case 0x81:  
                      set_imagen(2,6);			
                      teclas2=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto3c;
					  write_LCD(2,'$',teclas2);						//Producto 3
                      flujo_LCD3 = 26;	
                    break;
                    
                    case 0x82:  	
                      set_imagen(2,6); 			
                      teclas2=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto4c;
					  write_LCD(2,'$',teclas2);					//Otro producto
                      flujo_LCD3 = 26;		
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(2,46);            //Otras Opciones
                      flujo_LCD3 = 0;     
                    break;
                    
                    case 0x94:
                        set_imagen(2,112);
                        flujo_LCD3 = 14;
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;                                                                           
        
        case 26:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD1.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD1.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                               
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior                        
						set_imagen(2,112);
						flujo_LCD3 = 14;						
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2>=4){
                        flujo_LCD3 = 6;                         
                        set_imagen(2,57); 
                        Buffer_LCD1.valor[0]=teclas2;
						if(cambiar_precio(c.dir)!=0){ 	
							if(rventa.producto==producto1c){
								write_eeprom(423,Buffer_LCD1.valor);	//429						
							}
                            if(rventa.producto==producto3c){
								write_eeprom(435,Buffer_LCD1.valor); //423							
							}
							if(rventa.producto==producto2c){
								write_eeprom(429,Buffer_LCD1.valor);	//435						
							}                            							
                            if(rventa.producto==producto4c){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(2,60);	
							CyDelay(500);														
							set_imagen(2,112);
							flujo_LCD3 = 14;							
						}
						else{
                            set_imagen(2,85);   //Error de operacion
                            CyDelay(500);
                            set_imagen(2,46);
                            flujo_LCD3 = 0;
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
        
        case 27:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                      
                      set_imagen(2,88); 
					  Grado_LCD(2,0x00,0x73,1176); // primer producto 
					  Grado_LCD(2,0x00,0xCF,1178); // segundo producto			  
                      Grado_LCD(2,0x01,0x29,1180); // tercer producto
                      Grado_LCD(2,0x01,0x7B,1182); //cuarto producto
                      flujo_LCD3 = 28;   
                    break;
                    
                    case 0x80:  								 //Version de Digitos                      
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;
                    break;
                    
                    case 0x81:  								 //PPU                      
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;
                    break;
                    
                    case 0x82:  								 //Nombre de productos                      
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD3 = 14;	
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,46);
                      flujo_LCD3 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(2,112);
                      flujo_LCD3 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;
        
        case 28:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){                    
                    
                    case 0x80:  								 //Producto1                      
                      set_imagen(2,6); 	 
					  teclas2=0;
					  rventa.producto=1;
                      flujo_LCD3 = 31;
                    break;
                    
                    case 0x81:  								 //Producto2                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=2;
                      flujo_LCD3 = 31;
                    break; 
                    
                    
                    case 0x7F:								 	 //Producto3	                                                         
                      set_imagen(2,6); 
					  teclas2=0; 
					  rventa.producto=3;
                      flujo_LCD3 = 31;
                    break;
                    
                    case 0x82:  								 //Producto4                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=4;
                      flujo_LCD3 = 31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,46);
                      flujo_LCD3 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(2,112);
                      flujo_LCD3 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;	                               
        
        case 31:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=0){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD3.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD3.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,0);
                        flujo_LCD3 = 0;
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD3.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==1){                        
                        set_imagen(2,57);	
						Buffer_LCD3.valor[0]=teclas2;
						if(rventa.producto==1){
							producto1c=Buffer_LCD3.valor[1];
							write_eeprom(1176,Buffer_LCD3.valor);							
						}
						if(rventa.producto==2){
							producto2c=Buffer_LCD3.valor[1];
							write_eeprom(1178,Buffer_LCD3.valor);							
						}
                        if(rventa.producto==3){
							producto3c=Buffer_LCD3.valor[1];
							write_eeprom(1180,Buffer_LCD3.valor);							
						}
                        if(rventa.producto==4){
							producto4c=Buffer_LCD3.valor[1];
							write_eeprom(1182,Buffer_LCD3.valor);							
						}
						CyDelay(500);						
					    set_imagen(2,112);
                        flujo_LCD3 = 14;												
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;               	
        
        case 33:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3] == 0x0A)
                {
                    for(z=1;z<=(n_copias[0]&0x0F);z++){
                        imprimir(print2[1],grado3,1,c.dir);   //Impresión de número de copias por configuración
                        CyDelay(500);
                    }
                    set_imagen(2,12);					
				    CyDelay(500);
                    Buffer_LCD3.preset = 0x00;
                    set_imagen(2,46);
                    flujo_LCD3 = 0;
                }
                else{
                    set_imagen(2,12);
					CyDelay(500);               //Confirman NO, muestra imagen y sale
                    Buffer_LCD3.preset = 0x00;
                    set_imagen(2,46);
                    flujo_LCD3 = 0;
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }	
		        
		break;                                              
        
    }
}


/*
*********************************************************************************************************
*                                         void polling_pos4(void)
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


void polling_pos4(void){
    
    switch(flujo_LCD4){ 
        case 0:   
        if(venta_activa4 == 1){
            flujo_LCD4 = flujo_pos4;            
        }
        if(venta_activa3 == 1){
            flujo_LCD3 = flujo_pos3;
            seleccion_pos2 = 3;
        }
		    if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                    switch(LCD_2_rxBuffer[3]){
				        case 0x56:						//POS A                                                                                                                       
                                if(venta_activa3 == 1){
                                    flujo_LCD3 = flujo_pos3;
                                }else{
                                    set_imagen(2,99);           //Inicio opciones GRP 700
                                    seleccion_pos2 = 3;
                                    flujo_LCD3  = 1;
                                }
                                if (venta_activa4 == 1){
                                    flujo_LCD4 = flujo_pos4;
                                }else{
                                    flujo_LCD4 = 0;                            
                                }
					    break;
                            		       
					    case 0x57:  				    //POS B                                 
                                set_imagen(2,99);           //Inicio opciones GRP 700
							    seleccion_pos2 = 4;
                                if(venta_activa3 == 1){
                                    flujo_LCD3 = flujo_pos3;
                                }else{
                                    flujo_LCD3 = 0;
                                } 
                                if (venta_activa4 == 1){
                                    flujo_LCD4 = flujo_pos4;
                                }else{
                                    set_imagen(2,99);           //Inicio opciones GRP 700
							        seleccion_pos2 = 4;
                                    flujo_LCD4 = 1;                            
                                }
					    break;
                                                   
				    }
			    }
                CyDelay(100);
			    LCD_2_ClearRxBuffer();
		    }	                    
        break;
            
        case 1:
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                    switch(LCD_2_rxBuffer[3]){
                        case 0x5C:
                            set_imagen(2,73);  //Seleccion de tipo de venta
                            flujo_LCD4 = 2;
                        break;
                        
                        case 0x45:		  
					        set_imagen(2,37);	
                            teclas2=0; 						    //Otras opciones
                            flujo_LCD4 = 15;    
                        break; 

                        case 0x59:		 
                            set_imagen(2,43);                     //Cancelado por PC
                            CyDelay(500);
                            set_imagen(2,46);
                            flujo_LCD4 = 0;  
                        break;    
                            
                        case 0x7E:
                            set_imagen(2,46); //Regreso a pantalla de selección de posición
                            flujo_LCD4 = 0;
                        break;
                        
                        case 0x94:
                            set_imagen(2,46); //Regreso a pantalla de selección de posición
                            flujo_LCD4 = 0;
                        break;
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
        }
        break;
        
        case 2:     
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x5F:								 	 //Sin ID	                                                                              
                      set_imagen(2,11);                       
                      flujo_LCD4 = 10; 
                    break;
                    
                    case 0x5E:  								//Con ID                                         
                      set_imagen(2,29);
                      flujo_LCD4 = 11;
                    break;	
                    
                    case 0x7E:									//ir a menu
					  set_imagen(2,46);	
                      flujo_LCD4 = 0;     
                    break;	
                    
                    case 0x94:
                        set_imagen(2,99); //Regreso a pantalla de selección de posición
                        flujo_LCD4 = 1;
                    break;
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
        }        
        break;
        
        case 4:   
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				    Buffer_LCD4.preset&=0xFC;
                    switch(LCD_2_rxBuffer[3]){
                        case 0x0F:		   					 	 //Preset por dinero	                               
                            set_imagen(2,6);                   
                            teclas2=0;                           //Inicia el contador de teclas                                         
                            write_LCD(2,'$',0);
					        Buffer_LCD4.preset|=2;
                            flujo_LCD4 = 5;   
                        break;
                    
                        case 0x10:  	                         //Inicia el contador de teclas
                            set_imagen(2,13);              
                            teclas2=0;       
                            comas2=0;
                            write_LCD(2,'G',0);
					        Buffer_LCD4.preset|=1;				 // Preset por volumen
                            flujo_LCD4 = 5;   
                        break;
                    
                        case 0x43:  	                                     
                            set_imagen(2,7);					 //Full
                            flujo_LCD4 = 6;    
                        break;
                    
                        case 0x3B:                        	    //Cancel                     					  
		            	    set_imagen(2,46);			 						  
					        flujo_LCD4 = 0;  
                        break; 
					
                        case 0x7E:									//ir a menu
					        set_imagen(2,46);
                            flujo_LCD4 = 0;     
                        break;
                            
                        case 0x94:
                            set_imagen(2,11); //Regreso a pantalla de selección de posición
                            flujo_LCD4 = 10;
                        break;
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;                                
            
        case 5:   
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=(versurt-1)){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD4.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD4.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }  
                    if(LCD_2_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas2>=1 && comas2==0){
                            teclas2++;
                            Buffer_LCD4.valor[teclas2]=0x2C;
                            write_LCD(2,0x2C,teclas2);
                            comas2=1;
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,5);
                        flujo_LCD4 = 4;
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD4.valor[teclas2]==0x2C){
                            comas2 = 0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2>=1){
                        set_imagen(2,7);
						Buffer_LCD4.valor[0]=teclas2;
                        flujo_LCD4=6;                                                 				
                    }
                    
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }          
        break;    
            
        case 6:		 
            if(LCD_2_GetRxBufferSize()==8){
                if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                    if(LCD_2_rxBuffer[3]==0x7E){			//ir a menu
					    set_imagen(2,46);					//Cancel
					    flujo_LCD4 = 0;
                    }												
			    }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
			break;
		    }
            if(get_estado(d.dir)==7){													//Espera a que este en listo el equipo				
			CyDelay(50);
            grado4=estado_ex(d.dir);
			if((Buffer_LCD4.preset&0x02)==0x02||(Buffer_LCD4.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(d.dir,grado4,Buffer_LCD4.valor,(Buffer_LCD4.preset&0x03))==0){
					set_imagen(2,46);
					flujo_LCD4 = 0;
					break;
				}					
			}			
			grado4=estado_ex(d.dir);			
			Surtidor_PutChar(0x10|d.dir);									//Autoriza el surtidor
			set_imagen(2,8);
            flujo_LCD4 = 7;	
            flujo_pos4 = flujo_LCD4;
            venta_activa4 = 1;
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(d.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(d.dir)==1 && no_imprime2 == 0){	
		            set_imagen(2,57);
                    venta_activa4 = 0;                    
                    CyDelay(20);
                    flujo_LCD4 = 13;
				}
                if(venta(d.dir)==1 && no_imprime2 == 1){	
		            set_imagen(2,12);         //Finaliza venta sin impresión de recibo
                    venta_activa4 = 0;                    
                    CyDelay(500);
                    set_imagen(2,46);
                    flujo_LCD4 = 0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(d.dir)==1 && no_imprime2 == 0){	
		            set_imagen(2,57);
                    venta_activa4 = 0;                    
                    CyDelay(20);               //Finaliza venta con impresión de recibo
                    flujo_LCD4 = 13;
				}
                if(venta(d.dir)==1 && no_imprime2 == 1){	
		            set_imagen(2,12);         //Finaliza venta sin impresión de recibo
                    venta_activa4 = 0;                    
                    CyDelay(500);
                    set_imagen(2,46);
                    flujo_LCD4 = 0;
				}
			break;

	        case 0x06: 
                set_imagen(2,46); //No hizo venta
                venta_activa4 = 0;
                Buffer_LCD4.preset = 0x00;
				flujo_LCD4 = 0;
			break;		
            
            case 0x09:
                CyDelay(50);
                set_imagen(2,46);
                flujo_LCD4 = 0;                
            break;
             			 	
         }		
        break;
        
        case 9:
          if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){				
                if(teclas2<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD4.placa[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);                        
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas2++;                        
                        Buffer_LCD4.placa[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);                        
                    }  
                    if(LCD_2_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas2++;                        
                        Buffer_LCD4.placa[teclas2]='-';
                        write_LCD(2,'-',teclas2);                        
                    } 					
                    if(LCD_2_rxBuffer[3]>=0x1B && LCD_2_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_2_rxBuffer[3]==letras[x]){
                                teclas2++;                            
                                Buffer_LCD4.placa[teclas2]=x+0x41;
                                write_LCD(2,(x+0x41),teclas2);                            
                            }
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas2==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(2,46);                        
                        Buffer_LCD4.placa[0]=0;						
                        flujo_LCD4 = 0;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);                        
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){                                        //Enter pasa a imprimir
                    if(teclas2>=1){ 
                        set_imagen(2,5);
                        Buffer_LCD4.placa[0]=teclas2;    
                        Buffer_LCD4.posventa=1;
                        flujo_LCD4 = 4;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }    
		
        break;
        
        case 10:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x39:                          //Si pide impresión                         
                        set_imagen(2,10);               //Pasa a pedir placa
                        teclas2 = 0;                                              
                        no_imprime2 = 0;
                        flujo_LCD4 = 9;                       
                    break; 
                    
                    case 0x38:                          //Pide venta sin impresión
                        set_imagen(2,5);	            //Pasa a preset de venta
                        no_imprime2 = 1;
                        flujo_LCD4 = 4;                    
                    break;  
                    
                    case 0x7E:						   //ir a menu de selección de pos
					    set_imagen(2,46);	
                        flujo_LCD4 = 0;     
                    break;
                        
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 		 	
        break; 
        
        case 11:		
		if(read_memory_ibutton(4,1)>=6){
			CyDelay(10);
			if(read_memory_ibutton(2,0x21)>=6){
		        if(touch_present(2)==1){
		            LCD_2_PutChar(0x33);
		            if(touch_write(2,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD4.id[z]=touch_read_byte(2);
		                }				
						set_imagen(2,19);  //Id correctamente reconocida
						Buffer_LCD4.preset|=0x04;
						CyDelay(500);
						set_imagen(2,14); 
						teclas2=0;
						Buffer_LCD4.km[0]=0; //Pedir Kilometraje
						flujo_LCD4 = 12;
					}
				}	
			}
		}
	    if(LCD_2_GetRxBufferSize()==8){
	        if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
	            switch(LCD_2_rxBuffer[3]){
	                case 0x3B:
	                 set_imagen(2,73); //Kilometraje
	                 flujo_LCD4 = 2;	                 
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_2_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=5){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;                    
                        Buffer_LCD4.km[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas2++;                    
                        Buffer_LCD4.km[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas2==0){
                        set_imagen(2,5);
						flujo_LCD4 = 4;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){				//Enter
                    if(teclas2>=1 && Buffer_LCD4.km[1]!=0x30){                       
                        set_imagen(2,5);
                        Buffer_LCD4.km[0]=teclas1;   
                        flujo_LCD4 = 4;                                           
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print2[1], grado2,0,d.dir);
			if(copia_recibo2[1]==0){
				set_imagen(2,12);				
				CyDelay(500);
                Buffer_LCD4.preset = 0x00;
                set_imagen(2,46);
                flujo_LCD4 = 0;
			}
			else{
				set_imagen(2,49);					
				teclas2=0;					
                flujo_LCD4=33;
			}
        break; 
        
        case 14:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x58:								  //Configurar Fecha y Hora	 
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;  
                    break;
                    
                    case 0x5D:  							//Cambiar Precio                                        
                        set_imagen(2,92);
                        Precio_LCD(2,0x00,0xD6,429); // segundo campo en lcd  
					    Precio_LCD(2,0x00,0x89,423); // primer campo en el lcd			  
                        Precio_LCD(2,0x01,0x24,435); // tercer producto 
                        Precio_LCD(2,0x01,0x78,1000); //cuarto producto 
                        flujo_LCD4 = 19;                       
                    break;
                    
                   case 0x5A:  	
                        set_imagen(2,43);                     //Cancelado por PC
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;                   
                    break;
                    
                    case 0x5B:         
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;                   
                    break;   					
					
                    case 0x96:     
                        set_imagen(2,95); 
					    teclas2=0;                   			//Configurar Surtidor                   
                        flujo_LCD4 = 27;                    
                    break; 
					
                    case 0x65: 
                        set_imagen(2,43);                     //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;
                    break;
					
                    case 0x93:
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;                                                    
                    break;
                    
                    case 0x70:         
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;                         				
                    break;
					
                    case 0x7E:									//ir a menu
					  set_imagen(2,46);
                      flujo_LCD4 = 0;     
                    break;
                    
                    case 0x94:									//ir a menu
					  set_imagen(2,46);
                      flujo_LCD4 = 0;     
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3] == 0x7E){
                    set_imagen(2,49);
                    flujo_LCD4 = 0;
                }else if(LCD_2_rxBuffer[3] == 0x94){
                    set_imagen(2,99);
                    flujo_LCD4 = 1;
                }
                if(teclas2<=7){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD4.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,'*',teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD4.password[teclas2]=0x30;
                        write_LCD(2,'*',teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						set_imagen(2,57);
                        CyDelay(500);
                        set_imagen(2,46);
                        flujo_LCD4 = 0;
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas2>=1){
						y=0;
						for(x=0;x<teclas2;x++){						
							if(rventa.password[x+1]==(Buffer_LCD4.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(2,38);
							CyDelay(500);	                        							
						    set_imagen(2,112);
							}
                            flujo_LCD4 = 14;
						}
						else{
							y=0;
							for(x=0;x<teclas2;x++){						
								if(puk[x]==Buffer_LCD4.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(2,38);
								CyDelay(500);																							    set_imagen(1,93);                     																
								set_imagen(2,112);										
                                flujo_LCD4 = 14;
							}	
							else{		
								set_imagen(2,39);								
								CyDelay(500);
                                set_imagen(2,46);
                                flujo_LCD4 = 0;
							}		
						}
                    }					
                }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
            }                     		  
        break; 
        
        case 19:
        if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:		
                      set_imagen(2,6);   
                      teclas2=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=producto1d;	
					  write_LCD(2,'$',teclas2);					 //Producto 1	                               
                      flujo_LCD4 = 26; 
                    break;
                    
                    case 0x80:  	
                      set_imagen(2,6);		
                      teclas2=0;                            	  //Inicia el contador de teclas 
					  rventa.producto=producto2d;
					  write_LCD(2,'$',teclas2);					  //Producto 2
                      flujo_LCD4 = 26;              
                    break;
                    
                    case 0x81:  
                      set_imagen(2,6);			
                      teclas2=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto3d;
					  write_LCD(2,'$',teclas2);						//Producto 3
                      flujo_LCD4 = 26;	
                    break;
                    
                    case 0x82:  	
                      set_imagen(2,6); 			
                      teclas2=0;                            	//Inicia el contador de teclas 
					  rventa.producto=producto4d;
					  write_LCD(2,'$',teclas2);					//Otro producto
                      flujo_LCD4 = 26;		
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(2,46);                         //Otras Opciones
                      flujo_LCD4 = 0;     
                    break;
                    
                    case 0x94:
                        set_imagen(2,112);
                        flujo_LCD4 = 14;
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;
                       
        case 26:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD1.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD1.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                               
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior                        
						set_imagen(2,112);
						flujo_LCD4 = 14;						
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2>=4){
                        flujo_LCD4 = 6;                         
                        set_imagen(2,57);	
						Buffer_LCD1.valor[0]=teclas2;					
						if(cambiar_precio(d.dir)!=0){ 	
							if(rventa.producto==producto1){
								write_eeprom(423,Buffer_LCD1.valor);	//429						
							}
                            if(rventa.producto==producto3){
								write_eeprom(435,Buffer_LCD1.valor); //423							
							}
							if(rventa.producto==producto2){
								write_eeprom(429,Buffer_LCD1.valor);	//435						
							}
                            
							
                            if(rventa.producto==producto4){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(2,60);	
							CyDelay(500);														
							set_imagen(2,112);
							flujo_LCD4 = 14;							
						}
						else{
                            set_imagen(2,85);   //Error de operacion
                            CyDelay(500);
                            set_imagen(2,46);
                            flujo_LCD4 = 0;
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
        
        case 27:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                      
                      set_imagen(2,88); 
					  Grado_LCD(2,0x00,0x73,1184); // primer producto 
					  Grado_LCD(2,0x00,0xCF,1186); // segundo producto			  
                      Grado_LCD(2,0x01,0x29,1188); // tercer producto
                      Grado_LCD(2,0x01,0x7B,1190); //cuarto producto
                      flujo_LCD4 = 28;   
                    break;
                    
                    case 0x80: 
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;                      
                    break;
                    
                    case 0x81:  								 //PPU                      
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;	
                    break;
                    
                    case 0x82:  								                      
                        set_imagen(2,43);                      //Cancelado por PC 
                        CyDelay(500);
                        set_imagen(2,112);
                        flujo_LCD4 = 14;  
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,46);
                      flujo_LCD4 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(2,112);
                      flujo_LCD4 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;
        
        case 28:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Producto1	                                                         
                      set_imagen(2,6); 
					  teclas2=0; 
					  rventa.producto=3;
                      flujo_LCD4 = 31;
                    break;
                    
                    case 0x80:  								 //Producto2                      
                      set_imagen(2,6); 	 
					  teclas2=0;
					  rventa.producto=1;
                      flujo_LCD4 = 31;
                    break;
                    
                    case 0x81:  								 //Producto3                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=2;
                      flujo_LCD4 = 31;
                    break;
                    
                    case 0x82:  								 //Producto4                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=4;
                      flujo_LCD4 = 31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,46);
                      flujo_LCD4 = 0;     
                    break;	
                    
                    case 0x94:									
					  set_imagen(2,112);
                      flujo_LCD4 = 14;     //Volver a otras opciones
                    break;
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;	
        
        case 31:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=0){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD4.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD4.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,46);
                        flujo_LCD4 = 0;
                    }
                    else{
                        write_LCD(1,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD4.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==1){                        
                        set_imagen(2,57);	
						Buffer_LCD4.valor[0]=teclas2;
						if(rventa.producto==2){
							producto2d=Buffer_LCD4.valor[1];
							write_eeprom(1186,Buffer_LCD4.valor);							
						}
						if(rventa.producto==1){
							producto1d=Buffer_LCD4.valor[1];
							write_eeprom(1184,Buffer_LCD4.valor);							
						}
						if(rventa.producto==3){
							producto3d=Buffer_LCD4.valor[1];
							write_eeprom(1188,Buffer_LCD4.valor);							
						}
                        if(rventa.producto==4){
							producto4d=Buffer_LCD4.valor[1];
							write_eeprom(1190,Buffer_LCD4.valor);							
						}
						CyDelay(500);						
					    set_imagen(2,112);
                        flujo_LCD4 = 14;												
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
        
        case 33:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3] == 0x0A)
                {
                    for(z=1;z<=(n_copias[0]&0x0F);z++){
                        imprimir(print2[1],grado4,1,d.dir);
                        CyDelay(500);
                    }                    
                    set_imagen(2,12);
                    CyDelay(500);
                    Buffer_LCD4.preset = 0x00;
                    set_imagen(2,46);                    
                    flujo_LCD4=0;
                }
                else{
					set_imagen(2,12);					      // Si presionan NO o cualquier otro touch
				    CyDelay(500);
                    set_imagen(2,46);
                    flujo_LCD4=0;
                }                             
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }	
			 
		break;
       
        
        
    }
}



/*
*********************************************************************************************************
*                                         main( void )
*
* Description : Funcion principal GRP700 Prime, hace el llamado a la configuración del dispensador,
*               variables en general, pantallas y periféricos
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
int main()
{
    init();
    init_surt();  
    init_pantallas();    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    seleccion_pos  = 1; 
    seleccion_pos2 = 3;
    for(;;)
    {
        switch(seleccion_pos){
            case 1:
                polling_pos1();
            break;
                
            case 2:
                polling_pos2();
            break;                            
        }
        switch(seleccion_pos2){
            case 3:
                polling_pos3();
            break;
                
            case 4:
                polling_pos4();
            break;                            
        }
    }
}

/* [] END OF FILE */
