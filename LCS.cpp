#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;
 
pair<int**, int**> LCS_LENGTH(string, string);
void PRINT_LCS(int**, string, int, int);
static string result = "";
int main()
{	
	string s1 = "SJHSFASHFKEWKF";
	string s2 = "ASDJKFHADSUFHKA";
    pair<int**, int**> matrix = LCS_LENGTH(s1,s2);
	
	int i,j;
	int m = s1.length(); //7
	int n = s2.length(); //6

	cout<<"Matrix b"<<endl;
	for(i=0; i<=m; i++){
		for(j=0; j<=n; j++){
			cout<<matrix.first[i][j];
		}
		cout<<endl;
	}
	cout<<endl;	
	
	string s1_temp = s1;
	for(i=s1_temp.length(); i>=1; i--) s1_temp[i] = s1_temp[i-1];	
	PRINT_LCS(matrix.first, s1_temp, m, n);
	
	cout << result;
	return 0;
}

void PRINT_LCS(int** b, string X, int m, int n){	
	int i,j;
	printf("b[%d][%d]: %d\n",m,n,b[m][n]);
	
	if(m==0 || n==0) return;
	if(b[m][n]==1){
		PRINT_LCS(b,X,m-1,n-1);
		result += X[m];
	}else if(b[m][n]==2){
		PRINT_LCS(b,X,m-1,n);
	}else PRINT_LCS(b,X,m,n-1);
}

pair<int**, int**> LCS_LENGTH(string X, string Y) {
	for(int i=X.length(); i>=1; i--) X[i] = X[i-1];
	for(int i=Y.length(); i>=1; i--) Y[i] = Y[i-1];
		
	int i, j;
	int m = X.length(); //7
	int n = Y.length(); //6
	
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

