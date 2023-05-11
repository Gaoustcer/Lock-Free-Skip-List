#include "config.h"
#include <vector>
#include <optional>
#include <cstdlib>
#include <iostream>
enum TYPE{HEAD,TAIL,ELSE};
template<typename KEYTYPE>
class IndexNode{
private:
    KEYTYPE key;
    
    TYPE t;
    int level;
public:
    IndexNode * nextnode;
    IndexNode * nextlevel;
    IndexNode(KEYTYPE _key,int _level,TYPE _t = ELSE){
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
    friend std::ostream& operator<<(ostream &output,const IndexNode<KEYTYPE> & _indexnode){
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
    friend std::ostream& operator <<(ostream &output,Basenode<KEYTYPE,VALUETYPE> & _basenode){
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


