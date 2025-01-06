/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   testMain.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 08:33:11 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/09/03 16:26:42 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

//test target
# include "../ThreadPool.h"


// Project headers
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../_Tests/test.h"

// C++ headers
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <iostream>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);
extern int TestPart4(int testNumber);

int main(void)
{

    int testNumber = 1;
	/******************* TEST 1 ************************/
	std::cout << "\n************ ThreadPool tests *************" << std::endl;

    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
	testNumber = TestPart3(testNumber);
	testNumber = TestPart4(testNumber);

	std::cout << "*******************************************\n" << std::endl;
	
	return (0);
}





