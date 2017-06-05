#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

pair<int**, int**> CREATE_ARRAY(){
	int i,j;
	int** table1 = new int*[10];
	for (i = 0; i < 10; ++i){
	   table1[i] = new int[10];
	}
	for(i=0; i<10; i++){
		for(j=0; j<10; j++){
			table1[i][j] = i+j;
		}
	}
	
	int** table2 = new int*[10];
	for (i = 0; i < 10; ++i){
	   table2[i] = new int[10];
	}
	for(i=0; i<10; i++){
		for(j=0; j<10; j++){
			table2[i][j] = i*j;
		}
	}
	return make_pair(table1, table2);
}

int main(){
	pair<int**, int**> arr = CREATE_ARRAY();
	cout << arr.first[7][8] <<endl;
	cout << arr.second[7][8] <<endl;
	return 0;
}

