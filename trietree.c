#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fileutil.h"
#include "trietree.h"
#include "color.h"

// Constructor de nodo trie.
trieNode* newTrieNode(){
	trieNode* node = (trieNode*)malloc(sizeof(trieNode));
	node->father = NULL;
	node->numSons = 0;
	node->array = (trieNode**)malloc(sizeof(trieNode*)*MAXALPHA+1); // +1 para el simbolo de nodo final.
	int i;
	for(i=' '; i<'z'+1; i++)
		setSon(node, i, NULL);
	return node;
}

// Destructor de nodo trie.
void deleteTrieNode(trieNode* node){
	free(node->array);
	free(node);
}

// Devuelve el hijo i del nodo indicado.
trieNode* getSon(trieNode* node, char c){
	return node->array[c-32];
}

// Devuelve el padre del nodo indicado.
trieNode* getFather(trieNode* node){
	return node->father;
}

// Establece el hijo i del nodo indicado.
void setSon(trieNode* node, char c, trieNode* son){
	node->array[c-32] = son;
	if(son != NULL){
		node->numSons++;
		son->father = node;
	}
}

// Constructor de arbol trie.
trieTree* newTrieTree(){
	trieTree* tree = (trieTree*)malloc(sizeof(trieTree));
	tree->head = NULL;
	return tree;
}

// Destructor auxiliar recursivo del arbol trie.
void deleteTrieTreeAux(trieNode* node){
	int i;
	trieNode *son;
	for(i=' '; i<'z'+1; i++){
		if((son = getSon(node, i)) != NULL){
			deleteTrieTreeAux(son);
		}
	}
	deleteTrieNode(node);
}

// Destructor del arbol trie.
void deleteTrieTree(trieTree* tree){
	deleteTrieTreeAux(tree->head);
	free(tree);
}

// Funcion para añadir una cadena al arbol trie.
void addStr(trieTree* tree, char* str){
	if(tree->head == NULL)
		tree->head = newTrieNode();

	int i;
	trieNode* it = tree->head;
	for(i=0; i<strlen(str); i++){
		if(getSon(it, str[i]) == NULL)
			setSon(it, str[i], newTrieNode());
		
		it = getSon(it, str[i]); // avanzamos al nuevo hijo
	}
	
	it->array[LASTNODE] = it; // La marca de final apunta al mismo nodo.
}

// Funcion auxiliar para imprimir el arbol, a partir de una subcadena.
void printTreeAux(trieNode* head, char* str, int index){
	trieNode* node = head;
	
	char i;
	trieNode* son;
	for(i=' '; i<'z'+1; i++){
		if((son = getSon(node, i)) != NULL){
			str[index++] = i;
			printTreeAux(son,str,index);
			index--;
		}
	}	
		
	// Comprobamos marca final.
	if(getSon(node, 'z'+1) != NULL){	
		str[index] = '\0'; // \0 para imprimir.
		printf("%s \n", str);
		str[index] = 0; // borramos el barra 0.
	}
}

// Funcion para imprimir el arbol completo.
void printTree(trieTree* tree){
	char* str = malloc(32); // No hay comandos tan largos.
	memset(str,0,32);
	printTreeAux(tree->head, str, 0);
	free(str);
}

// Funcion auxiliar para imprimir predicciones dada una subcadena.
void predictAux(trieNode* node, char* str, int index){

	char i;
	trieNode* son;
	for(i=' '; i<'z'+1 && (str[index] != i) ; i++); // Solo recorremos hasta 

	if((son = getSon(node, i)) != NULL){
		index++;
		printTreeAux(son,str,index);
		index--;
	}

	// Comprobamos marca final.
	if(getSon(node, 'z'+1) != NULL){	
		str[index] = '\0'; // \0 para imprimir.
		printf("%s\n", str);
		str[index] = 0; // borramos el barra 0.
		
	}
}

// Funcion para imprimir predicciones dada una subcadena.
void predict(trieTree* tree, char* str){

	if(tree->head != NULL){
	
		printf("\n");
		setColor(COLOR_BACK_MAGENTA);
		printf("PREDICTIONS>");
		setColor(COLOR_RESET);
		printf("\n");
		
		if(str != NULL){
			int len = strlen(str);
			trieNode* it, *son;
			it = tree->head;
			int index = 0;
	
			while((index < len) && ((son = getSon(it, str[index])) != NULL)){
				it = son;
				index++;
			}
	
			if(index == len){
				char* strBuf = (char*)malloc(len+32); // sumando len, nos aseguramos que cabe, sea cual sea el tamaño. Asi no peta luego.
				int i;
				for(i=0; i<len; i++)
					strBuf[i] = str[i];
				predictAux(it->father, strBuf, len-1);
				free(strBuf);
			}
		}	
	}
}

// Funcion auxiliar para autocompletar una palabra dada subcadena.
/*  La palabra se completara hasta que haya un punto en el que existe mas de una posibilidad para predecir.
	Por ejemplo:
	
		Tenemos en nuestro directorio:
		* gettoken.c
		* gettoken.h
		* gettoken.o
		
		El usuario introduce:
		> getto (y pulsa tabulador)
		
		Automaticamente la palabra se autocompleta hasta ".":
		> gettoken.
		
		Si se pulsa dos veces tabulador, se muestran las predicciones:
		> gettoken.
		  PREDICCIONES>
		  gettoken.c
		  gettoken.h
		  gettoken.o
*/

char* autoCompleteAux(trieNode* node){

	int i=0;
	char* part = NULL;
	char c;
	trieNode* son;
	
	while(node->numSons == 1){
	
		if(part == NULL){
			part = malloc(32);
			memset(part,0,32);
		}
	
		// buscamos a ese unico hijo.
		for(c=' '; c<'z'+1 && ((son = getSon(node, c)) == NULL) ; c++);
		part[i++]=c;
		node = son;
			
	}

	return part;
}

// Funcion para autocompletar una palabra dada una subcadena.
char* autoComplete(trieTree* tree, char* str){

	char* part = NULL;

	if(tree->head != NULL){
		if(str != NULL){	
			int len = strlen(str);
			trieNode* it, *son;
			it = tree->head;
			int index = 0;
	
			while((index < len) && ((son = getSon(it, str[index])) != NULL)){
				it = son;
				index++;
			}
	
			if(index == len){
				part = autoCompleteAux(it);
			}
		}	
	}
	
	return part;
}

//Carga los ficheros de un directorio en el arbol.
void loadTrieTree(trieTree* tree, char* dirPath){
	
	DIR* dir = opendir(dirPath);
	if(dir == NULL){
		perror("Fallo al abrir el directorio.\n");
	}else{
	
		// Guardamos el directorio actual.
		char* auxDir = malloc(128);
		getcwd (auxDir, 128);
	
		chdir(dirPath);
	
		struct dirent *entry;
	
		// readdir devuelve NULL tanto si ha ido mal como si ha acabado de leer.
		// Por eso el error se comprueba con errno.
		errno = 0;
		while((entry = readdir(dir)) != NULL)
			addStr(tree, entry->d_name);
	
		if(errno){
			printf("Fallo al leer el fichero directorio.\n");
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

// Resetea el arbol con los valores por defecto y carga el directorio actual.
trieTree* resetTrieTree(char* currentDir){

	trieTree *tree = newTrieTree(); // inicializamos el diccionario de comandos.
	loadTrieTree(tree, "/usr/bin");
	loadTrieTree(tree, currentDir);
	addStr(tree, "cd");
	addStr(tree, "exit");
	addStr(tree, "bgproc");
	addStr(tree, "alarma");
	addStr(tree, "otherwc");
	addStr(tree, "findbysize");
	addStr(tree, "history");
	
	return tree;
}
