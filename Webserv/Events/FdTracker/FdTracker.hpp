

#ifndef FDTRACKER_HPP

# define FDTRACKER_HPP

# include "../../Ws_Namespace.h"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"

class FdTracker
{
	public:
		FdTracker(const size_t maxFdsEstimate);
		~FdTracker();
		FdTracker(const FdTracker& copy);
		FdTracker& operator=(const FdTracker& assign);

		void	markFdStale(Ws::fd fd);
		int		isFdStale(Ws::fd fd);
		void	clear();

	private:
		HeapArray<Ws::byte>	m_staleEvents;
		int					m_maxStaleFd;

		void                mf_expandTracker(size_t newFdMax);
};


#endif