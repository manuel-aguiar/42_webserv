

#ifndef ASSERT_EQUAL

	#ifndef NDEBUG
		#include <iostream>
		#include <cstdlib> // for abort
		#include "../../TestHelpers/TestHelpers.h"
		namespace AssertEqual
		{
			template <typename T, typename U>
			void AssertEqualImpl(const T& 			actual, 
								const U& 			expected, 
								const char* 		actualExpr, 
								const char* 		expectedExpr, 
								const std::string& 	message, 
								const char* 		file, 
								int 				line, 
								const char* 		function)
			{
				if (actual != static_cast<const T>(expected))
				{
					std::cerr 	<< "\n\n"                                               															\
								<< "\tABORTING PROGRAM\n" 							 																\
								<< TestHelpers::generateErrorMessage(actual, expected, actualExpr, expectedExpr, message, file, line, function)		\
								<< std::endl;
					std::abort();
				}
			}
		}
		#define ASSERT_EQUAL(actual, expected, message)                                		 				\
			AssertEqual::AssertEqualImpl((actual), (expected), #actual, #expected, (message), __FILE__, __LINE__, __FUNCTION__)

	#else

		#define ASSERT_EQUAL(actual, expected, message) ((void)0)

	#endif

#endif