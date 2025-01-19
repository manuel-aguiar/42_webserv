
# include "Event.hpp"
# include <cassert>
# include <cstddef>

Event::Event() :
	m_function(NULL),
	m_data(NULL)
{}

Event::~Event() {}

void    Event::setFlags(int flags)
{
	m_flags = flags;
}

void Event::setHandlerFunction_and_Data(HandlerFunction handler, HandlerData data)
{
	m_function = handler;
	m_data = data;
}

void Event::handle()
{
	assert(this->m_function != NULL && this->m_data != NULL);

	(this->m_function)(*this);
}

//private
Event::Event(const Event& copy)  {(void)copy;}
Event& Event::operator=(const Event& assign) {(void)assign; return (*this);}
