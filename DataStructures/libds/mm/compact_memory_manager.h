#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

namespace ds::mm {

    template<typename BlockType>
    class CompactMemoryManager : public MemoryManager<BlockType> {
    public:
        CompactMemoryManager();
        CompactMemoryManager(size_t size);
        CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
        ~CompactMemoryManager() override;

        BlockType* allocateMemory() override;
        BlockType* allocateMemoryAt(size_t index);
        void releaseMemory(BlockType* pointer) override;
        void releaseMemoryAt(size_t index);
        void releaseMemory();

        size_t getCapacity() const;

        CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
        void changeCapacity(size_t newCapacity);
        void shrinkMemory();
        void clear();
        bool equals(const CompactMemoryManager<BlockType>& other) const;
        void* calculateAddress(const BlockType& data);
        size_t calculateIndex(const BlockType& data);
        BlockType& getBlockAt(size_t index);
        void swap(size_t index1, size_t index2);

        void print(std::ostream& os);

    private:
        size_t getAllocatedBlocksSize() const;
        size_t getAllocatedCapacitySize() const;

    private:
        BlockType* base_;
        BlockType* end_;
        BlockType* limit_;

        static const size_t INIT_SIZE = 4;
    };

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
        CompactMemoryManager(INIT_SIZE)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
        base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
        end_(base_),
        limit_(base_ + size)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other) :
        CompactMemoryManager(other.getAllocatedBlockCount())
    {
        // TODO 02
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
    {
        // TODO 02
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
    {
        return this->allocateMemoryAt(this->end_ - this->base_);
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
    {
        if (this->end_ == this->limit_) {
            this->changeCapacity(this->getCapacity() > 0 ? 2 * this->getCapacity() : this->INIT_SIZE);
        }
        BlockType* adr = this->base_ + index;
        if (index != this->allocatedBlockCount_) {
            std::memmove(adr + 1, adr, (this->end_ - adr) * sizeof(BlockType)); //memmove nepos�va, ale kop�ruje
        }
        placement_new(adr);
        ++this->end_;
        ++this->allocatedBlockCount_;
        return adr;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
    {
        BlockType* cur = pointer;
        while (cur != this->end_) {
            destroy(cur);
            ++cur;
        }
        this->end_ = pointer;
        this->allocatedBlockCount_ = this->end_ - this->base_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
    {
        BlockType* adr = this->base_ + index;
        destroy(adr);
        std::memmove(adr, adr + 1, (this->end_ - adr - 1) * sizeof(BlockType));
        --this->end_;
        --this->allocatedBlockCount_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
    {
        this->releaseMemoryAt(this->end_ - this->base_ - 1);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
    {
        return this->limit_ - this->base_;
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign
    (const CompactMemoryManager<BlockType>& other)
    {
        if (this != &other) {
            releaseMemory(this->base_);
            this->allocatedBlockCount_ = other.getAllocatedBlockCount();
            void* newBase = std::realloc(this->base_, other.getAllocatedCapacitySize());
            if (newBase == nullptr) {
				throw std::bad_alloc();
			}
            this->base_ = static_cast<BlockType*>(newBase);
            this->end_ = this->base_ + this->allocatedBlockCount_;
            this->limit_ = this->base_ + (other.limit_ - other.base_);
            for (size_t i = 0; i < this->allocatedBlockCount_ - 1; ++i) {
                //BlockType* adr = this->base_ + i;
                //BlockType* adr = this->getBlockAt(other.base_ + i);
                //placement_new(adr);

			}
            return *this;
		}
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
    {
        size_t newCapacity = static_cast<size_t>(end_ - base_);

        if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
        {
            newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
        }

        this->changeCapacity(newCapacity);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
    {
        if (newCapacity == this->getCapacity()) {
			return;
		}
        void* newBase = nullptr;
        if (newCapacity >= this->allocatedBlockCount_) {
            newBase = std::realloc(this->base_, newCapacity * sizeof(BlockType));
        } 
        else {
            this->releaseMemory(this->base_ + newCapacity);
            newBase = std::realloc(this->base_, newCapacity * sizeof(BlockType));
		}
        if (newBase != this->base_) {
            this->base_ = static_cast<BlockType*>(newBase);
            this->limit_ = this->base_ + newCapacity;
            this->end_ = this->base_ + this->allocatedBlockCount_;
        }
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
    {
        this->releaseMemory(this->base_);
    }

    template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
    {
        return
            this == &other ||
            (this->allocatedBlockCount_ == other.allocatedBlockCount_ &&
                std::memcmp(this->base_, other.base_, this->allocatedBlockCount_ * sizeof(BlockType)) == 0);
    }

    template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
    {
        BlockType* cur = this->base_;
        while (cur != this->end_) {
            if (cur == &data) {
                return cur;
            }
            ++cur;
        }
        return nullptr;
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
    {
        if (&data >= this->base_ && &data < this->end_) {
            return &data - this->base_;
        }
        else {
            return INVALID_INDEX;
        }
    }

    template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
    {
        return *(this->base_ + index); //BlockType*
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
    {
        std::swap(this->getBlockAt(index1), this->getBlockAt(index2));
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
    {
        return (this->end_ - this->base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
    {
        return (this->limit_ - this->base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
    {
        os << "first = " << base_ << std::endl;
        os << "last = " << end_ << std::endl;
        os << "limit = " << limit_ << std::endl;
        os << "block size = " << sizeof(BlockType) << "B" << std::endl;

        BlockType* ptr = base_;
        while (ptr != limit_)
        {
            std::cout << ptr;
            os << PtrPrintBin<BlockType>(ptr);

            if (ptr == base_) {
                os << "<- first";
            }
            else if (ptr == end_) {
                os << "<- last";
            }
            os << std::endl;
            ++ptr;
        }

        os << limit_ << "|<- limit" << std::endl;
    }

}