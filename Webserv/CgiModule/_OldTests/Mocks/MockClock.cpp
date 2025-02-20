
#include "../../../Globals/Globals.hpp"
#include "../../../Globals/Clock/Clock.hpp"
#include "../../../Globals/LogFile/LogFile.hpp"


Clock::Clock(Globals* globals) : m_globals(globals), m_elapsed_ms(0)
{
	(void)globals;
}

Clock::~Clock()
{

}

void    Clock::setGlobals(Globals& globals)
{
	(void)globals;
}

void   Clock::start()
{

}

void    Clock::update()
{
	
}

inline
int Clock::mf_get_time()
{
	return (1);
}


const char* Clock::get_FormatedTime()
{
	return (m_buffer);
}
