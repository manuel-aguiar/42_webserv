

#include "Callback.hpp"

// getters

t_ptr_callback_data			Callback::getData() const
{
    return (m_data);
}

t_func_callback_handler		Callback::getHandler() const
{
    return (m_handler);
}

// setters

void	Callback::setData(const t_ptr_callback_data data)
{
    m_data = data;
}

void	Callback::setHandler(const t_func_callback_handler handler)
{
    m_handler = handler;
}

void    Callback::setData_Handler(const t_ptr_callback_data data, const t_func_callback_handler handler)
{
    m_data = data;
    m_handler = handler;
}
