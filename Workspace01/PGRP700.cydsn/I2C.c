/**
* @file I2C.c
* @Author Insepet LTDA
* @date 28/2/2016
* @brief Archivo principal, maneja librerías y ejecuta el polling de pantallas y dispensador
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "VariablesG.h"
#include "LCD.h"

/**
* @fn leer_hora
* @brief Función para leer la hora vía I2C, escribe la hora solicitada en rventa.hora
* @return 0 para error de lectura 1 si no hubo problema al leer la hora
*/
uint8 leer_hora(){
	uint8 status,i;
	for(i=1;i<=2;i++){
        I2C_1_MasterClearStatus();
        status = I2C_1_MasterSendStart(0x68, I2C_1_WRITE_XFER_MODE);
        if(I2C_1_MSTR_NO_ERROR == status)								 		/* Check if transfer completed without errors */
    	{
            status = I2C_1_MasterWriteByte(i);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
			else{
		        I2C_1_MasterSendStop(); 										/* Send Stop */
		        CyDelay(10);
		        status = I2C_1_MasterSendStart(0x68, I2C_1_READ_XFER_MODE);
		        if(I2C_1_MSTR_NO_ERROR == status){
		            rventa.hora[i-1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
		        }
				else{
					return 0;
				}
		        I2C_1_MasterSendStop();	
			}
        }
		else{
			return 0;
		}		
	}
	return 1;
}

/**
* @fn leer_fecha
* @brief Función para leer la hora vía I2C, escribe la hora solicitada en rventa.fecha
* @return 0 para error de lectura 1 si no hubo problema al leer la fecha
*/
uint8 leer_fecha(){
	uint8 status,i;
	for(i=4;i<=6;i++){
        I2C_1_MasterClearStatus();
        status = I2C_1_MasterSendStart(0x68, I2C_1_WRITE_XFER_MODE);
        if(I2C_1_MSTR_NO_ERROR == status)								 		/* Check if transfer completed without errors */
    	{
            status = I2C_1_MasterWriteByte(i);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
			else{
		        I2C_1_MasterSendStop(); 										/* Send Stop */
		        CyDelay(10);
		        status = I2C_1_MasterSendStart(0x68, I2C_1_READ_XFER_MODE);
		        if(I2C_1_MSTR_NO_ERROR == status){
		            rventa.fecha[i-4] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
		        }
				else{
					return 0;
				}
		        I2C_1_MasterSendStop();	
			}
        }
		else{
			return 0;
		}		
	}
	return 1;
}

/**
* write_hora
* @brief Función para escribir la hora vía I2C, escribe la hora de rventa.hora
* @return 0 para error de escritura 1 si no hubo problema al escribir la hora
*/

uint8 write_hora( void ){
	uint8 status, dato[3],i;
	dato[0]=1;
	dato[1]=rventa.hora[0];
	dato[2]=rventa.hora[1];
	I2C_1_MasterClearStatus();
    status = I2C_1_MasterSendStart(0x68, I2C_1_WRITE_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status) 
    {
        for(i=0; i<3; i++)
        {
            status = I2C_1_MasterWriteByte(dato[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
        }
    }
    else{
		return 0;
    }
    I2C_1_MasterSendStop();	
	return 1;
}


/**
* @fn write_fecha
* @brief Función para escribir la fecha vía I2C, escribe la hora de rventa.fecha
* @return 0 para error de escritura 1 si no hubo problema al escribir la fecha
*/
uint8 write_fecha( void ){
	uint8 status, dato[4],i;
	dato[0]=4;
	dato[1]=rventa.fecha[0];
	dato[2]=rventa.fecha[1];
	dato[3]=rventa.fecha[2];
	I2C_1_MasterClearStatus();
    status = I2C_1_MasterSendStart(0x68, I2C_1_WRITE_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status) 
    {
        for(i=0; i<4; i++)
        {
            status = I2C_1_MasterWriteByte(dato[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
        }
    }
    else{
		return 0;
    }
    I2C_1_MasterSendStop();	
	return 1;
}

/**
* write_eeprom
* @brief Función para escribir la eeprom externa
* @param page pagina de eeprom externa que se va a escribir
* @param *valor apuntador al valor que se va a escribir en eeprom
* @return 0 para error de escritura 1 si no hubo problema al escribir 
*/
uint8 write_eeprom(uint16 page, uint8 *valor){
	uint8 status, dato[4],i;
	dato[1]=(page&0xFF);
	page=(page>>8)&0xFF;
	dato[0]=page;
	Pin_WP_Write(0);
	I2C_1_MasterClearStatus();
    status = I2C_1_MasterSendStart(0x54, I2C_1_WRITE_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status) 
    {
        for(i=0; i<2; i++)
        {
            status = I2C_1_MasterWriteByte(dato[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
        }
        for(i=0; i<=valor[0]; i++)
        {
            status = I2C_1_MasterWriteByte(valor[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
			CyDelay(5);
        }		
    }
    else{
		return 0;
    }
    I2C_1_MasterSendStop();	
	CyDelay(6);
	Pin_WP_Write(1);
	return 1;
}

/**
* leer_eeprom
* @brief Función para leer datos de eeprom externa
* @param page espacio en eeprom que se va a leer
* @param size tamaño del vector que se lee
* @return 0 para error de lectura 1 si no hubo problema 
*/
uint8 leer_eeprom(uint16 page, uint8 size){
	uint16 status, dato[4],i;
	dato[1]=(page&0xFF);
	page=(page>>8)&0xFF;
	dato[0]=page;	
    I2C_1_MasterClearStatus();
    status = I2C_1_MasterSendStart(0x54, I2C_1_WRITE_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status)								 		/* Check if transfer completed without errors */
	{
        for(i=0;i<2;i++)
        {
            status = I2C_1_MasterWriteByte(dato[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
            	return 0;
            }
			CyDelay(5);
        }		
    }
	else{
		return 0;
	}
	I2C_1_MasterSendStop();	
    CyDelay(50);
    status = I2C_1_MasterSendStart(0x54, I2C_1_READ_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status){
		for(i=0;i<size;i++){
	        buffer_i2c[i] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
			CyDelay(5);
		}
    }
	else{
		return 0;
	}
    I2C_1_MasterSendStop();	
	CyDelay(6);	
	return 1;
}

/**
* write_psoc1
* @brief Función para enviar datos al micro encargado de controlar las impresoras
* @param puerto puerto al que se desea enviar los datos (impresora lado A, impresora lado B)
* @param valor dato a enviar
* @return 0 para error de lectura 1 si no hubo problema 
*/
uint8 write_psoc1( uint8 puerto, uint8 valor ){
	uint8 status, dato[2],i;
	dato[0]=puerto;
	dato[1]=valor;
	I2C_1_MasterClearStatus();
    status = I2C_1_MasterSendStart(7u, I2C_1_WRITE_XFER_MODE);
    if(I2C_1_MSTR_NO_ERROR == status) 
    {
        for(i=0; i<2; i++)
        {
            status = I2C_1_MasterWriteByte(dato[i]);
            if(status != I2C_1_MSTR_NO_ERROR)
            {
                return 0;
            }
        }
    }
    else{
		return 0;
    }
    I2C_1_MasterSendStop();	
	return 1;
}

/* [] END OF FILE */
