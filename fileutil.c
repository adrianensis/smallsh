#include "fileutil.h"

// argumento: descriptor de ficheros
struct stat getStat(int fileDesc){
	struct stat buf;
	if(fstat(fileDesc, &buf) == -1){
		perror("Error al obtener las estadisticas del fichero.");
		exit(EXIT_FAILURE);
	}
	return buf;
}

int isDir(char* name){

	int retValue; // La funcion devuelve si la entrada es un fichero reg o un dir

	// Abrir
	int fd = open(name, O_RDONLY);
	
	// TODO exit o tratar o notificar?
	if(fd==-1){
		perror("read");
		exit(EXIT_FAILURE);
	}

	struct stat buf = getStat(fd);
	retValue = S_ISDIR(buf.st_mode);
	
	// Cerrar
	if(close(fd) == -1){
		perror("close");
		exit(EXIT_FAILURE);
	}
		
	return retValue;

}

int isReg(char* name){

	int retValue; // La funcion devuelve si la entrada es un fichero reg o un dir

	// Abrir
	int fd = open(name, O_RDONLY);
	
	// TODO exit o tratar o notificar?
	if(fd==-1){
		perror("read");
		exit(EXIT_FAILURE);
	}

	struct stat buf = getStat(fd);
	retValue = S_ISREG(buf.st_mode);
	
	// Cerrar
	if(close(fd) == -1){
		perror("close");
		exit(EXIT_FAILURE);
	}
		
	return retValue;

}

int isExe(char* name){

	int retValue; // La funcion devuelve si la entrada es un fichero reg o un dir

	// Abrir
	int fd = open(name, O_RDONLY);
	
	// TODO exit o tratar o notificar?
	if(fd==-1){
		perror("read");
		exit(EXIT_FAILURE);
	}

	struct stat buf = getStat(fd);
	retValue = buf.st_mode & S_IXUSR;
	
	// Cerrar
	if(close(fd) == -1){
		perror("close");
		exit(EXIT_FAILURE);
	}
		
	return retValue;
}
