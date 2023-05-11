#include "node.h"
#include <optional>
template<typename KEYTYPE,typename VALUETYPE>
class Linklist{
public:
    Basenode<KEYTYPE,VALUETYPE> head{};
    Basenode<KEYTYPE,VALUETYPE> tail{};
    Linklist(){
        head = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),HEAD);
        tail = Basenode<KEYTYPE,VALUETYPE>(KEYTYPE(),VALUETYPE(),TAIL);
    }
    // void find()
};