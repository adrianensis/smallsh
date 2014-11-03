#include "findbysize.h"
#include "color.h"

// argumento: descriptor de ficheros
bool isDir(int fileDesc){
	/*usar:
	int fstat(int fileDescriptor, struct stat* buf);
	nos interesa obtener buf->st_mode.
	S_ISDIR(buf->st_mode);*/
}

bool isReg(int fileDesc){
	//S_ISREG(buf->st_mode);
}

bool sizeOK(int max, int min, int fileDesc){

}

#define REG 0
#define DIR 1
void printLine(char* name, int type){
	/*
	if type==REG
		print color1
	else if type==DIR
		print color2*/
}

// Procesar entrada
int processEntry(char* name){

	int retValue; // La funcion devuelve si la entrada es un fichero reg o un dir

	// Abrir
	int fd = open(name, O_RDONLY);
	
	// TODO exit o tratar o notificar?
	if(fd==-1){
		perror("read");
		exit(EXIT_FAILURE);
	}
	
	// Procesar
	struct stat statBuf;
	fstat(fd, &statBuf)
	
	if(isReg(fd)){
		retValue=REG;
		printLine(name,REG);
	}
	else if(isDir(fd)){
		retValue=DIR;
	}
		
	
	// Cerrar
	if(close(fd) == -1){
			perror("close");
			exit(EXIT_FAILURE);
		}
}

void deepFind(int max, int min, char* dir){
	/*analizar que hay en dir y mostrar ficheros reg aptos,
	si hay mas directorios:
	para cada subdir hacer deepFind(subdir)*/
	
	/********************************************
	
	DIR* dir = opendir("directorio");
	if(dir == NULL){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *entry;
	
	errno = 0;
	// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer
	while((entry = readdir(dir)) != NULL){
		printf("entrada: %s\n", entry->d_name);
	}
	
	if(errno){
		perror("lectura");
	}
	
	if(closedir(dir) == -1){
		perror("dir");
		exit(EXIT_FAILURE);
	}
	
	********************************************/
	
	printLine(dir,DIR);
	
	DIR* dir = opendir(dir);
	if(dir == NULL){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *entry;
	
	errno = 0;
	// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer
	
	
	// TODO MAL, hay que procesar los ficheros reg y por ultimo los dir.
	// TODO hacer un buffer de directorios?
	/*while((entry = readdir(dir)) != NULL){
		if(processEntry(entry->d_name)==DIR)
			deepFind(max,min,entry->d_name);
	}*/
	
}

void findbysize(int max, int min, int numDirs, char** dirs){
	// Para cada directorio abre una busqueda en profundidad (deepFind).
	
	/*IDEAS: 
		*Tabular resultados para dar aspecto mas arboreo.
		*Añadir indice que indica la profundidad.
		*Directorios de un color ficheros de otro.
	
		Por ejemplo:
		
		0) /home
			1) /home/adri
				2) /home/adri/Pictures
					3) /home/adri/Pictures/foto1.jpg
					3) /home/adri/Pictures/foto2.jpg
					3) /home/adri/Pictures/foto3.jpg
				2) /home/adri/Dropbox
					3) /home/adri/Dropbox/gifs
	*/
	
	for(int i=0; i<numDirs; i++){
		deepFind(max, min, dirs[i]);
	}

}
