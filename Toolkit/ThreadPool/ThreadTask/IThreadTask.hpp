

#ifndef ITHREADTASK_HPP

# define ITHREADTASK_HPP

class IThreadTask
{
	public:
		virtual 				~IThreadTask() {};
		virtual void			execute() = 0;
};


#endif
