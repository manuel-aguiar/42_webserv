

#ifndef HTTPSTATUSCODES_H

# define HTTPSTATUSCODES_H

typedef enum e_http_parsing_status
{
	HTTP_PARSING_OK = 0,
	HTTP_PARSING_ERROR = -1
}	t_http_parsing_status;

typedef enum e_http_status
{
	HTTP_OK = 200,
	HTTP_CREATED = 201
	//...
}	t_http_status;

#endif
