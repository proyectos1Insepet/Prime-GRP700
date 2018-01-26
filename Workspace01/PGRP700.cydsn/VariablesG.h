/**
* @file VariablesG.h
* @Author Insepet LTDA
* @date 28/2/2016
* @brief Contiene las variables globales que se usan en el programa y las principales estructuras
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

/**
*
*   <B>flujo_LCD,flujo_LCD2, flujo_LCD3,flujo_LCD4</B> <BR>
*/
/// <BR>
/// 0   Inicio de protector <BR>
/// 1   Mantiene el protector se verifica en la Int del puerto de la LCD<BR> 
/// 2   Pasa a pantalla inicial de opciones y limpia el buffer <BR>
/// 3   Espera a que elijan entre ventas u otras <BR>
/// 4   Venta en efectivo, elige dinero, galones o full<BR>
/// 5   Ingresa valor en pesos o volumen <BR>
/// 6   Espera a que suba la manija o cancele, luego emite sonido y autoriza <BR>
/// 7   Espera que baje la manija <BR>
/// 8   Pasa a imprimir o pedir placa segun version <BR>
/// 9   pedir placa efectivo <BR>
/// 10  pedir recibo y finalizar <BR>
/// 11  Leer Ibutton <BR>
/// 12  Kilometraje <BR>
/// 13  Imprimir <BR>
/// 14  Otras Opciones <BR>
/// 15  Ingresar Password <BR>
/// 16  Cambiar Hora/Fecha <BR>
/// 17  Cambiar Pasword <BR>
/// 18  Cambiar Datos Recibo <BR>
/// 19  Cambiar Precio <BR>
/// 20  Test <BR>
/// 21  Teclado hora <BR>
/// 22  Teclado fecha <BR>
/// 23	Teclado NIT, Telefono <BR>
/// 24	Teclado Nombre, Direccion, lema1, lema2 <BR>
/// 25	Venta con Id? <BR>
/// 26	Teclado Precio <BR>
/// 27	Configurar Surtidor <BR>
/// 28	Configurar Productos <BR>
/// 29	Version de digitos <BR>
/// 30	PPU <BR>
/// 31	Teclado Producto <BR>
/// 32	Cambiar Bandera <BR>
/// 33 	Numero de copias <BR>
/// 34	Corte <BR>
/// 35	Elegir el dato a grabar en el ibutton <BR>
/// 36  Grabar iButton	<BR>
/// 37	Sirven las impresoras? <BR>
/// 38  Cual Impresora sirve? <BR>
/// 39	Pasword corte <BR>
/// 40	Cambio Pasword Corte <BR>    
uint8 flujo_LCD,flujo_LCD2,flujo_LCD3,flujo_LCD4;
///Auxiliares para las funciones del teclado y organizacion de datos
uint8 cont1,teclas1,teclas2, comas1, comas2;  							                       
///Determina la version entre (1)no_extendida y (2)extendida
uint8 versurt;															                        
///Banderas para saber que productos hay
uint8 producto1,producto2,producto3,producto4,producto1b,producto2b,producto3b,producto4b;
///Banderas para saber que productos hay
uint8 producto1c,producto2c,producto3c,producto4c,producto1d,producto2d,producto3d,producto4d;
///Buffer de lectura del i2c
uint8 buffer_i2c[64];													
///Buffer que almacena el resultado de una operacion				
uint8 resultado[14];													
///Auxiliar que indica los datos del teclado en que variable guardarla
uint8 teclado,teclado2;													
///Indica si el PPU esta x10		
uint8 ppux10;															
///Bandera de la estacion
uint8 bandera[2];														
///tipo de impresora (panel o kiosko)
uint8 tipo_imp[2];                                                      
///Cantidad de Decimales en volumen
uint8 decimalV;															
///Cantidad de decimales en dinero
uint8 decimalD;															
///Fecha del ultimo corte
uint8 fecha_corte[6];													
///Posicion de memoria ibutton
uint8 pos_ibutton;														
///crc del ibutton
uint8 crc_total;														
///Puerto de las impresoras
uint8 print1[2], print2[2];												
///activar copia de recibo
uint8 copia_recibo[2];													
///activar copia de recibo lcd2
uint8 copia_recibo2[2];													
///Consecutivo de Venta
uint8 id_venta[6];														
///Para guardar en la estructura del nombre del producto
uint8 nombreproducto;
///nombres de productos en memoria
uint8 producto1n[10];                                                   
uint8 producto2n[10];
uint8 producto3n[10];
uint8 producto4n[10];
/// indica si ya se pidió copia de recibo
uint8 a_copias;
/// cantidad de copias a imprimir
uint8 n_copias [1];
/// indica si se pidió recibo
uint8 no_imprime;
uint8 no_imprime2;
uint8 no_imprime3;
uint8 no_imprime4;
/// auxiliar para conservar estado mientras otra posición hace un proceso en pantalla
uint8 flujo_pos;
/// auxiliar para conservar estado mientras otra posición hace un proceso en pantalla
uint8 flujo_pos2;
/// auxiliar para conservar estado mientras otra posición hace un proceso en pantalla
uint8 flujo_pos3;
/// auxiliar para conservar estado mientras otra posición hace un proceso en pantalla
uint8 flujo_pos4;
///Selecion de posicion en LCD 1 POS A, 2 POS B
uint8 seleccion_pos;                                                   
///Selecion de posicion en LCD 1 POS A, 2 POS B
uint8 seleccion_pos2;                                                  
uint8 venta_activa,venta_activa3;
uint8 venta_activa2,venta_activa4;
///consecutivo del corte
uint16 id_corte;														
uint8 placa_aux;
/// numero de venta cuando es copia1
uint8 no_ventacopia1;
/// numero de venta cuando es copia2
uint8 no_ventacopia2;
/// numero de venta cuando es copia3
uint8 no_ventacopia3;
/// numero de venta cuando es copia4
uint8 no_ventacopia4;

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
		
/**
* @struct estructura para el manejo de los datos ingresados por display en cada posición
*
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

/**
* @struct estructura con los datos que hacen consulta a la posición
*
*/
struct pos{
    uint8 dir;
    uint8 estado;
    uint8 manguera;
    ///1=ver_estado, 2=esperando_autor, 3=autorizar, 4=surtiendo, 5=totales, 6=cambio_precio, 7=ver_precio
    uint8 funcion;      
    volatile uint8 totales[3][24]; 
};



   struct pos a;       ///Posiciones del Prime
   struct pos b;
   struct pos c;
   struct pos d;

/**
* @struct estructura con los datos de venta, estación y cliente para generar el recibo
*
*/
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
