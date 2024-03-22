#include "include/videoDriver.h"
#include "include/font.h"
#include "../include/naiveConsole.h"
#include <lib.h>
#include "../include/keyboard_buffer.h"

unsigned int SCREEN_WIDTH = 1024;
unsigned int SCREEN_HEIGHT = 768;

struct vbe_mode_info_structure { 
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00; 

uint64_t bg_color = 0x000000;
uint64_t font_color = 0xFFFFFF;

uint32_t cursorX  = 0;
uint32_t cursorY  = 0;
double size = DEFAULT_FONT_SIZE;

unsigned int getMaxHeight() {
	return SCREEN_HEIGHT;
}

unsigned int getMaxWidth() {
	return SCREEN_WIDTH;
}


void putPixel(uint64_t hexColor, uint32_t x, uint32_t y) {
	uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void put_square(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor) {
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = 0; j < size; j++) {
            putPixel(hexColor, x + i, y + j);
        }
    }
}

void drawRectangle(uint64_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putPixel(color, x + j, y + i);
        }
    }
    return;
}

void clearColor(uint64_t hexColor){
	for (int x = 0; x < VBE_mode_info->width; x++){
		for (int y = 0; y < VBE_mode_info-> height; y++){
			putPixel(hexColor,x,y);
		}
	}
    cursorX=0;
    cursorY=0;
	return;
}

void clear(){
	clearColor(bg_color);
	return;
}

void drawChar(uint64_t hexColor, char character) {
    int a = cursorX;  // Posición horizontal actual
    int x = a;  // Posición horizontal actual
    int y = cursorY;  // Posición vertical actual
    int start = character - 33;  // Índice de inicio en el vector de fuentes
    
    // Si el carácter es minúscula, ajusta el índice de inicio
    if (isMinusc(character))
        start = character - 'a';
    
    if (character == ' ') {
        return;  // Si es un espacio, no se dibuja nada
    }

    for (int i = 0; i < 32; i++) {
        if (i % 2 == 0 && i != 0) {
            y += size;  // Salto a la siguiente fila de píxeles
            a = x;  // Reinicia la posición horizontal al inicio
        }
        
        // Comprueba el bit correspondiente en la fuente para determinar si se debe dibujar un píxel
        font[i + (start * 32)] & (char)0x01 ? put_square(a, y, size, hexColor) : 0;
        
        a += size;  // Avanza a la siguiente posición horizontal
        
        uint8_t aux = 0x02;
        for (int j = 0; j < 8; j++) {
            // Comprueba cada bit de la fuente y dibuja un píxel si está activo
            ((uint8_t)font[i + (start * 32)] & (uint8_t)aux) >> j ? put_square(a, y, size, hexColor) : 0;
            a += size;  // Avanza a la siguiente posición horizontal
            aux <<= 1;  // Desplaza el bit auxiliar hacia la izquierda
        }
    }
}

void clearScreen(){
    drawRectangle(bg_color,0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    cursorX = 0;
    cursorY = 0;
}

void backspace(){
    if (cursorX > size*56) {
        cursorX -= size*8;
    }
	drawRectangle(bg_color, cursorX, cursorY, size*8, size*16);
}

void newline(){
    cursorX = 0;
    cursorY += (size*16);
    return;
}

void tab(){
    int tabWidth = 15;
        int spaces = tabWidth - (cursorX / (int)size*8) % tabWidth;

        for (int i = 0; i < spaces; i++) {
            drawChar(WHITE, ' ');
            cursorX += size*8;
        }
        return;
}

void moveOneLineUp() {
    char* dst = (char*)(uintptr_t)(VBE_mode_info->framebuffer); // Puntero al framebuffer
    char* src = dst + VBE_mode_info->pitch * (int)size * 16; // Puntero a la línea de origen
    uint64_t numBytes = VBE_mode_info->pitch * (VBE_mode_info->height - size * 16); // Cantidad de bytes a copiar

    memcpy(dst, src, numBytes); // Copia los bytes desde la línea de origen a la línea de destino
    // memset((void*)(uintptr_t)(VBE_mode_info->framebuffer + VBE_mode_info->pitch * (VBE_mode_info->height - size * 16)), 0, VBE_mode_info->pitch * size * 16); // Rellena con ceros la parte de la línea de destino copiada
    drawRectangle(bg_color, 0, VBE_mode_info->height - size*16, 1024, size*16 );
    cursorY -= (size * 16); // Actualiza la posición del cursor en el eje Y
}

void character(uint64_t hexColor, char c){
        if (c == '\b') { // backspace
            backspace();
            return;
        }
        if (c == '\t') { // Tab
            tab();
            return;
        }
        if (c == '\n') { // Salto de línea
            newline();
            return;
        }
        if (c == ' '){
            cursorX += size*8;
            return;
        }
        //Carácter
        if (cursorX >= getMaxWidth()) {
            cursorX = 0;
            cursorY += size*16;
        }
        if (cursorY >= getMaxHeight()){ 
            cursorX = 0;
            moveOneLineUp();
        }
        drawChar(hexColor, c);
        cursorX += size*8;
        return;
}

void drawWordColor(uint64_t hexColor, char* word) {
    for (int i=0; word[i] != 0; i++) {
        character(hexColor, word[i]);
    }
}

void drawWord(char* word) {
    drawWordColor(GREEN, word);
}

void drawRegisters(int value){
        char buffer[256] = {0};
        uintToBase(value, buffer, 16);
        drawWordColor(WHITE, "0x");
        drawWordColor(WHITE, buffer);
     newline();
}

void changeSize(double new_size){
    cleanBuffer();
    if ((size + new_size) == 0)
        return;
    if ((size + new_size) == 5)
        return;
    size += new_size; 
}


void setSize(int new_size){
    cleanBuffer();
    if(new_size > 0){
        size = new_size;
    }
    
}

void drawCursor(){
    drawRectangle(GREEN, cursorX, cursorY, size*8, size*16);
}

void deleteCursor(){
    drawRectangle(bg_color, cursorX, cursorY, size*8, size*16);
}

void putCharInSetSpace(char c, int color, int x, int y){
    cursorX = x; 
    cursorY = y;
    drawChar(COLORS[color], c);
}

void drawSquareInSetPosition(int x, int y, int size, int color){
    put_square(x, y, size, COLORS[color]);

}