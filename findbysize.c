#include "smallsh.h"

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

#define R 0
#define D 1
 
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
void deepFind(int max, int min, char* path, char* dirname){

	// Guardamos el directorio actual.
	char* auxDir = malloc(128);
	getcwd (auxDir, 128);

	/*analizar que hay en dir y mostrar ficheros reg aptos,
	si hay mas directorios:
	para cada subdir hacer deepFind(subdir)*/
	
	DIR* dir = opendir(dirname);
	if(dir == NULL){
		perror("Fallo al abrir el directorio.\n");
	}else{
		
		// La primera vez se le asigna el directorio actual a la variable path.
		if(path == NULL){
			printf("%s\n", dirname);
			path = malloc(256);
			sprintf(path, "%s", dirname);
		}
	
		// Nos movemos al directorio.
		chdir(dirname);
	
		struct dirent *entry;
	
		// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer.
		// Por eso el error se comprueba con errno.
		errno = 0;
		int type;
		while((entry = readdir(dir)) != NULL){	
			type = processEntry(entry->d_name);
			if((type != -1) && (type==R) && sizeOK(max, min, entry->d_name))
				printf("%s/%s\n", path, entry->d_name);
				
			else if((type != -1) && (type==D) &&(strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)){
			
				level++;
				printf("%s/%s\n", path, entry->d_name);
				
				// Guardamos el path.
				char* aux = malloc(256);
				sprintf(aux,"%s", path);
				
				// Concatenamos el directorio al path y seguimos buscando dentro.
				sprintf(path, "%s/%s", path, entry->d_name);
				deepFind(max,min,path,entry->d_name);
				
				// Restauramos el path.
				sprintf(path,"%s", aux);
				free(aux);
				
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
	
	// Si no hay directorios se toma el actual por defecto.
	if(cline[param] == NULL){
		char* path = NULL;
		deepFind(max, min, path, "."); // Para cada dir hacer una busqueda en profundidad.
		free(path);	
	}
		
	while(cline[param] != NULL){
		char* path = NULL;
		deepFind(max, min, path, cline[param]); // Para cada dir hacer una busqueda en profundidad.
		free(path);
		param++;
	}

}
