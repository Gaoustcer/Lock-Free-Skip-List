#include "fileread.hpp"
#include <vector>
#include <thread>
void Searchworker(SkipList<string,int> &skiplist,
    int id, // id = 0,1,SEARCHPROCESS - 1
    vector<operations>& Search,clock_t starttime)
{
    for(int i = 0;i < Search.size();i++){
        if(i & SEARCHPROCESS == id){
            optional<int> findresult = skiplist.search(Search[i].searchkey);
            if(findresult.has_value()){
                Search[i].value = findresult.value();
            }
        }
    }
    cout << "time cost for thread " << id << " " << clock()  - starttime << endl;
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
    clock_t startsearchtime = clock();
    for(int i = 0;i < SEARCHPROCESS;i++){
        Worker.push_back(thread(Searchworker,std::ref(skiplist),i,std::ref(Insert),clock()));
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

void validate(SkipList<string,int> &skiplist,string filename){
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
        else if(op == "F"){
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
        else if(op == "D"){
            // string delkey;
            skiplist.deletekey(opstr);
        }
        
        // cout << str << "op"<<endl;
        // cout << str << "content" <<endl;
    }
}