

# include "FdTracker.hpp"
# include <dirent.h>
# include <cstdlib>

FdTracker::FdTracker(const size_t maxFdsEstimate) :
	m_maxStaleFd(0)
{
	mf_expandTracker(1000 + maxFdsEstimate);
}

FdTracker::~FdTracker()
{
}

FdTracker::FdTracker(const FdTracker& copy) :
	m_staleEvents(copy.m_staleEvents),
	m_maxStaleFd(copy.m_maxStaleFd)
{
}

FdTracker& 
FdTracker::operator=(const FdTracker& assign)
{
	if (this == &assign)
		return (*this);

	m_staleEvents = assign.m_staleEvents;
	m_maxStaleFd = assign.m_maxStaleFd;

	return (*this);
}

void
FdTracker::markFdStale(Ws::fd fd)
{
	if (fd == Ws::FD_NONE)
		return ;

	m_maxStaleFd = (fd > m_maxStaleFd) ? fd : m_maxStaleFd;
	if ((size_t)m_maxStaleFd >= m_staleEvents.size() * 8)
		mf_expandTracker(m_maxStaleFd * 2);
	size_t index = fd / 8;
	size_t bit = fd % 8;
	m_staleEvents[index] |= (1 << bit);
}

int
FdTracker::isFdStale(Ws::fd fd)
{
	if (fd == Ws::FD_NONE)
		return (1);
	if ((size_t)fd >= m_staleEvents.size() * 8)
		mf_expandTracker(fd * 2);
	size_t index = fd / 8;
	size_t bit = fd % 8;
	return ((m_staleEvents[index] & (1 << bit)) != 0);
}

void
FdTracker::mf_expandTracker(size_t newFdMax)
{
	HeapArray<Ws::byte> staleEvents(newFdMax / 8 + 1, 0);
	std::memcpy(staleEvents.getArray(), m_staleEvents.getArray(), sizeof(m_staleEvents[0]) * m_staleEvents.size());
	std::memset(staleEvents.getArray() + m_staleEvents.size(), 0, sizeof(staleEvents[0]) * (staleEvents.size() - m_staleEvents.size()));
	m_staleEvents.swap(staleEvents);
}


void
FdTracker::clear()
{
	std::memset(m_staleEvents.getArray(), 0, sizeof(m_staleEvents[0]) * (((m_maxStaleFd / 8)) + 1));
	m_maxStaleFd = 0;
}