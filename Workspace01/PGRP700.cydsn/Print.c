/*
*********************************************************************************************************
*                                           GRP500 CODE
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
*                                               GRP500 CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : Print.c
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <device.h>
#include "VariablesG.h"
#include "I2C.h"
#include "Protocolo.h"
#include "Logos.h"

/*
*********************************************************************************************************
*                                            MENSAJES
*********************************************************************************************************
*/
uint8 msn_eds[20]="ESTACION DE SERVICIO";
uint8 msn_nit[4]="NIT ";
uint8 msn_tel[4]="TEL:";
uint8 msn_numero[8]="Numero: ";
uint8 msn_fecha[7]="Fecha:";
uint8 msn_hora[7]="Hora:  ";
uint8 msn_pos[10]="Posicion: ";
uint8 msn_ppu[14]="PPU:      $/G ";
uint8 msn_vol[13]="Volumen:  G ";
uint8 msn_din[13]="Dinero:   $ ";
uint8 msn_placa[10]="Placa:    ";
uint8 msn_cuenta[11]="Cuenta:    ";
uint8 msn_km[11]="Km:        ";
uint8 msn_id[11]="Id:        ";
uint8 msn_copia[6]="COPIA ";
uint8 msn_producto[10]="Producto: ";
uint8 msn_fcorte[16]="Fecha corte: ";
uint8 msn_hcorte[16]="Hora corte:  ";
uint8 msn_idcorte[16]="Consecutivo:    ";
uint8 msn_ufcorte[24]="Ultima Fecha corte:";
uint8 msn_uhcorte[24]="Ultima Hora corte:  ";
uint8 msn_consecutivo[24]="Ultima Hora corte: ";


/*
*********************************************************************************************************
*                             void operacion(uint8 tipo, uint8 *valor1, uint8 *valor2)
*
* Description : Realiza una suma o resta entre dos punteros y lo guarda en el vector global resultado
*               
*
* Argument(s) : uint8 tipo 1 Suma 2 Resta
				uint8 *valor1 = Operador uno (En el caso de la resta es el mayor)
				uint8 *valor2 = Operador dos
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : Se usa para hacer operaciones entre la tramas enviadas por el surtidor.
*********************************************************************************************************
*/

void operacion(uint8 tipo, uint8 *valor1, uint8 *valor2){
	uint8 carrier,i;
	uint8 max_valor[12]={9,9,9,9,9,9,9,9,9,9,9,9};
	carrier=0;
	if(tipo==1){
		for(i=1;i<=valor1[0];i++){
			resultado[i]=valor1[i]+valor2[i]+carrier;
			if(resultado[i]>9){
				resultado[i]=resultado[i]-10;
				carrier=1;
			}
			else{
				carrier=0;
			}
		}
		if(carrier==1){
			resultado[i+1]=1;
			resultado[0]=valor1[0]+1;
		}
		else{
			resultado[0]=valor1[0];
		}
	}
	if(tipo==2){
		if(valor1[valor1[0]]<valor2[valor1[0]]){
			for(i=1;i<=valor1[0];i++){
				if((max_valor[i]-carrier)<valor2[i]){
					resultado[i]=((max_valor[i]+10)-carrier)-valor2[i];
					carrier=1;			
				}
				else{
					resultado[i]=(max_valor[i]-carrier)-valor2[i];
					carrier=0;
				}
			}
			resultado[0]=valor1[0];	
			for(i=1;i<=valor1[0];i++){
				resultado[i]=valor1[i]+resultado[i]+carrier;
				if(resultado[i]>9){
					resultado[i]=resultado[i]-10;
					carrier=1;
				}
				else{
					carrier=0;
				}
			}
			if(carrier==1){
				resultado[i+1]=1;
				resultado[0]=valor1[0]+1;
			}
			else{
				resultado[0]=valor1[0];
			}			
		}
		else{
			for(i=1;i<=valor1[0];i++){
				if((valor1[i]-carrier)<valor2[i]){
					resultado[i]=((valor1[i]+10)-carrier)-valor2[i];
					carrier=1;			
				}
				else{
					resultado[i]=(valor1[i]-carrier)-valor2[i];
					carrier=0;
				}
			}
			resultado[0]=valor1[0];
		}
	}
}

/*
*********************************************************************************************************
*                          				void print_logo_k(uint8 val, uint8 logo)
*
* Description : Envia por I2C los datos de los vectores de los logos. Usando impresora Kiosco
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
void print_logo_k(uint8 val, uint8 logo){
	uint16 i;
	switch(logo){
        case 0:
        break;
		case 15:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,biomax[i]);			
			}
		break;

		case 14:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,brio[i]);			
			}
		break;			

		case 7:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,cencosud[i]);			
			}
		break;	
			
		case 10:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,ecospetrol[i]);			
			}
		break;

		case 4:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,esso[i]);			
			}
		break;
			
		case 3:
			for(i=0;i<=944;i++){
        		write_psoc1(val,exito[i]);			
			}
		break;
			
		case 9:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,National[i]);			
			}
		break;	
			
		case 8:
			for(i=0;i<=944;i++){
        		write_psoc1(val,mineroil[i]);			
			}
		break;	
			
		case 5:
			for(i=0;i<=944;i++){
        		write_psoc1(val,mobil[i]);			
			}
		break;	
			
		case 13:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,petrobras[i]);			
			}
		break;
			
		case 12:
			for(i=0;i<=755;i++){
        		write_psoc1(val,plus[i]);			
			}
		break;	
			
		case 2:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,terpel[i]);			
			}
		break;
			
		case 1:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,texaco[i]);			
			}
		break;	
			
		case 6:
			for(i=0;i<=755;i++){
        		write_psoc1(val,zeus[i]);			
			}
		break;
			
		case 11:
			for(i=0;i<=1511;i++){
        		write_psoc1(val,petromil[i]);			
			}
		break;		
	}	
	write_psoc1(val,10);
	write_psoc1(val,0x1D);
	write_psoc1(val,0x4C);
	write_psoc1(val,0x07);
	write_psoc1(val,0x00);
	write_psoc1(val,10);
}



/*
*********************************************************************************************************
*                          				void print_logo_p(uint8 val, uint8 logo)
*
* Description : Envia por I2C los datos de los vectores de los logos. Para impresora panel
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
void print_logo_p(uint8 val, uint8 logo){
	uint16 i;
	switch(logo){
        
        case 0:
        break;
        
		case 15:
			for(i=0;i<=943;i++){
        		write_psoc1(val,biomax1[i]);			
			}
		break;

		case 14:
			for(i=0;i<=943;i++){
        		write_psoc1(val,brio1[i]);			
			}
		break;			

		case 7:
			for(i=0;i<=943;i++){
        		write_psoc1(val,cencosud1[i]);			
			}
		break;	
			
		case 10:
			for(i=0;i<=943;i++){
        		write_psoc1(val,ecospetrol1[i]);			
			}
		break;

		case 4:
			for(i=0;i<=943;i++){
        		write_psoc1(val,esso1[i]);			
			}
		break;
			
		case 3:
			for(i=0;i<=943;i++){
        		write_psoc1(val,exito[i]);			
			}
		break;
			
		case 9:
			for(i=0;i<=943;i++){
        		write_psoc1(val,National1[i]);			
			}
		break;	
			
		case 8:
			for(i=0;i<=943;i++){
        		write_psoc1(val,mineroil1[i]);			
			}
		break;	
			
		case 5:
			for(i=0;i<=943;i++){
        		write_psoc1(val,mobil1[i]);			
			}
		break;	
			
		case 13:
			for(i=0;i<=943;i++){
        		write_psoc1(val,petrobras1[i]);			
			}
		break;
			
		case 12:
			for(i=0;i<=943;i++){
        		write_psoc1(val,plus1[i]);			
			}
		break;	
			
		case 2:
			for(i=0;i<=943;i++){
        		write_psoc1(val,terpel1[i]);			
			}
		break;
			
		case 1:
			for(i=0;i<=943;i++){
        		write_psoc1(val,texaco1[i]);			
			}
		break;	
			
		case 6:
			for(i=0;i<=943;i++){
        		write_psoc1(val,zeus1[i]);			
			}
		break;
			
		case 11:
			for(i=0;i<=943;i++){
        		write_psoc1(val,petromil1[i]);			
			}
		break;		
	}	
	write_psoc1(val,0x1B);
	write_psoc1(val,0x40);
	write_psoc1(val,10);
	write_psoc1(val,10);
}


/*
*********************************************************************************************************
*                                       void print_totales(void)
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

void print_totales(uint8 val){
	uint8 digito;
    write_psoc1(val,10);
	for(x=0;x<=12;x++){											//DINERO							
		write_psoc1(val,msn_din[x]);
	}
	digito=0;
	for(x=buffer_i2c[0];x>(buffer_i2c[0]/2);x--){
        if((buffer_i2c[x]==0)&&(digito==0)){
            
        }
        else{
            digito=1;
            if(x==((buffer_i2c[0]/2)+decimalD)){
				write_psoc1(val,44);  ///decimal total 
			}
            write_psoc1(val,buffer_i2c[x]+48);
        }
	}
	write_psoc1(val,10);
	for(x=0;x<=12;x++){											//VOLUMEN							
		write_psoc1(val,msn_vol[x]);
	}
	digito=0;
	for(x=(buffer_i2c[0]/2);x>=1;x--){
        if((buffer_i2c[x]==0)&&(digito==0)){
            
        }
        else{
            digito=1;
			if(x==2){
				write_psoc1(val,44);
			}
            write_psoc1(val,buffer_i2c[x]+48);
        }
	}
	write_psoc1(val,10);
	write_psoc1(val,10);
}

/*
*********************************************************************************************************
*                               void print_totalesac(uint8 digitos)
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

void print_totalesac(uint8 digitos, uint8 val){
	uint8 digito,i;
    write_psoc1(val,10);
	for(x=0;x<=12;x++){											//DINERO							
		write_psoc1(val,msn_din[x]);
	} 
	digito=0;
	for(i=digitos;i>=1;i--){
        if((rventa.tdinero[i]==0)&&(digito==0)){
            
        }
        else{
            digito=1;
            write_psoc1(val,rventa.tdinero[i]+48);
            if(i==(decimalD)){
				write_psoc1(val,44);  ///decimal total AC
			}
        }
	}
	write_psoc1(val,rventa.tdinero[0]+48);
	write_psoc1(val,10);
	for(x=0;x<=12;x++){											//VOLUMEN							
		write_psoc1(val,msn_vol[x]);
	}
	digito=0;
	for(i=digitos;i>=1;i--){
        if((rventa.tvolumen[i]==0)&&(digito==0)){
            
        }
        else{
            digito=1;
			if(i==1){
				write_psoc1(val,44);
			}
            write_psoc1(val,rventa.tvolumen[i]+48);
        }
	}
	write_psoc1(val,rventa.tvolumen[0]+48);
	write_psoc1(val,10);
	write_psoc1(val,10);
}

/*
*********************************************************************************************************
*                               void print_valor(uint8 *valor)
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

void print_valor(uint8 *valor, uint8 val){
	uint8 digito,i;
	digito=0;
	for(i=valor[0];i>=1;i--){
        if((valor[i]==0)&&(digito==0)){
            
        }
        else{
            digito=1;
            if(i==(decimalD)){
				write_psoc1(val,44);  ///decimal total AC
			}
            write_psoc1(val,valor[i]+48);
        }
	}
    
	write_psoc1(val,10);
	write_psoc1(val,10);
}

/*
*********************************************************************************************************
*                                         void imprimir(uint8 val)
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

void imprimir(uint8 val, uint8 producto, uint8 copia, uint8 pos){
    uint8 digito;
    leer_eeprom(1012,2);   ///Carga tipo de impresora
    if(buffer_i2c[0]==1){    
	    tipo_imp[0] = buffer_i2c[0];
        tipo_imp[1] = (buffer_i2c[1]&0x0f);
    }
	if(bandera[0]==1){
        if (tipo_imp[1] == 1){
		    print_logo_k(val,bandera[1]);
        }
        if (tipo_imp[1] == 2){
		    print_logo_p(val,bandera[1]);
        }
	}
	for(x=0;x<=((31-20)/2);x++){
		write_psoc1(val,0x20);			
	}		
	for(x=0;x<=19;x++){										//ESTACION DE SERVICIO
		write_psoc1(val,msn_eds[x]);
	}
    write_psoc1(val,10);
	for(x=0;x<=((31-rventa.nombre[0])/2);x++){				//NOMBRE
		write_psoc1(val,0x20);		
	}
	for(x=1;x<=rventa.nombre[0];x++){
		write_psoc1(val,rventa.nombre[x]);			
	}		
    write_psoc1(val,10);
	if(rventa.nit[0]>=1){
		for(x=0;x<=((31-(rventa.nit[0]+4))/2);x++){			//NIT
			write_psoc1(val,0x20);			
		}		
		for(x=0;x<=3;x++){									
			write_psoc1(val,msn_nit[x]);
		}
		for(x=1;x<=rventa.nit[0];x++){
			write_psoc1(val,rventa.nit[x]);			
		}
		write_psoc1(val,10);
	}
	if(rventa.telefono[0]>=1){
		for(x=0;x<=((31-(rventa.telefono[0]+4))/2);x++){	//TELEFONO
			write_psoc1(val,0x20);			
		}		
		for(x=0;x<=3;x++){									
			write_psoc1(val,msn_tel[x]);
		}
		for(x=1;x<=rventa.telefono[0];x++){
			write_psoc1(val,rventa.telefono[x]);			
		}			
	    write_psoc1(val,10);
	    write_psoc1(val,10);
	}
	for(x=0;x<=((31-rventa.direccion[0])/2);x++){			//DIRECCION
		write_psoc1(val,0x20);			
	}
	for(x=1;x<=rventa.direccion[0];x++){
		write_psoc1(val,rventa.direccion[x]);			
	}
    write_psoc1(val,10);
    write_psoc1(val,10);
    write_psoc1(val,10);
    	for(x=0;x<=7;x++){										//NUMERO DE VENTA							
		write_psoc1(val,msn_numero[x]);
	}
        
	if(copia==0){
		no_venta++;
	}	
	id_venta[5]=(no_venta/10000)+48;
	id_venta[4]=((no_venta%10000)/1000)+48;
	id_venta[3]=(((no_venta%10000)%1000)/100)+48;
	id_venta[2]=((((no_venta%10000)%1000)%100)/10)+48;	
	id_venta[1]=((((no_venta%10000)%1000)%100)%10)+48;
	for(x=5;x>=1;x--){										//NUMERO DE VENTA							
		write_psoc1(val,id_venta[x]);
	}	
	write_eeprom(978,id_venta);
    
	write_psoc1(val,10);
	for(x=0;x<=6;x++){										//FECHA								
		write_psoc1(val,msn_fecha[x]);
	}	
	if(leer_fecha()==1){
		write_psoc1(val,(((rventa.fecha[0]&0x30)>>4)+48));
		write_psoc1(val,((rventa.fecha[0]&0x0F)+48));
		write_psoc1(val,'/');
		write_psoc1(val,(((rventa.fecha[1]&0x10)>>4)+48));
		write_psoc1(val,((rventa.fecha[1]&0x0F)+48));	
		write_psoc1(val,'/');
		write_psoc1(val,(((rventa.fecha[2]&0xF0)>>4)+48));
		write_psoc1(val,((rventa.fecha[2]&0x0F)+48));			
	}			
    write_psoc1(val,10);
	for(x=0;x<=6;x++){									
		write_psoc1(val,msn_hora[x]);
	}
	if(leer_hora()==1){										//HORA
		write_psoc1(val,(((rventa.hora[1]&0x10)>>4)+48));
		write_psoc1(val,((rventa.hora[1]&0x0F)+48));
		write_psoc1(val,':');
		write_psoc1(val,(((rventa.hora[0]&0xF0)>>4)+48));
		write_psoc1(val,((rventa.hora[0]&0x0F)+48));
		if((rventa.hora[1]&0x20)==0x20){
			write_psoc1(val,'p');
		}
		else{
			write_psoc1(val,'a');						
		}			
		write_psoc1(val,'m');
	}
	if(copia==1){											//COPIA
	    write_psoc1(val,10);
	    write_psoc1(val,10); 
		for(w=0;w<=((31-7)/2);w++){
			write_psoc1(val,0x20);			
		}		
		for(x=0;x<=5;x++){									
			write_psoc1(val,msn_copia[x]);
		}
		write_psoc1(val,(z+48));
	}
    write_psoc1(val,10);
    write_psoc1(val,10);
	for(x=0;x<10;x++){										//POSICION								
		write_psoc1(val,msn_pos[x]);
	}
    write_psoc1(val,((pos/10)+48));
	write_psoc1(val,((pos%10)+48));
    write_psoc1(val,10); 
	for(x=0;x<10;x++){										//PRODUCTO								
		write_psoc1(val,msn_producto[x]);
	}
    if(((producto==(producto3&0x0f))&&(pos==a.dir))||((producto==(producto3b&0x0f))&&(pos==b.dir))){
		for(x=0;x<10;x++){																			
			write_psoc1(val,producto3n[x]);
		}
    }
    if(((producto==(producto2&0x0f))&&(pos==a.dir))||((producto==(producto2b&0x0f))&&(pos==b.dir))||((producto==(producto2&0x0f))&&(pos==c.dir))||((producto==(producto2b&0x0f))&&(pos==d.dir))){
		for(x=0;x<10;x++){																		
			write_psoc1(val,producto2n[x]);
		} 
    }
    if(((producto==(producto1&0x0f))&&(pos==a.dir))||((producto==(producto1b&0x0f))&&(pos==b.dir))||((producto==(producto1b&0x0f))&&(pos==d.dir))||((producto==(producto1&0x0f))&&(pos==c.dir))){
		for(x=0;x<10;x++){																			
			write_psoc1(val,producto1n[x]);
		} 
    }
    if(((producto==(producto4&0x0f))&&(pos==a.dir))||((producto==(producto4b&0x0f))&&(pos==b.dir))){
		for(x=0;x<10;x++){																			
			write_psoc1(val,producto4n[x]);
		} 
    }
    write_psoc1(val,10);
    
	for(x=0;x<14;x++){											//PPU								
		write_psoc1(val,msn_ppu[x]);
	}
    digito=0;
    
	if(pos==a.dir){
		leer_eeprom(475,7);
	}                             //Seleción de cara PPU
	else{
		leer_eeprom(500,7);	
	}
    
	for(x=buffer_i2c[0];x>=1;x--){
        if((buffer_i2c[x]==0)&&(digito==0)){
        }
        else{
            digito=1;
            write_psoc1(val,(buffer_i2c[x]+48));
            if(decimalD >= 1 && ppux10 == 0)
            {
                if(x==decimalD+1){
		            write_psoc1(val,44);
		        }
            }
            
        }
	}
	if(ppux10==1){
		write_psoc1(val,48);	
	}
    write_psoc1(val,10);
	for(x=0;x<=12;x++){											//VOLUMEN							
		write_psoc1(val,msn_vol[x]);
	}
	if(pos==a.dir){
		leer_eeprom(466,9);
	}
	else{
		leer_eeprom(491,9);	
	}
    for(x=buffer_i2c[0];x>=5;x--){
        if(buffer_i2c[x]!=0){
			
            break;
        }
    }
    for(y=x;y>0;y--){
        if(y==decimalV){
            write_psoc1(val,44);
        }
        write_psoc1(val,(buffer_i2c[y]+48));
    }
    write_psoc1(val,10);
    
    
	for(x=0;x<=12;x++){											//DINERO							
		write_psoc1(val,msn_din[x]);
	} 
    digito=0;
	if(pos==a.dir){
		leer_eeprom(457,9);
	}
	else{
		leer_eeprom(482,9);	
	}
	if((versurt==5)||(versurt==7)){
		for(x=buffer_i2c[0];x>1;x--){
            if((buffer_i2c[x]==0)&&(digito==0)){
                
            }
            else{
                if(x==decimalD+1){                      //Cantidad de decimales para 7 y 5 digitos
		            write_psoc1(val,44);
		        }
                digito=1;
                write_psoc1(val,(buffer_i2c[x]+48));
                
            }
		}
        if(ppux10==1 && decimalD == 0){
		    write_psoc1(val,48);	
	    }
	}
	else{
		for(x=buffer_i2c[0];x>=1;x--){
            if((buffer_i2c[x]==0)&&(digito==0)){
                
            }
            else{
		        if(x==decimalD){
		            write_psoc1(val,44);
		        }				
                digito=1;
                write_psoc1(val,(buffer_i2c[x]+48));
            }
		}
        if(ppux10==1 && decimalD == 0){
		    write_psoc1(val,48);	
	    }
	}
	if(pos == a.dir){											//PLACA
		if(Buffer_LCD1.posventa==1){
			write_psoc1(val,10);	
			for(x=0;x<10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD1.placa[0];x++){
				write_psoc1(val,Buffer_LCD1.placa[x]);	
			}
		}
		if((Buffer_LCD1.preset&0x04)==0x04){						//DATOS IBUTTON
			write_psoc1(val,10);	
			write_psoc1(val,10);		
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_cuenta[x]);
			} 
			for(x=1;x<=Buffer_LCD1.cuenta[0];x++){
				write_psoc1(val,Buffer_LCD1.cuenta[x]);	
			}			
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD1.placa[0];x++){
				write_psoc1(val,Buffer_LCD1.placa[x]);	
			}
			if(Buffer_LCD1.km[0]>0){
				write_psoc1(val,10);	
				for(x=0;x<=10;x++){																		
					write_psoc1(val,msn_km[x]);
				}
				for(x=1;x<=Buffer_LCD1.km[0];x++){
					write_psoc1(val,Buffer_LCD1.km[x]);	
				}				
			}
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_id[x]);
				
			}
			for(x=6;x>=1;x--){
				if(((Buffer_LCD1.id[x]>>4)&0x0F)>=10){
					write_psoc1(val,(((Buffer_LCD1.id[x]>>4)&0x0F)+55));
				}
				else{
					write_psoc1(val,(((Buffer_LCD1.id[x]>>4)&0x0F)+48));				
				}
				if((Buffer_LCD1.id[x]&0x0F)>=10){
					write_psoc1(val,((Buffer_LCD1.id[x]&0x0F)+55));
				}
				else{
					write_psoc1(val,((Buffer_LCD1.id[x]&0x0F)+48));				
				}				
			}	
		}
	}
	if(pos == b.dir){															//DATOS PLACA LCD 2
		if(Buffer_LCD2.posventa==1){
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD2.placa[0];x++){
				write_psoc1(val,Buffer_LCD2.placa[x]);	
			}
		}
		if((Buffer_LCD2.preset&0x04)==0x04){
			write_psoc1(val,10);
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_cuenta[x]);
			} 
			for(x=1;x<=Buffer_LCD2.cuenta[0];x++){
				write_psoc1(val,Buffer_LCD2.cuenta[x]);	
			}	
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD2.placa[0];x++){
				write_psoc1(val,Buffer_LCD2.placa[x]);	
			}
			if(Buffer_LCD2.km[0]>0){
				write_psoc1(val,10);
				for(x=0;x<=10;x++){																		
					write_psoc1(val,msn_km[x]);
				}
				for(x=1;x<=Buffer_LCD2.km[0];x++){
					write_psoc1(val,Buffer_LCD2.km[x]);	
				}				
			}
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_id[x]);
				
			}			
			for(x=6;x>=1;x--){
				if(((Buffer_LCD2.id[x]>>4)&0x0F)>=10){
					write_psoc1(val,(((Buffer_LCD2.id[x]>>4)&0x0F)+55));
				}
				else{
					write_psoc1(val,(((Buffer_LCD2.id[x]>>4)&0x0F)+48));				
				}
				if((Buffer_LCD2.id[x]&0x0F)>=10){
					write_psoc1(val,((Buffer_LCD2.id[x]&0x0F)+55));
				}
				else{
					write_psoc1(val,((Buffer_LCD2.id[x]&0x0F)+48));				
				}				
			}
		}	
	}
    
    if(pos == c.dir){															//DATOS PLACA LCD 2
		if(Buffer_LCD3.posventa==1){
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD3.placa[0];x++){
				write_psoc1(val,Buffer_LCD3.placa[x]);	
			}
		}
		if((Buffer_LCD3.preset&0x04)==0x04){
			write_psoc1(val,10);
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_cuenta[x]);
			} 
			for(x=1;x<=Buffer_LCD3.cuenta[0];x++){
				write_psoc1(val,Buffer_LCD3.cuenta[x]);	
			}	
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD3.placa[0];x++){
				write_psoc1(val,Buffer_LCD3.placa[x]);	
			}
			if(Buffer_LCD3.km[0]>0){
				write_psoc1(val,10);
				for(x=0;x<=10;x++){																		
					write_psoc1(val,msn_km[x]);
				}
				for(x=1;x<=Buffer_LCD3.km[0];x++){
					write_psoc1(val,Buffer_LCD3.km[x]);	
				}				
			}
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_id[x]);
				
			}			
			for(x=6;x>=1;x--){
				if(((Buffer_LCD3.id[x]>>4)&0x0F)>=10){
					write_psoc1(val,(((Buffer_LCD3.id[x]>>4)&0x0F)+55));
				}
				else{
					write_psoc1(val,(((Buffer_LCD3.id[x]>>4)&0x0F)+48));				
				}
				if((Buffer_LCD3.id[x]&0x0F)>=10){
					write_psoc1(val,((Buffer_LCD3.id[x]&0x0F)+55));
				}
				else{
					write_psoc1(val,((Buffer_LCD3.id[x]&0x0F)+48));				
				}				
			}
		}	
	}
    
    if(pos == d.dir){															//DATOS PLACA LCD 2
		if(Buffer_LCD4.posventa==1){
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD4.placa[0];x++){
				write_psoc1(val,Buffer_LCD4.placa[x]);	
			}
		}
		if((Buffer_LCD4.preset&0x04)==0x04){
			write_psoc1(val,10);
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_cuenta[x]);
			} 
			for(x=1;x<=Buffer_LCD4.cuenta[0];x++){
				write_psoc1(val,Buffer_LCD4.cuenta[x]);	
			}	
			write_psoc1(val,10);
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_placa[x]);
			} 
			for(x=1;x<=Buffer_LCD4.placa[0];x++){
				write_psoc1(val,Buffer_LCD4.placa[x]);	
			}
			if(Buffer_LCD4.km[0]>0){
				write_psoc1(val,10);
				for(x=0;x<=10;x++){																		
					write_psoc1(val,msn_km[x]);
				}
				for(x=1;x<=Buffer_LCD4.km[0];x++){
					write_psoc1(val,Buffer_LCD4.km[x]);	
				}				
			}
			write_psoc1(val,10);	
			for(x=0;x<=10;x++){																		
				write_psoc1(val,msn_id[x]);
				
			}			
			for(x=6;x>=1;x--){
				if(((Buffer_LCD4.id[x]>>4)&0x0F)>=10){
					write_psoc1(val,(((Buffer_LCD4.id[x]>>4)&0x0F)+55));
				}
				else{
					write_psoc1(val,(((Buffer_LCD4.id[x]>>4)&0x0F)+48));				
				}
				if((Buffer_LCD4.id[x]&0x0F)>=10){
					write_psoc1(val,((Buffer_LCD4.id[x]&0x0F)+55));
				}
				else{
					write_psoc1(val,((Buffer_LCD4.id[x]&0x0F)+48));				
				}				
			}
		}	
	}
    
    write_psoc1(val,10);
    write_psoc1(val,10);
	for(x=0;x<=((31-rventa.lema1[0])/2);x++){					//LEMAS
		write_psoc1(val,0x20);			
	}
	for(x=1;x<=rventa.lema1[0];x++){
		write_psoc1(val,rventa.lema1[x]);			
	}
    write_psoc1(val,10);
	for(x=0;x<=((31-rventa.lema2[0])/2);x++){
		write_psoc1(val,0x20);			
	}
	for(x=1;x<=rventa.lema2[0];x++){
		write_psoc1(val,rventa.lema2[x]);			
	}
    write_psoc1(val,10); 
    write_psoc1(val,10); 
    write_psoc1(val,10);
    write_psoc1(val,10);
	write_psoc1(val,10);
	write_psoc1(val,0x1D);
	write_psoc1(val,0x56);
	write_psoc1(val,0x31);	
}


/*
*********************************************************************************************************
*                                       void imprimir_corte(void)
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

void imprimir_corte(uint8 val){
    write_psoc1(val,10);
    write_psoc1(val,10);
	for(x=0;x<=((31-20)/2);x++){
		write_psoc1(val,0x20);			
	}		
	for(x=0;x<=19;x++){										//ESTACION DE SERVICIO
		write_psoc1(val,msn_eds[x]);
	}
    write_psoc1(val,10);
	for(x=0;x<=((31-rventa.nombre[0])/2);x++){				//NOMBRE
		write_psoc1(val,0x20);		
	}
	for(x=1;x<=rventa.nombre[0];x++){
		write_psoc1(val,rventa.nombre[x]);			
	}		
    write_psoc1(val,10);
	if(rventa.nit[0]>=1){
		for(x=0;x<=((31-(rventa.nit[0]+4))/2);x++){			//NIT
			write_psoc1(val,0x20);			
		}		
		for(x=0;x<=3;x++){									
			write_psoc1(val,msn_nit[x]);
		}
		for(x=1;x<=rventa.nit[0];x++){
			write_psoc1(val,rventa.nit[x]);			
		}
		write_psoc1(val,10);
	}
	if(rventa.telefono[0]>=1){
		for(x=0;x<=((31-(rventa.telefono[0]+4))/2);x++){	//TELEFONO
			write_psoc1(val,0x20);			
		}		
		for(x=0;x<=3;x++){									
			write_psoc1(val,msn_tel[x]);
		}
		for(x=1;x<=rventa.telefono[0];x++){
			write_psoc1(val,rventa.telefono[x]);			
		}			
	    write_psoc1(val,10);
	    write_psoc1(val,10);
	}
	for(x=0;x<=((31-rventa.direccion[0])/2);x++){			//DIRECCION
		write_psoc1(val,0x20);			
	}
	for(x=1;x<=rventa.direccion[0];x++){
		write_psoc1(val,rventa.direccion[x]);			
	}
    write_psoc1(val,10);
    write_psoc1(val,10);
    write_psoc1(val,10);
	for(x=0;x<=15;x++){										//CONSECUTIVO								
		write_psoc1(val,msn_idcorte[x]);
	}
	if(leer_eeprom(668,3)==1){
		if(buffer_i2c[0]==2){
			id_corte=((buffer_i2c[2]<<8)|buffer_i2c[1]);
			write_psoc1(val,(id_corte/1000)+48);
			write_psoc1(val,((id_corte%1000)/100)+48);
			write_psoc1(val,((id_corte%100)/10)+48);
			write_psoc1(val,(id_corte%10)+48);
			id_corte++;
			if(id_corte==10000){
				id_corte=0;
			}
			buffer_i2c[0]=2;
			buffer_i2c[1]=(id_corte&0xFF);
			buffer_i2c[2]=(id_corte>>8)&0xFF;
			write_eeprom(668,buffer_i2c);		 
		}
		else{
			write_psoc1(val,49);
			id_corte=2;
			buffer_i2c[0]=2;
			buffer_i2c[1]=(id_corte&0xFF);
			buffer_i2c[2]=(id_corte>>8)&0xFF;
			write_eeprom(668,buffer_i2c);
		}
	}
	
	write_psoc1(val,10);
	for(x=0;x<=15;x++){										//FECHA CORTE								
		write_psoc1(val,msn_fcorte[x]);
	}
	if(leer_fecha()==1){
		write_psoc1(val,(((rventa.fecha[0]&0x30)>>4)+48));
		write_psoc1(val,((rventa.fecha[0]&0x0F)+48));
		write_psoc1(val,'/');
		write_psoc1(val,(((rventa.fecha[1]&0x10)>>4)+48));
		write_psoc1(val,((rventa.fecha[1]&0x0F)+48));	
		write_psoc1(val,'/');
		write_psoc1(val,(((rventa.fecha[2]&0xF0)>>4)+48));
		write_psoc1(val,((rventa.fecha[2]&0x0F)+48));			
	}			
    write_psoc1(val,10);
	for(x=0;x<=15;x++){									
		write_psoc1(val,msn_hcorte[x]);
	}
	if(leer_hora()==1){
		write_psoc1(val,(((rventa.hora[1]&0x10)>>4)+48));
		write_psoc1(val,((rventa.hora[1]&0x0F)+48));
		write_psoc1(val,':');
		write_psoc1(val,(((rventa.hora[0]&0xF0)>>4)+48));
		write_psoc1(val,((rventa.hora[0]&0x0F)+48));
		if((rventa.hora[1]&0x20)==0x20){
			write_psoc1(val,'p');
		}
		else{
			write_psoc1(val,'a');						
		}			
		write_psoc1(val,'m');
	}
    write_psoc1(val,10);
	write_psoc1(val,10);
	leer_eeprom(242,6);
	for(x=0;x<=23;x++){									
		write_psoc1(val,msn_ufcorte[x]);
	}
	if(leer_fecha()==1){
		write_psoc1(val,(((buffer_i2c[1]&0x30)>>4)+48));
		write_psoc1(val,((buffer_i2c[1]&0x0F)+48));
		write_psoc1(val,'/');
		write_psoc1(val,(((buffer_i2c[2]&0x10)>>4)+48));
		write_psoc1(val,((buffer_i2c[2]&0x0F)+48));	
		write_psoc1(val,'/');
		write_psoc1(val,(((buffer_i2c[3]&0xF0)>>4)+48));
		write_psoc1(val,((buffer_i2c[3]&0x0F)+48));			
	}			
    write_psoc1(val,10);
	for(x=0;x<=23;x++){									
		write_psoc1(val,msn_uhcorte[x]);
	}
	if(leer_hora()==1){
		write_psoc1(val,(((buffer_i2c[5]&0x10)>>4)+48));
		write_psoc1(val,((buffer_i2c[5]&0x0F)+48));
		write_psoc1(val,':');
		write_psoc1(val,(((buffer_i2c[4]&0xF0)>>4)+48));
		write_psoc1(val,((buffer_i2c[4]&0x0F)+48));
		if((rventa.hora[1]&0x20)==0x20){
			write_psoc1(val,'p');
		}
		else{
			write_psoc1(val,'a');						
		}			
		write_psoc1(val,'m');
	}
    write_psoc1(val,10); 
   	write_psoc1(val,10);	
	for(x=0;x<=10;x++){										//POSICION	A (1)							
		write_psoc1(val,msn_pos[x]);
	}
    write_psoc1(val,((a.dir/10)+48));
	write_psoc1(val,((a.dir%10)+48));
    write_psoc1(val,10);  
	get_totales(a.dir,val);    
    write_psoc1(val,10); 
	write_psoc1(val,10); 
     for(x=0;x<=10;x++){																	
		write_psoc1(val,msn_pos[x]);                        //POSICION	C (2)
	}
    write_psoc1(val,((c.dir/10)+48));
	write_psoc1(val,((c.dir%10)+48));	
	write_psoc1(val,10);
	get_totales(c.dir,val);
    write_psoc1(val,10);
    write_psoc1(val,10);
	for(x=0;x<=10;x++){																	
		write_psoc1(val,msn_pos[x]);                        //POSICION	B (3)
	}
    write_psoc1(val,((b.dir/10)+48));
	write_psoc1(val,((b.dir%10)+48));	
	write_psoc1(val,10);
	get_totales(b.dir,val);    
    write_psoc1(val,10);
    write_psoc1(val,10);
   
    for(x=0;x<=10;x++){										//POSICION	D (4)							
		write_psoc1(val,msn_pos[x]);
	}
    write_psoc1(val,((d.dir/10)+48));
	write_psoc1(val,((d.dir%10)+48));	
	write_psoc1(val,10);
	get_totales(d.dir,val);
	fecha_corte[0]=5;
	fecha_corte[1]=rventa.fecha[0];
	fecha_corte[2]=rventa.fecha[1];
	fecha_corte[3]=rventa.fecha[2];
	fecha_corte[4]=rventa.hora[0];
	fecha_corte[5]=rventa.hora[1];	
	write_eeprom(242,fecha_corte);
}


/* [] END OF FILE */
