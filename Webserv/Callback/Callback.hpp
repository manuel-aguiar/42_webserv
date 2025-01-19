

#ifndef CALLBACK_HPP

# define CALLBACK_HPP

# include "../GenericUtils/Webserver_Definitions.h"

//callback typedefs

class Callback
{
	public:
		Callback();
		~Callback();
		Callback(const Callback& copy);
		Callback& operator=(const Callback& assign);

		//methods
		void						execute();
		void						reset();

		//getters
		t_ptr_callback_data			getData()				const;
		t_func_callback_handler		getHandler()			const;

		//setters
		void						setData(const t_ptr_callback_data data);
		void						setHandler(const t_func_callback_handler handler);
		void						setData_Handler(const t_ptr_callback_data data, const t_func_callback_handler handler);

	private:
		t_ptr_callback_data			m_data;
		t_func_callback_handler		m_handler;

};


#endif