//
//  test.cpp
//  ConcurrentHashMap
//
//  Created by Yunsheng Bai on 7/18/16.
//  Copyright © 2016 Yunsheng Bai. All rights reserved.
//

#include "ConcurrentHashMap.h"
#include "Semaphore.h"
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>
#include <iostream>
using namespace std;

class Creator {
public:
    unique_ptr<Semaphore> operator()() const {
        return unique_ptr<Semaphore>(new Semaphore);
    }
};

int main() {
    using Pointer = unique_ptr<const Semaphore>;
    ConcurrentHashMap<int, Pointer, Creator> cm;
    const Pointer sema_ptr0 = move(cm.getWithCreate(7, Creator()));
    const Pointer sema_ptr1 = cm.remove(7);
    try {
        cm.get(7);
    } catch (const exception &e) {
        cout << e.what() << endl;
    }
    Pointer sema_ptr2(new Semaphore);
    cm.put(7, move(sema_ptr2));
    Pointer sema_ptr3 = move(cm.get(7));
    return 0;
}
