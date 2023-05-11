#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "skiplist.h"
// #define VALIDATE 
using namespace std;
void validate(SkipList<string,int> &skiplist,string filename = "./data/operations.txt"){
    ifstream infile;
    infile.open(filename,ios::in);
    // while(infile.end())
    string op;
    string opstr;
    int value;
    while(infile.eof() == false){
        char str[20];
        infile.getline(str,20);
        string s_str(str);
        istringstream s(s_str);
        // cout << s
        s>>op;
        s>>opstr;
        // cout << op <<"op " << opstr << endl;
        if(op == "I"){
            string valuestr;
            s>>valuestr;
            value = stoi(valuestr);
            skiplist.insert(opstr,value);
            // cout << "length " << skiplist.getlength() << endl;
        }
        else{
            auto ret = skiplist.search(opstr);
            // #define VALIDATE
            #ifdef VALIDATE
            if(ret.has_value()){
                cout << ret.value() << endl;
            }
            else{
                cout << -1 << endl;
            }
            #endif
        }
        
        // cout << str << "op"<<endl;
        // cout << str << "content" <<endl;
    }
}