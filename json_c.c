#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef _JSON_PARSING_LIB_
#define _JSON_PARSING_LIB_
#include "JSON_PARSING.h"
//#include "DYNAMIC_MASSIVE/IntDynamicMassive.c"
#include "_OBJECTS_.c"

#define MAX_BUFFER_SIZE 256000
#define MAX_JSON_LENGTH 2048

typedef struct JSON_FILE{
	char* path;
	FILE* file;
} JSON_FILE;

JSON_FILE* jsonOpenToRead(char* path){
	JSON_FILE* jsonFile = (JSON_FILE*)malloc(sizeof(JSON_FILE));
	int len = strlen(path);
	jsonFile->path = (char*)malloc((len + 1) * sizeof(char));
	for (int i = 0; i < len; i++){
		jsonFile->path[i] = path[i];
	}
	jsonFile->path[len] = '\0';
	jsonFile->file = fopen(path, "r");
	return jsonFile;
}

JSON_FILE* jsonOpenToWrite(char* path){
	JSON_FILE* jsonFile = (JSON_FILE*)malloc(sizeof(JSON_FILE));
	int len = strlen(path);
	jsonFile->path = (char*)malloc((len + 1) * sizeof(char));
	for (int i = 0; i < len; i++){
		jsonFile->path[i] = path[i];
	}
	jsonFile->path[len] = '\0';
	jsonFile->file = fopen(path, "w");
	return jsonFile;
}

/*
{
		DynamicMassiveObj* list = createDynamicMassiveObj();
		char buffer[1024];
		int i = 0;
		char curs;
		for (int j = startIndex; j < stopIndex; j++){
			curs = buffer[i];
			if 		(curs == '[') countBrackets[0]++;
			else if (curs == '{') countBrackets[1]++;
			else if (curs == ']') countBrackets[2]++;
			else if (curs == '}') countBrackets[3]++;

			buffer[i] = string[startIndex++];
			i++;
		}
		buffer[i] = '\0';
		printf("->%s<-\n", buffer);
	}
*/

int __addToStorage(type storage, type items[2]){
	ID* storageID = storage;
	switch(storageID->id){
	case dynamicMassiveID:
		DynamicMassiveObj* list = storage;
		appendDynamicMassiveObj(list, items[0]);
		items[0] = None;
		break;
	case hashMapID:
		HashMapObj* hashmap = storage;
		addItemsToHashMapObj(hashmap, items[0], items[1]);
		items[0] = None;
		items[1] = None;
		break;
	}
}


int __parseString(char* string, type storage, int startIndex){
	type anotherStorage;
	type items[2];

	char inString = 0;
	char parsingValue = 0;
	char buffer[128];

	char inNumber = 0;
	char firstZero = 0;
	char isFloat = 0;

	int i = startIndex;
	for (; string[i] != '\0'; i++){
		char curs = string[i];
		if (!inNumber){
			if (!inString){
				if (curs == '['){
					anotherStorage = createDynamicMassiveObj();
					i = __parseString(string, anotherStorage, i + 1);
					items[parsingValue] = anotherStorage;
					parsingValue = 0;
				} else if ((curs == '{')){
					anotherStorage = createHashMapObj();
					i = __parseString(string, anotherStorage, i + 1);
					items[parsingValue] = anotherStorage;
					parsingValue = 0;
				} else if (curs == '\"'){
					inString++;
				} else if (('0' <= curs && curs <= '9') || (curs == '-')){
					if (curs == '0') firstZero++;
					buffer[inNumber++] = curs;
				} else if (curs == ':'){
					parsingValue = 1;
				} else if (curs == ','){
					__addToStorage(storage, items);
				} else if (curs == '}'){
					__addToStorage(storage, items);
					return i;
				} else if (curs == ']'){
					__addToStorage(storage, items);
					return i;
				}
			} else if (curs == '\"'){
				buffer[inString - 1] = '\0';
				inString = 0;
				items[parsingValue] = createStrObject(buffer);
				parsingValue = 0;
			} else {
				buffer[inString - 1] = curs;
				inString++;
			}
		} else if ('0' <= curs && curs <= '9'){
			if (firstZero && !isFloat){
				printf("Parsing Error 3 (Only decimal number)!\n");
				exit(1);
			}
			buffer[inNumber++] = curs;
		} else if (curs == '.'){
			isFloat = 1;
			firstZero = 0;
			buffer[inNumber++] = curs;
		} else {
			buffer[inNumber] = '\0';
			if (isFloat){
				double valueFromBuffer = atof(buffer);
				//printf("%lf\n", valueFromBuffer);
				items[parsingValue] = createDoubleObject(valueFromBuffer);
				isFloat = 0;
			} else {
				int valueFromBuffer = atoi(buffer);
				//printf("%d\n", valueFromBuffer);
				items[parsingValue] = createIntObject(valueFromBuffer);
			}
			inNumber = 0;
			parsingValue = 0;
			firstZero = 0;
			i--;
		}
	}
	return i;
}

type jsonParse(JSON_FILE* file){
	FILE* _file = file->file;
	char buffer[MAX_BUFFER_SIZE];
	char stack[MAX_JSON_LENGTH];
	int countBrackets[4] = {0};
	/*
	for countBrackets:
	INDEX
		0	<-->  '['
		1	<-->  '{'
		2	<-->  ']'
		3	<-->  '}'
	*/
	int currentLen = 0;
	char curs;  // curs is "current Symbol"
	char inString = 0;
	while (fgets(buffer, MAX_BUFFER_SIZE, _file) != NULL){
		for (int i = 0; buffer[i] != '\0'; i++){
			curs = buffer[i];
			if (!inString){
				if ((curs != '\n') && (curs != ' ') && (curs != '\t')){
					if 		(curs == '[') countBrackets[0]++;
					else if (curs == '{') countBrackets[1]++;
					else if (curs == ']') countBrackets[2]++;
					else if (curs == '}') countBrackets[3]++;
					else if (curs == '\"') inString = 1;
					stack[currentLen++] = curs;
					if ((countBrackets[0] - countBrackets[2]) < 0 || (countBrackets[1] - countBrackets[3]) < 0){
						printf("Parsing Error 1!\n");
						return None;
					}
				}
			} else {
				if (curs == '\"') inString = 0;
				stack[currentLen++] = curs;
			}
		}
		stack[currentLen] = '\0';
	}
	if ((countBrackets[0] - countBrackets[2]) != 0 || (countBrackets[1] - countBrackets[3]) != 0){
		printf("Parsing Error 2!\n");
		return None;
	}
	//printf("%s\n", stack);
	type storage = NULL;
	if (stack[0] == '['){
		storage = createDynamicMassiveObj();
	} else if ((stack[0] == '{')){
		storage = createHashMapObj();
	} else {
		printf("Parsing Error 4!\n");
		return None;
	}
	__parseString(stack, storage, 1);
	return storage;
}

int jsonDump(JSON_FILE* file, type storage){
	ID* storageID = storage;
	char buff[2048] = {0};
	switch(storageID->id){
	case dynamicMassiveID:
		DynamicMassiveObj* list = storage;
		__snprintObject(buff, 2048, list, "");
		break;
	case hashMapID:
		HashMapObj* hashmap = storage;
		__snprintObject(buff, 2048, hashmap, "");
		break;
	}
	fputs(buff, file->file);
}

int jsonClose(JSON_FILE* file){
	fclose(file->file);
	free(file->path);
	free(file);
}

#endif
