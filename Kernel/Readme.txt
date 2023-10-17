libasm.asm
aca van las funciones de assembler que directamente trabajan con el procesador, memoria y perifericos
se las implementan aqui mismo

lib.h
aca hay 2 opciones, las funciones de assembler implementadas anteriormente puedo:
1. utilizar las por separado
2. solo usarlas dentro de otra funcion y que no se pueda tener acceso a estas individualmente
para el primer caso, aca van los prototipos de las funciones de assembler
sino, no declaro nada aca

naiveConsole.c
primero van los prototipos de las funciones de assembler, igual que en lib.h de asi quererlo
luego las funciones auxiliares
por ultimo la funcion principal a implementar, que utiliza las funciones auxiliares y las funciones de assembler

naiveConsole.h
aca va el prototipo de la funcion principal, la del ultimo paso de antes

kernel.c
dentro del main, utilizo la funcion principal. la declarada en naiveConsole.h