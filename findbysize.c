#include "findbysize.h"
#include "fileutil.h"
#include "color.h"

int level = 0; // Nivel de profundiad.

// Funcion para saber si el fichero se encuentra entre los tamaños min y max.
int sizeOK(int max, int min, char* name){

	int retValue = 0;

	// Abrir
	int fd = open(name, O_RDONLY);
	
	if(fd==-1){
		perror("Fallo al abrir el fichero.");
	}else{
		struct stat buf = getStat(fd);
		retValue = ((max >= buf.st_size) && (buf.st_size >= min));
	
		// Cerrar
		if(close(fd) == -1){
			perror("Fallo al cerrar el fichero");
		}
	}

	
	return retValue;
}

// Funcion que imprime un numero de tabulados proporcional al nivel de profundidad.
// Sirve para hacer mas grafica la jerarquia de directorios.
void printTabs(int lvl){
	int i;
	for(i=0;i<lvl;i++)
		printf("\t");
}

#define R 0 // regular
#define D 1 // directorio

// Funcion que imprime una linea con diferente formato segun sea del tipo D (directorio) o R(regular).
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

// Funcion que devuelve si la entrada es un fichero reg o un dir
int processEntry(char* name){

	int retValue = -1;
	
	if((isReg(name) != -1) && (isReg(name))){
		retValue=R;
	}
	else if((isDir(name) != -1) && (isDir(name))){
		retValue=D;
	}
		
	return retValue;
}

// Funcion recursiva que imprime toda la jerarquia de directorios a partir de un directorio raiz dado.
// Solo muestra los ficheros que se ajustan a los tamaños especificados.
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
	}else{
	
		chdir(dirname);
	
		struct dirent *entry;
	
		// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer.
		// Por eso el error se comprueba con errno.
		errno = 0;
		int type;
		while((entry = readdir(dir)) != NULL){	
			type = processEntry(entry->d_name);
			if((type != -1) && (type==R) && sizeOK(max, min, entry->d_name))
				printLine(entry->d_name,R);
			else if((type != -1) && (type==D) &&(strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)){
				level++;
				deepFind(max,min,entry->d_name);
				level--;
			}
		}
		
		if(errno){
			printf("Fallo al leer el directorio.\n");
			errno = 0;
		}
	
		if(closedir(dir) == -1){
			perror("Fallo al cerrar el directorio.\n");
		}
	
		// Volvemos al directorio
		chdir(auxDir);
		free(auxDir);
	}
}

// Funcion que recibe un array los tamaños entre los que hay que buscar ficheros y
void findbysize(char** cline){

	int min = atoi(cline[1]);
	int max = atoi(cline[2]);

	/* 
	 La lista de directorios no tiene limite aparentemente,
	 se introduce como: <dir1> <dir2> ...
	*/
	
	int param = 3; // empiezan en el tercer parametro.
	while(cline[param] != NULL){
		deepFind(max, min, strdup(cline[param])); // Para cada dir hacer una busqueda en profundidad.
		param++;
	}

}
