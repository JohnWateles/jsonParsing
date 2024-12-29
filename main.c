#include <stdio.h>
#include "json_c.c"

static int test_parse1(){
	char* paths[5] = {"keys.json", "keys2.json", "keys3.json", "keys4.json", "keys5.json"};
	//char buff[2048] = {0};
	type storage;
	for (int i = 0; i < 5; i++){
		//buff[0] = '\0';
		JSON_FILE* file = jsonOpenToRead(paths[i]);
		type jsonValue = jsonParse(file);
		//printObject(jsonValue);
		if (i == 1) storage = jsonValue;
		else __destroyAllObject(jsonValue);
		jsonClose(file);
	}

	JSON_FILE* file2 = jsonOpenToWrite("keys6.json");
	jsonDump(file2, storage);
	jsonClose(file2);

	printObject(storage);
	type value = popLastDynamicMassiveObj(storage);
	printObject(value);
	//type valueOfValue = getValueStrKey(value, "key55");
	//printObject(valueOfValue);
	//__destroyAllObject(storage);
	return 0;
}

static int test_parse2(){
	JSON_FILE* file = jsonOpenToRead("calculates.json");
	type jsonValue = jsonParse(file);
	printObject(getValueStrKey(jsonValue, "OY"));
}

int main(){
	test_parse1();
	return 0;
}