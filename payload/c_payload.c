/*
  (C) COPYRIGHT 2016 TECHNOLUTION B.V., GOUDA NL
  =======          I                   ==          I    =
     I             I                    I          I
|    I   ===   === I ===  I ===   ===   I  I    I ====  I   ===  I ===
|    I  /   \ I    I/   I I/   I I   I  I  I    I  I    I  I   I I/   I
|    I  ===== I    I    I I    I I   I  I  I    I  I    I  I   I I    I
|    I  \     I    I    I I    I I   I  I  I   /I  \    I  I   I I    I
|    I   ===   === I    I I    I  ===  ===  === I   ==  I   ===  I    I
|                 +---------------------------------------------------+
+----+            |  +++++++++++++++++++++++++++++++++++++++++++++++++|
     |            |             ++++++++++++++++++++++++++++++++++++++|
     +------------+                          +++++++++++++++++++++++++|
                                                        ++++++++++++++|
                                                                 +++++|
 */

#include <stdio.h>

void payload(void);

extern int start;

void payload(void){

	const char * hacked = "\n##     ##    ###     ######  ##    ## ######## ########  \r\n"
	"##     ##   ## ##   ##    ## ##   ##  ##       ##     ## \r\n"
	"##     ##  ##   ##  ##       ##  ##   ##       ##     ## \r\n"
	"######### ##     ## ##       #####    ######   ##     ## \r\n"
	"##     ## ######### ##       ##  ##   ##       ##     ## \r\n"
	"##     ## ##     ## ##    ## ##   ##  ##       ##     ## \r\n"
	"##     ## ##     ##  ######  ##    ## ######## ########  \r\n";


    printf("\n\n%s\n\n",hacked);
	int num = 0;
    for(;;){
        setLeds(0x9700, 0);
    }
}