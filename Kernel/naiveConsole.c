#include <naiveConsole.h>

//DECLARO EL PROTOTIPO DE TODAS LAS FUNCIONES DE ASSEMBLER QUE NECESITE
//IMPLEMENTO TODAS LAS FUNCIONES AUXILIARES NECESARIAS
//IMPLEMENTO LA "FUNCION PRINCIPAL" QUE UTILIZA LAS FUNCIONES ANTERIORES,
//ESTA VA A SER LA UNICA QUE PODRA SER INVOCADA EN EL MAIN DEL KERNEL.C

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

//EJ1
//implemento las funciones print de naiveConsole.h
//no utilizan funciones de la libasm.asm
#define defaultColor 0x07
#define defaultBack 0x00

/*
static void scroll() {
	if (currentVideo >= video + 2*width*(height-1)) {
		for (int i = 0; i < 2*width*(height-1); i++) {
			video[i] = video[i + 2*width];
		}
	}
	currentVideo -= 2*width;
	for (int j = 0; j < width; j++) {
		currentVideo[j*2] = ' ';
		currentVideo[2*j+1] = 0x07;
	}
}
*/

static uint8_t makeColor(char foreground, char background) {
    return (uint8_t)(foreground | background << 4);
}

void ncPrintCharColor(char character, char foreground, char background)
{
    *currentVideo = character;

	currentVideo++;

	*currentVideo = makeColor(foreground, background);
    currentVideo++;
}

void ncPrintColor(const char * string, char foreground, char background)
{
    int i;
    
    for (i = 0; string[i] != 0; i++)
        ncPrintCharColor(string[i], foreground, background);
}

//fin EJ1

void ncBackspace() {
	if((uintptr_t)currentVideo >= 0xB8002){ // si no es el primer caracter
		currentVideo -= 2;
		*currentVideo = ' ';
	}
}

void ncPrint(const char * string)
{
    ncPrintColor(string, defaultColor, defaultBack);
}

void ncPrintChar(char character)
{
    ncPrintCharColor(character, defaultColor, defaultBack);
}


void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

//EJ3
//declaro los prototipos de las funciones de assembler, estas no son accedidas por el main del kernel.c
//luego creo las funciones auxiliares, que tampoco pueden ser accedidas
//ninguna de estas pueden ser accedidas pues no estan declaradas en naiveConsole.h
//por ultimo implemento la funcion principal que es usada en kernel.c, pues esta declarada en naiveConsole.h
int get_hours();
int get_minutes();
int get_seconds();
int get_weekDay();
int get_monthDay();
int get_month();
int get_year();

void convertToGMTMinus3(int *hours, int *days, int *month, int *year) {
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	
    int monthDays = daysInMonth[(*month - 1) % 12];
	
    if ( *year % 4 == 0 && (*year % 100 != 0 || *year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    *hours -= 3;
    if (*hours < 0) {
        *hours += 24;
        *days -= 1;
        if (*days < 1) {
            *month -= 1;
			monthDays=daysInMonth[(*month - 1) % 12];
            if (*month < 1) {
                *month = 12;
                *year -= 1;
            }
            *days = monthDays;
        }
    }
}

 
char * TimeClock(){
	static char buffer[121];
	char * days[]={"sun", "lun", "mar", "mie", "jue", "vie", "sab"};
	int  hours = get_hours();
	int  minutes = get_minutes();
	int  seconds = get_seconds();
	int  weekday = get_weekDay();
	int  monthDay = get_monthDay();
	int  month = get_month();
	int  year = get_year();
    convertToGMTMinus3(&hours, &monthDay, &month, &year);
	int digits = uintToBase(hours, buffer, 10);
	buffer[digits++] = ':';
	digits += uintToBase(minutes, buffer+digits, 10);
	buffer[digits++] = ':';
	digits += uintToBase(seconds, buffer+digits, 10);
	buffer[digits++] =' ';
	for(int i = 0; i < 3; i++)
		buffer[digits++] = days[weekday-1][i];
	buffer[digits++] =' ';
	digits += uintToBase(monthDay, buffer+digits, 10);
	buffer[digits++] ='/';
	digits += uintToBase(month, buffer+digits, 10);
	buffer[digits++] ='/';
	digits += uintToBase(year, buffer+digits, 10);
	buffer[digits++] =' ';
	buffer[digits] = 0;
	return buffer;
}

//fin EJ3