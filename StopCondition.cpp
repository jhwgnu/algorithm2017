#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;
 
pair<int**, int**> LCS_LENGTH(string, string);
void PRINT_LCS(int**, string, string, int, int, string*, string*);
pair<string, string> ALIGN(string, string);
int STOP_POSITION(string, string);

//static string s1_aligned = "";
//static string s2_aligned = "";
int main()
{	
	string s1 = "GCACGATAGATATACGATAATTA";
	string s2 = "TCGAGTGTGACAGAGATGGGATT";
		
	pair<string, string> result = ALIGN(s1,s2);
	string s1_aligned = result.first;
	string s2_aligned = result.second;
	cout <<s1_aligned <<endl;
	cout <<s2_aligned <<endl;	
	
	int STOP_POS = STOP_POSITION(s1_aligned, s2_aligned);
	return 0;
	
}

int STOP_POSITION(string s1, string s2){
	cout << "STOP_POSITION() called" << endl;
	int POS=s1.length()-2;
	int a = 4;
	int b = 9;
	int cnt_indel = 0;
	int cnt_match = 0;
	int i, j;
	while(pos>=0){		
		if(s1[pos]!=s2[pos]) continue;	// condition 1
		if(s1[pos+1]==s2[pos+1]) continue; // condition 2
		
		cnt_match = 0;
		for(i=0; i<b; i++){
			
		}
		pos--;				
	}
	cout << endl;
	return 0;
}

pair<string, string> ALIGN(string s1, string s2){
	string *s1_result = new string;
	string *s2_result = new string;
	*s1_result = "";
	*s2_result = "";	
	pair<int**, int**> matrix = LCS_LENGTH(s1,s2);
	PRINT_LCS(matrix.first, s1, s2, s1.length(), s2.length(), s1_result, s2_result);	
	return make_pair(*s1_result, *s2_result);
}

void PRINT_LCS(int** b, string X, string Y, int m, int n, string* s1_aligned, string* s2_aligned){	
	int i,j;
	printf("b[%d][%d]: %d\n",m,n,b[m][n]);
	if(m==0 && n==0) {		
		return;	
	}
	if(m!=0 && n==0){
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,'-');
		cout << "s1_aligned: " << (*s1_aligned) << endl;
		cout << "s2_aligned: " << (*s2_aligned) << endl;
		PRINT_LCS(b,X,Y,m-1,n,s1_aligned,s2_aligned);
		return;
	}
	if(m==0 && n!=0){
		(*s1_aligned).insert(0,1,'-');
		(*s2_aligned).insert(0,1,Y[n-1]);		
		cout << "s1_aligned: " << (*s1_aligned) << endl;
		cout << "s2_aligned: " << (*s2_aligned) << endl;
		PRINT_LCS(b,X,Y,m,n-1,s1_aligned,s2_aligned);	
		return;
	}
	
	if(b[m][n]==1){		
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,Y[n-1]);
		cout << "s1_aligned: " << (*s1_aligned) << endl;
		cout << "s2_aligned: " << (*s2_aligned) << endl;
		PRINT_LCS(b,X,Y,m-1,n-1,s1_aligned,s2_aligned);
		
	}else if(b[m][n]==2){
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,'-');		
		cout << "s1_aligned: " << (*s1_aligned) << endl;
		cout << "s2_aligned: " << (*s2_aligned) << endl;
		PRINT_LCS(b,X,Y,m-1,n,s1_aligned,s2_aligned);
		
	}else{
		(*s1_aligned).insert(0,1,'-');
		(*s2_aligned).insert(0,1,Y[n-1]);		
		cout << "s1_aligned: " << (*s1_aligned) << endl;
		cout << "s2_aligned: " << (*s2_aligned) << endl;
		PRINT_LCS(b,X,Y,m,n-1,s1_aligned,s2_aligned);	
		
	}
}

pair<int**, int**> LCS_LENGTH(string X, string Y) {
	for(int i=X.length(); i>=1; i--) X[i] = X[i-1];
	for(int i=Y.length(); i>=1; i--) Y[i] = Y[i-1];
		
	int i, j;
	int m = X.length();
	int n = Y.length();
	
	int** b = new int*[m+1];
	for (i=0; i<m+1; i++){
	   b[i] = new int[n+1];
	}
	
	int** c = new int*[m+1];
	for (i=0; i<m+1; i++){
	   c[i] = new int[n+1];
	}			
		
	for(i=1; i<=m; i++){		
		c[i][0] = 0;
		b[i][0] = 0;
	}
	
	for(j=0; j<=n; j++){		
		c[0][j] = 0;
		b[0][j] = 0;
	}
	
	for(i=1; i<=m; i++){
		for(j=1; j<=n; j++){
			if(X[i]==Y[j]){
				c[i][j] = c[i-1][j-1] + 1;
				b[i][j] = 1;
			}else if(c[i-1][j]>=c[i][j-1]){
				c[i][j] = c[i-1][j];
				b[i][j] = 2;				
			} else {
				c[i][j] = c[i][j-1];
				b[i][j] = 3;
			}
		}
	}		
	return make_pair(b,c);	
} 

