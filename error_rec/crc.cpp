#include <bits/stdc++.h>
using namespace std;

string calculate(string process, string polynomial, int n){
	string rem;
	int i=0;
	while(i<n){
		while(i<n && process[i]=='0')i++;
		if(i>=n){
			break;
		}
		for(int j=0; j<polynomial.size(); j++){
			process[i+j]=(char)(((polynomial[j]-'0')^(process[i+j]-'0'))+'0');
		}
	}
	rem=process.substr(process.size()-polynomial.size()+1, polynomial.size()-1);
	return rem;
}

int main(){
	string polynomial;
	cin>>polynomial;

	int n=polynomial.size()-1;

	string message;
	cin>>message;
	
	int m=message.size();
	
	string process=message;
	
	for(int i=0; i<n; i++)
			process+="0";
	
	string rem=calculate(process, polynomial, m);
	
	string send=message+rem;
	string ans=calculate(send, polynomial, m);
	
	int fl=0;
	int i=0;
	while(i<ans.size())
		if(ans[i++]=='1')fl=1;

	if(fl)cout<<"Error...";
	else cout<<"No Error Found...";

	return 0;
}