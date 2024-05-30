#include <time.h>
#include <speaker.h>

//Play sound using built-in speaker
 static void playSound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
 }
 
 //Make a beep
 void beep(uint64_t ticks, uint64_t freq) {
 	 playSound(freq);
 	 sleep(ticks);
 	 nosound();
 }

 void playEasterEggSong(){
	beep(4, DOFREQ);
	beep(4, DOSOSTFREQ);
	beep(4, SOLFREQ);
	beep(4, MIFREQ);
	beep(4, SOLFREQ);
	beep(4, MIFREQ);
	
	beep(4, DOSOSTFREQ);
	beep(4, DOFREQ);
	beep(4, SOLSOSTFREQ);
	beep(4, FAFREQ);
	beep(4, SOLSOSTFREQ);
	beep(4, FAFREQ);

	beep(4, DOFREQ);
	beep(4, DOSOSTFREQ);
	beep(4, SOLFREQ);
	beep(4, MIFREQ);
	beep(4, SOLFREQ);
	beep(4, MIFREQ);

	beep(2, FAFREQ);
	beep(3, FASOSTFREQ);
	beep(2, SOLFREQ);
	beep(3, SOLSOSTFREQ);
	beep(2, LAFREQ);
	beep(3, SIFREQ);
	beep(2, DOMENFREQ);
 }