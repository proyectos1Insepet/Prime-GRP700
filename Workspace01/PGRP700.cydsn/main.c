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
//#include "Print.h"
//#include "Sonido.h"
#include "LCD.h"
//#include "ibutton.h"
#include "I2C.h"
#include <project.h>




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
* Description : Verifica el serial, inicia los perosfericos, la version y los datos de la estación.
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
	/****Inicio de perifericos****/
    CyGlobalIntEnable;
    Surtidor_Start();
    //Impresora_Start();
    LCD_1_Start();
    LCD_2_Start();
    Surtidor_EnableRxInt();    
    //I2C_1_Start();    
    //Impresora_EnableRxInt();
    LCD_1_EnableRxInt();
    //LCD_2_EnableRxInt();       
    CyDelay(5);	
    a_copias = 0;
    
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
		    if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    switch(LCD_1_rxBuffer[3]){
				        case 0x56:						//POS A
                            set_imagen(1,99);           //Inicio opciones GRP 700
                            seleccion_pos = 1;
					        flujo_LCD  = 1;
                            flujo_LCD2 = 0;
					    break;
                            		       
					    case 0x57:  				    //POS B
                            set_imagen(1,99);           //Inicio opciones GRP 700
							seleccion_pos = 2;	
                            flujo_LCD  = 0;
                            flujo_LCD2 = 1;
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
                            flujo_LCD = 2;
                        break;
                    
                        case 0x7E:
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
                    case 0x5F:								 	 //Sin ID	                                                                              
                      set_imagen(1,11);                       
                      flujo_LCD=10; 
                    break;
                    
                    case 0x5E:  								//Con ID                                         
                      set_imagen(1,29);
                      flujo_LCD=11;
                    break;	

                    case 0x7E:									//ir a menu
					  set_imagen(1,46);	
                      flujo_LCD=0;     
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
			if((Buffer_LCD1.preset&0x02)==0x02||(Buffer_LCD1.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(a.dir,producto1,Buffer_LCD1.valor,(Buffer_LCD1.preset&0x03))==0){
					set_imagen(1,46);
					flujo_LCD=0;
					break;
				}					
			}			
			grado1=estado_ex(a.dir);			
			Surtidor_PutChar(0x10|a.dir);									//Autoriza el surtidor
			set_imagen(1,8);
            flujo_LCD=7;				
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(a.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(a.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);
                    CyDelay(20);
                    flujo_LCD=13;
				}
                if(venta(a.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD=0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(a.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);
                    CyDelay(20);               //Finaliza venta con impresión de recibo
                    flujo_LCD=13;
				}
                if(venta(a.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD=0;
				}
			break;

	        case 0x06: 
                set_imagen(1,46); //No hizo venta
				flujo_LCD=0;
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
                if(LCD_1_rxBuffer[3]==0x0C){                                        //Enter pasa a imprimir
                    if(teclas1>=1){ 
                        set_imagen(1,5);
                        Buffer_LCD1.placa[0]=teclas1;                                                
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
		    if(LCD_1_GetRxBufferSize()==8){
                if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                    switch(LCD_1_rxBuffer[3]){
				        case 0x56:						//POS A
                            set_imagen(1,99);           //Inicio opciones GRP 700
                            seleccion_pos = 1;
					        flujo_LCD  = 1;
                            flujo_LCD2 = 0;
					    break;
                            		       
					    case 0x57:  				    //POS B
                            set_imagen(1,99);           //Inicio opciones GRP 700
							seleccion_pos = 2;	
                            flujo_LCD  = 0;
                            flujo_LCD2 = 1;
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
                    
                        case 0x7E:
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
                            flujo_LCD2 = 5;   
                        break;
                    
                        case 0x10:  	                         //Inicia el contador de teclas
                            set_imagen(1,13);              
                            teclas1=0;       
                            comas1=0;
                            write_LCD(1,'G',0);
					        Buffer_LCD1.preset|=1;				 // Preset por volumen
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
			if((Buffer_LCD1.preset&0x02)==0x02||(Buffer_LCD1.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(b.dir,producto1,Buffer_LCD1.valor,(Buffer_LCD1.preset&0x03))==0){
					set_imagen(1,46);
					flujo_LCD2 = 0;
					break;
				}					
			}			
			grado1=estado_ex(b.dir);			
			Surtidor_PutChar(0x10|b.dir);									//Autoriza el surtidor
			set_imagen(1,8);
            flujo_LCD2 = 7;				
			}
         
        break;
            
        case 7:
		 CyDelay(50);
		 switch(get_estado(b.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(b.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);
                    CyDelay(20);
                    flujo_LCD2 = 13;
				}
                if(venta(b.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD2 = 0;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(b.dir)==1 && no_imprime == 0){	
		            set_imagen(1,57);
                    CyDelay(20);               //Finaliza venta con impresión de recibo
                    flujo_LCD2 = 13;
				}
                if(venta(b.dir)==1 && no_imprime == 1){	
		            set_imagen(1,12);         //Finaliza venta sin impresión de recibo
                    CyDelay(500);
                    set_imagen(1,46);
                    flujo_LCD2 = 0;
				}
			break;

	        case 0x06: 
                set_imagen(1,46); //No hizo venta
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
                        Buffer_LCD1.placa[0]=teclas1;                                                
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
                        no_imprime = 0;
                        flujo_LCD2 = 9;                       
                    break; 
                    
                    case 0x38:                          //Pide venta sin impresión
                        set_imagen(1,5);	            //Pasa a preset de venta
                        no_imprime = 1;
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
    seleccion_pos2 = 1;
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
        
        
       
        
    }
}

/* [] END OF FILE */
