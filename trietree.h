#ifndef __TRIETREE_H
#define __TRIETREE_H

#define MAXALPHA 91
#define LASTNODE 91

typedef struct trieNode{
	struct trieNode* father;
	int numSons;
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
void predict(trieTree* tree, char* str); // imprime las posibles palabras candidatas para completar str.
char* autoComplete(trieTree* tree, char* str);
void loadTrieTree(trieTree* tree, char* dirPath); // Se cargaran en el arbol el nombre de todos los ejecutables existentes en el directorio.
trieTree* resetTrieTree(char* currentDir);

#endif
