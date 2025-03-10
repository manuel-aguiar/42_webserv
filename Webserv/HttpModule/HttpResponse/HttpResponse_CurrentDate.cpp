
# include <sstream>
# include <iomanip>
# include <ctime>
# include "HttpResponse.hpp"

# include "time.h"

namespace Http
{
	std::string Response::mf_getCurrentDate()
	{
		// Get current time
		time_t rawTime;
		std::time(&rawTime);

		// Convert time to UTC (GMT) in a thread-safe manner
		std::tm gmtTime;
		::gmtime_r(&rawTime, &gmtTime);

		// Days of the week and months as required by HTTP format
		const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

		// Format the date string in the desired HTTP format
		std::stringstream dateStream;
		dateStream << days[gmtTime.tm_wday] << ", "
				<< std::setw(2) << std::setfill('0') << gmtTime.tm_mday << " "
				<< months[gmtTime.tm_mon] << " "
				<< (1900 + gmtTime.tm_year) << " "
				<< std::setw(2) << std::setfill('0') << gmtTime.tm_hour << ":"
				<< std::setw(2) << std::setfill('0') << gmtTime.tm_min << ":"
				<< std::setw(2) << std::setfill('0') << gmtTime.tm_sec << " GMT";

		return dateStream.str();
	}
}
