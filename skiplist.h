#ifndef SKIPLIST
#define SKIPLIST
#include "node.h"

#include "config.h"

template<typename KEYTYPE,typename VALUETYPE>
class SkipList{
private:
    std::vector<IndexNode<KEYTYPE>*> head;
    std::vector<IndexNode<KEYTYPE>*> tail;
    Basenode<KEYTYPE,VALUETYPE> Basehead{};
    Basenode<KEYTYPE,VALUETYPE> Basetail{};
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
    SkipList(int _maxheight = MAXHEIGHT){
        
        Basehead = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),HEAD);
        Basetail = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),TAIL);
        // cout << "Basetail " << Basehead << endl;
        Basehead.nextnode = &Basetail;
        maxheight = _maxheight;
        for(int i = 0;i < _maxheight;i++){
            head.push_back(new IndexNode<KEYTYPE>(KEYTYPE(),i + 1,HEAD));     
            tail.push_back(new IndexNode<KEYTYPE>(KEYTYPE(),i + 1,TAIL));
            head[i]->nextnode = tail[i];
            if(i){
                head[i]->nextlevel = (IndexNode<KEYTYPE>*)head[i-1];
                tail[i]->nextlevel = (IndexNode<KEYTYPE>*)tail[i-1];
            }
            else{// next level is Basehead and Base tail
                head[i]->nextlevel = ((IndexNode<KEYTYPE>*)&Basehead);
                tail[i]->nextlevel = ((IndexNode<KEYTYPE>*)&Basetail);
            }
        }
    }
    std::optional<VALUETYPE> search(KEYTYPE _key){
        return find(_key);
    }
    std::optional<VALUETYPE> find(KEYTYPE _key){
        // IndexNode<KEYTYPE> * ptr = 
        IndexNode<KEYTYPE>* start = head[maxheight - 1];
        IndexNode<KEYTYPE>* end = tail[maxheight - 1];
        for(int i = maxheight - 1;i >= 0;i--){
            IndexNode<KEYTYPE>* _head = start;
            while (1)
            {
                if(compare<KEYTYPE>(*_head,_key)  * compare<KEYTYPE>(*(_head->nextnode),_key) == -1){
                    // search in the next level
                    start = _head -> nextlevel;
                    end = _head -> nextnode -> nextlevel;
                    break;
                }
                else{
                    // _head = _head->nextnode;
                    _head = _head->nextnode;
                }
                /* code */
            }
        }
        // search until the last layer
        Basenode<KEYTYPE,VALUETYPE> * _end = (Basenode<KEYTYPE,VALUETYPE>*)(end);
        Basenode<KEYTYPE,VALUETYPE> * base = (Basenode<KEYTYPE,VALUETYPE> *) (start);
        while(base->getkey() != _key){
            base = base -> nextnode;
            if(base == _end){
                return std::nullopt;
            }
        }
        return base->getvalue();
    }
    void deletekey(KEYTYPE _key){
        // delete a key-value pair based on its kay
        std::optional<VALUETYPE> findresult = this->find(_key);
        if(findresult.has_value()){
            // delete process
            IndexNode<KEYTYPE> * _start = head[maxheight - 1];
            IndexNode<KEYTYPE> * _end = tail[maxheight - 1];
            for(int i = maxheight - 1;i >= 0;i--){
                IndexNode<KEYTYPE> * node = _start;
                while(node != _end){
                    if(compare<KEYTYPE>(*node,_key) * compare<KEYTYPE>(*(node->nextnode),_key)==-1){
                        _start = node->nextlevel;
                        _end = node->nextnode->nextlevel;
                        if(node->nextnode->getkey() == _key){
                            // while(node->nextlevel){
                            IndexNode<KEYTYPE> * freenode = node->nextnode;
                            node->nextnode = freenode->nextnode;
                            // node = node->nextlevel;
                            delete freenode;
                        }
                        break;
                    }
                    else{
                        node = node -> nextnode;
                    }
                }
            }
            Basenode<KEYTYPE,VALUETYPE> * _basestart = (Basenode<KEYTYPE,VALUETYPE> *)_start;
            Basenode<KEYTYPE,VALUETYPE> * _baseend = (Basenode<KEYTYPE,VALUETYPE> *)_end;
            auto node = _basestart;
            while(node != _baseend){
                if(node->nextnode->getkey() == _key){
                    auto tmp = node->nextnode;
                    node->nextnode = tmp->nextnode;
                    delete tmp;
                    break;
                }
                node = node->nextnode;
            }
        }
        else{
            return ;
        }
        return ;
    }
    void insert(KEYTYPE _key,VALUETYPE _value){
        int _level = level(maxheight,rand());
        cout << "Index level " << _level << endl;
        std::optional<VALUETYPE> findresult = this->find(_key);
        if(findresult.has_value()){
            return ;
        }
        else{
            IndexNode<KEYTYPE>* start = head[maxheight - 1];
            IndexNode<KEYTYPE>* end = tail[maxheight - 1];
            IndexNode<KEYTYPE>* tmp = NULL;
            for(int i = maxheight - 1;i >= 0;i--){
                IndexNode<KEYTYPE>* _head = start;
                while (1)
                {
                    if(compare<KEYTYPE>(*_head,_key) * compare<KEYTYPE>(*(_head->nextnode),_key) == -1){
                        // search in the next level
                        start = _head -> nextlevel;
                        end = _head -> nextnode -> nextlevel;
                        if(i < _level){
                            IndexNode<KEYTYPE> * Index = new IndexNode<KEYTYPE>(_key,i + 1);
                            // _head->nextnode = Index;
                            Index->nextnode = _head->nextnode;
                            _head->nextnode = Index;
                            if(tmp != NULL){
                                tmp->nextlevel = Index;
                            }
                            tmp = Index;
                        }
                        break;
                    }
                    else{
                        // _head = _head->nextnode;
                        _head = _head->nextnode;
                    }
                    /* code */
                }
            }
            // search in the last layer
            Basenode<KEYTYPE,VALUETYPE> * _start = (Basenode<KEYTYPE,VALUETYPE> *)(start);
            Basenode<KEYTYPE,VALUETYPE> * _end = (Basenode<KEYTYPE,VALUETYPE> *)(end);
            while(1){
                Basenode<KEYTYPE,VALUETYPE> * nextnode = _start -> nextnode;
                if(compare(*_start,_key) * compare(*nextnode,_key) == -1){
                    Basenode<KEYTYPE,VALUETYPE> * id = new Basenode<KEYTYPE,VALUETYPE>(_key,_value,ELSE);
                    // _start -> nextnode = id;
                    // id -> nextnode = nextnode;
                    id->nextnode = _start->nextnode;
                    _start->nextnode = id;
                    if(tmp)
                        tmp -> nextlevel = (IndexNode<KEYTYPE> *)(id);
                    return ;
                }
                else{
                    _start = _start->nextnode;
                }
                // _start = nextnode;
            }
        }
    }
    
};
#endif