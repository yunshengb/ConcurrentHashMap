//
//  ConcurrentHashMap.h
//  ConcurrentHashMap
//
//  Created by Yunsheng Bai on 7/18/16.
//  Copyright © 2016 Yunsheng Bai. All rights reserved.
//

#ifndef ConcurrentHashMap_h
#define ConcurrentHashMap_h

#include <mutex>
#include <unordered_map>
#include <utility>

// A thread-safe unordered map.
template <typename K, typename V, typename C>
class ConcurrentHashMap {
public:
    ConcurrentHashMap();
    ~ConcurrentHashMap();
    void put(const K &key, V &value);
    V& getWithCreate(const K &key, const C &creator);
    V& get(const K &key);
    const V& get(const K &key) const;
    V getAndRemove(const K &key);
    ConcurrentHashMap(const ConcurrentHashMap &cm) = delete;
    ConcurrentHashMap(const ConcurrentHashMap &&cm) = delete;
    ConcurrentHashMap &operator=(const ConcurrentHashMap &cm) = delete;
    ConcurrentHashMap &operator=(const ConcurrentHashMap &&cm) = delete;
private:
    std::mutex mutex_;
    std::unordered_map<K, V> map_;
};

template <typename K, typename V, typename C>
ConcurrentHashMap<K, V, C>::ConcurrentHashMap() {}

template <typename K, typename V, typename C>
ConcurrentHashMap<K, V, C>::~ConcurrentHashMap() {}

template <typename K, typename V, typename C>
void ConcurrentHashMap<K, V, C>::put(const K &key, V &value) {
    std::lock_guard<std::mutex> guard(mutex_);
    map_.insert(std::make_pair(key, std::move(value)));
}

template <typename K, typename V, typename C>
V& ConcurrentHashMap<K, V, C>::getWithCreate(const K &key, const C &creator) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (map_.find(key) != map_.end()) {
        return map_[key];
    }
    map_.insert(std::make_pair(key, creator()));
    return map_[key];
}

template <typename K, typename V, typename C>
V& ConcurrentHashMap<K, V, C>::get(const K &key) {
    std::lock_guard<std::mutex> guard(mutex_);
    return map_.at(key);
}

template <typename K, typename V, typename C>
const V& ConcurrentHashMap<K, V, C>::get(const K &key) const {
    std::lock_guard<std::mutex> guard(mutex_);
    return map_.at(key);
}

template <typename K, typename V, typename C>
V ConcurrentHashMap<K, V, C>::getAndRemove(const K &key) {
    std::lock_guard<std::mutex> guard(mutex_);
    V save = std::move(map_.at(key));
    map_.erase(key);
    return save;
}

#endif /* ConcurrentHashMap_h */
