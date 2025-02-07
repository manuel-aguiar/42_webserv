

#include "SlabImpl.hpp"


// Implementation
SlabImpl::SlabImpl(t_slot_pointer array, size_t nodeSize, size_t capacity)
    : m_begin(array), m_nodeSize(nodeSize), m_elemCount(0), m_capacity(capacity), m_freeSlot(NULL) {}

SlabImpl::~SlabImpl() {}

SlabImpl::SlabImpl(const SlabImpl& memoryPool) :
    m_begin(memoryPool.m_begin),
    m_nodeSize(memoryPool.m_nodeSize),
    m_elemCount(memoryPool.m_elemCount),
    m_capacity(memoryPool.m_capacity),
    m_freeSlot(memoryPool.m_freeSlot) {}

SlabImpl& SlabImpl::operator=(const SlabImpl& assign)
{
    if (this == &assign)
        return (*this);

    m_begin = assign.m_begin;
    m_nodeSize = assign.m_nodeSize;
    m_elemCount = assign.m_elemCount;
    m_capacity = assign.m_capacity;
    m_freeSlot = assign.m_freeSlot;
    
    return (*this);
}

void
SlabImpl::reset(t_slot_pointer array, size_t nodeSize, size_t capacity)
{
    m_begin = array;
    m_nodeSize = nodeSize;
    m_elemCount = 0;
    m_capacity = capacity;
    m_freeSlot = NULL;
}

size_t SlabImpl::size() const { return m_elemCount; }
size_t SlabImpl::capacity() const { return m_capacity; }
size_t SlabImpl::getElemCount() const { return m_elemCount; }