#include "fileread.hpp"
#include <vector>
#include <thread>
vector<operations> Search;

void Searchworker(SkipList<string,int> &skiplist,
    int id // id = 0,1,SEARCHPROCESS - 1
    )
{
    clock_t starttime = clock();
    size_t count = 0;
    for(int i = id;i < Search.size();i += SEARCHPROCESS){
        optional<int> findresult = skiplist.search(Search[i].searchkey);
        if(findresult.has_value()){
            // std::cout << "find result " << findresult.value() << std::endl;
            Search[i].value = findresult.value();
        }
        count += 1;
    }
    cout << "time cost for thread " << id << " " << clock()  - starttime << " search count " << count << endl;
    // return clock() - starttime;
}

clock_t linearsearch(SkipList<string,int> &skiplist,size_t numsearch){
    clock_t starttime = clock();
    int i = 0;
    for(i = 0;i < Search.size() && i <= numsearch;i++){
        optional<int> findresult = skiplist.search(Search[i].searchkey);
    }
    cout << "Search time for " << numsearch << " element " << " time cost is " << clock() - starttime << endl;
    return clock() - starttime;
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
    Search = Insert;
    #ifdef LINEARSEARCH
    
    for(int i = 1;i <= 16;i++){
        linearsearch(skiplist,i * 1024 * 128); 
    }
    
    #else
    vector<thread> Worker;
    clock_t startsearchtime = clock();
    for(int i = 0;i < SEARCHPROCESS;i++){
        Worker.push_back(thread(Searchworker,std::ref(skiplist),i));
    }
    for(int i = 0;i < SEARCHPROCESS;i++){
        Worker[i].join();
    }
    cout << "total search time " << clock() - startsearchtime << endl;
    ofstream file;
    file.open(outputfile,ios::out);
    for(auto & result:Search){
        file << result.searchkey << " " << result.value << endl;
        // if(result.value != -1)
            // cout << result.searchkey << " " << result.value << endl;
    }
    #endif
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