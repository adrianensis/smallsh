#include "findbysize.h"
#include "fileutil.h"
#include "color.h"

int level = 0; // Nivel de profundiad.

int sizeOK(int max, int min, int fileDesc){
	struct stat buf = getStat(fileDesc);
	return ((max >= buf.st_size) && (buf.st_size >= min));
}

// TODO Hacer funcion para el nivel de tabulados.
void printTabs(int lvl){
	int i;
	for(i=0;i<lvl;i++)
		printf("\t");
}

#define R 0 // regular
#define D 1 // directorio
void printLine(char* name, int type){
	printTabs(level);
	if(type==R){
		printf("\t");
		setColor(COLOR_BACK_GREEN);
		printf("%s", name);
	}else if(type==D){
		setColor(COLOR_BACK_YELLOW);
		printf("%s", name);
	}
	setColor(COLOR_RESET);
	printf("\n");
}

// Procesar entrada
int processEntry(char* name){

	int retValue; // La funcion devuelve si la entrada es un fichero reg o un dir
	
	if(isReg(name)){
		retValue=R;
	}
	else if(isDir(name)){
		retValue=D;
	}
		
	return retValue;
}

void deepFind(int max, int min, char* dirname){

	// Guardamos el directorio actual.
	char* auxDir = malloc(128);
	getcwd (auxDir, 128);

	/*analizar que hay en dir y mostrar ficheros reg aptos,
	si hay mas directorios:
	para cada subdir hacer deepFind(subdir)*/
	
	printLine(dirname,D);
	
	DIR* dir = opendir(dirname);
	if(dir == NULL){
		perror("Fallo al abrir el directorio.\n");
		exit(EXIT_FAILURE);
	}
	
	chdir(dirname);
	
	struct dirent *entry;
	
	// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer.
	// Por eso el error se comprueba con errno.
	errno = 0;
	while((entry = readdir(dir)) != NULL){	
		if(processEntry(entry->d_name)==R)
			printLine(entry->d_name,R);
		else if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)){
			level++;
			deepFind(max,min,entry->d_name);
			level--;
		}
	}
	if(errno){
		printf("Fallo al leer el fichero directorio.\n");
		errno = 0;
	}
	
	if(closedir(dir) == -1){
		perror("Fallo al cerrar el directorio.\n");
		exit(EXIT_FAILURE);
	}
	
	// Volvemos al directorio
	chdir(auxDir);
	free(auxDir);
	
}

void findbysize(char** cline){

	int min = atoi(cline[1]);
	int max = atoi(cline[2]);

	/* 
	 La lista de directorios no tiene limite aparentemente,
	 se introduce como: <dir1> <dir2> ...
	*/
	
	int counter = 0; // Contador para los directorios.
	char** arrayDir = malloc(sizeof(char*)*50); // Pongamos que se puede analizar 50 directorios como maximo.
	int param = 3; // empiezan en el tercer parametro.
	while(cline[param] != NULL){
		arrayDir[counter] = strdup(cline[param]);
		param++;
		counter++;
	}
		
	// Para cada directorio abre una busqueda en profundidad (deepFind).
	
	
	
	if(counter > 0){
		int i;
		for(i=0; i<counter; i++){
			deepFind(max, min, arrayDir[i]);
		}
	}
	
	free(arrayDir);

}
