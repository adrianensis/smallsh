#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fileutil.h"
#include "trietree.h"
#include "color.h"

trieNode* newTrieNode(){
	trieNode* node = (trieNode*)malloc(sizeof(trieNode));
	node->father = NULL;
	node->array = (trieNode**)malloc(sizeof(trieNode*)*MAXALPHA+1); // +1 para el simbolo de nodo final.
	int i;
	for(i=' '; i<'z'+1; i++)
		setSon(node, i, NULL);
	return node;
}

void deleteTrieNode(trieNode* node){
	free(node->array);
	free(node);
}

trieNode* getSon(trieNode* node, char c){
	return node->array[c-32];
}

trieNode* getFather(trieNode* node){
	return node->father;
}

void setSon(trieNode* node, char c, trieNode* son){
	node->array[c-32] = son;
	if(son != NULL)
		son->father = node;
}

trieTree* newTrieTree(){
	trieTree* tree = (trieTree*)malloc(sizeof(trieTree));
	tree->head = NULL;
	return tree;
}

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

void deleteTrieTree(trieTree* tree){
	deleteTrieTreeAux(tree->head);
	free(tree);
}

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

void printTree(trieTree* tree){
	char* str = malloc(32); // No hay comandos tan largos.
	memset(str,0,32);
	printTreeAux(tree->head, str, 0);
	free(str);
}

void autoCompleteAux(trieNode* head, char* str, int index){

	trieNode* node = head;

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

void autoComplete(trieTree* tree, char* str){

	printf("\n");
	setColor(COLOR_BACK_MAGENTA);
	printf("PREDICTIONS>");
	setColor(COLOR_RESET);
	printf("\n");

	if(tree->head != NULL){
		if(str == NULL){
			printTree(tree);
		}else {	
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
				autoCompleteAux(it->father, strBuf, len-1);
				free(strBuf);
			}
		}	
	}
}

//Carga los ficheros de un directorio en el arbol.
void loadTrieTree(trieTree* tree, char* dirPath){
	// Guardamos el directorio actual.
	char* auxDir = malloc(128);
	getcwd (auxDir, 128);
	
	DIR* dir = opendir(dirPath);
	if(dir == NULL){
		perror("Fallo al abrir el directorio.\n");
		exit(EXIT_FAILURE);
	}
	
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
		exit(EXIT_FAILURE);
	}
	
	// Volvemos al directorio
	chdir(auxDir);
	free(auxDir);
}
