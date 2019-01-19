/* * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Autor: Stefano Vidaurre Olite                   *
 * Fecha: 24/11/2016                               *
 * Descripcion: Libreria encargada de la gestion de*
 * los graficos, renderizacion, y transmision de   *
 * los frames generados, al TFT mediante SPI.      *
 *                                                 *
 * V: 2.0.1                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GPUCORE_H__
#define	__GPUCORE_H__

#include <stdint.h>
#include <stdlib.h>
#include "../IOCore/IOCore.h"
#include <xc.h>

#define GPU_TILES_SIZE 256
#define GPU_FONT_SIZE 26
#define GPU_TILESMAP_SIZE 1024*20
#define GPU_ANIM_SIZE 8
#define GPU_PIXELS_SIZE_I 4
#define GPU_PIXELS_SIZE_J 8
#define GPU_PALETTES_SIZE 8
#define GPU_COLORS_SIZE 16
#define GPU_SPRITES_SIZE 256
#define GPU_WIDTH 320
#define GPU_HEIGHT 240
#define GPU_TILES_Y 30
#define GPU_TILES_X 40



typedef struct GPU *GPU;
typedef struct Pixel Pixel;
typedef struct Palette *Palette;
typedef struct TableDrawables TableDrawables;
typedef struct Drawable Drawable;
typedef struct InfoDraw InfoDraw;
typedef struct InfoMove InfoMove;
typedef struct InfoAnim InfoAnim;
typedef struct Map Map;

//===== FONT ======
// Indices del conjunto de letras y numeros
#define GPU_FONT_INI GPU_TILES_SIZE - GPU_FONT_SIZE
#define GPU_FONT_A GPU_FONT_INI
#define GPU_FONT_B GPU_FONT_INI + 1
#define GPU_FONT_C GPU_FONT_INI + 2
#define GPU_FONT_D GPU_FONT_INI + 3
#define GPU_FONT_E GPU_FONT_INI + 4
#define GPU_FONT_F GPU_FONT_INI + 5
#define GPU_FONT_G GPU_FONT_INI + 6
#define GPU_FONT_H GPU_FONT_INI + 7
#define GPU_FONT_I GPU_FONT_INI + 8
#define GPU_FONT_J GPU_FONT_INI + 9
#define GPU_FONT_K GPU_FONT_INI + 10
#define GPU_FONT_L GPU_FONT_INI + 11
#define GPU_FONT_M GPU_FONT_INI + 12
#define GPU_FONT_N GPU_FONT_INI + 13
#define GPU_FONT_O GPU_FONT_INI + 14
#define GPU_FONT_P GPU_FONT_INI + 15
#define GPU_FONT_Q GPU_FONT_INI + 16
#define GPU_FONT_R GPU_FONT_INI + 17
#define GPU_FONT_S GPU_FONT_INI + 18
#define GPU_FONT_T GPU_FONT_INI + 19
#define GPU_FONT_U GPU_FONT_INI + 20
#define GPU_FONT_V GPU_FONT_INI + 21
#define GPU_FONT_W GPU_FONT_INI + 22
#define GPU_FONT_X GPU_FONT_INI + 23
#define GPU_FONT_Y GPU_FONT_INI + 24
#define GPU_FONT_Z GPU_FONT_INI + 25

//===== Pixel =====
// Clase usada para guardar la informacion de dos pixels, uno situado (color2)
// inmediatamente debajo del otro (color1)
struct Pixel{
    uint8_t color1 : 4; // Color del pixel superior
    uint8_t color2 : 4; // Color del pixel inferior
};

//===== Tile =====
// Clase usada para dibujar un mapa de pixels. Es la unidad minima de dibujo
// que permite el motor grafico, la cual requiera una renderizacion. Permite
// definir tanto fondos, como sprites o baldosas estaticas
struct Tile{
    Pixel pixels[GPU_PIXELS_SIZE_I][GPU_PIXELS_SIZE_J]; // Matriz de pixeles
};

struct __GPU_COLORS_DRAW{
    uint16_t color1;
    uint16_t color2;
};

//===== Palette =====
// Clase usada para proporcionar un ahorro significativo de memoria. Permitiendo
// acceder a toda la paleta de colores de 16bits con tan solo 4bits. Cada paletta
// por defecto puede almacenar 16 colores.
struct Palette{
    uint16_t colors[GPU_COLORS_SIZE];
};

//===== Map =====
// Clase usada para posicionar las baldosas en un plano 2D. Nos permite saber
// que baldosa va en cada lugar en todo momento. Tambien nos permite movernos
// a lo largo de un mapa mas grande que la pantalla gracias al puntero, el 
// cual nos indica que posicion empezamos a visualizar
struct Map{
    uint16_t height; //Altura del mapa (Se usa para leer el array)
    uint16_t width; //Anchura del mapa (Se usa para leer el array)
    int32_t pY; //Indica donde empieza a leer en Y (Son necesarios los negativos para el scroll)
    int32_t pX; //Indica donde empueza a leer en X (Son necesarios los negativos para el scroll)
    uint8_t tilesMap[GPU_TILESMAP_SIZE]; //Puntero que contiene el mapa de baldosas
};

//===== InfoDraw =====
struct InfoDraw{
    uint8_t sup;         // Posicion de la baldosa superior (0 si no hay)
    uint8_t back;        // Posicion de la baldosa de fondo
    uint8_t palette : 3; // Paleta seleccionada para esta baldosa[0-7]
    uint8_t visible : 1;  // Nos indica si hay que dibujarlo o no
};

//===== InfoMove =====
struct InfoMove{
    uint16_t sY;         // Posicion en el eje y en el tilesMap
    uint16_t sX;         // Posicion en el eje x en el tilesMap
    uint8_t step : 1;    // Nos indica si se puede pisar dicha baldosa o no
    uint8_t sprite : 1;  // Nos indica si es una baldosa estatica o movible
};

//===== InfoAnim =====
struct InfoAnim{
    uint8_t ini;     // Baldosa inicial del ciclo de animacion       
    uint8_t view;    // Baldosa que se esta viendo actualmente
    uint8_t timer[GPU_ANIM_SIZE];  // Velocidad de la animacion
    uint8_t next[GPU_ANIM_SIZE];   // Baldosa a la que pasa dentro del sprite
    uint8_t tile[GPU_ANIM_SIZE];
};

//===== Drawable =====
// Clase usada para administrar las baldosas que son dinamicas, Sprites y fondos
// dinamicos. Se encarga de organizarlos para facilitar la renderizacion dinamica,
// dado que al no ser staticos en el tiempo hay que hacer modificaciones en el 
// tilesMap.
struct Drawable{
    struct Tile *tile; //Puntero hacia la baldosa que se muestra
    InfoDraw infoDraw;
    InfoMove infoMove;
    InfoAnim infoAnim;
};

//===== TableDrawables =====
// Clase que almacena el conjunto de objetos dibujables existentes asi como del
// tiempo global usado para las animaciones pasivas.
struct TableDrawables{
    Drawable drawables[GPU_SPRITES_SIZE];
    uint8_t length;
    uint8_t globalTimer;
};

//===== GPU =====
// Clase principal que engloba a todas las demas. Esta clase es la unica que se debe de
// manejar mas haya de los constructores de Tile y Palette. La api proporciona todas
// las posibilidades de la GPU usando unicamente esta clase, de esta forma obtenemos
// una buena encapsulacion.
struct GPU{
    struct Tile tiles[GPU_TILES_SIZE];
    struct Palette palettes[GPU_PALETTES_SIZE];
    TableDrawables tableDrawables;
    Map map;
};

// GPU_init()
// Ent: --
// Sal: --
// Des: Se encarga de inicializar la GPU, es necesario ejecutarlo al principio
// del programa, sin el no funciona la API.
void GPU_init();

// GPU_newTile()
// Ent: 
//  index -> Indice de memoria donde guardar la baldosa
//  pixels -> Array de pixeles que formaran la baldosa  
// Sal: --
// Des: Permite crear baldosas de forma independiente, de este modo se pueden
// guardar dentro de la GPU o en una memoria externa para ser cargadas despues
void GPU_newTile(uint8_t index, uint8_t pixels[GPU_PIXELS_SIZE_I][GPU_PIXELS_SIZE_J]);

// GPU_newPalette()
// Ent:
//  index -> Indice de memoria donde guardar la paleta
//  colors -> Array de colores formado por los tres componentes RGB  
// Sal: --
// Des: Permite crear paletas de forma independiente, de este modo se pueden
// guardar dentro de la GPU o en una memoria externa para ser cargadas despues
void GPU_newPalette(uint8_t index, uint8_t colors[GPU_COLORS_SIZE][3]);

// GPU_loadMap()
// Ent: 
//  tilesMap -> Direccion en la que se encuentra almacenado el mapa de baldosas
//  height -> Altura del mapa
//  width -> Anchura del mapa
// Sal: --
// Des: Funcion que carga un mapa (matriz de uint8_t), la cual mostrara la GPU
// por la pantalla. Esta formada por los indices de los objetos dibujables!!
void GPU_loadMap(uint8_t *tilesMap, uint16_t height, uint16_t width);

// GPU_RGB()
// Ent: 
//  r,g,b -> Intensidad de cada color en escala 255
// Sal:
//  Color -> Color en 16bits 
// Des: Funcion que pasa de rgb255 a color en 16bits
uint16_t GPU_RGB(uint8_t r, uint8_t g, uint8_t b);

// GPU_black()
// Ent: --
// Sal: --
// Des: Pantallazo negro
void GPU_black();

// GPU_scroll()
// Ent: 
//  pX -> Numero de casillas que nos desplazamos en horizontal (>0 derecha, <0 izquierda)
//  pY -> Numero de casillas que nos desplazamos en vertical (>0 bajar, <0 subir)
// Sal:
//  Bool -> 1 en caso de poder hacer el scroll, 0 en caso de no ser posible
// Des: Funcion que nos permite hacer un scroll de la pantalla en cualquier direccion
// siempre y cuando los limites del mapa nos lo permitan
uint8_t GPU_scroll(int pX, int pY);

// GPU_draw()
// Ent: --
// Sal: --
// Des: Funcion que hace que la GPU mande el frame actual a la pantalla, hay que tener
// en cuenta que el envio de frames afecta a las animaciones pasivas, dado que el contador
// que utilician se guia por refrescos de pantalla (frames enviados)
void GPU_draw();

// GPU_run()
// Ent: --
// Sal: --
// Des: funcion que da inicio a la ejecucion del bucle principal del programa, incluye
// los refrescos de pantalla, el manejo de eventos y el control del paso de frames. De este
// modo, si se usa esta funcion solo es necesario implementar GPU_controller, IO_interrupt y
// IO_listener, todo lo demas se gestiona de forma opaca.
void GPU_run(); 

void GPU_pause(uint8_t enable);

uint8_t GPU_addSprite(uint8_t ini, uint8_t length, uint8_t step);
uint8_t GPU_addStatic(uint8_t ini, uint16_t length, uint8_t timer, uint8_t back, uint8_t step);
uint8_t GPU_spriteMove(uint8_t idA, uint16_t sX, uint16_t sY);
uint8_t GPU_spriteDisplace(uint8_t idA, uint16_t sX, uint16_t sY);
uint8_t GPU_staticSet(uint8_t idA, uint16_t sX, uint16_t sY);
uint8_t GPU_animationNext(uint8_t index);
uint8_t GPU_animationReorder(uint8_t draw, uint8_t tile, uint8_t next);
uint8_t GPU_animationTimer(uint8_t draw, uint8_t tile, uint8_t timer);
void GPU_paletteChange(uint8_t idA, uint8_t palette);

void GPU_loadFont(uint8_t palette);

#endif	/* GPUCORE_H */

