#ifndef SKIPLIST
#define SKIPLIST
#include "node.hpp"
#include <mutex>
#include <fstream>
#include <ctime>
#include "config.hpp"
// #define LAYERLOCK
// #include "config.h"
// #define GLOBALLOCK
// std::mutex mtx;
template<typename KEYTYPE,typename VALUETYPE>
class SkipList{
private:
    std::vector<IndexNode<KEYTYPE>*> head;
    std::vector<IndexNode<KEYTYPE>*> tail;
    Basenode<KEYTYPE,VALUETYPE> Basehead{};
    Basenode<KEYTYPE,VALUETYPE> Basetail{};
    vector<size_t> timecost;
    #ifdef LAYERLOCK
    std::mutex *mtxarray;
    #endif
    #ifdef GLOBALLOCK
    std::mutex mtx; 
    #endif
    int maxheight = 0;
public:
    friend std::ostream& operator<<(std::ostream &output,SkipList<KEYTYPE,VALUETYPE> &_skiplist){
        for(int i = _skiplist.depth() - 1;i >= 0;i++){
            IndexNode<KEYTYPE> * index = _skiplist.iteratorindex(i) -> nextnode;
            IndexNode<KEYTYPE> * tail = _skiplist.iteratorindex(i);
            while(index != tail){
                output << *index;
            }
            output << std::endl;
        }
    }
    size_t getlength(){
        size_t nsize = 0;
        auto node = &Basehead;
        while(node){
            // std::cout << node << std::endl;
            node = node->nextnode;
            
            nsize += 1;
        }
        return nsize - 2;
    }
    IndexNode<KEYTYPE>* iteratortail(int index){
        static_assert(index < maxheight);
        return &tail[index];
    }
    IndexNode<KEYTYPE>* iteratorindex(int index){
        static_assert(index < maxheight);
        return &head[index];
    }
    int depth(){
        return maxheight;
    }
    void save(){
        ofstream fileout("./data/time.txt");
        // fileout.open("./data/time.txt",ios::out);
        cout << "time length " << timecost.size() << "\n";
        for(auto &time:timecost){
            fileout << time << endl;
        }
    }
    SkipList(int _maxheight = MAXHEIGHT){
        #ifdef LAYERLOCK
        mtxarray = new std::mutex[_maxheight + 1];
        #endif
        Basehead = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),HEAD);
        Basetail = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),TAIL);
        Basehead.setfullylinked();
        Basetail.setfullylinked();
        // cout << "Basetail " << Basehead << endl;
        Basehead.nextnode = &Basetail;
        maxheight = _maxheight;
        head.push_back((IndexNode<KEYTYPE> *)&Basehead);
        tail.push_back((IndexNode<KEYTYPE> *)&Basetail);
        for(int i = 0;i < _maxheight;i++){
            head.push_back(new IndexNode<KEYTYPE>(KEYTYPE(),i + 1,HEAD));     
            tail.push_back(new IndexNode<KEYTYPE>(KEYTYPE(),i + 1,TAIL));
            head[i + 1]->nextnode = tail[i + 1];
            head[i + 1]->setfullylinked();
            tail[i + 1]->setfullylinked();
            // set nextlevel and nextnode
            head[i + 1]->nextlevel = (IndexNode<KEYTYPE> *)head[i];
            tail[i + 1]->nextlevel = (IndexNode<KEYTYPE> *)tail[i];
            // if(i){
            //     head[i + 1]->nextlevel = (IndexNode<KEYTYPE>*)head[i-1];
            //     tail[i]->nextlevel = (IndexNode<KEYTYPE>*)tail[i-1];
            // }
            // else{// next level is Basehead and Base tail
            //     head[i]->nextlevel = ((IndexNode<KEYTYPE>*)&Basehead);
            //     tail[i]->nextlevel = ((IndexNode<KEYTYPE>*)&Basetail);
            // }
        }
        
    }
    std::optional<VALUETYPE> search(KEYTYPE _key){
        // mtx.lock();
        // auto start = clock();
        // auto result = find(_key);
        // if(clock() - start > 2000)
            // cout << "search cost for an element " << clock() - start  << " max range " << 2000 << endl;
        // timecost.push_back(clock() - start);
        // std::cout << " append timecost " << timecost.size() << "\n";
        // mtx.unlock();
        // mtx.lock();
        auto result = this->find(_key);
        // mtx.unlock();
        return result;
    }
    std::optional<VALUETYPE> find(KEYTYPE _key){
        // mtx.lock();
        // IndexNode<KEYTYPE> * ptr = 
        IndexNode<KEYTYPE>* start = head[maxheight];
        IndexNode<KEYTYPE>* end = tail[maxheight];
        for(int i = maxheight - 1;i >= 0;i--){
            #ifdef LAYERLOCK
            mtxarray[i + 1].lock();
            #endif
            IndexNode<KEYTYPE>* _head = start;
            
            while (1)
            {
                if(compare<KEYTYPE>(*_head,_key)  * compare<KEYTYPE>(*(_head->nextnode),_key) == -1){
                    // search in the next level
                    start = _head -> nextlevel;
                    end = _head -> nextnode -> nextlevel;
                    // #ifdef LAYERLOCK
                    // mtxarray[i + 1].unlock();
                    // #endif
                    break;
                }
                else{
                    // _head = _head->nextnode;
                    #ifdef NODEMUTEX
                    while(_head->nextnode->conditioncheck()){
                        // not fully linked or is marked to be deleted
                    }
                    #endif
                    _head = _head->nextnode;
                }
                /* code */
            }
            #ifdef LAYERLOCK
            mtxarray[i + 1].unlock();
            // cout << "unlock\n";
            #endif
        }
        // search until the last layer
        
        Basenode<KEYTYPE,VALUETYPE> * _end = (Basenode<KEYTYPE,VALUETYPE>*)(end);
        Basenode<KEYTYPE,VALUETYPE> * base = (Basenode<KEYTYPE,VALUETYPE> *) (start);
        #ifdef LAYERLOCK
        mtxarray[0].lock();
        #endif 
        while(base->getkey() != _key){
            base = base -> nextnode;
            if(_end->nextnode == base){
                // mtx.unlock();
                #ifdef LAYERLOCK
                mtxarray[0].unlock();
                #endif
                return std::nullopt;
            }
        }
        #ifdef LAYERLOCK
        mtxarray[0].unlock();
        #endif
        // mtx.unlock();
        return base->getvalue();
    }
    void deletekey(KEYTYPE _key){
        #ifdef GLOBALLOCK
        mtx.lock();
        #endif
        // delete a key-value pair based on its kay
        std::optional<VALUETYPE> findresult = this->find(_key);
        if(findresult.has_value()){
            // delete process
            // cout << "delete key " << _key << endl;
            // exit()
            IndexNode<KEYTYPE> * _start = head[maxheight];
            IndexNode<KEYTYPE> * _end = tail[maxheight];
            for(int i = maxheight - 1;i >= 0;i--){
                #ifdef LAYERLOCK
                mtxarray[i + 1].lock();
                #endif
                IndexNode<KEYTYPE> * node = _start;
                while(node != _end){
                    if(compare<KEYTYPE>(*node,_key) * compare<KEYTYPE>(*(node->nextnode),_key)==-1){
                        _start = node->nextlevel;
                        _end = node->nextnode->nextlevel;
                        if(node->nextnode->getkey() == _key){
                            
                            // while(node->nextlevel){
                            IndexNode<KEYTYPE> * freenode = node->nextnode;
                            #ifdef NODEMUTEX
                            freenode->setmarked();
                            node->lock();
                            #endif
                            // IndexNode<KEYTYPE> * freenode = node->nextnode;
                            // freenode->setmarked();

                            node->nextnode = freenode->nextnode;
                            // node = node->nextlevel;
                            delete freenode;
                            #ifdef NODEMUTEX
                            node->unlock();
                            #endif
                        }
                        break;
                    }
                    else{
                        #ifdef NODEMUTEX
                        while(node->nextnode->conditioncheck()){

                        }
                        #endif
                        node = node -> nextnode;
                    }
                }
                #ifdef LAYERLOCK
                mtxarray[i + 1].unlock();
                #endif
            }
            Basenode<KEYTYPE,VALUETYPE> * _basestart = (Basenode<KEYTYPE,VALUETYPE> *)_start;
            Basenode<KEYTYPE,VALUETYPE> * _baseend = (Basenode<KEYTYPE,VALUETYPE> *)_end;
            #ifdef LAYERLOCK
            mtxarray[0].lock();
            #endif
            Basenode<KEYTYPE,VALUETYPE> * node = _basestart;
            while(node != _baseend){
                if(node->nextnode->getkey() == _key){
                    
                    node->lock();
                    Basenode<KEYTYPE,VALUETYPE> * tmp = node->nextnode;
                    node->nextnode = tmp->nextnode;
                    tmp->setmarked();
                    delete tmp;
                    node->unlock();
                    break;
                }
                node = node->nextnode;
            }
            #ifdef LAYERLOCK
            mtxarray[0].unlock();
            #endif
        }
        else{
            #ifdef GLOBALLOCK
            mtx.unlock();
            #endif
            return ;
        }
        #ifdef GLOBALLOCK
        mtx.unlock();
        #endif
        return ;
    }
    void insert(KEYTYPE _key,VALUETYPE _value){
        #ifdef GLOBALLOCK
        mtx.lock();
        #endif
        int _level = getlevel(maxheight,rand());
        // cout << "Index level " << _level << endl;
        std::optional<VALUETYPE> findresult = this->find(_key);
        // std::cout << "find result " << findresult.has_value() << endl;
        if(findresult.has_value()){
            #ifdef GLOBALLOCK
            mtx.unlock();
            #endif
            return ;
        }
        else{
            IndexNode<KEYTYPE>* start = head[maxheight];
            IndexNode<KEYTYPE>* end = tail[maxheight];
            IndexNode<KEYTYPE>* tmp = NULL;
            for(int i = maxheight - 1;i >= 0;i--){
                IndexNode<KEYTYPE>* _head = start;
                #ifdef LAYERLOCK
                mtxarray[i + 1].lock();
                // cout << "lock for layer " << i + 1 << endl;
                #endif
                while (1)
                {
                    if(compare<KEYTYPE>(*_head,_key) * compare<KEYTYPE>(*(_head->nextnode),_key) == -1){
                        // search in the next level
                        start = _head -> nextlevel;
                        end = _head -> nextnode -> nextlevel;
                        if(i < _level){
                            
                            IndexNode<KEYTYPE> * Index = new IndexNode<KEYTYPE>(_key,i + 1);
                            // _head->nextnode = Index;
                            _head->lock();
                            Index->nextnode = _head->nextnode;
                            _head->nextnode = Index;
                            Index->setfullylinked();
                            if(tmp != NULL){
                                tmp->lock();
                                tmp->nextlevel = Index;
                                tmp->unlock();
                            }
                            _head->unlock();
                            tmp = Index;
                            
                        }
                        break;
                    }
                    else{
                        // _head = _head->nextnode;
                        while(_head->nextnode->conditioncheck()){

                        }
                        _head = _head->nextnode;
                    }
                    /* code */
                }
                #ifdef LAYERLOCK
                mtxarray[i + 1].unlock();
                #endif
            }
            // search in the last layer
            Basenode<KEYTYPE,VALUETYPE> * _start = (Basenode<KEYTYPE,VALUETYPE> *)(start);
            Basenode<KEYTYPE,VALUETYPE> * _end = (Basenode<KEYTYPE,VALUETYPE> *)(end);
            while(1){
                while(_start->nextnode->conditioncheck()){

                }
                Basenode<KEYTYPE,VALUETYPE> * nextnode = _start -> nextnode;
                if(compare(*_start,_key) * compare(*nextnode,_key) == -1){
                    #ifdef LAYERLOCK
                    mtxarray[0].lock();
                    #endif
                    _start->lock();
                    Basenode<KEYTYPE,VALUETYPE> * id = new Basenode<KEYTYPE,VALUETYPE>(_key,_value,ELSE);
                    // _start -> nextnode = id;
                    // id -> nextnode = nextnode;
                    id->nextnode = _start->nextnode;
                    _start->nextnode = id;
                    id->setfullylinked();
                    if(tmp){
                        tmp->lock();
                        tmp -> nextlevel = (IndexNode<KEYTYPE> *)(id);
                        tmp->unlock();
                    }
                    _start->unlock(); 
                    #ifdef GLOBALLOCK  
                    mtx.unlock();
                    #endif
                    #ifdef LAYERLOCK
                    mtxarray[0].unlock();
                    #endif
                    return ;
                }
                else{
                    while(_start->nextnode->conditioncheck()){

                    }
                    _start = _start->nextnode;
                }
                // _start = nextnode;
            }
        }
    }
    
};
#endif