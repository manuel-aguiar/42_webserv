

#ifndef CALLBACK_HPP

# define CALLBACK_HPP

class Callback
{
	public:
		typedef void* Data;
		typedef void (*Handler)(Callback& callback);

		Callback();
		~Callback();
		Callback(const Callback& copy);
		Callback& operator=(const Callback& assign);

		//methods
		void		execute();
		void		reset();

		//getters
		Data		getData()		const;
		Handler		getHandler()	const;

		//setters
		void		setData(const Callback::Data data);
		void		setHandler(const Callback::Handler handler);
		void		setData_Handler(const Callback::Data data, const Callback::Handler handler);

	private:
		Data		m_data;
		Handler		m_handler;

};


#endif