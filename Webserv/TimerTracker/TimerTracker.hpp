

#ifndef TIMERTRACKER_HPP

# define TIMERTRACKER_HPP


# include <memory>

template <typename T, typename U, typename Allocator = std::allocator<std::pair<const T, U> > >
class TimerTracker;

# include "TimerTracker.tpp"

#endif
