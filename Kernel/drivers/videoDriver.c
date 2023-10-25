#include <videoDriver.h>
#include <font.h>
#include <lib.h>

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

uint32_t cursorX  = 0;
uint32_t cursorY  = 0;
uint32_t size = 1;

void changeSize(uint32_t new_size) {
    size += new_size;
}


void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

static void put_square(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor) {
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = 0; j < size; j++) {
            putPixel(hexColor, x + i, y + j);
        }
    }
}

// Funcion que limpia la pantalla con un color
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

// Funcion que limpia la pantalla
void clear(){
	clearColor(0x000000);
	return;
}


void drawChar(uint64_t hexColor, char character) {
    int a = cursorX;
    int x = a;
    int y = cursorY;
    int start = character - 33;
    int flag = 1;

    if (a >= SCREEN_WIDTH - size * 8 && character != '\n') {
        flag = 0;
        newline();
    }

    for (int i = 0; i < 32; i++) {
        if (i % 2 == 0 && i != 0) {
            y += size;
            a = x;
        }
        
        font[i + (start * 32)] & (char)0x01 ? put_square(a, y, size, hexColor) : 0;
        
        a += size;
        
        uint8_t aux = 0x02;
        for (int j = 0; j < 8; j++) {
            ((uint8_t)font[i + (start * 32)] & (uint8_t)aux) >> j ? put_square(a, y, size, hexColor) : 0;
            a += size;
            aux <<= 1;
        }
    }
    if (flag) {
        cursorX += DEFAULT_CHAR_WIDTH*size;
    }
}

void drawString(uint64_t hexColor, char* word) {
    for (int i=0; word[i] != 0; i++) {
        drawChar(hexColor, word[i]);
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

void backspace(){
     if (cursorX > 0) {
            cursorX -= size*8;
        } else if (cursorY > 0 && cursorX == 0) { // El cursor está al principio de una línea
            // Borra el último carácter de la línea anterior
            cursorY -= size*16;
            cursorX = SCREEN_WIDTH - size*8; // Establece el cursorX al último carácter de la línea anterior
            
        }
        // uint64_t hex_backspace = bg_color;
		drawRectangle(0x000000, cursorX, cursorY, size*8, size*16);
}

void space() {
    if (cursorX == SCREEN_WIDTH) {
        newline();
    }
    cursorX += size*8;
}

void newline(){
    cursorX = 0;
    cursorY += (size*16);
    return;
}

void tab(){
    int tabWidth = 15;
        int spaces = tabWidth - (cursorX / size*8) % tabWidth;

        for (int i = 0; i < spaces; i++) {
            drawChar(0x000000, ' ');
            cursorX += size*8;
        }
        return;
}

void moveOneLineUp() {
    char* dst = (char*)(uintptr_t)(VBE_mode_info->framebuffer); // Puntero al framebuffer
    char* src = dst + VBE_mode_info->pitch * size * 16; // Puntero a la línea de origen
    uint64_t numBytes = VBE_mode_info->pitch * (VBE_mode_info->height - size * 16); // Cantidad de bytes a copiar

    memcpy(dst, src, numBytes); // Copia los bytes desde la línea de origen a la línea de destino
    // memset((void*)(uintptr_t)(VBE_mode_info->framebuffer + VBE_mode_info->pitch * (VBE_mode_info->height - size * 16)), 0, VBE_mode_info->pitch * size * 16); // Rellena con ceros la parte de la línea de destino copiada
    drawRectangle(0x000000, 0, VBE_mode_info->height - size*16, 1024, size*16 );
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
        if (cursorX >= SCREEN_WIDTH) {
            cursorX = 0;
            cursorY += size*16;
        }
        if (cursorY >= SCREEN_HEIGHT){ 
            cursorX = 0;
            moveOneLineUp();
        }
        drawChar(hexColor, c);
        cursorX += size*8;
        return;
}

