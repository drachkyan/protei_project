//
// Created by vova on 01.03.2026.
//

#ifndef INCLUDE_DATAPOOL_H
#define INCLUDE_DATAPOOL_H
#include "../include/vector.hpp"

struct PoolItem {

    void* vector;
    PoolItem* next;
    Types type;
    PoolItem(void*vector_, Types type_) {
        next = nullptr;
        vector = vector_;
        type = type_;
    }
    ~PoolItem() = default;
};

class DataPool {
    PoolItem* start;
    PoolItem* end;
public:
    DataPool();
    ~DataPool();
    void insert(void* vector, Types type);
    std::pair<void *, Types> first();
};


#endif  // INCLUDE_DATAPOOL_H