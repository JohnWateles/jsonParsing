typedef void* type;
typedef struct ID ID;
typedef struct noneObj noneObj;

typedef struct strObj strObj;
type createStrObject(char* string);
typedef struct intObj intObj;
type createIntObject(int value);
typedef struct floatObj floatObj;
type createFloatObject(float value);
typedef struct doubleObj doubleObj;
type createDoubleObject(double value);
typedef struct tupleObj tupleObj;
type createTupleObject(type* objects, int len);

int destroyObject(type);

long long getHashFromObject(type object);
int checkNULL_IN_OBJECTS_REALIZATION(void*);
void printObject(type object);
void __printObject(type object, char* end);
void __snprintObject(char buffer[], int len, type object, char* end);
int compareObjects(type obj1, type);

typedef struct DynamicMassiveObj DynamicMassiveObj;
DynamicMassiveObj* createDynamicMassiveObj();
int destroyDynamicMassiveObj(DynamicMassiveObj* list);
int __destroyAllDynamicMassiveObj(DynamicMassiveObj* list);
int lenDynamicMassiveObj(DynamicMassiveObj* list);
int existValueDynamicMassiveObj(DynamicMassiveObj* list, type value);
int appendDynamicMassiveObj(DynamicMassiveObj* list, type value);
int insertDynamicMassiveObj(DynamicMassiveObj* list, int index, type value);
type popLastDynamicMassiveObj(DynamicMassiveObj* list);
type popIndexDynamicMassiveObj(DynamicMassiveObj* list, int index);
int removeValueDynamicMassiveObj(DynamicMassiveObj* list, type value);
void printDynamicMassiveObj(DynamicMassiveObj* list);
void __printDynamicMassiveObj(DynamicMassiveObj* list, char* end);
void __snprintDynamicMassiveObj(char buffer[], int len, DynamicMassiveObj* list, char* end);

typedef struct HashMapObj HashMapObj;
int destroyHashMapObj(HashMapObj* hashmap);
int __destroyAllHashMapObj(HashMapObj* hashmap);
void printHashMapObj(HashMapObj* hashmap);
void __printHashMapObj(HashMapObj* hashmap, char* end);
void __snprintHashMapObj(char buffer[], int len, HashMapObj* hashmap, char* end);

void pass();
