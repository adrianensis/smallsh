#include "findbysize.h"
#include "color.h"
#include "errno.h"

// argumento: descriptor de ficheros

struct stat getStat(int fileDesc){
	struct stat buf;
	if(fstat(fileDesc, &buf) == -1){
		perror("Error al obtener las estadisticas del fichero.");
		exit(EXIT_FAILURE);
	}
	return buf;
}

int isDir(int fileDesc){
	struct stat buf = getStat(fileDesc);
	return S_ISDIR(buf.st_mode);
}

int isReg(int fileDesc){
	struct stat buf = getStat(fileDesc);
	return S_ISREG(buf.st_mode);
}

int sizeOK(int max, int min, int fileDesc){
	
}

#define R 0 // regular
#define D 1 // directorio
void printLine(char* name, int type){
	if(type==R){
		setColor(COLOR_BACK_YELLOW);
		printf("%s\n", name);
		setColor(COLOR_RESET);
	}else if(type==D)
		printf("%s\n", name);
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
	fstat(fd, &statBuf);
	
	if(isReg(fd)){
		retValue=R;
		printLine(name,R);
	}
	else if(isDir(fd)){
		retValue=D;
	}
		
	
	// Cerrar
	if(close(fd) == -1){
		perror("close");
		exit(EXIT_FAILURE);
	}
		
	return retValue;
}

void deepFind(int max, int min, char* dirname){
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
	
	printLine(dirname,D);
	
	DIR* dir = opendir(dirname);
	if(dir == NULL){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *entry;
	
	// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer.
	// Por eso el error se comprueba con errno.
	errno = 0;
	
	while((entry = readdir(dir)) != NULL){
		if(processEntry(entry->d_name)==REG)
			printf(entry->d_name,R);
		else
			deepFind(max,min,entry->d_name);
	}
	
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
	
	int i;
	for(i=0; i<numDirs; i++){
		deepFind(max, min, dirs[i]);
	}

}
