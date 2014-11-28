#include "fileutil.h"

// Funcion que devuelve las estadisticas de un fichero a partir de su descriptor.
struct stat getStat(int fileDesc){
	struct stat buf;
	if(fstat(fileDesc, &buf) == -1){
		perror("Error al obtener las estadisticas del fichero.");
	}
	
	return buf;
}

// Funcion para saber si el fichero es un directorio o no.
int isDir(char* name){

	int retValue = -1;

	// Abrir
	int fd = open(name, O_RDONLY);
	
	if(fd==-1){
		perror("Fallo al abrir el fichero.");
	}else{
		struct stat buf = getStat(fd);
		retValue = S_ISDIR(buf.st_mode);
	
		// Cerrar
		if(close(fd) == -1){
			perror("Fallo al cerrar el fichero");
		}
	}
		
	return retValue;
}

// Funcion para saber si el fichero es regular o no.
int isReg(char* name){

	int retValue = -1;

	// Abrir
	int fd = open(name, O_RDONLY);
	
	if(fd==-1){
		perror("Fallo al abrir el fichero.");
	}else{
		struct stat buf = getStat(fd);
		retValue = S_ISREG(buf.st_mode);
	
		// Cerrar
		if(close(fd) == -1){
			perror("Fallo al cerrar el fichero");
		}
	}
		
	return retValue;
}

// Funcion para saber si el fichero es ejecutable o no.
int isExe(char* name){

	int retValue = -1;

	// Abrir
	int fd = open(name, O_RDONLY);
	
	if(fd==-1){
		perror("Fallo al abrir el fichero.");
	}else{
		struct stat buf = getStat(fd);
		retValue =  buf.st_mode & S_IXUSR;
	
		// Cerrar
		if(close(fd) == -1){
			perror("Fallo al cerrar el fichero");
		}
	}
		
	return retValue;
}
