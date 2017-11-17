# avr-example2

The Example demonstrates a command line interface.

The code uses peter fleurys uart library. In order to be able to use stdio functions,
the stdin and stdout filedescriptors are rewritten.

The Library with documentation is here http://homepage.hispeed.ch/peterfleury/uartlibrary.zip


The code can be built and flashed :
make 
make flash

The Make file assumes an Arduino Mega @ COM8
