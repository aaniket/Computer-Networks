//new.cpp
#include<bits/stdc++.h>
using namespace std;
int main(){
int a[6];
int n=6;
for(int i=0;i<n;i++)
cin>>a[i];
for(int i=0;i<n;i++){
for(int j=i+1;j<n;j++){
if(a[j]<a[i]){
int temp=a[j];
a[j]=a[i];
a[i]=temp;
}
}
}
return 0;
}