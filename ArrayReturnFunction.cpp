#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;
 

pair<int*, int*> foo(){
	static int arr1[] = {1,2,3};
	static int arr2[] = {4,5,6};
	return make_pair(arr1, arr2);
}

int main()
{
	pair<int*, int*> arrays = foo();
	cout<< arrays.first[1] <<endl;
	cout<< arrays.second[2] <<endl;
	return 0;
}



