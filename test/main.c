#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "strlist.h"

int main(){

	strlist* list = newStrList();
	
	pushBack(list, "Cadena1");
	pushBack(list, "Cadena2");
	pushBack(list, "Cadena3");
	pushBack(list, "Cadena4");
	
	printf("%s\n", getCurrent(list));
	
	nextStr(list);
	
	printf("%s\n", getCurrent(list));
	
	nextStr(list);
	
	printf("%s\n", getCurrent(list));

	prevStr(list);
	
	printf("%s\n", getCurrent(list));
	
	deleteStrList(list);

	return 0;
}
