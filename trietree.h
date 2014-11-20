#ifndef __TRIETREE_H
#define __TRIETREE_H

#include <stdlib.h>

#define MAXALPHA 91
#define LASTNODE 91

typedef struct trieNode{
	struct trieNode* father;
	struct trieNode** array; 
}trieNode;

trieNode* newTrieNode();
void deleteTrieNode(trieNode* node);
trieNode* getSon(trieNode* node, char c);
void setSon(trieNode* node, char c, trieNode* son);
trieNode* getFather(trieNode* node);

typedef struct trieTree{
	struct trieNode* head; 
}trieTree;

trieTree* newTrieTree();
void deleteTrieTree(trieTree* tree);
void addStr(trieTree* tree, char* str);
void printTree(trieTree* tree);
void printTreeAux(trieNode* head, char* str, int index);
void autoComplete(trieTree* tree, char* str); // imprime las posibles palabras candidatas para completar str.
void loadTrieTree(trieTree* tree, char* dirPath); // Se cargaran en el arbol el nombre de todos los ejecutables existentes en el directorio.

#endif
