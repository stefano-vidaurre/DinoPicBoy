/*
 * File:   main.c
 * Author: fano
 *
 *
 */

#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
#pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by Port Function)

// DEVCFG2
#pragma config FPLLIDIV = DIV_12        // PLL Input Divider (12x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_256       // System PLL Output Clock Divider (PLL Divide by 256)

// DEVCFG1
#pragma config FNOSC = PRI              // Oscillator Selection Bits (Fast RC Osc (FRC))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = HS            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/4)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#define STEPPED 1
#define JUMPHEIGH 7

#include <stdint.h>
#include "GPUCore/GPUCore.h"
#include "IOCore/IOCore.h"
#include <xc.h>

void jump();
uint8_t t_rex[9];
uint8_t captus[6];
int8_t velY = -JUMPHEIGH;
uint8_t velX = 1;
uint8_t captusX = 38;
uint8_t background;
uint8_t pause = 0;

void main(void){
    GPU_init();

    uint8_t tiles[26][8][8] = {
        // Fondo Blanco [0]
        {{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}}
        // Suelo 1 [1]
        ,{{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12},
        {0x22, 0x21, 0x21, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x21, 0x21}}
        // Suelo 2 [2]
        ,{{0x22, 0x22, 0x21, 0x11, 0x12, 0x12, 0x11, 0x21},
        {0x12, 0x12, 0x12, 0x22, 0x22, 0x22, 0x22, 0x12},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}}
        // Suelo 3 [3]
        ,{{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12},
        {0x21, 0x22, 0x22, 0x22, 0x22, 0x22, 0x12, 0x12},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}}
        // Suelo 4 [4]
        ,{{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}}
        // T-Rex culo [5]
        ,{{0x11, 0x11, 0x01, 0x00, 0x00, 0x00, 0x01, 0x11},
        {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},
        {0x00, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},
        {0x00, 0x00, 0x00, 0x10, 0x11, 0x11, 0x11, 0x11}}
        // T-Rex torso [6]
        ,{{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x11},
        {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00},
        {0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00}}
        // T-Rex ojo [7]
        ,{{0x00, 0x00, 0x11, 0x11, 0x01, 0x11, 0x11, 0x11},
        {0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},
        {0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x01},
        {0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00}}
        // T-Rex morro [8]
        ,{{0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00},
        {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // T-Rex nuca [9]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11}}
        // T-Rex frente [10]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00}}
        // T-rex cola [11]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // T-rex pie izquierdo 1 [12]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x10, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // T-rex pie izquierdo 2 [13]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x10},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // T-rex pie derecho 1 [14]
        ,{{0x00, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // T-rex pie derecho 2 [15]
        ,{{0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // Nube 1 [16]
        ,{{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2f, 0x2f},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0xff, 0xf2, 0x22},
        {0x2f, 0xff, 0xf2, 0x22, 0xf2, 0xf2, 0x22, 0x22}}
        // Nube 2 [17]
        ,{{0x22, 0x22, 0x2f, 0x2f, 0x2f, 0xff, 0xf2, 0xf2},
        {0x2f, 0xff, 0xf2, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0xf2, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f}}
        // Nube 3 [18]
        ,{{0xf2, 0xf2, 0xff, 0x2f, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0xff, 0x2f, 0x2f, 0x2f, 0x22},
        {0x22, 0x2f, 0xf2, 0x22, 0x22, 0x22, 0xf2, 0xf2},
        {0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f}}
        // Nube 4 [19]
        ,{{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0xf2, 0xff, 0x2f, 0x22, 0x22, 0x22, 0x22, 0x22},
        {0x2f, 0x2f, 0x2f, 0xff, 0x22, 0x22, 0x22, 0x22}}
        // Captus pie I [20]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11}}
        // Captus pie D [21]
        ,{{0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // Captus tronco I [22]
        ,{{0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x11, 0x11, 0x11, 0x01, 0x01, 0x11, 0x11},
        {0x00, 0x00, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11}}
        // Captus tronco D [23]
        ,{{0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00},
        {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10, 0x00},
        {0x11, 0x11, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
        // Captus copa I [24]
        ,{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x01, 0x11, 0x01, 0x00, 0x00, 0x11, 0x11},
        {0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11}}
        // Captus copa D [25]
        ,{{0x11, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00},
        {0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00}}
    };

    uint8_t palettes[2][16][3] = {
        {{0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0x96, 0x96, 0x96}},
        {{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF},}};
    
    uint16_t i;
    for(i = 0; i < 26; i++){
        GPU_newTile(i, tiles[i]);
    }
    
    GPU_newPalette(0, palettes[0]);
    GPU_newPalette(1, palettes[1]);

    // El mapa esta prefabricado con lo que ya sabemos que indices van a tener los static del juego
    // IPORTANTE: No cambiar el orden de carga de los static, ya que en el mapa de baldosas se han
    // puesto sin usar las variables de referencia a los indices (Mala praxis!!)
    background = GPU_addStatic(0, 1, 0, 0, STEPPED);
    uint8_t suelo1 = GPU_addStatic(1, 4, velX, background, STEPPED);
    uint8_t suelo2 = GPU_addStatic(1, 4, velX, background, STEPPED);
    GPU_animationNext(suelo2);
    uint8_t suelo3 = GPU_addStatic(1, 4, velX, background, STEPPED);
    GPU_animationNext(suelo3);
    GPU_animationNext(suelo3);
    uint8_t suelo4 = GPU_addStatic(1, 4, velX, background, STEPPED);
    GPU_animationNext(suelo4);
    GPU_animationNext(suelo4);
    GPU_animationNext(suelo4);
    uint8_t nube1 = GPU_addStatic(16, 1, 0, background, STEPPED);
    uint8_t nube2 = GPU_addStatic(17, 1, 0, background, STEPPED);
    uint8_t nube3 = GPU_addStatic(18, 1, 0, background, STEPPED);
    uint8_t nube4 = GPU_addStatic(19, 1, 0, background, STEPPED);
    uint8_t tilesMap[30][40] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    GPU_loadMap(&tilesMap[0][0], 30, 40);
    
    // Cargamos el T-rex
    // Los pies tienen animacion pasiva con lo que hay que configurar la animacion
    uint8_t pieI = GPU_addSprite(12, 2, !STEPPED); 
    uint8_t pieD = GPU_addSprite(14, 2, !STEPPED);
    // Pie izquierdo
    // Lo posicionamos en el mapa
    GPU_spriteMove(pieI, 4, 21);
    // Configuramos la velocioad de la primera baldosa que se ve en la anim
    GPU_animationTimer(pieI, 0, velX);
    // Configuramos la velocioad de la segunda baldosa que se ve en la anim
    GPU_animationTimer(pieI, 1, velX);
    
    // Pie derecho
    GPU_spriteMove(pieD, 5, 21);
    GPU_animationTimer(pieD, 0, velX);
    GPU_animationTimer(pieD, 1, velX);
    
    // Resto del T-rex (Estos no estan animados pasivamente)   
    t_rex[6] = GPU_addSprite(5, 1, !STEPPED); //Culo
    t_rex[5] = GPU_addSprite(6, 1, !STEPPED); //Torso
    t_rex[3] = GPU_addSprite(7, 1, !STEPPED); //Ojo
    t_rex[2] = GPU_addSprite(8, 1, !STEPPED); //Morro
    t_rex[0] = GPU_addSprite(9, 1, !STEPPED); //Nuca
    t_rex[1] = GPU_addSprite(10, 1, !STEPPED); //Frente
    t_rex[4] = GPU_addSprite(11, 1, !STEPPED); //Cola
    t_rex[7] = pieI;
    t_rex[8] = pieD;
    // Posicionamos el cuerpo del T-rex en el mapa, ya que su forma no es regular
    // no se puede hacer mediante un bucle...
    GPU_spriteMove(t_rex[0], 4, 20); //Culo
    GPU_spriteMove(t_rex[1], 5, 20); //Torso
    GPU_spriteMove(t_rex[2], 5, 19); //Ojo
    GPU_spriteMove(t_rex[3], 6, 19); //Morro
    GPU_spriteMove(t_rex[4], 5, 18); //Nuca
    GPU_spriteMove(t_rex[5], 6, 18); //Frente
    GPU_spriteMove(t_rex[6], 4, 19); //Cola
    
    // Carga del captus
    for(i = 0; i < 6; i++){
        uint8_t j = i%2;
        uint8_t k = i/2;
        captus[i] = GPU_addSprite(i+20, 1, !STEPPED);
        GPU_spriteMove(captus[i], 38+j, 21-k);
    }
    
    
    // Cargamos los caracteres
    GPU_loadFont(1);
    
    GPU_run();
}
void controller(){
    if(!pause){
        if(velY >= 1-JUMPHEIGH){
            jump();
            velY-=2;
        }
        if(captusX == 0){
            uint8_t i;
            for(i = 0; i < 6; i++){
                uint8_t j = i%2;
                uint8_t k = i/2;
                GPU_spriteMove(captus[i], 38+j, 21-k);
            }
            captusX = 38;
        }
        else{
            uint8_t i;
            for(i = 0; i < 6; i++){
                uint8_t hit = GPU_spriteDisplace(captus[i], -1, 0);
                if(hit){
                    GPU_staticSet(GPU_FONT_G, 16, 13);
                    GPU_staticSet(GPU_FONT_A, 17, 13);
                    GPU_staticSet(GPU_FONT_M, 18, 13);
                    GPU_staticSet(GPU_FONT_E, 19, 13);

                    GPU_staticSet(GPU_FONT_O, 21, 13);
                    GPU_staticSet(GPU_FONT_V, 22, 13);
                    GPU_staticSet(GPU_FONT_E, 23, 13);
                    GPU_staticSet(GPU_FONT_R, 24, 13);
                    pause=1;
                }
            }
            captusX--;
        }
    }
    else{
        GPU_pause(1);
    }
}

void IO_listener(){
    if(EVENT_BUTTON_0 || EVENT_BUTTON_2){
        if(velY < 1-JUMPHEIGH)
            velY = JUMPHEIGH-1;
    }
    if(EVENT_BUTTON_3)
        if(!pause){
            GPU_staticSet(GPU_FONT_G, 16, 13);
            GPU_staticSet(GPU_FONT_A, 17, 13);
            GPU_staticSet(GPU_FONT_M, 18, 13);
            GPU_staticSet(GPU_FONT_E, 19, 13);
            
            GPU_staticSet(GPU_FONT_O, 21, 13);
            GPU_staticSet(GPU_FONT_V, 22, 13);
            GPU_staticSet(GPU_FONT_E, 23, 13);
            GPU_staticSet(GPU_FONT_R, 24, 13);
            pause=1;
        }
        else{
            GPU_staticSet(background, 16, 13);
            GPU_staticSet(background, 17, 13);
            GPU_staticSet(background, 18, 13);
            GPU_staticSet(background, 19, 13);
            
            GPU_staticSet(background, 21, 13);
            GPU_staticSet(background, 22, 13);
            GPU_staticSet(background, 23, 13);
            GPU_staticSet(background, 24, 13);
            uint8_t i;
            for(i = 0; i < 6; i++){
                uint8_t j = i%2;
                uint8_t k = i/2;
                GPU_spriteMove(captus[i], 38+j, 21-k);
            }
            GPU_spriteMove(t_rex[0], 1, 1); //Culo
            GPU_spriteMove(t_rex[1], 2, 1); //Torso
            GPU_spriteMove(t_rex[2], 3, 1); //Ojo
            GPU_spriteMove(t_rex[3], 4, 1); //Morro
            GPU_spriteMove(t_rex[4], 5, 1); //Nuca
            GPU_spriteMove(t_rex[5], 6, 1); //Frente
            GPU_spriteMove(t_rex[6], 7, 1); //Cola
            GPU_spriteMove(t_rex[7], 8, 1); //Frente
            GPU_spriteMove(t_rex[8], 9, 1); //Cola
            
            GPU_spriteMove(t_rex[0], 4, 20); //Culo
            GPU_spriteMove(t_rex[1], 5, 20); //Torso
            GPU_spriteMove(t_rex[2], 5, 19); //Ojo
            GPU_spriteMove(t_rex[3], 6, 19); //Morro
            GPU_spriteMove(t_rex[4], 5, 18); //Nuca
            GPU_spriteMove(t_rex[5], 6, 18); //Frente
            GPU_spriteMove(t_rex[6], 4, 19); //Cola
            GPU_spriteMove(t_rex[7], 4, 21);
            GPU_spriteMove(t_rex[8], 5, 21);
            velY = 0;
            pause=0;
            GPU_pause(0);
        }
}

void IO_interrupt(){
    
}

void jump(){
    uint8_t i;
    int8_t j = velY == 1 || velY == -1 ? velY : velY/2;
    if(j > 0){
        for(i = 0; i < 9; i++){
            GPU_spriteDisplace(t_rex[i], 0, -j);
        }
    }
    else{
        for(i = 8; i >= 0; i--){
            GPU_spriteDisplace(t_rex[i], 0, -j);
        }
    }
}
