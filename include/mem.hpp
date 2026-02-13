#pragma once

#include <mem.h>
#include <stddef.h>
#include <types.h>

#include <Unknown/UnkMemFuncs.h>

extern "C" u8 _heap_start[];
extern "C" u8 _overlay_end[];

#define FREE 'EERF'
#define USED 'DESU'

struct HeapHandler {
    u32 magic;
    void* mHeapLo;
    void* mHeapHi;
    size_t mHeapSize;

    struct HeapSlot {
        u32 state;
        size_t size;
        HeapSlot* prev;
        HeapSlot* next;

        HeapSlot() { this->Reset(); }

        void* GetStart() { return (void*)((u8*)this + sizeof(HeapSlot)); }

        void SetFree() { this->state = FREE; }

        void SetUsed() { this->state = USED; }

        bool IsFree() { return this->state == FREE; }

        void Clear() { memset(this->GetStart(), 0, this->size); }

        void Reset() {
            this->SetFree();
            this->size = 0;
            this->prev = nullptr;
            this->next = nullptr;
        }
    };

    HeapHandler() {
        this->magic = 'HZGY';
        this->mHeapLo = (void*)((u8*)_heap_start + sizeof(HeapHandler));
        this->mHeapHi = (void*)_overlay_end;
        this->mHeapSize = _overlay_end - _heap_start;
        ((HeapSlot*)this->mHeapLo)->SetFree();
    }

    size_t GetHeapSize() { return this->mHeapSize; }

    HeapSlot* FindSlot(size_t size);
    void* Alloc(size_t size);
    void Free(void* ptr);
};

extern HeapHandler gHeapHandler;

inline void* operator new(size_t size) {
    // abort if the requested size doesn't fit
    if (size > gHeapHandler.GetHeapSize() || size == 0) {
        size = 1;
    }

    // do the allocation with the requested size aligned
    return gHeapHandler.Alloc((size + (4 - 1)) & ~(4 - 1));
}

inline void* operator new[](size_t size) {
    if (size > gHeapHandler.GetHeapSize() || size == 0) {
        size = 1;
    }

    return gHeapHandler.Alloc((size + (4 - 1)) & ~(4 - 1));
}

inline void operator delete(void* ptr) {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}

inline void operator delete[](void* ptr) {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}

inline void operator delete(void* ptr, unsigned int) {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}

inline void operator delete[](void* ptr, unsigned int) {
    if (ptr != nullptr) {
        gHeapHandler.Free(ptr);
    }
}
