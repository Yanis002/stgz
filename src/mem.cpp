#include "mem.hpp"

HeapHandler gHeapHandler;

HeapHandler::HeapSlot* HeapHandler::FindSlot(size_t size) {
    HeapSlot* pSlot = (HeapSlot*)this->mHeapLo;
    HeapSlot* pPrev = NULL;
    HeapSlot* pNext = NULL;

    while (pSlot < this->mHeapHi) {
        pNext = (HeapSlot*)((u8*)pSlot->GetStart() + size);

        // consider the block available if:
        // - the current slot is free to use
        // - the current slot's size is unset or the size of whatever was
        //   allocated before doesn't exceed what we're trying to allocate
        if (pSlot->IsFree() && (pSlot->size == 0 || size <= pSlot->size)) {
            // mark the next slot as free if we reach never-allocated space
            if (pSlot->next == nullptr) {
                pNext->SetFree();
            }

            // update the slot's informations
            pSlot->SetUsed();
            pSlot->size = size;
            pSlot->prev = pPrev;
            pSlot->next = pNext;

            // clear garbage data
            pSlot->Clear();
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
        pSlot->SetFree();
    }
}
