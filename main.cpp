#include <bits/stdc++.h>
using namespace std;


int main() {

  while(true){
  cout << unitbuf;
  cerr << unitbuf;

  
  cout << "$ ";

  string input;
  getline(cin, input);  
  stringstream ss(input);
  string a,b;
  ss>>a>>b;
  if(a=="exit"){
    int tmp = 0;
    if(!b.empty()){
      try{
        tmp = stoi(b);
      }
      catch(const exception& e){
        cerr<<"Invalid exit code"<<b<<endl;
        continue;
      }
    }
    return tmp;
  }
    cout<<input<<": command not found"<<endl;
  }
  

}
