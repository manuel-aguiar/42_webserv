

#include "SlabImpl.hpp"


// Implementation
SlabImpl::SlabImpl(t_slot_pointer array, size_t nodeSize, size_t capacity)
    : m_begin(array), m_nodeSize(nodeSize), m_elemCount(0), m_capacity(capacity), m_freeSlot(NULL) {}

SlabImpl::~SlabImpl() {}

SlabImpl::SlabImpl(const SlabImpl& memoryPool) {(void)memoryPool;}
SlabImpl& SlabImpl::operator=(const SlabImpl& assign) {(void)assign; return(*this);}

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