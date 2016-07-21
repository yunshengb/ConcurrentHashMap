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
    ConcurrentHashMap<int, unique_ptr<Semaphore>, Creator> cm;
    unique_ptr<Semaphore> sema_ptr0 = move(cm.getWithCreate(7, Creator()));
    unique_ptr<Semaphore> sema_ptr1 = cm.getAndRemove(7);
    try {
        cm.get(7);
    } catch (const exception &e) {
        cout << e.what() << endl;
    }
    return 0;
}
