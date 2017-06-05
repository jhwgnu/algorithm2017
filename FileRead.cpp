#include <fstream>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	cout << argv[1] << endl;
	ifstream ifs("DNAseq.fasta");
	string headLine;
	getline(ifs, headLine);
	string content( (std::istreambuf_iterator<char>(ifs) ),
	                   (std::istreambuf_iterator<char>()    ) );
	cout << content ;

  return 0;
}
