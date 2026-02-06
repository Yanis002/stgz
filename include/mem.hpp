#pragma once

#include <stddef.h>
#include <mem.h>
#include <types.h>

#include <Unknown/UnkMemFuncs.h>

struct HeapHandler {
    const int sHeapSize = 0x3F00;
    const void* sHeapLo = (void*)0x021A6480;
    const void* sHeapHi = (void*)((u8*)sHeapLo + sHeapSize);

    struct HeapSlot {
        u8 state;
        size_t size;
        HeapSlot* prev;
        HeapSlot* next;

        HeapSlot() {
            this->Reset();
        }

        void* GetStart() {
            return (void*)((u8*)this + sizeof(HeapSlot));
        }

        void SetFree() {
            this->state = 'F';
        }

        void SetUsed() {
            this->state = 'U';
        }

        bool IsFree() {
            return this->state == 'F';
        }

        void Reset() {
            this->SetFree();
            this->size = 0;
            this->prev = nullptr;
            this->next = nullptr;
        }
    };

    HeapHandler() {
        ((HeapSlot*)sHeapLo)->SetFree();
    }

    HeapSlot* FindSlot(size_t size);
    void* Alloc(size_t size);
    void Free(void* ptr);
    size_t GetHeapSize();
};

extern HeapHandler gHeapHandler;

inline void* operator new(size_t size) {
    if (size > gHeapHandler.GetHeapSize()) {
        return NULL;
    }

    return gHeapHandler.Alloc(size);
}

inline void operator delete(void* ptr) {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}

inline void operator delete(void* ptr, unsigned int) noexcept {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}

