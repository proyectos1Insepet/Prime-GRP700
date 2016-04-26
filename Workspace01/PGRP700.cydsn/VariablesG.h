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
* Filename      : VariablesG.h
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/

#ifndef VARIABLESG_H
#define VARIABLESG_H
#include <device.h>	
/*****************Imagenes*********************/	
#define iprotector1     0
#define iprotector2     1
#define iprotector3     2
#define iprotector4     3
#define iprotector5     20
#define iprotector6     21
#define iprotector7     22
#define iprotector8     23
#define iprotector9     24
#define iprotector10    25
#define iprotector11    26
#define iprotector12    27

uint8 flujo_LCD,flujo_LCD2,flujo_LCD3,flujo_LCD4;/*
0   Inicio de protector
1   Mantiene el protector se verifica en la Int del puerto de la LCD
2   Pasa a pantalla inicial de opciones y limpia el buffer
3   Espera a que elijan entre ventas u otras
4   Venta en efectivo, elige dinero, galones o full
5   Ingresa valor en pesos o volumen
6   Espera a que suba la manija o cancele, luego emite sonido y autoriza
7   Espera que baje la manija
8   Pasa a imprimir o pedir placa segun version
9   pedir placa efectivo 
10  pedir recibo y finalizar
11  Leer Ibutton
12  Kilometraje
13  Imprimir
14  Otras Opciones
15  Ingresar Password
16  Cambiar Hora/Fecha
17  Cambiar Pasword
18  Cambiar Datos Recibo
19  Cambiar Precio
20  Test
21  Teclado hora
22  Teclado fecha
23	Teclado NIT, Telefono
24	Teclado Nombre, Direccion, lema1, lema2
25	Venta con Id?
26	Teclado Precio
27	Configurar Surtidor
28	Configurar Productos
29	Version de digitos
30	PPU
31	Teclado Producto
32	Cambiar Bandera
33 	Numero de copias
34	Corte
35	Elegir el dato a grabar en el ibutton
36  Grabar iButton	
37	Sirven las impresoras?
38  Cual Impresora sirve?
39	Pasword corte
40	Cambio Pasword Corte	
	
//Temporal para definición:
    //producto1 = extra, producto2=diesel, producto3=corriente, producto4=kero 
	
*/
//uint8 count_protector,count_protector2;
//uint8 ok_LCD1, ok_LCD2;       											                    //Banderas para indicar que hay datos para enviar
uint8 cont1,teclas1,teclas2, comas1, comas2;  							                        //Auxiliares para las funciones del teclado y organizacion de datos
uint8 versurt;															                        //Determina la version entre (1)no_extendida y (2)extendida
uint8 producto1,producto2,producto3,producto4,producto1b,producto2b,producto3b,producto4b;		//Banderas para saber que productos hay
uint8 producto1c,producto2c,producto3c,producto4c,producto1d,producto2d,producto3d,producto4d;
uint8 buffer_i2c[64];													//Buffer de lectura del i2c 
uint8 resultado[14];													//Buffer que almacena el resultado de una operacion				
uint8 teclado,teclado2;													//Auxiliar que indica los datos del teclado en que variable gusrdarla
uint8 ppux10;															//Indica si el PPU esta x10		
uint8 bandera[2];														//Bandera de la estacion
uint8 tipo_imp[2];                                                      //tipo de impresora (panel o kiosko)
uint8 decimalV;															//Cantidad de Decimales en volumen
uint8 decimalD;															//Cantidad de decimales en dinero
uint8 fecha_corte[6];													//Fecha del ultimo corte
uint8 pos_ibutton;														//Posicion de memoria ibutton
uint8 crc_total;														//crc del ibutton
uint8 print1[2], print2[2];												//Puerto de las impresoras
uint8 copia_recibo[2];													//activar copia de recibo
uint8 copia_recibo2[2];													//activar copia de recibo lcd2
uint8 id_venta[6];														//Consecutivo de Venta
uint8 nombreproducto;                                                  //Para guardar en la estructura del nombre del producto
uint8 producto1n[10];                                                   //nombres de productos en memoria
uint8 producto2n[10];
uint8 producto3n[10];
uint8 producto4n[10];
uint8 a_copias;
uint8 n_copias [1];
uint8 no_imprime;
uint8 no_imprime2;
uint8 imagen;
uint8 flujo_pos;
uint8 flujo_pos2;
uint8 flujo_pos3;
uint8 flujo_pos4;
uint8 estado_lcd1;                                                     //Estado LCD1 1 ocupado, 0 libre
uint8 estado_lcd2;                                                     //Estado LCD2 1 ocupado, 0 libre
uint8 seleccion_pos;                                                   //Selecion de posicion en LCD 1 POS A, 2 POS B
uint8 seleccion_pos2;                                                  //Selecion de posicion en LCD 1 POS A, 2 POS B
uint8 venta_activa,venta_activa3;
uint8 venta_activa2,venta_activa4;
uint16 id_corte;														//consecutivo del corte
uint8 placa_aux;



volatile int no_venta;													//Consecutivo de Venta entero	 
/*
1	Petrobras
2	Biomax
3	ESSO
4	MOBIL
5	Terpel
6	Gulfg
7	Brio
8	Texaco
*/
		

struct buffer{
    uint8 size;
    uint8 pos;
    uint8 comando;
    uint8 preset;
    uint8 km[7];
    uint8 valor[30];
    uint8 id[30];
	uint8 cuenta[30];
    uint8 placa[30];
    uint8 posventa;
    uint8 cedula[10];
    uint8 password[9];
	uint8 turno;    
};
struct buffer Buffer_LCD1;
struct buffer Buffer_LCD2;
struct buffer Buffer_LCD3;
struct buffer Buffer_LCD4;
/**********************************************/
uint8 estado_0;

struct pos{
    uint8 dir;
    uint8 estado;
    uint8 manguera;
    uint8 funcion;      //1=ver_estado, 2=esperando_autor, 3=autorizar, 4=surtiendo, 5=totales, 6=cambio_precio, 7=ver_precio
    volatile uint8 totales[3][24]; 
};



   struct pos a;       //Posiciones del Prime
   struct pos b;
   struct pos c;
   struct pos d;

struct recibo{
    uint8 nombre[30];
    uint8 nit[15];
	uint8 configppu[2];
    uint8 telefono[20];
    uint8 direccion[30];
	uint8 lema1[30];
	uint8 lema2[30];
    uint8 fecha[3];
    uint8 hora[2];
    uint8 posicion;
    uint8 producto; 
    uint8 ppu[7];
    uint8 dinero[9];	
    uint8 volumen[9];
	uint8 tvolumen[13];		//Totales de volumen por manguera
	uint8 tdinero[13];		//Totales de dinero por manguera
	uint8 password[9];
	uint8 manguera;
};
uint32 x,y,z,w;   //Variables Genericas para ciclos
uint8  modulo[2][200];  //Datos que llegan del CDG 0 pos, 1 estado, 2-99 datos
uint8  grado1, grado2, grado3, grado4,producto;
//struct surtidor lado;   //lado del surtidor
struct recibo rventa;   //datos de la venta, la estacion y totales
struct recibo rventa2;   //datos de la venta, la estacion y totales

#endif

//[] END OF FILE
