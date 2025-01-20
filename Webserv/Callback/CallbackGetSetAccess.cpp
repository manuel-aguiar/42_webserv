

#include "Callback.hpp"

// getters

Callback::Data			Callback::getData() const
{
    return (m_data);
}

Callback::Handler		Callback::getHandler() const
{
    return (m_handler);
}

// setters

void	Callback::setData(const Callback::Data data)
{
    m_data = data;
}

void	Callback::setHandler(const Callback::Handler handler)
{
    m_handler = handler;
}

void    Callback::setData_Handler(const Callback::Data data, const Callback::Handler handler)
{
    m_data = data;
    m_handler = handler;
}
