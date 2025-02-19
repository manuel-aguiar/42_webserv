

#ifndef CGI_RESPONSE_HPP

# define CGI_RESPONSE_HPP



namespace Cgi
{
	class Response
	{
		public:
			Response();
			~Response();

		

		private:
			//status at the beginning
			//mandatory header: content-type
			//forbidden headers, Connection, Transfer-encoding, Upgrade

	};
}




#endif