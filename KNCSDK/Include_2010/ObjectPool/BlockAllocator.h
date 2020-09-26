#ifndef __BlockAllocator
#define __BlockAllocator

#include <vector>
#include <windows.h>


template <class T, size_t blocksPerBatch=100, size_t blockAlignment=4> 
class BlockAllocator
{
public:
    void* operator new(size_t)
    {
        return s_Store.AllocateBlock();
    }

    void operator delete(void* pBlock)
    {
        s_Store.ReleaseBlock((T*)pBlock);
    }

    //* These can be called directly, but be warned, the constructors 
    //* and destructors on the blocks will not be called!
    static T*   AllocateBlock()         { return s_Store.AllocateBlock(); }
    static void ReleaseBlock(T* pBlock) { s_Store.ReleaseBlock(pBlock); }

private:
    struct BlockStore
    {
        BlockStore() 
        { 
            ppNextBlock = 0;
            InitializeCriticalSection( &m_cs );
        }
        ~BlockStore()
        {
            //* Check for memory leaks...

            // Must clean up these pointers
            size_t iNum = batches.size();
            for (size_t i=0; i<iNum; ++i)
            {
                unsigned char* p = (unsigned char*)batches[i];
                delete [] p;
            }
            
            DeleteCriticalSection( &m_cs );
        }
        void Lock() { EnterCriticalSection( &m_cs ); }
        void Unlock() { LeaveCriticalSection( &m_cs ); }

        T* AllocateBlock()
        {   
            Lock();
            
            //* Is there any room?
            if (!ppNextBlock || !*ppNextBlock)
            {
                // determine the allligned size of the blocks
                static const size_t blockSize = (sizeof(T)+blockAlignment-1)&(~(blockAlignment-1));

                // make a new batch 
                unsigned char *pNewBatch = new unsigned char[blocksPerBatch*blockSize+15];
                batches.push_back(pNewBatch);

                //* Align the block on a 16-byte boundary
                unsigned char* pAlignedPtr =(unsigned char*)((unsigned int)(pNewBatch+15)&(~15));

                // fill the pointers with the new blocks
                ppNextBlock = (unsigned char**)pAlignedPtr;
                for (int i=0; i<blocksPerBatch-1; ++i)
                {
                    *((unsigned int*)(pAlignedPtr + i*blockSize)) = (unsigned int)(pAlignedPtr + (i+1)*blockSize);
                }
                *((unsigned int*)(pAlignedPtr + (blocksPerBatch-1)*blockSize)) = (unsigned int)0;
            }

            unsigned char* pBlock = (unsigned char*)ppNextBlock;
            ppNextBlock = (unsigned char**)*ppNextBlock;
            
            Unlock();

            return (T*)pBlock;
        }

        void ReleaseBlock(T* pBlock)
        {
            Lock();

            if(pBlock)
            {
                *((unsigned int*)pBlock) = (unsigned int)ppNextBlock;
                ppNextBlock = (unsigned char**)((unsigned char*)pBlock);
            }
            
            Unlock();
        }
        
        typedef std::vector<unsigned char*> BatchPtrVector;

        unsigned char**     ppNextBlock;        // Pointer to the next available block pointer
        BatchPtrVector      batches;            // Array of pointers to batches
        CRITICAL_SECTION    m_cs;
    };

    static BlockStore           s_Store;
};

template<class T, size_t blocksPerBatch, size_t blockAlignment> 
typename BlockAllocator<T, blocksPerBatch, blockAlignment>::BlockStore BlockAllocator<T, blocksPerBatch, blockAlignment>::s_Store;

#endif

