/**
* @file ibuton.c
* @Author Insepet LTDA
* @date 28/2/2016
* @brief Archivo para la lectura del ibutton
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <device.h>
#include "VariablesG.h"
#include "LCD.h"

/**
* touch_present
* @brief indica el lado donde se encuentra el ibutton
* @return 0 no hay ibutton , 1 si hay ibutton
*
*/
uint8 touch_present(uint8 ibutton){  
    uint8 present;
	if(ibutton==1){
	    IB1_Write(0);
	    CyDelayUs(500);
	    IB1_Write(1);
	    CyDelayUs(5);

	    if(!IB1_Read()){
	        return 0;
	    }

	    CyDelayUs(65);
	    present=!IB1_Read();
	    CyDelayUs(240);
	    if(present){
	        return 1;
	    }
	    else{
	        return 0;
	    } 
	}
    if(ibutton==3){
	    IB1_Write(0);
	    CyDelayUs(500);
	    IB1_Write(1);
	    CyDelayUs(5);

	    if(!IB1_Read()){
	        return 0;
	    }

	    CyDelayUs(65);
	    present=!IB1_Read();
	    CyDelayUs(240);
	    if(present){
	        return 1;
	    }
	    else{
	        return 0;
	    } 
	}
    
	else{
	    IB2_Write(0);
	    CyDelayUs(500);
	    IB2_Write(1);
	    CyDelayUs(5);

	    if(!IB2_Read()){
	        return 0;
	    }

	    CyDelayUs(65);
	    present=!IB2_Read();
	    CyDelayUs(240);
	    if(present){
	        return 1;
	    }
	    else{
	        return 0;
	    }	
	}
}

/**
* touch_write
* @brief Escribe un byte en el Ibutton
* @param ibutton lado
* @param dato byte que se va a escribir
* @return 0 operacion correcta , 1 operacion incorrecta
*
*/
uint8 touch_write(uint8 ibutton, uint8 dato){
    uint8 i;
	if(ibutton==1){
	    for(i=0;i<=7;i++){
	        IB1_Write(0); 
	        CyDelayUs(10);
	        if(((dato>>i)&1)){
	           IB1_Write(1);
	           CyDelayUs(10);
	           if(!IB1_Read()){
	                return 0;                
	           }
	        }
	        else{
	            IB1_Write(0); 
	            CyDelayUs(10);
	           if(IB1_Read()){
	                return 0;                
	           }            
	        }
	        CyDelayUs(50);
	        IB1_Write(1);
	        CyDelayUs(50);
	    }
	    return 1;
	}
	else{
	    for(i=0;i<=7;i++){
	        IB2_Write(0); 
	        CyDelayUs(10);
	        if(((dato>>i)&1)){
	           IB2_Write(1);
	           CyDelayUs(10);
	           if(!IB2_Read()){
	                return 0;                
	           }
	        }
	        else{
	            IB2_Write(0); 
	            CyDelayUs(10);
	           if(IB2_Read()){
	                return 0;                
	           }            
	        }
	        CyDelayUs(50);
	        IB2_Write(1);
	        CyDelayUs(50);
	    }
	    return 1;	
	}
}

/**
* touch_read_byte
* @brief Lee un byte del ibutton, se usa para leer los bytes del serial
* @param ibutton lado
* @return 0 no hubo lectura , =!0 dato leido
*
*/
uint8 touch_read_byte(uint8 ibutton){
    uint8 i, dato=0;
	if(ibutton==1){
	    for(i=0;i<=7;i++){
	        IB1_Write(0);    
	        CyDelayUs(14);
	        IB1_Write(1);
	        CyDelayUs(7);
	        dato|=(IB1_Read()<<i);
	        CyDelayUs(100);
	    }
	    return dato;
	}
	else{
	    for(i=0;i<=7;i++){
	        IB2_Write(0);    
	        CyDelayUs(14);
	        IB2_Write(1);
	        CyDelayUs(7);
	        dato|=(IB2_Read()<<i);
	        CyDelayUs(100);
	    }
	    return dato;	
	}
} 

/**
* read_memory_ibutton
* @brief Lee posiciones de memoria del ibutton con más datos
* @param ibutton lado del lector
* @param pos lado del equipo, posición 1 ó 2
* @return 0 no hubo lectura , =!0 dato leido
*
*/
uint8 read_memory_ibutton(uint8 ibutton, uint8 pos){
    uint8 i, check=0;
	if(ibutton==1){
		if(touch_present(1)==1){				
			CyDelay(200);
			if(touch_present(1)==1){
				CyDelay(200);
				touch_write(1,0xCC);
				touch_write(1,0xF0);	
				touch_write(1,pos);
				touch_write(1,0x00);
				if(pos==1){
	                Buffer_LCD1.placa[0]=touch_read_byte(1);
					if((Buffer_LCD1.placa[0]!=0xFF)&&(Buffer_LCD1.placa[0]<=30)){
						check++;
					}	
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD1.placa[0];i++){
		                Buffer_LCD1.placa[i]=touch_read_byte(1);
						if(Buffer_LCD1.placa[i]!=0xFF){
							check++;
						}
					}
					return check;
				}
				else{
	                Buffer_LCD1.cuenta[0]=touch_read_byte(1);
					if((Buffer_LCD1.cuenta[0]!=0xFF)&&(Buffer_LCD1.cuenta[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD1.cuenta[0];i++){
		                Buffer_LCD1.cuenta[i]=touch_read_byte(1);
						if(Buffer_LCD1.cuenta[i]!=0xFF){
							check++;
						}
					}
					return check;				
				}
			}
			else{
				return check;
			}
		}
		else{
			return check;	
		}
	}
    
    if(ibutton==3){
		if(touch_present(3)==1){				
			CyDelay(200);
			if(touch_present(3)==1){
				CyDelay(200);
				touch_write(1,0xCC);
				touch_write(1,0xF0);	
				touch_write(1,pos);
				touch_write(1,0x00);
				if(pos==1){
	                Buffer_LCD2.placa[0]=touch_read_byte(1);
					if((Buffer_LCD2.placa[0]!=0xFF)&&(Buffer_LCD2.placa[0]<=30)){
						check++;
					}	
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD2.placa[0];i++){
		                Buffer_LCD2.placa[i]=touch_read_byte(1);
						if(Buffer_LCD2.placa[i]!=0xFF){
							check++;
						}
					}
					return check;
				}
				else{
	                Buffer_LCD2.cuenta[0]=touch_read_byte(1);
					if((Buffer_LCD2.cuenta[0]!=0xFF)&&(Buffer_LCD2.cuenta[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD2.cuenta[0];i++){
		                Buffer_LCD2.cuenta[i]=touch_read_byte(1);
						if(Buffer_LCD2.cuenta[i]!=0xFF){
							check++;
						}
					}
					return check;				
				}
			}
			else{
				return check;
			}
		}
		else{
			return check;	
		}
	}
    
    if(ibutton==4){
		if(touch_present(2)==1){				
			CyDelay(200);
			if(touch_present(2)==1){
				CyDelay(200);
				touch_write(2,0xCC);
				touch_write(2,0xF0);	
				touch_write(2,pos);
				touch_write(2,0x00);
				if(pos==1){
	                Buffer_LCD4.placa[0]=touch_read_byte(2);
					if((Buffer_LCD4.placa[0]!=0xFF)&&(Buffer_LCD4.placa[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD4.placa[0];i++){
		                Buffer_LCD4.placa[i]=touch_read_byte(2);
						if(Buffer_LCD4.placa[i]!=0xFF){
							check++;
						}
					}
					return check;
				}
				else{
	                Buffer_LCD4.cuenta[0]=touch_read_byte(2);
					if((Buffer_LCD4.cuenta[0]!=0xFF)&&(Buffer_LCD4.cuenta[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD4.cuenta[0];i++){
		                Buffer_LCD4.cuenta[i]=touch_read_byte(2);
						if(Buffer_LCD4.cuenta[i]!=0xFF){
							check++;
						}
					}
					return check;				
				}
			}
			else{
				return check;
			}
		}
		else{
			return check;	
		}	
	}
    
	else{
		if(touch_present(2)==1){				
			CyDelay(200);
			if(touch_present(2)==1){
				CyDelay(200);
				touch_write(2,0xCC);
				touch_write(2,0xF0);	
				touch_write(2,pos);
				touch_write(2,0x00);
				if(pos==1){
	                Buffer_LCD3.placa[0]=touch_read_byte(2);
					if((Buffer_LCD3.placa[0]!=0xFF)&&(Buffer_LCD3.placa[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD3.placa[0];i++){
		                Buffer_LCD3.placa[i]=touch_read_byte(2);
						if(Buffer_LCD3.placa[i]!=0xFF){
							check++;
						}
					}
					return check;
				}
				else{
	                Buffer_LCD3.cuenta[0]=touch_read_byte(2);
					if((Buffer_LCD3.cuenta[0]!=0xFF)&&(Buffer_LCD3.cuenta[0]<=30)){
						check++;
					}
					else{
						return 0;
					}
					for(i=1;i<=Buffer_LCD3.cuenta[0];i++){
		                Buffer_LCD3.cuenta[i]=touch_read_byte(2);
						if(Buffer_LCD3.cuenta[i]!=0xFF){
							check++;
						}
					}
					return check;				
				}
			}
			else{
				return check;
			}
		}
		else{
			return check;	
		}	
	}
}

/**
* touch_write_memoria
* @brief escribe posiciones de memoria del ibutton con más datos (placa)
* @param ibutton lado del lector
* @param cantidad número de datos a escribir
* @param pos lado del equipo, posición 1 ó 2
* @param *valor apuntador con los datos a escribir
* @return 0 no escribió , 1 escrito correctamente
*
*/
uint8 touch_write_memoria(uint8 ibutton, uint8 cantidad, uint8 pos, uint8 *valor){
	uint8 i,check=0;
	if(ibutton==1){
		if(touch_present(1)==1){
			CyDelay(200);
	        touch_write(1,0xCC);
			touch_write(1,0x0F);
			touch_write(1,pos);
			touch_write(1,0x00);
			for(i=0;i<=cantidad;i++){
				touch_write(1,valor[i]);
			}
			CyDelay(750);
			if(touch_present(1)==1){
				CyDelay(200);
				touch_write(1,0xCC);
				touch_write(1,0xAA);	
				for(x=0;x<=cantidad+2;x++){
	                Buffer_LCD1.id[x]=touch_read_byte(1);
				}
				CyDelay(750);
				if(touch_present(1)==1){
					CyDelay(200);
					touch_write(1,0xCC);
					touch_write(1,0x55);	
					touch_write(1,pos);
					touch_write(1,0x00);
					touch_write(1,Buffer_LCD1.id[2]);					
					CyDelay(750);
					if(touch_present(1)==1){
						CyDelay(200);
						touch_write(1,0xCC);
						touch_write(1,0xF0);	
						touch_write(1,pos);
						touch_write(1,0x00);
						for(z=0;z<=cantidad;z++){
			                Buffer_LCD1.id[z]=touch_read_byte(1);
							if(Buffer_LCD1.id[z]==Buffer_LCD1.valor[z]){
								check++;	
							}									
						}
						if(check==(cantidad+1)){
							return 1;
						}
					}	
				}	
			}			
	    }	
		else{
			return 0;
		}
	}
	else{
		if(touch_present(2)==1){
			CyDelay(200);
	        touch_write(2,0xCC);
			touch_write(2,0x0F);
			touch_write(2,pos);
			touch_write(2,0x00);
			for(i=0;i<=cantidad;i++){
				touch_write(2,valor[i]);
			}
			CyDelay(750);
			if(touch_present(2)==1){
				CyDelay(200);
				touch_write(2,0xCC);
				touch_write(2,0xAA);	
				for(x=0;x<=cantidad+2;x++){
	                Buffer_LCD2.id[x]=touch_read_byte(2);
				}
				CyDelay(750);
				if(touch_present(2)==1){
					CyDelay(200);
					touch_write(2,0xCC);
					touch_write(2,0x55);	
					touch_write(2,pos);
					touch_write(2,0x00);
					touch_write(2,Buffer_LCD2.id[2]);					
					CyDelay(750);
					if(touch_present(2)==1){
						CyDelay(200);
						touch_write(2,0xCC);
						touch_write(2,0xF0);	
						touch_write(2,pos);
						touch_write(2,0x00);
						for(z=0;z<=cantidad;z++){
			                Buffer_LCD2.id[z]=touch_read_byte(2);
							if(Buffer_LCD2.id[z]==Buffer_LCD2.valor[z]){
								check++;	
							}									
						}
						if(check==(cantidad+1)){
							return 1;
						}
					}	
				}	
			}			
	    }	
		else{
			return 0;
		}	
	}
	return 0;
}

/**
* crc_check
* @brief verifica los datos leídos del ibutton
* @param crc dato obtenido
* @param dato byte de ibutton
* @return crc calculado
*
*/
uint8 crc_check(uint8 crc, uint8 dato){
    uint8 i;
    crc = crc ^ dato;
    for (i = 0; i < 8; i++)
    {
        if (crc & 0x01){
            crc = (crc >> 1) ^ 0x8C;
		}	
        else{
            crc >>= 1;
		}	
    }
    return crc;
}


/* [] END OF FILE */
