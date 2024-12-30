#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.c"

#ifndef _OBJECTS_REALIZATIONS_
#define _OBJECTS_REALIZATIONS_
#include "_OBJECTS_.h"

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||| OBJECTS REALIZATION |||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

int checkNULL_IN_OBJECTS_REALIZATION(void* ptr){
	if (ptr == NULL){
		printf("Memory allocation error!\n");
		exit(1);
	}
	return 0;
}

void pass(){
	return;
}

#define NoneType 0
#define StringType 1
#define IntType 2
#define FloatType 3 
#define DoubleType 4
#define TupleType 5
#define DynamicMassiveType 6
#define HashMapType 7
#define HashSetType 8

typedef void* type;
typedef enum {noneID, strID, intID, floatID, doubleID, tupleID, dynamicMassiveID, hashMapID, hashSetID} objectID;

typedef struct ID {
    objectID id;
} ID;

typedef struct noneObj{
	objectID id;
} noneObj;

noneObj __NoneObject = {noneID};
noneObj* const None = &__NoneObject;
//noneObj* None = (void*)&NoneObject;

typedef struct strObj{
	objectID id;
	char* string;
    int len;
} strObj;

type createStrObject(char* string){
	strObj* obj = (strObj*)malloc(sizeof(strObj));
	checkNULL_IN_OBJECTS_REALIZATION(obj);
	obj->id = strID;
	obj->len = strlen(string);
	obj->string = (char*)malloc((obj->len + 1) * sizeof(char));
	checkNULL_IN_OBJECTS_REALIZATION(obj->string);
	for (int i = 0; i < obj->len + 1; i++){
		obj->string[i] = string[i];
	}
	return (type)obj;
}

typedef struct intObj{
	objectID id;
	int value;
} intObj;

type createIntObject(int value){
	intObj* obj = (intObj*)malloc(sizeof(intObj));
	checkNULL_IN_OBJECTS_REALIZATION(obj);
	obj->id = intID;
	obj->value = value;
	return (type)obj;
}

typedef struct floatObj{
	objectID id;
	float value;
} floatObj;

type createFloatObject(float value){
	if (value == (int)value) return createIntObject(value);

	floatObj* obj = (floatObj*)malloc(sizeof(floatObj));
	checkNULL_IN_OBJECTS_REALIZATION(obj);
	obj->id = floatID;
	obj->value = value;
	return (type)obj;
}

typedef struct doubleObj{
	objectID id;
	double value;
} doubleObj;

type createDoubleObject(double value){
	if (value == (int)value) return createIntObject(value);

	doubleObj* obj = (doubleObj*)malloc(sizeof(doubleObj));
	checkNULL_IN_OBJECTS_REALIZATION(obj);
	obj->id = doubleID;
	obj->value = value;
	return (type)obj;
}

typedef struct tupleObj{
	objectID id;
	type* objects;
	int len;
} tupleObj;

type createTupleObject(type* objects, int len){
	tupleObj* obj = (tupleObj*)malloc(sizeof(tupleObj));
	checkNULL_IN_OBJECTS_REALIZATION(obj);
	obj->id = tupleID;
	obj->len = len;
	obj->objects = (type*)malloc(len * sizeof(type));
	checkNULL_IN_OBJECTS_REALIZATION(obj->objects);
	for (int i = 0; i < len; i++){
		obj->objects[i] = objects[i];
	}
	return obj;
}

int destroyObject(type object){
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		free(strObject->string);
		free(strObject);
		break;
	case intID:
		intObj* intObject = object;
		free(intObject);
		break;
	case floatID:
		floatObj* floatObject = object;
		free(floatObject);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		free(doubleObject);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		free(tupleObject->objects);
		free(tupleObject);
		break;
	case dynamicMassiveID:
		DynamicMassiveObj* list = object;
		destroyDynamicMassiveObj(list);
		break;
	case hashMapID:
		HashMapObj* hashmap = object;
		destroyHashMapObj(hashmap);
		break;
	case noneID:
		break;
	}
	return 0;
}

int __destroyAllObject(type object){
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		free(strObject->string);
		free(strObject);
		break;
	case intID:
		intObj* intObject = object;
		free(intObject);
		break;
	case floatID:
		floatObj* floatObject = object;
		free(floatObject);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		free(doubleObject);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		for (int i = 0; i < tupleObject->len; i++){
			__destroyAllObject(tupleObject->objects[i]);
		}
		free(tupleObject->objects);
		free(tupleObject);
		break;
	case dynamicMassiveID:
		DynamicMassiveObj* list = object;
		__destroyAllDynamicMassiveObj(list);
		break;
	case hashMapID:
		HashMapObj* hashmap = object;
		__destroyAllHashMapObj(hashmap);
		break;
	case noneID:
		break;
	}
	return 0;
}

void __snprintObject(char buffer[], int len, type object, char* end){
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		snprintf(buffer + strlen(buffer), len, "\"%s\"%s", strObject->string, end);
		break;
	case intID:
		intObj* intObject = object;
		snprintf(buffer + strlen(buffer), len, "%d%s", intObject->value, end);
		break;
	case floatID:
		floatObj* floatObject = object;
		snprintf(buffer + strlen(buffer), len, "%f%s", floatObject->value, end);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		snprintf(buffer + strlen(buffer), len, "%lf%s", doubleObject->value, end);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		snprintf(buffer + strlen(buffer), len, "(");
		for (int i = 0; i < tupleObject->len; i++){
			__snprintObject(buffer + strlen(buffer), len, tupleObject->objects[i], "");
			snprintf(buffer + strlen(buffer), len, (i + 1) == tupleObject->len ? "" : ", ");
		}
		snprintf(buffer + strlen(buffer), len, ")");
		break;
	case dynamicMassiveID:
		DynamicMassiveObj* list = object;
		__snprintDynamicMassiveObj(buffer + strlen(buffer), len, list, end);
		break;
	case hashMapID:
		HashMapObj* hashmap = object;
		__snprintHashMapObj(buffer + strlen(buffer), len, hashmap, end);
		break;
	case noneID:
		snprintf(buffer + strlen(buffer), len, "None%s", end);
		break;
	}
}

void __printObject(type object, char* end){
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		printf("\"%s\"%s", strObject->string, end);
		break;
	case intID:
		intObj* intObject = object;
		printf("%d%s", intObject->value, end);
		break;
	case floatID:
		floatObj* floatObject = object;
		printf("%f%s", floatObject->value, end);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		printf("%lf%s", doubleObject->value, end);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		printf("(");
		for (int i = 0; i < tupleObject->len; i++){
			__printObject(tupleObject->objects[i], "");
			printf((i + 1) == tupleObject->len ? "" : ", ");
		}
		printf(")");
		break;
	case dynamicMassiveID:
		DynamicMassiveObj* list = object;
		__printDynamicMassiveObj(list, end);
		break;
	case hashMapID:
		HashMapObj* hashmap = object;
		__printHashMapObj(hashmap, end);
		break;
	case noneID:
		printf("None%s", end);
		break;
	}
}


void printObject(type object){
	if (object == NULL){
		printf("(null)\n");
		return;
	}
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		printf("\"%s\"\n", strObject->string);
		break;
	case intID:
		intObj* intObject = object;
		printf("%d\n", intObject->value);
		break;
	case floatID:
		floatObj* floatObject = object;
		printf("%f\n", floatObject->value);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		printf("%lf\n", doubleObject->value);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		printf("(");
		for (int i = 0; i < tupleObject->len; i++){
			__printObject(tupleObject->objects[i], "");
			printf((i + 1) == tupleObject->len ? "" : ", ");
		}
		printf(")\n");
		break;
	case dynamicMassiveID:
		DynamicMassiveObj* list = object;
		printDynamicMassiveObj(list);
		break;
	case hashMapID:
		HashMapObj* hashmap = object;
		printHashMapObj(hashmap);
		break;
	case noneID:
		printf("None\n");
		break;
	}
}

long long getHashFromObject(type object){
	ID* currentID = object;
	long long hash = 0;
	switch (currentID->id){
	case strID:
		strObj* strObject = object;
		for (int i = 0; i < strObject->len; i++){
			hash += (long long)strObject->string[i];
			hash *= 10;
			hash = hash << (strObject->string[i] % 10 + strObject->string[i] / 10);
		}
		break;
	case intID:
		intObj* intObject = object;
		hash += (long long)(intObject->value * intObject->value * 0x1FFAAFFFFFAAFF12);
		hash = hash >> (((int)intObject->value % 3) * 17 + 1);
		break;
	case floatID:
		floatObj* floatObject = object;
		hash += (long long)((int)floatObject->value * (int)floatObject->value * 0x1FFAAFFFFFAAFF12);
		hash = hash >> (((int)floatObject->value % 3) * 17 + 1);

		hash += (long long)((floatObject->value - (int)floatObject->value) * 1337 * 127);
		break;
	case doubleID:
		doubleObj* doubleObject = object;
		hash += (long long)((int)doubleObject->value * (int)doubleObject->value * 0x1FFAAFFFFFAAFF12);
		hash = hash >> (((int)doubleObject->value % 3) * 17 + 1);

		hash += (long long)((doubleObject->value - (int)doubleObject->value) * 1337 * 127);
		break;
	case tupleID:
		tupleObj* tupleObject = object;
		for (int i = 0; i < tupleObject->len; i++){
			hash += getHashFromObject(tupleObject->objects[i]);
			hash *= 127;
		}
		break;
	case dynamicMassiveID:
		//DynamicMassiveObj* list = object;
		printf("DynamicMassive is not hashable!\n");
		return -1;
	case hashMapID:
		printf("HashMapObj is not hashable!\n");
		break;
	case hashSetID:
		printf("HashSetObj is not hashable!\n");
		break;
	case noneID:
		break;
	}
	hash ^= 0xFAFCFEAFBCFEAD17;
	return hash;
}

int compareStrObjects(strObj* string1, strObj* string2){
    if (string1->len == string2->len){
        return strcmp(string1->string, string2->string) == 0;
    }
    return 0;
}

int compareIntObjects(intObj* integer1, intObj* integer2){
    return integer1->value == integer2->value;
}

int compareFloatObjects(floatObj* float1, floatObj* float2){
    return float1->value == float2->value;
}

int compareDoubleObjects(doubleObj* dnum1, doubleObj* dnum2){
	return dnum1->value == dnum2->value;
}

int compareObjects(type object1, type object2){
    ID* firstObjID = object1;
    ID* secondObjID = object2;
    if (firstObjID->id == secondObjID->id){
        switch (firstObjID->id) {
        case strID:
            strObj* string1 = object1;
            strObj* string2 = object2;
            return compareStrObjects(string1, string2);
        case intID:
            intObj* integer1 = object1;
            intObj* integer2 = object2;
            return compareIntObjects(integer1, integer2);
        case floatID:
            floatObj* floatNumber1 = object1;
            floatObj* floatNumber2 = object2;
            return compareFloatObjects(floatNumber1, floatNumber2);
        case doubleID:
        	doubleObj* doubleNum1 = object1;
        	doubleObj* doubleNum2 = object2;
        	return compareDoubleObjects(doubleNum1, doubleNum2);
        case noneID:
            //noneObj* none = object1;
            return 1;
        case tupleID:
            pass();
            break;
        case dynamicMassiveID:
			DynamicMassiveObj* list1 = object1;
			DynamicMassiveObj* list2 = object2;
			pass();
			return 0;
			break;
		case hashMapID:
			pass();
			break;
		case hashSetID:
			pass();
			break;
        }
	}
	return 0;
}

objectID getObjType(type object){
/*
	INDEX		   ID name
	  0 <-------> noneID
	  1 <-------> strID
	  2 <-------> intID
	  3 <-------> floatID
	  4 <-------> doubleID
	  5 <-------> tupleID
	  6 <-------> dynamicMassiveID
	  7 <-------> hashMapID
	  8 <-------> hashSetID
*/
	ID* currentID = object;
	switch (currentID->id){
	case strID:
		return strID;
		break;
	case intID:
		return intID;
		break;
	case floatID:
		return floatID;
		break;
	case doubleID:
		return doubleID;
		break;
	case tupleID:
		return tupleID;
		break;
	case dynamicMassiveID:
		return dynamicMassiveID;
		break;
	case hashMapID:
		return hashMapID;
		break;
	case hashSetID:
		return hashSetID;
		break;
	case noneID:
		return noneID;
		break;
	}
	return -1;
}

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||| Dynamic massive for objects ||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

typedef struct DynamicMassiveObj{
	objectID id;
    type* massive;
    int len;
    int capacity;
} DynamicMassiveObj;

DynamicMassiveObj* createDynamicMassiveObj(){
    DynamicMassiveObj* list = (DynamicMassiveObj*)malloc(sizeof(DynamicMassiveObj));
    checkNULL_IN_OBJECTS_REALIZATION(list);
    list->id = dynamicMassiveID;
    list->len = 0;
    list->capacity = 5;
    list->massive = (type*)malloc(list->capacity * sizeof(type));
    checkNULL_IN_OBJECTS_REALIZATION(list->massive);
    return list;
}

int __destroyAllDynamicMassiveObj(DynamicMassiveObj* list){
	for (int i = 0; i < list->len; i++){
		__destroyAllObject(list->massive[i]);
	}
	destroyDynamicMassiveObj(list);
	return 0;
}


int destroyDynamicMassiveObj(DynamicMassiveObj* list){
    free(list->massive);
    free(list);
    return 0;
}

int lenDynamicMassiveObj(DynamicMassiveObj* list){
    return list->len;
}


int existValueDynamicMassiveObj(DynamicMassiveObj* list, type value){
    for (int i = 0; i < list->len; i++){
        if (compareObjects(list->massive[i], value)) return 1;
    }
    return 0;
}

int appendDynamicMassiveObj(DynamicMassiveObj* list, type value){
    if (list->len == list->capacity){
        int currentCap = list->capacity;
        list->capacity = (int)(currentCap * 1.4);
        //printf("len: %d\n", list->capacity);
        type* newMassive = (type*)malloc(list->capacity * sizeof(type));
        checkNULL_IN_OBJECTS_REALIZATION(newMassive);
        for (int i = 0; i < list->len; i++) {
            newMassive[i] = list->massive[i];
        }
        free(list->massive);
        list->massive = newMassive;
    }
    list->massive[list->len] = value;
    list->len++;
}

int insertDynamicMassiveObj(DynamicMassiveObj* list, int index, type value){
    if (list->len == list->capacity){
        int currentCap = list->capacity;
        list->capacity = (int)(currentCap * 1.4);
        //printf("len: %d\n", list->capacity);
        type* newMassive = (type*)malloc(list->capacity * sizeof(type));
        checkNULL_IN_OBJECTS_REALIZATION(newMassive);
        for (int i = 0; i < list->len; i++) {
            newMassive[i] = list->massive[i];
        }
        free(list->massive);
        list->massive = newMassive;
    }
    if (index < 0){
        if (abs(index) > list->len + 1){
            printf("IndexError: Index limit exceeded!\n");
            return 1;
        }
        index = list->len + index + 1;
    } else {
        if (index > list->len){
            printf("IndexError: Index limit exceeded!\n");
            return 1;
        }
    }
    type currentValue = list->massive[index];
    for (int i = index + 1; i < list->len + 1; i++){
        type helpValue = list->massive[i];
        list->massive[i] = currentValue;
        currentValue = helpValue;
    }
    list->massive[index] = value;
    list->len++;
    return 0;
}

type popLastDynamicMassiveObj(DynamicMassiveObj* list){
    if (list->len == 0){
        printf("IndexError: DynamicMassiveObj length = 0\n");
        return None;
    }
    type returnValue = list->massive[list->len - 1];
    if (list->capacity / list->len > 1.4){
        int currentCap = list->capacity;
        int maybeNewCap = currentCap / 1.4;
        if (maybeNewCap >= 5) {
            list->capacity = maybeNewCap;
            type* newMassive = (type*)malloc(list->capacity * sizeof(type));
            checkNULL_IN_OBJECTS_REALIZATION(newMassive);
            for (int i = 0; i < list->len; i++) {
                newMassive[i] = list->massive[i];
            }
            free(list->massive);
            list->massive = newMassive;
        }
    }
    list->len--;
    return returnValue;
}

type popIndexDynamicMassiveObj(DynamicMassiveObj* list, int index){
    if (list->len == 0){
        printf("IndexError: DynamicMassiveObj length = 0!\n");
        return None;
    }
    type returnValue;
    if (index < 0){
        if (abs(index) > list->len){
            printf("IndexError: Index limit exceeded!\n");
            return None;
        }
        index = list->len + index;
        returnValue = list->massive[index];

    } else{
        if (index >= list->len){
            printf("IndexError: Index limit exceeded!\n");
            return None;
        }
        returnValue = list->massive[index];
    }
    for (int i = index; i < list->len - 1; i++){
        list->massive[i] = list->massive[i + 1];
    }
    if (list->capacity / list->len > 1.4){
        int currentCap = list->capacity;
        int maybeNewCap = currentCap / 1.4;
        if (maybeNewCap >= 5) {
            list->capacity = maybeNewCap;
            type* newMassive = (type*)malloc(list->capacity * sizeof(type));
            checkNULL_IN_OBJECTS_REALIZATION(newMassive);
            for (int i = 0; i < list->len; i++) {
                newMassive[i] = list->massive[i];
            }
            free(list->massive);
            list->massive = newMassive;
        }
    }
    list->len--;
    return returnValue;
}

// bad func
int removeValueDynamicMassiveObj(DynamicMassiveObj* list, type value){
    for (int i = 0; i < list->len; i++){
        if (list->massive[i] == value){
            popIndexDynamicMassiveObj(list, i);
            return 1;
        }
    }
    return 0;
}

void printDynamicMassiveObj(DynamicMassiveObj* list){
    printf("[");
    for (int i = 0; i < list->len; i++){
        __printObject(list->massive[i], "");
        printf((i + 1) == list->len ? "" : ", ");
    }
    printf("]\n");
}

void __printDynamicMassiveObj(DynamicMassiveObj* list, char* end){
    printf("[");
    for (int i = 0; i < list->len; i++){
        __printObject(list->massive[i], "");
        printf((i + 1) == list->len ? "" : ", ");
    }
    printf("]%s", end);
}

void __snprintDynamicMassiveObj(char buffer[], int len, DynamicMassiveObj* list, char* end){
	snprintf(buffer + strlen(buffer), len, "[");
    for (int i = 0; i < list->len; i++){
        __snprintObject(buffer + strlen(buffer), len, list->massive[i], "");
        snprintf(buffer + strlen(buffer), len, (i + 1) == list->len ? "" : ", ");
    }
    snprintf(buffer + strlen(buffer), len, "]%s", end);
}


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||| Hashmap for objects ||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

typedef struct HashMapObj{
	objectID id;
    DynamicMassiveObj* massive; // Contains (Type)objects 
    LinkedIntList** indexes; // Contains indexes
    int capacity;
} HashMapObj;

HashMapObj* createHashMapObj(){
    HashMapObj* hashmap = (HashMapObj*)malloc(sizeof(HashMapObj));
    checkNULL_IN_OBJECTS_REALIZATION(hashmap);
    hashmap->id = hashMapID;
    hashmap->capacity = 5;
    hashmap->massive = createDynamicMassiveObj();
    hashmap->indexes = (LinkedIntList**)malloc(hashmap->capacity * sizeof(LinkedIntList*));
    checkNULL_IN_OBJECTS_REALIZATION(hashmap->indexes);
    for (int i = 0; i < hashmap->capacity; i++){
        hashmap->indexes[i] = createNewLinkedIntList();
    }
    return hashmap;
}

int __destroyAllHashMapObj(HashMapObj* hashmap){
	for (int i = 0; i < hashmap->capacity; i++){
		destroyLinkedIntList(hashmap->indexes[i]);
	}
	free(hashmap->indexes);

	DynamicMassiveObj* list = hashmap->massive;
	__destroyAllDynamicMassiveObj(list);
	free(hashmap);
	return 0;
}

int destroyHashMapObj(HashMapObj* hashmap){
	DynamicMassiveObj* list = hashmap->massive;

	for (int i = 0; i < hashmap->capacity; i++){
		destroyLinkedIntList(hashmap->indexes[i]);
	}
	free(hashmap->indexes);

	for (int i = 0; i < list->len; i++){
		destroyObject(list->massive[i]);
	}
	destroyObject(list);
	free(hashmap);
	return 0;
}

int lenHashMapObj(HashMapObj* hashmap){
    return hashmap->massive->len;
}

int greatMod(int num1, int num2){
	int res;
	if ((res = (num1 % num2)) < 0) res += num2;
	return res;
}

type search(HashMapObj* hashmap, type object){
    // object is key
    ID* currentID = object;
    long long hash = getHashFromObject(object);
    switch (currentID->id) {
        case strID:
            strObj* string = object;
            // get hash etc...
            break;
        case intID:
            intObj* integer = object;
            // hash
            break;
        case floatID:
            floatObj* floatNumber = object;
            // hash
            break;
        case noneID:
            noneObj* none = object;
            // hash
            break;
        case tupleID:
            tupleObj* tuple2 = object;
            // hash
            break;
    }
}

void __checkExtend(HashMapObj* hashmap){
    pass(); // 
}

// bad func
int addItemsToHashMapObj(HashMapObj* hashmap, type object, type value){
    // object is key
    __checkExtend(hashmap);
    long long hash = getHashFromObject(object);
    int indexToInsert = greatMod(hash, hashmap->capacity);
    int indexInMassive = hashmap->massive->len;
    //__printObject(object, ":");
    //printf(" %d\n", indexToInsert);

    Node* currentNode = hashmap->indexes[indexToInsert]->head;
	while (currentNode != NULL){
		tupleObj* currentTuple = hashmap->massive->massive[currentNode->value];
		if (compareObjects(object, currentTuple->objects[0])){
			currentTuple->objects[1] = value;
			return 0;
		}
		if (currentNode->next == NULL) break;
		currentNode = currentNode->next;
	}	

    type items[2] = {object, value};
    appendDynamicMassiveObj(hashmap->massive, createTupleObject(items, 2));
    appendValue(hashmap->indexes[indexToInsert], indexInMassive);
    return 0;
}

type getValueObjKey(HashMapObj* hashmap, type object){
    // object is key
    long long hash = getHashFromObject(object);
    int indexInIndexes = greatMod(hash, hashmap->capacity);
    LinkedIntList* list = hashmap->indexes[indexInIndexes];
    Node* currentNode = list->head;
    while (currentNode != NULL){
        int possibleIndex = currentNode->value;
        tupleObj* tuple = popIndexDynamicMassiveObj(hashmap->massive, possibleIndex);
        type possibleKey = tuple->objects[0];
        type possibleValue = tuple->objects[1];
        long long possibleHash = getHashFromObject(possibleKey);
        if (hash == possibleHash){if (compareObjects(object, possibleKey)){return possibleValue;}}
        currentNode = currentNode->next;
    }
    return None;
}

type getValueStrKey(HashMapObj* hashmap, char* str){
	if (hashmap->id != HashMapType){
		printf("TypeError: object is not HashMapObj!\n");
		return None;
	}
    // object is key
    strObj objectOnStack = {strID, str, strlen(str)};
    strObj* object = &objectOnStack; //&objectOnStack
    long long hash = getHashFromObject(object);
    int indexInIndexes = greatMod(hash, hashmap->capacity);
    LinkedIntList* list = hashmap->indexes[indexInIndexes];
    Node* currentNode = list->head;
    while (currentNode != NULL){
        int possibleIndex = currentNode->value;
        tupleObj* tuple = popIndexDynamicMassiveObj(hashmap->massive, possibleIndex);
        type possibleKey = tuple->objects[0];
        type possibleValue = tuple->objects[1];
        long long possibleHash = getHashFromObject(possibleKey);
        if (hash == possibleHash){
        	if (compareObjects(object, possibleKey)){
        		//destroyObject(object);
        		return possibleValue;
        	}
    	}
        currentNode = currentNode->next;
    }
    //destroyObject(object);
    printf("KeyError: key didn't exist!\n");
    return None;
}

type getValueIntKey(HashMapObj* hashmap, int key){
    // object is key
    strObj* object = createIntObject(key);
    long long hash = getHashFromObject(object);
    int indexInIndexes = greatMod(hash, hashmap->capacity);
    LinkedIntList* list = hashmap->indexes[indexInIndexes];
    Node* currentNode = list->head;
    while (currentNode != NULL){
        int possibleIndex = currentNode->value;
        tupleObj* tuple = popIndexDynamicMassiveObj(hashmap->massive, possibleIndex);
        type possibleKey = tuple->objects[0];
        type possibleValue = tuple->objects[1];
        long long possibleHash = getHashFromObject(possibleKey);
        if (hash == possibleHash){
        	if (compareObjects(object, possibleKey)){
        		destroyObject(object);
        		return possibleValue;
        	}
    	}
        currentNode = currentNode->next;
    }
    destroyObject(object);
    return None;
}

type getValueFloatKey(HashMapObj* hashmap, float key){
    // object is key
    strObj* object = createFloatObject(key);
    long long hash = getHashFromObject(object);
    int indexInIndexes = greatMod(hash, hashmap->capacity);
    LinkedIntList* list = hashmap->indexes[indexInIndexes];
    Node* currentNode = list->head;
    while (currentNode != NULL){
        int possibleIndex = currentNode->value;
        tupleObj* tuple = popIndexDynamicMassiveObj(hashmap->massive, possibleIndex);
        type possibleKey = tuple->objects[0];
        type possibleValue = tuple->objects[1];
        long long possibleHash = getHashFromObject(possibleKey);
        if (hash == possibleHash){
        	if (compareObjects(object, possibleKey)){
        		destroyObject(object);
        		return possibleValue;
        	}
    	}
        currentNode = currentNode->next;
    }
    destroyObject(object);
    return None;
}

type getValueDoubleKey(HashMapObj* hashmap, double key){
    // object is key
    strObj* object = createDoubleObject(key);
    long long hash = getHashFromObject(object);
    int indexInIndexes = greatMod(hash, hashmap->capacity);
    LinkedIntList* list = hashmap->indexes[indexInIndexes];
    Node* currentNode = list->head;
    while (currentNode != NULL){
        int possibleIndex = currentNode->value;
        tupleObj* tuple = popIndexDynamicMassiveObj(hashmap->massive, possibleIndex);
        type possibleKey = tuple->objects[0];
        type possibleValue = tuple->objects[1];
        long long possibleHash = getHashFromObject(possibleKey);
        if (hash == possibleHash){
        	if (compareObjects(object, possibleKey)){
        		destroyObject(object);
        		return possibleValue;
        	}
    	}
        currentNode = currentNode->next;
    }
    destroyObject(object);
    return None;
}

void __snprintHashMapObj(char buffer[], int len, HashMapObj* hashmap, char* end){
	snprintf(buffer + strlen(buffer), len, "{");
    for (int i = 0; i < hashmap->massive->len; i++){
        tupleObj* tuple = hashmap->massive->massive[i];
        __snprintObject(buffer + strlen(buffer), len, tuple->objects[0], "");
        snprintf(buffer + strlen(buffer), len, ": ");
        __snprintObject(buffer + strlen(buffer), len, tuple->objects[1], "");
        if (i == hashmap->massive->len - 1){
            snprintf(buffer + strlen(buffer), len, "}%s", end);
            return;
        }
        snprintf(buffer + strlen(buffer), len, ", ");
    }
    snprintf(buffer + strlen(buffer), len, "}%s", end);
}

void __printHashMapObj(HashMapObj* hashmap, char* end){
	printf("{");
    for (int i = 0; i < hashmap->massive->len; i++){
        tupleObj* tuple = hashmap->massive->massive[i];
        __printObject(tuple->objects[0], "");
        printf(": ");
        __printObject(tuple->objects[1], "");
        if (i == hashmap->massive->len - 1){
            printf("}%s", end);
            return;
        }
        printf(", ");
    }
    printf("}%s", end);
}

void printHashMapObj(HashMapObj* hashmap){
    printf("{");
    for (int i = 0; i < hashmap->massive->len; i++){
        tupleObj* tuple = hashmap->massive->massive[i];
        __printObject(tuple->objects[0], "");
        printf(": ");
        __printObject(tuple->objects[1], "");
        if (i == hashmap->massive->len - 1){
            printf("}\n");
            return;
        }
        printf(", ");
    }
    printf("}\n");
}

#endif

/*
int main(){
	strObj* obj1 = createStrObject("Hello, guyss");
	intObj* obj2 = createIntObject(1011);
	doubleObj* obj3 = createDoubleObject(1011.2);
	noneObj* obj4 = None;
    type objects_massive[4] = {obj1, obj2, obj3, obj4};
	tupleObj* obj5 = createTupleObject(objects_massive, 4);

	DynamicMassiveObj* list = createDynamicMassiveObj();
    appendDynamicMassiveObj(list, createIntObject(10));
    appendDynamicMassiveObj(list, obj5);
    appendDynamicMassiveObj(list, obj2);
    appendDynamicMassiveObj(list, obj3);
    //printObject(list);

	HashMapObj* hashmap = createHashMapObj();
	addItemsToHashMapObj(hashmap, obj1, obj2);
	addItemsToHashMapObj(hashmap, obj3, obj5);
	addItemsToHashMapObj(hashmap, obj4, list);
	addItemsToHashMapObj(hashmap, None, obj1);
	printObject(hashmap);
	//printObject(_getValueDoubleKey(hashmap, 1011.2));
	return 0;
}*/

/*
int main(){
    DynamicMassiveObj* list = createDynamicMassiveObj();
    appendDynamicMassiveObj(list, createIntObject(10));
    type obj1 = createStrObject("Hello, guyss");
	type obj2 = createIntObject(1011);
	type obj3 = createDoubleObject(1011);
	type obj4 = None;
    type objects_massive[4] = {obj1, obj2, obj3, obj4};
	type obj5 = createTupleObject(objects_massive, 4);

    appendDynamicMassiveObj(list, obj5);
    appendDynamicMassiveObj(list, obj2);
    appendDynamicMassiveObj(list, obj3);
    printObject(list);
}*/

/*
int main(){
	type obj1 = createStrObject("Hello, guyss");
	type obj2 = createIntObject(1011);
	type obj3 = createDoubleObject(1011);
	type obj4 = None;
	type objects_massive[4] = {obj1, obj2, obj3, obj4};
	type obj5 = createTupleObject(objects_massive, 4);
	return 0;
}*/