#include "mem.hpp"

HeapHandler gHeapHandler;

HeapHandler::HeapSlot* HeapHandler::FindSlot(size_t size) {
    HeapSlot* pSlot = (HeapSlot*)this->mHeapLo;
    HeapSlot* pPrev = NULL;
    HeapSlot* pNext = NULL;

    while (pSlot < this->mHeapHi) {
        pNext = (HeapSlot*)((u8*)pSlot + size);

        if (pSlot->IsFree()) {
            pNext->SetFree();
            pSlot->SetUsed();
            pSlot->size = size;
            pSlot->prev = pPrev;
            pSlot->next = pNext;
            break;
        }

        pPrev = pSlot;
        pSlot = pNext;
    }

    return pSlot;
}

void* HeapHandler::Alloc(size_t size) {
    HeapSlot* pSlot = this->FindSlot(size);

    if (pSlot >= this->mHeapHi) {
        return NULL;
    }

    return pSlot->GetStart();
}

void HeapHandler::Free(void* ptr) {
    if (ptr != NULL) {
        HeapSlot* pSlot = (HeapSlot*)((u8*)ptr - sizeof(HeapSlot));
        pSlot->Reset();
    }
}
