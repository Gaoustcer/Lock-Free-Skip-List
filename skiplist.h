#include "node.h"
#include "config.h"

template<typename KEYTYPE,typename VALUETYPE>
class SkipList{
private:
    std::vector<IndexNode<KEYTYPE>> head;
    std::vector<IndexNode<KEYTYPE>> tail;
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
        Basehead.nextnode = &Basetail;
        maxheight = _maxheight;
        for(int i = 0;i < MAXHEIGHT;i++){
            head.push_back(IndexNode<KEYTYPE>(KEYTYPE(),i + 1,HEAD));     
            tail.push_back(IndexNode<KEYTYPE>(KEYTYPE(),i + 1,TAIL));
            head[i].nextnode = &tail[i];
            if(i){
                head[i].insertnextlevel((IndexNode<KEYTYPE>*)&head[i-1]);
                tail[i].insertnextlevel((IndexNode<KEYTYPE>*)&tail[i-1]);
            }
            else{// next level is Basehead and Base tail
                head[i].insertnextlevel((IndexNode<KEYTYPE>*)&Basehead);
                tail[i].insertnextlevel((IndexNode<KEYTYPE>*)&Basetail);
            }
        }
    }

    std::optional<VALUETYPE> find(KEYTYPE _key){
        // IndexNode<KEYTYPE> * ptr = 
        IndexNode<KEYTYPE>* start = &head[maxheight - 1];
        IndexNode<KEYTYPE>* end = &tail[maxheight - 1];
        for(int i = maxheight - 1;i >= 0;i--){
            IndexNode<KEYTYPE>* _head = start;
            while (_head != end)
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
    void insert(KEYTYPE _key,VALUETYPE _value){
        int _level = level(rand());
        
        std::optional<VALUETYPE> findresult = this->find(_key);
        if(findresult.has_value()){
            return ;
        }
        else{
            IndexNode<KEYTYPE>* start = &head[maxheight - 1];
            IndexNode<KEYTYPE>* end = &tail[maxheight - 1];
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
                            IndexNode<KEYTYPE> * Index = new IndexNode<KEYTYPE>(_key,i);
                            start->nextnode = Index;
                            Index->nextnode = end;
                            if(tmp != NULL){
                                tmp->insertnextlevel(Index);
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
                auto nextnode = _start -> nextnode;
                if(compare(*_start,_key) * compare(*nextnode,_key) == -1){
                    Basenode<KEYTYPE,VALUETYPE> * id = new Basenode<KEYTYPE,VALUETYPE>(_key,_value,ELSE);
                    _start -> nextnode = id;
                    id -> nextnode = nextnode;
                    return ;
                }
                _start = nextnode;
            }
        }
    }
    
};