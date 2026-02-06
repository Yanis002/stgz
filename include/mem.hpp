#pragma once

#include <stddef.h>
#include <mem.h>
#include <types.h>

#include <Unknown/UnkMemFuncs.h>

extern "C" u8 _heap_start[];
extern "C" u8 _overlay_end[];

struct HeapHandler {
    u32 magic;
    void* mHeapLo;
    void* mHeapHi;
    size_t mHeapSize;

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
        this->magic = 'HZGY';
        this->mHeapLo = (void*)((u8*)_heap_start) + sizeof(HeapHandler);
        this->mHeapHi = (void*)_overlay_end;
        this->mHeapSize = _overlay_end - _heap_start;
        ((HeapSlot*)this->mHeapLo)->SetFree();
    }

    size_t GetHeapSize() {
        return this->mHeapSize;
    }

    HeapSlot* FindSlot(size_t size);
    void* Alloc(size_t size);
    void Free(void* ptr);
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

