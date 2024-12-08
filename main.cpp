#include <bits/stdc++.h>
using namespace std;

int main()
{
  unordered_set<string> st = {"echo", "exit", "type"};


  while (true)
  {
    cout << unitbuf;
    cerr << unitbuf;
    cout << "$ ";

    string input;
    getline(cin, input);

    if (cin.eof())
    {
      break;
    }

    stringstream ss(input);
    string command;
    ss >> command;
    if (command == "exit")
    {
      int extCode = 0;
      string arg;
      ss >> arg;
      if (!arg.empty())
      {
        try
        {
          extCode = stoi(arg);
        }
        catch (const exception &e)
        {
          cerr << "Invalid exit code" << arg << endl;
          continue;
        }
      }
      return extCode;
    }
    else if (command == "echo")
    {
      string message;
      getline(ss, message);

      if (!message.empty() && message[0] == ' ')
      {
        message = message.substr(1);
      }
      cout << message << endl;
    }
    else if(command=="type"){
      string str;
      ss>>str;
      if(st.find(str)!=st.end()){
        cout<<str<<" is a shell builtin"<<endl;
      }
      else if(!str.empty()){
        cout<<str<<": not found"<<endl;
      }
    }
    else if (!command.empty())
    {
      cout << input << ": command not found" << endl;
    }
  }
}
