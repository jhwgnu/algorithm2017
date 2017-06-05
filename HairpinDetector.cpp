#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

void CREATE_RANDOM_SEQ();
pair<int, int> CALC_CAND_REGION(int, int);
int FIND_PALINDROME(int);
void PRINT_IF_HAIRPIN(int, int, int, int, int, int);

pair<int**, int**> LCS_LENGTH(string, string);
void PRINT_LCS(int**, string, string, int, int, string*, string*);
pair<string, string> ALIGN(string, string);
bool CHECK_OUTER_PART(string, string, string);
bool CHECK_INNER_PART(string, string, string);

string seq = "";

static int N = 100;
static int k = 4;
static int a = 4;
static int b = 5;
//static int MIN_hairpin = 40;
static int MAX_hairpin = 30;
static int MAX_loop = 15;

	
int main() {
	cout<< "compile test"<<endl;
	int s1,e1,s2,e2,cand_start,cand_end;
	pair<int, int> cand_region;
	
	CREATE_RANDOM_SEQ();
//	seq = "ASDASDASDASDASDASDTGGACGTGTATTGGCCAGGAADSADASDASDADASDASDASDASDASDAD"	;
		
	for(s1=0; s1<=N-2*k; s1++){
		e1 = s1+k-1;
		s2 = FIND_PALINDROME(s1);
		if(s2<0) continue;
		
		e2 = s2+k-1;
		pair<int, int> cand_region = CALC_CAND_REGION(s1, e2);
		cand_start = cand_region.first;
		cand_end = cand_region.second;
		
		PRINT_IF_HAIRPIN(cand_start, cand_end, s1, e1, s2, e2);		
	}
}


void PRINT_IF_HAIRPIN(int cs, int ce, int s1, int e1, int s2, int e2){
	printf("%d, %d, %d, %d, %d, %d\n", cs, s1, e1, s2, e2, ce);
	string SL, KMER, SM, KMER_R, SR;
	SL = seq.substr(cs,s1-cs);
	KMER = seq.substr(s1,k);
	SM = seq.substr(e1+1,s2-e1-1);
	KMER_R = seq.substr(s2,k);
	SR = seq.substr(e2+1,ce-e2);
	
	string SM_R(SM);
	reverse(SM_R.begin(), SM_R.end());	
	string SR_R(SR);
	reverse(SR_R.begin(), SR_R.end());
		
	cout << "SL: " << SL << endl;
	cout << "KMER: " << KMER << endl;
	cout << "SM: " << SM << endl;
	cout << "SM_R: " << SM_R << endl;
	cout << "KMER_R: " << KMER_R << endl;
	cout << "SR: " << SR << endl;			
	cout << "SR_R: " << SR_R << endl;	
	
	
	pair<string, string> outer_LCS = ALIGN(SL,SR_R);
	string SL_aligned = outer_LCS.first;
	string SR_R_aligned = outer_LCS.second;		
	
	cout << endl;
	cout << "SL_aligned:   " << SL_aligned << endl;	
	cout << "SR_R_aligned: " << SR_R_aligned << endl;	
	
	CHECK_OUTER_PART(SL_aligned, SR_R_aligned, KMER);
	
	cout <<endl<<endl;	
	return;
}

bool CHECK_OUTER_PART(string s1, string s2, string kmer){
	bool satisfied = false;
	bool stop_pos_found = false;
	cout << endl;
	int pos = s1.length()-2;		
	s1.append(kmer);
	s2.append(kmer);
	cout << "SL_aligned_appended:   " << s1 << endl;
	cout << "SR_R_aligned_appended: " << s2 << endl;
	
	cout << endl;	
	int cnt_indel = 0;
	int i, j;
	for(; pos>=0; pos--){		
		cout << "***Examined Position: " << s1[pos] << " " << s2[pos] << endl;
		if(s1[pos+1]==s2[pos+1]) continue; // condition 2
		if(s1[pos]!=s2[pos]) continue;	// condition 1		
		
		cout << "***Examined Strings" << endl;
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(pos+i<0) cout << '@';
			else cout << s1[pos+i];
		} cout << endl;
		
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(pos+i<0) cout << '#';
			else cout << s2[pos+i];
		} cout << endl;
		
		
		cnt_indel = 0;
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(pos+i<0) cnt_indel++;
			else if(s1[pos+i]!=s2[pos+i]) cnt_indel++;
		}		
		if(cnt_indel>=a){
			cout << "***STOP POSITION FOUND cnt_indel = " << cnt_indel << endl;
			stop_pos_found = true;
			break;
		}
	}
	if(stop_pos_found) return false;
	cout << "왼쪽은 PASS!" <<endl;
	cout << endl;
	return true;
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
	if(m==0 && n==0) {		
		return;	
	}
	if(m!=0 && n==0){
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,'-');
		PRINT_LCS(b,X,Y,m-1,n,s1_aligned,s2_aligned);
		return;
	}
	if(m==0 && n!=0){
		(*s1_aligned).insert(0,1,'-');
		(*s2_aligned).insert(0,1,Y[n-1]);		
		PRINT_LCS(b,X,Y,m,n-1,s1_aligned,s2_aligned);	
		return;
	}
	
	if(b[m][n]==1){		
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,Y[n-1]);
		PRINT_LCS(b,X,Y,m-1,n-1,s1_aligned,s2_aligned);
		
	}else if(b[m][n]==2){
		(*s1_aligned).insert(0,1,X[m-1]);
		(*s2_aligned).insert(0,1,'-');		
		PRINT_LCS(b,X,Y,m-1,n,s1_aligned,s2_aligned);
		
	}else{
		(*s1_aligned).insert(0,1,'-');
		(*s2_aligned).insert(0,1,Y[n-1]);		
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

int FIND_PALINDROME(int s1){
	bool no_match = false;
	bool found = false;
	
	int s2,e2,m;
	for(s2=s1+MAX_loop-k; s2<=s1+MAX_hairpin-k; s2++){
		e2 = s2+k-1;			
		for(m=0; m<k; m++) {
			if(seq[s1+m]!=seq[e2-m]){					
				no_match = true;
				break;
			}
		}
		if(no_match) {
			no_match = false;
			continue;	
		}
		//continue 안 당하고 여기까지 왔으면 짝 찾은거임 
		found = true;
		cout<<seq.substr(s1,k)<<" ";
		printf("[%d-%d] ",s1,s1+k-1);		
		cout << seq.substr(s2,k);			
		printf(" [%d-%d] ",s2,e2);
		break; // 짝 하나 찾으면 끝내도 됨 
	}
	
	if(found) return s2;
	else{
//		cout << endl;
		return -1;	
	} 
}

pair<int, int> CALC_CAND_REGION(int s1, int e2){
	int c_start, c_end;
	c_start = s1 - (MAX_hairpin-(e2-s1+1))/2;
	c_end = e2 + (MAX_hairpin-(e2-s1+1))/2;	
	if(c_start < 0){
		c_start = 0;
		c_end = e2 + s1;
	} else if(c_end >= N){
		c_end = N-1;
		c_start = s1 - (N-1-e2);	
	}	
	return make_pair(c_start,c_end);
}

void CREATE_RANDOM_SEQ(){
	srand(time(NULL));	
	char nucleotide[4] = {'A','C','T','G'};	
	for(int i=0; i<N; i++)	{		
		seq += nucleotide[rand()%4];
	}	
	cout << seq << endl;	
}

