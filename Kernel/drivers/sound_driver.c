#include "include/sound_driver.h"
#include "./lib.h"

extern uint8_t inb(uint16_t in1);
extern void outb(uint16_t out1, uint8_t in1);

// Produces a sound in speaker
 static void play_sound(uint32_t nFrequence) {              // code source: https://wiki.osdev.org/PC_Speaker
 	uint32_t Div;
 	uint8_t tmp;
 
        // configuartion of speaker
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        // produces sound
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
// mutes speaker output
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 
 	outb(0x61, tmp);
 }
 
 
 // makes beep sound
 void beep(int freq) {
 	 play_sound(freq);
 	 sleepms(1);
 	 nosound();
 }