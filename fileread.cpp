#include "fileread.hpp"
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>
atomic<int> opcount{0};
vector<operations> Search;
extern SkipList<string,int> skiplist;
// mutex mtx;
void Searchworker(
    int id // id = 0,1,SEARCHPROCESS - 1
    )
{
    // clock_t starttime = clock();
    size_t count = 0;
    size_t start = id * Search.size()/SEARCHPROCESS;
    size_t end = (id + 1) * Search.size()/SEARCHPROCESS;
    for(int i = start;i < end;i += 1){
        // clock_t startsearch = clock();
        // cout << "iteration " << i << endl;
        optional<int> findresult = skiplist.search(Search[i].searchkey);
        // searchtime += clock() - startsearch;
        if(findresult.has_value()){
            // std::cout << "find result " << findresult.value() << " " << i << std::endl;
            Search[i].value = findresult.value();
        }
        // cout << "search " << i << endl;
        count += 1;
    }
    // clock_t searchtime = clock() - starttime;
    // cout << "time cost for thread " << id << " " << clock()  - starttime << " search count " << count <<  " element per time " << searchtime/count << endl;
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

void insertworker(string filename){
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
        if(s_str.size() == 0){
            break;
        }
        // cout << "str len " << s_str.size() << endl;
        istringstream s(s_str);
        s >> op;
        s >> opstr;
        // get op [I,D,F] and operated string
        if(op == "I"){
            string valuestr;
            s >> valuestr;
            // cout << "value " << valuestr << endl;
            skiplist.insert(opstr,stoi(valuestr));
            // cout << "insert an element " << endl;
        }
        else if (op == "D"){
            // cout << "delete key " << opstr << endl;
            skiplist.deletekey(opstr);
        }
        else{
            Insert.push_back(operations(opstr,-1));
        }
    
        opcount.fetch_add(1);
        // cout << "count " << opcount << endl;
    }
    Search = Insert;
}

void parallelinsert(vector<string> insertfilepath,string searchfilepath,string outputfile){
    // insertfilepath store 4 independent insert sequence
    // searchfilepath store all F instructions
    // outputfile store find result;
    
    size_t parallelinsert = insertfilepath.size();
    // vector<thread> searchthread;
    #ifdef ONEINSERT
    // thread T(insertworker,)
    for(auto &filename:insertfilepath){
        insertworker(filename);
        // std::thread T(insertworker,filename);
        // T.join();
    }
    #else
    // cout << "finish insert " << endl;
    vector<thread> searchvec;
    for(auto &filename:insertfilepath){
        searchvec.push_back(thread(insertworker,filename));
    }
    for(int i = 0;i < parallelinsert;i++){
        searchvec[i].join();
    }
    #endif
    cout << "finish insert\n";
    insertworker(searchfilepath);
    vector<thread> Worker;
    clock_t startsearchtime = clock();
    // size_t CREATEPROCESS = 1;
    for(int i = 0;i < SEARCHPROCESS;i++){
        // auto f = std::bind(Searchworker,i);
        Worker.push_back(thread(Searchworker,i));
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
    // readsearch
    
}
// void parallelinsert()

void multiprocessvalidate(string filename,string outputfile){
    cout << "Num of Thread " << SEARCHPROCESS << endl;
    insertworker(filename);
    #ifdef LINEARSEARCH
    
    for(int i = 1;i <= 16;i++){
        linearsearch(skiplist,i * 1024 * 128); 
    }
    
    #else
    vector<thread> Worker;
    clock_t startsearchtime = clock();
    // size_t CREATEPROCESS = 1;
    for(int i = 0;i < SEARCHPROCESS;i++){
        // auto f = std::bind(Searchworker,i);
        Worker.push_back(thread(Searchworker,i));
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
    skiplist.save();
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