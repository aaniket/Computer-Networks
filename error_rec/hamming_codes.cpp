#include <bits/stdc++.h>

using namespace std;

int main(){
	string message;
	cin>>message;

	int n=message.size();
	int r=0;
	while((1<<r)<n+1+r)r++;

	string newm="";
	for(int i=1; i<=r+n; i++)
			newm+="0";
	int j=0;
	int c=0;
	for(int i=1; i<=r+n; i++){
		if(i==(1<<c))c++;
		else{
			newm[i-1]=message[j++];
		}
	}

	for(int i=0; i<r; i++){
		int c=0;
		for(int j=1; j<=r+n; j++){
			if((j>>i)&1)c++;
		}
		if(c&1)newm[(1<<i)-1]='1';
	}

	string received;
	cin>>received;

	//Error Check
	int p=0;
	for(int i=0; i<r; i++){
		int c=0;
		for(int j=1; j<=n+r; j++){
			if((j>>i)&1){
				if((received[j-1]-'0')&1)c++;
			}
		}
		if(c&1)p+=(1<<i);
	}
	if(p==0)cout<<"No Error Found...\n";
	else
	cout<<"Error found at ->  "<<p<<endl;
	return 0;
}