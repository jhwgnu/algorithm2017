#include <iostream>
#include <string>

using namespace std;

int main() {
	string s = "";	
	string in1 = "a";
	string in2 = "bc";
	char ch1 = 'd';
	char ch2 = 'e';
	
	s.insert(0,in1);
	s.insert(0,in2);
	s.insert(0,1,ch1);
	s.insert(0,1,ch2);
	cout << s;
}
