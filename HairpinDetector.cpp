#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;

void CREATE_RANDOM_SEQ();
pair<int, int> CALC_CAND_REGION(int, int);
int FIND_PALINDROME(int);
bool FIND_HAIRPIN(int, int, int, int, int, int);

pair<int**, int**> LCS_LENGTH(string, string);
void PRINT_LCS(int**, string, string, int, int, string*, string*);
pair<string, string> ALIGN(string, string);
pair<int, int> CHECK_OUTER_PART(string, string, string, int, string*);
pair<int, int> CHECK_INNER_PART(string, string, string, int, string*);

string seq = "";

static int k = 10;
static int a = 4;
static int b = 9;
static int MIN_hairpin = 200;
static int MAX_hairpin = 400;
static int MAX_loop = 50;

int main(int argc, char** argv) {
	// cout<< "compile test"<<endl;
	int s1,e1,s2,e2,cand_start,cand_end;
	pair<int, int> cand_region;

//	CREATE_RANDOM_SEQ();
//	seq = "CCGCTTATCTTGTGCCTCCTACAACACGACTATGTGAATAGGCCCGCAAGATAAGGATTATACCGGCAAACCTGTAATAAGGACTGGAGGCTGGTTCCGG";
	ifstream ifs(argv[1]);
	string headLine;
	getline(ifs, headLine);
	string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	seq = content;


	for(s1=0; s1<=seq.length()-2*k; s1++){
		e1 = s1+k-1;
		s2 = FIND_PALINDROME(s1);
		if(s2<0) continue;

		e2 = s2+k-1;
		pair<int, int> cand_region = CALC_CAND_REGION(s1, e2);
		cand_start = cand_region.first;
		cand_end = cand_region.second;

//		cout << "candidate region: " << seq.substr(cand_start, cand_end-cand_start+1);
//		printf("[%d-%d]\n",cand_start,cand_end);

		bool found = FIND_HAIRPIN(cand_start, cand_end, s1, e1, s2, e2);
		if(found) s1 = cand_end+1;
//		s1 = cand_end+1;

	}
}


bool FIND_HAIRPIN(int cs, int ce, int s1, int e1, int s2, int e2){
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

//	cout << "SL:   " << SL << endl;
//	cout << "KMER: " << KMER << endl;
//	cout << "SM:   " << SM << endl;
//	cout << "SM_R: " << SM_R << endl;
//	cout << "SR:   " << SR << endl;
//	cout << "SR_R: " << SR_R << endl;
	// cout << endl;

	string *LCS_left = new string;
	string *LCS_right = new string;
	*LCS_left = "";
	*LCS_right = "";

//	cout << endl << "##Check outer part" << endl;
	pair<string, string> outer_LCS = ALIGN(SL,SR_R);
	string SL_aligned = outer_LCS.first;
	string SR_R_aligned = outer_LCS.second;
//	cout << "SL_aligned:   " << SL_aligned << endl;
//	cout << "SR_R_aligned: " << SR_R_aligned << endl;
	pair<int,int> outer_part = CHECK_OUTER_PART(SL_aligned, SR_R_aligned, KMER, s1, LCS_left);
	int leftarm_left = outer_part.first;
	int rightarm_right = outer_part.second;

//	cout << endl<< "##Check inner part" << endl;
	pair<string, string> inner_LCS = ALIGN(SM,SM_R);
	string SM_aligned = inner_LCS.first;
	string SM_R_aligned = inner_LCS.second;
//	cout << "SM_aligned:   " << SM_aligned << endl;
//	cout << "SM_R_aligned: " << SM_R_aligned << endl;
	pair<int,int> inner_part = CHECK_INNER_PART(SM_aligned, SM_R_aligned, KMER, e1, LCS_right);
	int leftarm_right = inner_part.first;
	int rightarm_left = inner_part.second;

//	cout << "[DEBUG] LCS_left : " << *LCS_left << endl;
//	cout << "[DEBUG] LCS_right : " << *LCS_right << endl;

//	cout << "leftarm_left = " << leftarm_left << endl;
//	cout << "rightarm_right = " << rightarm_right << endl;
	// cout << "s2 = "	 << s2 <<endl;
	// cout << "e1 = " << e1 <<endl;
	// cout << "leftarm_right = " << leftarm_right << endl;
	// cout << "rightarm_left = " << rightarm_left << endl;

	// MIN_hairpin <= whole hairpin length <= MAX_hairpin
	int h_start = s1-leftarm_left;
	int h_end = e2+rightarm_right;
	int h_length = h_end - h_start + 1;

	int loop_length = (s2-e1-1)-leftarm_right-rightarm_left;
	if(loop_length <= 0){
		loop_length = 0;
		// *LCS_right = seq.substr(e1+1, s2-e1+1);
	}

	// cout << "LCS_left: " << *LCS_left << endl;
	// cout << "LCS_right: " << *LCS_right << endl <<endl;

	// cout << "hairpin: " << seq.substr(s1-leftarm_left, h_length) << endl;
	// cout << "LCS sequence: " << *LCS_left << KMER << *LCS_right << endl;
	// cout << "loop: " << seq.substr(e1+leftarm_right+1, loop_length) << endl << endl;

	if(h_length < MIN_hairpin || h_length > MAX_hairpin){
		// printf("hairpin boundary: %d ~ %d\n",MIN_hairpin,MAX_hairpin);
		// printf("hairpin length: %d\n", h_length);
		return false;
	}


	if(loop_length < 0 || loop_length > MAX_loop){
		// printf("loop boundary: %d ~ %d\n",0,MAX_loop);
		// printf("loop length: %d\n", loop_length);
		return false;
	}
	// cout << endl << "*******hairpin Found*******" <<endl;
	// cout << "<hairpin> "<< endl;
	cout << seq.substr(s1-leftarm_left, h_length) << endl;
	// cout << "<LCS sequence> " << endl;
	cout << *LCS_left << KMER << *LCS_right << endl;
	// cout << "<loop>" << endl;
	cout << seq.substr(e1+leftarm_right+1, loop_length) << endl << endl;

	return true;
}

pair<int, int> CHECK_INNER_PART(string s1, string s2, string kmer, int kmer_end, string* LCS_right){
	string SM_aligned = s1;
	string SM_R_aligned = s2;
	int aligned_length = SM_aligned.length();
	int leftarm_right = 0;
	int rightarm_left = 0;

//	string LCS_right = "";

	bool stop_pos_found = false;
	int recent_match = -1;

	// cout <<"aligned_length: " << aligned_length <<endl;
	// cout << "SM_aligned:                        " << s1 << endl;
	// cout << "SM_R_aligned:                      " << s2 << endl;

	string kmer_temp(kmer);
	s1 = kmer.append(s1);
	s2 = kmer_temp.append(s2);

	// cout << "SM_aligned_appended:     " << s1 << endl;
	// cout << "SM_R_aligned_appended:   " << s2 << endl;

	int cnt_indel = 0;
	int i, j;
	for(int pos=k; pos< k+aligned_length/2; pos++){

		if(s1[pos-1]==s2[pos-1]){ // condition 2
			if(s1[pos]==s2[pos]) recent_match = pos;
			continue;
		}
		if(s1[pos]!=s2[pos]) continue;	// condition 1
		// cout << "***Examined Strings" << endl;
		// for(i=-(b-1)/2; i<=(b-1)/2; i++){
		// 	if(pos+i>=s1.length()) cout << '@';
		// 	else cout << s1[pos+i];
		// } cout << endl;

		// for(i=-(b-1)/2; i<=(b-1)/2; i++){
		// 	if(pos+i>=s1.length()) cout << '#';
		// 	else cout << s2[pos+i];
		// } cout << endl;

		cnt_indel = 0;
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(pos+i>=s1.length()) cnt_indel++;
			else if(s1[pos+i]!=s2[pos+i]) cnt_indel++;
		}
		if(cnt_indel>=a){
//			cout << "***STOP POSITION FOUND" << endl;
			stop_pos_found = true;
			break;
		}
		// if(recent_match-k+1 >= aligned_length/2) break;

		// printf("(%c, %c), POS=%d, RECENT_MATCH=%d\n", s1[pos], s2[pos], pos, recent_match);
		if(s1[pos]==s2[pos]) {
			recent_match = pos;
		}

		// cout << "***Examined Position: " <<pos << " "<< s1[pos] << " " << s2[pos] << endl;


	}
	// cout << "Final RECENT_MATCH: " << recent_match << endl;
	// cout << "STOP POS FOUND?: " << stop_pos_found << endl;
	if(recent_match < 0) return make_pair(0,0);
	if(!stop_pos_found){
		cnt_indel = 0;
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(s1[recent_match+i]!=s2[recent_match+i]) cnt_indel++;
		}
		if(cnt_indel>=a)  return make_pair(0,0);
	}

	for(i=k; i<=recent_match; i++) {
		if(s1[i] != '-') leftarm_right++;
	}
	for(i=k; i<=recent_match; i++){
		if(s2[i] != '-') rightarm_left++;
	}
	for(i=k; i<=recent_match; i++){
		if(s1[i] == s2[i]) *LCS_right += s1[i];
	}
	// cout << "LCS_right: " << *LCS_right << endl;
	return make_pair(leftarm_right,rightarm_left);
}

pair<int, int> CHECK_OUTER_PART(string s1, string s2, string kmer, int kmer_start, string* LCS_left){
	string SL_aligned = s1;
	string SR_R_aligned = s2;
	int leftarm_left = 0;
	int rightarm_right = 0;

//	string LCS_left = "";

	int recent_match = -1;
	bool stop_pos_found = false;

	int SL_length = s1.length();
	int pos = s1.length()-1;
	s1.append(kmer);
	s2.append(kmer);
//	cout << "SL_aligned_appended:   " << s1 << endl;
//	cout << "SR_R_aligned_appended: " << s2 << endl;

	int cnt_indel = 0;
	int i, j;
	for(; pos>=0; pos--){

//		cout << "***Examined Position: " <<pos << " " << s1[pos] << " " << s2[pos] << endl;
		if(s1[pos+1]==s2[pos+1]){ // condition 2
			if(s1[pos]==s2[pos]) recent_match = pos;
			continue;
		}
		if(s1[pos]!=s2[pos]) continue;	// condition 1

//		cout << "***Examined Strings" << endl;
//		for(i=-(b-1)/2; i<=(b-1)/2; i++){
//			if(pos+i<0) cout << '@';
//			else cout << s1[pos+i];
//		} cout << endl;
//
//		for(i=-(b-1)/2; i<=(b-1)/2; i++){
//			if(pos+i<0) cout << '#';
//			else cout << s2[pos+i];
//		} cout << endl;

		cnt_indel = 0;
		for(i=-(b-1)/2; i<=(b-1)/2; i++){
			if(pos+i<0) cnt_indel++;
			else if(s1[pos+i]!=s2[pos+i]) cnt_indel++;
		}
		if(cnt_indel>=a){
//			cout << "***STOP POSITION FOUND" << endl;
			stop_pos_found = true;
			break;
		}
		if(s1[pos]==s2[pos]) recent_match = pos;
	}
	// cout << endl;
	if(stop_pos_found) {
		if(recent_match < 0) return make_pair(0,0);
//		cout << "recent_match = " << recent_match << endl;
//		cout << "SL_aligned.length() = " << SL_aligned.length() << endl;
		for(i=recent_match; i<SL_aligned.length(); i++) {
			if(SL_aligned[i] != '-') leftarm_left++;
		}
		for(i=recent_match; i<SR_R_aligned.length(); i++){
			if(SR_R_aligned[i] != '-') rightarm_right++;
		}

//		cout << "LCS_left: " << *LCS_left << endl;
		for(i=recent_match; i<SR_R_aligned.length(); i++){
//			cout << "[DEBUG] " << i;
//			printf("[DEBUG] SL_aligned[i] = %c ",SL_aligned[i]);
//			printf("[DEBUG] SR_R_aligned[i] = %c\n",SR_R_aligned[i]);
			if(SL_aligned[i] == SR_R_aligned[i]) *LCS_left += SL_aligned[i];
		}
//		cout << "LCS_left: " << *LCS_left << endl;
//		cout << "leftarm_left = " << leftarm_left << endl;
//		cout << "rightarm_right = " << rightarm_right << endl;
		return make_pair(leftarm_left, rightarm_right);
	}else return make_pair(0,0);
//	}else if(pos<0 && recent_match==-1) return make_pair(0,0);

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
		// cout<<seq.substr(s1,k)<<" ";
		// printf("[%d-%d] ",s1,s1+k-1);
		// cout << seq.substr(s2,k);
		// printf(" [%d-%d] \n",s2,e2);
		break; // 짝 하나 찾으면 끝내도 됨
	}

	if(found) return s2;
	else{
		return -1;
	}
}

pair<int, int> CALC_CAND_REGION(int s1, int e2){
//	cout << "seq: " << seq << endl;
	int c_start, c_end;
	c_start = s1 - (MAX_hairpin-(e2-s1+1))/2;
	c_end = e2 + (MAX_hairpin-(e2-s1+1))/2;
	if(c_start < 0){
		c_start = 0;
		c_end = e2 + s1;
	} else if(c_end >= seq.length()){
		c_end = seq.length()-1;
		c_start = s1 - (seq.length()-1-e2);
	}
	return make_pair(c_start,c_end);
}

void CREATE_RANDOM_SEQ(){
//	int N = 100;
	srand(time(NULL));
	char nucleotide[4] = {'A','C','T','G'};
	for(int i=0; i<seq.length(); i++)	{
		seq += nucleotide[rand()%4];
	}
	cout << seq << endl;
}

