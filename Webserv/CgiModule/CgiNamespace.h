

#ifndef CGINAMESPACE_H

# define CGINAMESPACE_H

# include <string>
# include "../../Toolkit/Arrays/DynArray/DynArray.hpp"
# include "../GenericUtils/BufferView/BufferView.hpp"

class BaseBuffer;

namespace Ws {typedef int fd;}

namespace Cgi
{

	typedef std::string ScriptPath;
	typedef std::string InterpExtension;
	typedef std::string InterpPath;
	typedef std::string	EnvKey;
	typedef std::string	EnvValue;

	typedef void*		User;

	namespace Env
	{
		class Enum
		{
			public:
				typedef enum
				{
					AUTH_TYPE,
					CONTENT_LENGTH,
					CONTENT_TYPE,
					GATEWAY_INTERFACE,
					PATH_INFO,
					PATH_TRANSLATED,
					QUERY_STRING,
					REMOTE_ADDR,
					REMOTE_HOST,
					REMOTE_IDENT,
					REMOTE_USER,
					REQUEST_METHOD,
					REDIRECT_STATUS,
					SCRIPT_FILENAME,
					SCRIPT_NAME,
					SERVER_NAME,
					SERVER_PORT,
					SERVER_PROTOCOL,
					SERVER_SOFTWARE,
					COUNT
				}	Type;
		};

		namespace Str
		{
			const char* const AUTH_TYPE = "AUTH_TYPE";
			const char* const CONTENT_LENGTH = "CONTENT_LENGTH";
			const char* const CONTENT_TYPE = "CONTENT_TYPE";
			const char* const GATEWAY_INTERFACE = "GATEWAY_INTERFACE";
			const char* const PATH_INFO = "PATH_INFO";
			const char* const PATH_TRANSLATED = "PATH_TRANSLATED";
			const char* const QUERY_STRING = "QUERY_STRING";
			const char* const REMOTE_ADDR = "REMOTE_ADDR";
			const char* const REMOTE_HOST = "REMOTE_HOST";
			const char* const REMOTE_IDENT = "REMOTE_IDENT";
			const char* const REMOTE_USER = "REMOTE_USER";
			const char* const REQUEST_METHOD = "REQUEST_METHOD";
			const char* const REDIRECT_STATUS = "REDIRECT_STATUS";
			const char* const SCRIPT_FILENAME = "SCRIPT_FILENAME";
			const char* const SCRIPT_NAME = "SCRIPT_NAME";
			const char* const SERVER_NAME = "SERVER_NAME";
			const char* const SERVER_PORT = "SERVER_PORT";
			const char* const SERVER_PROTOCOL = "SERVER_PROTOCOL";
			const char* const SERVER_SOFTWARE = "SERVER_SOFTWARE";
		}
	}
	typedef Cgi::Env::Enum				CgiEnvEnum;

	struct EnvVariables
	{
		DynArray<std::pair<Cgi::Env::Enum::Type, 	EnvValue> >	envBase;
		DynArray<std::pair<EnvKey, 					EnvValue> >	envExtra;
	};

    struct Header
    {
		Header(const char* key, const char* value);
		Header(const BufferView& key, const BufferView& value);

        BufferView key;
        BufferView value;
    
        bool operator<(const Header& other) const
        {
            return (key < other.key);
        }
    
        bool operator==(const Header& other) const
        {
            return (key == other.key);
        }
    
        bool operator!=(const Header& other) const
        {
            return (key != other.key);
        }
    };

	namespace Options
	{
		enum
		{
			HOLD_READ 		= (1 << 1),
			RESTART_READ 	= (1 << 2),
			CANCEL_READ 	= (1 << 3),
			HOLD_WRITE 		= (1 << 4),
			RESTART_WRITE 	= (1 << 5),
			CANCEL_WRITE 	= (1 << 6)
		};
		typedef int 	Mask;
	}

	namespace Notify
	{
		typedef enum
		{
			ON_SUCCESS,
			ON_ERROR,
			COUNT
		} 	Type;
		typedef void	(*Callback)	(User user);
	}

	class HeaderData;

	namespace IO
	{
		typedef enum
		{
			READ_HEADERS,
			READ_BODY,
			WRITE,
			COUNT
		} 	Type;
		
		typedef enum
		{
			CONTINUE,
			CLOSE
		}	State;

		typedef int     	BytesCount;

		typedef Cgi::IO::State 				(*OnReadWrite)(Cgi::User user, const Ws::fd fd);
		typedef Cgi::IO::State  			(*ReceiveHeaderData)(Cgi::User user, const Cgi::HeaderData& headers);
	}


	namespace RequestState
	{
		typedef enum
		{
			IDLE,
			ACQUIRED,
			QUEUED,
			EXECUTING,
			CANCELLED
		} 	Type;
	}
}

#endif
