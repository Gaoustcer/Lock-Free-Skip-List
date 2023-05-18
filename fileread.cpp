#include "fileread.h"
#include <vector>
#include <thread>
void Searchworker(SkipList<string,int> &skiplist,
    int id, // id = 0,1,SEARCHPROCESS - 1
    vector<operations> Search)
{
    for(int i = 0;i < Search.size();i++){
        if(i & SEARCHPROCESS == id){
            optional<int> findresult = skiplist.search(Search[i].searchkey);
            if(findresult.has_value()){
                Search[i].value = findresult.value();
            }
        }
    }
}
void multiprocessvalidate(SkipList<string,int> &skiplist,string filename,string outputfile){
    vector<operations> Insert;
    ifstream infile;
    infile.open(filename,ios::in);
    string op;
    string opstr;
    // int value;
    while(infile.eof() == false){
        char buffer[30];
        infile.getline(buffer,30);
        string s_str(buffer);
        istringstream s(s_str);
        s >> op;
        s >> opstr;
        // get op [I,D,F] and operated string
        if(op == "I"){
            string valuestr;
            s >> valuestr;
            skiplist.insert(opstr,stoi(valuestr));
        }
        else if (op == "D"){
            skiplist.deletekey(opstr);
        }
        else{
            Insert.push_back(operations(opstr,-1));
        }
    }
    vector<thread> Worker;
    for(int i = 0;i < SEARCHPROCESS;i++){
        Worker.push_back(thread(Searchworker,i,Insert));
    }
    for(int i = 0;i < SEARCHPROCESS;i++){
        Worker[i].join();
    }
    ofstream file;
    file.open(outputfile,ios::out);
    for(auto & result:Insert){
        file << result.searchkey << " " << result.value << endl;
    }
    
}