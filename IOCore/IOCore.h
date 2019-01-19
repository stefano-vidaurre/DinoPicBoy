/* * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Autor: Stefano Vidaurre Olite                   *
 * Fecha: 24/11/2016                               *
 * Descripcion: Libreria encargada de inplentar las*
 * diferentes maneras de comunicacion del pic con  *
 * el dispositivos externos. Ademas de la          * 
 * comunicacion interna mediate eventos.           *
 * V: 1.1.8                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __IOCORE_H__
#define	__IOCORE_H__

#include <stdint.h>
#include <xc.h>
#include <sys/attribs.h>

//-----  Macros para manejo de pines -----//

#define SDI1_ENABLE SDI1R=0b0001            //Pin 5 gestiona SDI1
#define SDO1_ENABLE RPB2R=0b0011            //Pin 6 gestiona SDO1

#define SPI_TFT_CS_ENABLE TRISBbits.TRISB15=0;  // Pin 26 gestiona CS del TFT
#define SPI_MEM_CS_ENABLE TRISBbits.TRISB9=0;   // Pin 18 gestiona CS de la MEM
#define SPI_TFT_DC_ENABLE TRISBbits.TRISB13=0;  // Pin 24 gestiona DC del TFT
#define SPI_TFT_CS LATBbits.LATB15          //||
#define SPI_TFT_DC LATBbits.LATB13          //Macros axiliares
#define SPI_MEM_CS LATBbits.LATB9

//-----  Macros de seleccion de modo -----//
#define SPI_TFT_SELECT SPI_TFT_CS=0               //Activa CS del TFT
#define SPI_TFT_DESELECT SPI_TFT_CS=1             //Desactiva CS del TFT
#define SPI_TFT_CMD SPI_TFT_DC=0                  //Indica al TFT que se le pasa comando
#define SPI_TFT_DATA SPI_TFT_DC=1                 //Indoca al TFT que se le pasa datos
#define SPI_MODE_32 SPI1CONbits.MODE32=1;
#define SPI_MODE_16 SPI1CONbits.MODE32=0;
#define SPI_MEM_SELECT SPI_MEM_CS=0
#define SPI_MEM_DESELECT SPI_MEM_CS=1

#define EVENT_ANALOG_0 PORTAbits.RA0
#define EVENT_ANALOG_1 PORTAbits.RA1
#define EVENT_BUTTON_0 PORTBbits.RB0==0
#define EVENT_BUTTON_1 PORTBbits.RB3==0
#define EVENT_BUTTON_2 PORTBbits.RB4==0
#define EVENT_BUTTON_3 PORTBbits.RB5==0
#define EVENT_BUTTON_INT PORTBbits.RB7==0

//----- Macros manejo crono -----//
#define CLICK_ON T4CONbits.ON=1
#define CLICK_OFF T4CONbits.ON=0
#define CLICK_RUN T4CONbits.ON

// GPU_transfer()
// Ent: 
//  data -> Dato que deseamos enviar por el modulo SPI (4 bytes)
// Sal:
//  data -> Dato leido del modulo SPI
// Des: Funcion que nos permite hacer una transferencia SPI de 32bits de tamaño,
// no hay que olvidar que en SPI se envia y se recive a la vez, de modo que si
// queremos enviar introducimos el dato en la entrada y si queremos leer, la 
// entrada deben de ser todo unos y nos retornara la salida. 
uint32_t SPI_transfer(uint32_t data);

// GPU_writeData()
// Ent: 
//  data -> Dato que deseamos enviar por al TFT (4 bytes)
// Sal: --
// Des: Funcion que nos permite enviar un dato al TFT (no olvidar que son 32bits)
void SPI_writeData(uint32_t data);

// GPU_writeCmd()
// Ent: 
//  cmd -> Comando que deseamos enviar por al TFT (4 bytes)
// Sal: --
// Des: Funcion que nos permite enviar un comando al TFT (no olvidar que son 32bits)
void SPI_writeCmd(uint32_t cmd);

// GPU_init()
// Ent: --
// Sal: --
// Des: Funcion que inicializa el modulo SPI, la interrupcion externa, el timer y
// los pines de entrada. Prepara el PIC para gestionar las entradas y las salidas
// de forma correcta.
void IO_init();

// GPU_next()
// Ent: --
// Sal: --
// Des: Funcion que se encarga de revisar si ha habido alguna peticion de entrada
// y de fraccionar en margenes de tiempo iguales por cada CLICK, de este modo 
// podemos obtener frames mas o menos estables, haciendo que duren todos lo mismo
// En caso de estar usando la libreria GPU y el modelo establecido por GPU_run,
// esta funcion es gestionada de forma opaca y no es necesaria para el programador
inline void CLICK_next();

// GPU_next() ##### ABSTRACT #####
// Ent: --
// Sal: --
// Des: Funcion abstracta que debe ser implementada por el programador en la que
// se incluira la logica de cada boton.
void  IO_listener(); 

// GPU_next() ##### ABSTRACT #####
// Ent: --
// Sal: --
// Des: Funcion abstracta que debe ser implementada por el programador en la que
// se incluira la logica de la interrupcion externa. La interrupcion no es gestionada
// por CLICK de modo que es asincrona y se ejecuta en cuanto ocurre. Tambien hay que
// decir que no es persistente de modo que solo se ejecuta una unica vez y es necesario
// que pase a desactivada y luego a activada otra vez para que se vuelva a considerar.
void IO_interrupt();

void SPI_writeMem(uint16_t address, uint16_t data);
uint16_t SPI_readMem(uint16_t address);

#endif	/* IOCORE_H */

