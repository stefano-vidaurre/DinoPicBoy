#include "IOCore.h"

uint32_t SPI_transfer(uint32_t data){
    SPI1BUF = data;
    while(SPI1STATbits.SPIBUSY != 0);
    return SPI1BUF;
}

void SPI_writeCmd(uint32_t cmd){
    SPI_TFT_CMD;
    SPI_MODE_16;
    SPI_TFT_SELECT;
    SPI_transfer(cmd);
    SPI_TFT_DESELECT;
}

void SPI_writeData(uint32_t data){
    SPI_TFT_DATA;
    SPI_MODE_16;
    SPI_TFT_SELECT;
    SPI_transfer(data);
    SPI_TFT_DESELECT;
}

void SPI_writeMem(uint16_t address, uint16_t data){
    SPI_MEM_SELECT;
    SPI_MODE_16;
    SPI_transfer(0x02);
    SPI_transfer(address);
    SPI_transfer(data);
    SPI_MEM_DESELECT;
}
uint16_t SPI_readMem(uint16_t address){
    SPI_MEM_SELECT;
    SPI_MODE_16;
    SPI_transfer(0x02);
    SPI_transfer(address);
    uint16_t buff = SPI_transfer(0xFF);
    SPI_MEM_DESELECT;
    return buff;
}

void Event_init(){
    //---- TENER CUIDADO CON LAS PRIORIDADES, DADO QUE EL SISTEMA PUEDE ESTAR ----//
    //----------- MANEJANDO MAS DE UNA INTERRUPCION DE FORMA SIMULTANEA ----------//
    __builtin_disable_interrupts();
    
    IEC0bits.INT0IE = 0;    // Desactivamos la interrupcion 
    IPC0bits.INT0IP = 4;    // Indicamos prioridad 4
    IPC0bits.INT0IS = 1;    // Indicamos sub-prioridad 1
    IFS0bits.INT0IF = 0;    // Limpiamos los flags de la interrupcion
    INTCONbits.INT0EP = 0;  // Seleccionamos la polaridad (1 == HIGH, 0 == LOW)
    IEC0bits.INT0IE = 1;    // Activamos la interrupcion
    
    IEC0bits.T4IE = 0;  // Apagamos la interrupcion del timer4
    IFS0bits.T4IF = 0;  // Limpiamos el flag del timer4
    T4CONbits.ON = 0;   // Apagamos el modulo del timer
    T4CONbits.T32 = 0;  // Seleccionamos que sea de 16bits (32 hace que T4 y T5 se junten y sean uno)
    T4CONbits.SIDL = 0; // En modo idle sigue funcionando con == 0
    T4CONbits.TCS = 0;  // Seleccionamos que use el reloj interno
    T4CONbits.TCKPS = 7; // Seleccionamos una escala de 1:256
    PR4 = 12500;        // Funcionando a 32Mhz realiza unos 10frames/s
    __builtin_enable_interrupts();
    
    ANSELAbits.ANSA0 = 1; // Activamos el pin 2 como lectura analogica
    TRISAbits.TRISA0 = 1; // Activamos el pin 2 en lectura
    ANSELAbits.ANSA1 = 1; // Activamos el pin 3 como lectura analogica
    TRISAbits.TRISA1 = 1; // Activamos el pin 3 en lectura
    
    ANSELB = 0;
    ODCB = 0;
    TRISB = 0;
    PORTB = 0;
    
    TRISBbits.TRISB0 = 1; // Activamos el pin 4 como lectura digital
    TRISBbits.TRISB3 = 1; // Activamos el pin 7 como lectura digital
    TRISBbits.TRISB4 = 1; // Activamos el pin 11 como lectura digital
    TRISBbits.TRISB5 = 1; // Activamos el pin 14 como lectura digital
    TRISBbits.TRISB7 = 1; // Activamos el pin 7 como lectura digital
}

void SPI_init(){
    int rData;
    SPI1CON = 0;         // Reiniciamos la configuracion de SPI1 y lo apagamos
    rData = SPI1BUF;     // Vaciamos el buffer de SPI1
    SPI1BRG = 0x00;      // Elimina cualquier divisor de la frecuencia de reloj SPI1
    SPI1STATbits.SPIROV = 0; // Limpiamos el flag de overflow de SPI1
    SDI1_ENABLE;         // Activamos el pin de MISO
    SDO1_ENABLE;         // Activamos el pin de MOSI
    SPI_TFT_CS_ENABLE;   // Activamos el pin de CS para el TFT
    SPI_TFT_DC_ENABLE;   // Activamos el pin de DC para el TFT
    SPI_MEM_CS_ENABLE;   // Activamos el pin de CS para la MEM
    SPI1CONbits.ENHBUF = 1; // Activar modo mejorado de buffer, (colas de tx y rx)
    SPI1CONbits.MODE32 = 1; // Transmision de bloques de 32bits
    SPI1CONbits.MODE16 = 1; // Transmision de bloques de 16bit, no activa pero si preparada
    SPI1CONbits.SMP = 1; // Muestrear el dato de entrada despues de enviar el dato de salida
    SPI1CONbits.CKE = 1; // Se considera pulso el paso de activado a desactivado
    SPI1CONbits.CKP = 0; // CKP == 0 -> Activo = HIGH | Inactivo = LOW
    SPI1CONbits.MSTEN = 1; // Modo master
    SPI1CONbits.ON = 1; // Encendemos el modulo SPI1
    
}

void IO_init(){
    Event_init();
    SPI_init();
}

inline void CLICK_next(){
    IO_listener();
    while(IFS0bits.T4IF == 0);
    IFS0bits.T4IF = 0;
}

void __ISR(_EXTERNAL_0_VECTOR, IPL4SOFT) _DefaultInterrupt(void){
    IO_interrupt();
    while(EVENT_BUTTON_INT);
    IFS0bits.INT0IF = 0;
}