//
// Created by vova on 01.03.2026.
//

#include "../include/DataPool.h"

#include "spdlog/spdlog.h"

DataPool::DataPool() {
    start=nullptr;
    end=nullptr;
}

DataPool::~DataPool() {
    PoolItem* current = start;
    while (current != nullptr) {
        PoolItem* next = current->next;
        dispatch(current->type, current->vector, [](auto* v) {
            delete v;
        });
        delete current;
        current = next;
    }
    start = nullptr;
    end = nullptr;
}

void DataPool::insert(void *vector, Types type) {
    if (!end) {
        this->start = new PoolItem(vector, type);
        this->end = start;
        return;
    }
    this->end->next=new PoolItem(vector, type);
    this->end=this->end->next;
}

std::pair<void *, Types> DataPool::first() {
    if (!start) {
        spdlog::info("Нет векторов");
        return std::pair{nullptr, Types::None};
    }

    void* vector = start->vector;
    Types type = start->type;
    const PoolItem* tempPtr = start;
    start=start->next;
    delete tempPtr;
    std::pair<void *, Types> result(vector, type);
    return result;
}
