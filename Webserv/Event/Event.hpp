

#ifndef EVENT_HPP

# define EVENT_HPP

class Event
{
	public:
		Event();
		~Event();

		//typedefs
		typedef void	(*HandlerFunction)(Event& event);
		typedef void*	HandlerData;

		//methods
		void				handle();

		//getters
		HandlerData			getData()		const;
		HandlerFunction		getFunction()	const;
		int					getFlags()		const;

		//setters
		void				setHandlerFunction_and_Data(HandlerFunction function, HandlerData data);
		void				setFlags(int flags);




	private:
		HandlerFunction		m_function;
		HandlerData			m_data;
		int					m_flags;

		Event(const Event& copy);
		Event& operator=(const Event& assign);
};

#endif
