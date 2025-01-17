

#ifndef CUSTOMASSERT_HPP

# define CUSTOMASSERT_HPP

	#ifndef NDEBUG

		# include <cassert>
		
	    #define CUSTOM_ASSERT(expr, msg) \
		if (!(expr)) { \
		    std::cerr << "Assertion failed: " << #expr << "\n" \
			    << "Message: " << msg << "\n" \
			    << "File: " << __FILE__ << "\n" \
			    << "Line: " << __LINE__ << std::endl; \
		    assert(expr); \
		}

	#endif

#endif