// #include "config.hpp"
#include <vector>
#include <optional>
#include <cstdlib>
#include <iostream>
#include <atomic>
#include <mutex>
using namespace std;
enum TYPE{HEAD,TAIL,ELSE};
template<typename KEYTYPE>
class IndexNode{
private:
    KEYTYPE key;
    mutex _mtx;
    atomic<bool> _marked{false};
    atomic<bool> _fullylinked{false};
    // bool _marked;
    // bool _fullylinked;
    TYPE t;
    int level;
public:
    IndexNode * nextnode{nullptr};
    IndexNode * nextlevel{nullptr};
    IndexNode<KEYTYPE>& operator=(const IndexNode<KEYTYPE> & index){
        _marked = false;
        _fullylinked = false;
        key = index.key;
        t = index.t;
        level = index.level;
        nextnode = (IndexNode<KEYTYPE> *)(index.nextnode);
        nextlevel = (IndexNode<KEYTYPE> *)(index.nextlevel);
    }
    bool conditioncheck() const{
        return (_marked == true) || (_fullylinked == false); // has been marked or not linked
    }
    void lock(){
        _mtx.lock();
    }
    void unlock(){
        _mtx.unlock();
    }
    void setmarked(){
        _marked = true;
    }
    void setfullylinked(){
        _fullylinked = true;
    }
    bool marked(){
        return bool(_marked);
    }
    bool fullylinked(){
        return bool(_fullylinked);
    }
    IndexNode(KEYTYPE _key,int _level,TYPE _t = ELSE){
        _marked = {false};
        _fullylinked = {false};
        key = _key;
        t = _t;
        level = _level;
        nextnode = NULL;
        nextlevel = NULL;
    }
    IndexNode(){
        key = KEYTYPE();
        level = 0;
    }
    KEYTYPE getkey() const{
        #ifdef NODEMUTEX
        while(this->conditioncheck()){
            // wait until the node is finished
        }
        #endif
        return key;
    }
    void insertnext(IndexNode * _nextnode){
        nextnode = _nextnode;
    }
    void insertnextlevel(IndexNode * _nextlevel){
        nextlevel = _nextlevel;
    }
    TYPE gettype() const{
        return t;
    }
    friend std::ostream& operator<<(std::ostream &output,const IndexNode<KEYTYPE> & _indexnode){
        output << "key :" << _indexnode.getkey << " ";
    }

};

template<typename KEYTYPE,typename VALUETYPE>
class Basenode:public IndexNode<KEYTYPE>{
private:
    VALUETYPE value;
public:
    Basenode<KEYTYPE,VALUETYPE>* nextnode;
    Basenode<KEYTYPE,VALUETYPE>* nextlevel;
    // Basenode& operator=(const Basenode &copybasenode){
    //     // _marked = false;
    //     // _fullylinked = false;
    //     // key = index.key;
    //     // t = index.t;
    //     // level = index.level;
    //     nextnode = (Basenode<KEYTYPE,VALUETYPE> *)(copybasenode.nextnode);
    //     nextlevel = (Basenode<KEYTYPE,VALUETYPE> *)(copybasenode.nextlevel);
    // }
    void insertnext(Basenode<KEYTYPE,VALUETYPE> * _nextnode){
        nextnode = _nextnode;
    }
    Basenode(KEYTYPE _key,VALUETYPE _value,TYPE _t):IndexNode<KEYTYPE>(_key,0,_t){
        // IndexNode<KEYTYPE>(_key,0,ELSE);
        value = _value;
        nextnode = NULL;
        nextlevel = NULL;
    }
    VALUETYPE & getvalue(){
        return value;
    }
    friend std::ostream& operator <<(std::ostream &output,Basenode<KEYTYPE,VALUETYPE> & _basenode){
        output << "key " << _basenode.getkey() << " value " << _basenode.getvalue() << " "; 
    }
    Basenode():IndexNode<KEYTYPE>(){}
};
template<typename KEYTYPE>
int compare(const IndexNode<KEYTYPE> &keynode,const KEYTYPE &k2){
    if(keynode.gettype() == HEAD){
        // return small
        return -1;
    }
    else if(keynode.gettype() == TAIL){
        return 1;
    }
    else{
        KEYTYPE key = keynode.getkey();
        if(key < k2){
            return -1;
        }
        else{
            return 1;
        }
    }
}
template<typename KEYTYPE,typename VALUETYPE>
int compare(const Basenode<KEYTYPE,VALUETYPE> &node,const KEYTYPE &_key){
    if(node.gettype() == HEAD){
        return -1;
    }
    else if(node.gettype() == TAIL){
        return 1;
    }
    else{
        KEYTYPE key = node.getkey();
        if(key < _key){
            return -1;
        }
        else{
            return 1;
        }
    }
}


