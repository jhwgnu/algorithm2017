#include <iostream>
#include <string>

using namespace std;

void foo(string s){
	s+='o';
}

void bar(string *s){
	*s+='o';
}

int main(){
	string *str = new string;
	*str = "hi";
//	foo(*str);
	bar(str);
	cout << *str << endl;
}

