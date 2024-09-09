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

# include <iostream>
# include "../include/Concrete/ThreadTask.tpp"
# include "../include/Concrete/ThreadPool.hpp"
//# include "_legacy/SharedTask.hpp"
//# include "_legacy/IndependentTask.hpp"
# include <unistd.h>

/*
	(cd ../.. && ./gitcommit.sh)
	(cd .. && make) && c++ -Wall -Wextra -Werror testMain.cpp -L../ -lpthread -lthreadpool -o tptest

	valgrind --tool=helgrind ./tptest 10
*/

pthread_mutex_t globalLock;
void	lockWrite(const std::string& toWrite);

long fibBad(unsigned int n) {
	if (n <= 1)
		return n;
	return (fibBad(n - 1) + fibBad(n - 2));
}

long fibGood(unsigned int n)
{
	long a;
	long b;
	long temp;

	if (n <= 1)
		return (n);
	a = 0;
	b = 1;
	while (n > 0)
	{
		temp = b;
		b = a + b;
		a = temp;
		n--;
	}
	return (a);
}

long fib(unsigned int n)
{
	return (fibBad(n));
}

long fibprint(unsigned long n) {
	lockWrite(std::to_string(fib(n)));
	return (fib(n));
}

void	voidfibprint(unsigned long n)
{
	lockWrite("void " + std::to_string(fib(n)));
}

void	printf(const std::string& str)
{
	lockWrite(str);
}

void nada()
{
	lockWrite("nada");
}

unsigned int* allSameThreadUnsafe(unsigned int number)
{
	unsigned int* treta;

	treta = new unsigned int[number];
	return treta;
}



// Non-const methods
int ArgYesReturnYes(unsigned int number)
{
	lockWrite("NON-MEMB Arg Yes Return Yes " + std::to_string(number));
	return number;
}

int ArgNoReturnYes(void)
{
	lockWrite("NON-MEMB Arg No, Return Yes 52");
	return 42;
}

void ArgYesReturnNo(int number)
{
	lockWrite("NON-MEMB Arg Yes, Return No " + std::to_string(number));
}

void ArgNoReturnNo(void)
{
	lockWrite("NON-MEMB Arg No, Return No 42");
}

int TwoArgReturnYes(unsigned int number, char c)
{
	(void)c;
	lockWrite("NON-MEMB TWO ARGS Return Yes " + std::to_string(number));
	return number;
}

int ThreeArgReturnYes(unsigned int number, int hey, char c)
{
	(void)hey;
	(void)c;
	lockWrite("NON-MEMB THREE ARGS Return Yes " + std::to_string(number));
	return number;
}

void ThreeArgReturnNo(unsigned int number, int hey, char c)
{ (void)number; (void)hey; (void)c;
	lockWrite("NON-MEMB THREE ARGS Return No");
}

int FourArgReturnYes(unsigned int number, int hey, char c, float f)
{ (void)hey; (void)c; (void)f;
	lockWrite("NON-MEMB FOUR ARGS Return Yes " + std::to_string(number));
	return number;
}

void FourArgReturnNo(unsigned int number, int hey, char c, float f)
{ (void)number; (void)hey; (void)c; (void)f;
	lockWrite("NON-MEMB FOUR ARGS Return No");
}

int FiveArgReturnYes(unsigned int number, int hey, char c, float f, double d)
{ (void)hey; (void)c; (void)f; (void)d;
	lockWrite("NON-MEMB FIVE ARGS Return Yes " + std::to_string(number));
	return number;
}

void FiveArgReturnNo(unsigned int number, int hey, char c, float f, double d)
{ (void)number; (void)hey; (void)c; (void)f; (void)d;
	lockWrite("NON-MEMB FIVE ARGS Return No");
}

class ITest
{
	public:
		virtual ~ITest() {};
		virtual int derived(unsigned int number) = 0;

};

class Test : public ITest
{
public:
    // Static method
    static void StaticMethod()
    {
        lockWrite("HELLO");
    }

	int derived(unsigned int number)
	{
		lockWrite("Derived Method " + std::to_string(number));
		return (number);
	}

    // Non-const methods
    int ArgYesReturnYes(unsigned int number)
    {
        lockWrite("Arg Yes Return Yes " + std::to_string(number));
        return number;
    }

    int ArgNoReturnYes(void)
    {
        lockWrite("Arg No, Return Yes 52");
        return 42;
    }

    void ArgYesReturnNo(int number)
    {
        lockWrite("Arg Yes, Return No " + std::to_string(number));
    }

    void ArgNoReturnNo(void)
    {
        lockWrite("Arg No, Return No 42");
    }

    int TwoArgReturnYes(unsigned int number, char c)
    {
        (void)c;
        lockWrite("TWO ARGS Return Yes " + std::to_string(number));
        return number;
    }

    int ThreeArgReturnYes(unsigned int number, int hey, char c)
    {
        (void)hey;
        (void)c;
        lockWrite("THREE ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void ThreeArgReturnNo(unsigned int number, int hey, char c)
    { (void)number; (void)hey; (void)c;
        lockWrite("THREE ARGS Return No");
    }

    int FourArgReturnYes(unsigned int number, int hey, char c, float f)
    { (void)hey; (void)c; (void)f;
        lockWrite("FOUR ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void FourArgReturnNo(unsigned int number, int hey, char c, float f)
    { (void)number; (void)hey; (void)c; (void)f;
        lockWrite("FOUR ARGS Return No");
    }

    int FiveArgReturnYes(unsigned int number, int hey, char c, float f, double d)
    { (void)hey; (void)c; (void)f; (void)d;
        lockWrite("FIVE ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void FiveArgReturnNo(unsigned int number, int hey, char c, float f, double d)
    { (void)number; (void)hey; (void)c; (void)f; (void)d;
        lockWrite("FIVE ARGS Return No");
    }

    // Const methods
    int ArgYesReturnYes_Const(unsigned int number) const
    {
        lockWrite("Const: Arg Yes Return Yes " + std::to_string(number));
        return number;
    }

    int ArgNoReturnYes_Const(void) const
    {
        lockWrite("Const: Arg No, Return Yes 52");
        return 42;
    }

    void ArgYesReturnNo_Const(int number) const
    {
        lockWrite("Const: Arg Yes, Return No " + std::to_string(number));
    }

    void ArgNoReturnNo_Const(void) const
    {
        lockWrite("Const: Arg No, Return No 42");
    }

    int TwoArgReturnYes_Const(unsigned int number, char c) const
    {
        (void)c;
        lockWrite("Const: TWO ARGS Return Yes " + std::to_string(number));
        return number;
    }

    int ThreeArgReturnYes_Const(unsigned int number, int hey, char c) const
    {
        (void)hey;
        (void)c;
        lockWrite("Const: THREE ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void ThreeArgReturnNo_Const(unsigned int number, int hey, char c) const
    { (void)number; (void)hey; (void)c;
        lockWrite("Const: THREE ARGS Return No");
    }

    int FourArgReturnYes_Const(unsigned int number, int hey, char c, float f) const
    { (void)hey; (void)c; (void)f;
        lockWrite("Const: FOUR ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void FourArgReturnNo_Const(unsigned int number, int hey, char c, float f) const
    { (void)number; (void)hey; (void)c; (void)f;
        lockWrite("Const: FOUR ARGS Return No");
    }

    int FiveArgReturnYes_Const(unsigned int number, int hey, char c, float f, double d) const
    { (void)hey; (void)c; (void)f; (void)d;
        lockWrite("Const: FIVE ARGS Return Yes " + std::to_string(number));
        return number;
    }

    void FiveArgReturnNo_Const(unsigned int number, int hey, char c, float f, double d) const
    {
        (void)number; (void)hey; (void)c; (void)f; (void)d;
        lockWrite("Const: FIVE ARGS Return No");
    }
};



void	lockWrite(const std::string& toWrite)
{
	pthread_mutex_lock(&globalLock);
	std::cout << toWrite << std::endl;
	pthread_mutex_unlock(&globalLock);
}



int main(int ac, char **av)
{
	(void)ac;

	if (ac != 2)
	{
		std::cout << "wrong number of arguments, 1 arg as a number of threads up to 100" << std::endl;
		return (0);
	}

	int numThreads = std::atoi(av[1]);


	if (numThreads <= 0 || numThreads > 100)
	{
		std::cout << "number of threads: " << numThreads << " is invalid, must be between 1 and 100" << std::endl;
		return (0);
	}

	std::vector<int> returnValues(18);		//saving return values
	Test	dummy;
	ITest*  testptr;

	testptr = &dummy;
	ThreadPool tp(numThreads);


	pthread_mutex_init(&globalLock, NULL);

	tp.addThread();			//testing adding a thread
	tp.removeThread();		//testing removing a thread

	{
		unsigned int i = 0;
		int j = 0;

		// 1. Non member functions
		tp.addTask(ArgYesReturnYes, i++);                     // Single argument, returns int
		tp.addTask(ArgYesReturnNo, static_cast<int>(i));     // Single argument, no return
		tp.addTask(ArgNoReturnYes);                          // No arguments, returns int
		tp.addTask(ArgNoReturnNo);                           // No arguments, no return
		tp.addTask(TwoArgReturnYes, i++, 'c');                 // Two arguments, returns int
		tp.addTask(ThreeArgReturnYes, i++, 42, 'c');           // Three arguments, returns int
		tp.addTask(ThreeArgReturnNo, i++, 42, 'c');            // Three arguments, no return
		tp.addTask(FourArgReturnYes, i++, 42, 'c', 3.14f);     // Four arguments, returns int
		tp.addTask(FourArgReturnNo, i++, 42, 'c', 3.14f);      // Four arguments, no return
		tp.addTask(FiveArgReturnYes, i++, 42, 'c', 3.14f, 2.718); // Five arguments, returns int
		tp.addTask(FiveArgReturnNo, i++, 42, 'c', 3.14f, 2.718);  // Five arguments, no return


    	// 2. Static method
		tp.addTask(Test::StaticMethod);

		// 3. Non-const member functions
		tp.addTask(ArgYesReturnYes, i++);
		tp.addTask(dummy, &Test::ArgYesReturnYes, i++);
		tp.addTask(dummy, &Test::ArgYesReturnNo, static_cast<int>(i));
		tp.addTask(dummy, &Test::ArgNoReturnYes);
		tp.addTask(dummy, &Test::ArgNoReturnNo);
		tp.addTask(dummy, &Test::TwoArgReturnYes, i++, 'c');
		tp.addTask(dummy, &Test::ThreeArgReturnYes, i++, 42, 'c');
		tp.addTask(dummy, &Test::ThreeArgReturnNo, i++, 42, 'c');
		tp.addTask(dummy, &Test::FourArgReturnYes, i++, 42, 'c', 3.14f);
		tp.addTask(dummy, &Test::FourArgReturnNo, i++, 42, 'c', 3.14f);
		tp.addTask(dummy, &Test::FiveArgReturnYes, i++, 42, 'c', 3.14f, 2.718);
		tp.addTask(dummy, &Test::FiveArgReturnNo, i++, 42, 'c', 3.14f, 2.718);

		// 4. Const member functions
		tp.addTask(dummy, &Test::ArgYesReturnYes_Const, i++);
		tp.addTask(dummy, &Test::ArgYesReturnNo_Const, static_cast<int>(i++));
		tp.addTask(dummy, &Test::ArgNoReturnYes_Const);
		tp.addTask(dummy, &Test::ArgNoReturnNo_Const);
		tp.addTask(dummy, &Test::TwoArgReturnYes_Const, i++, 'c');
		tp.addTask(dummy, &Test::ThreeArgReturnYes_Const, i++, 42, 'c');
		tp.addTask(dummy, &Test::ThreeArgReturnNo_Const, i++, 42, 'c');
		tp.addTask(dummy, &Test::FourArgReturnYes_Const, i++, 42, 'c', 3.14f);
		tp.addTask(dummy, &Test::FourArgReturnNo_Const, i++, 42, 'c', 3.14f);
		tp.addTask(dummy, &Test::FiveArgReturnYes_Const, i++, 42, 'c', 3.14f, 2.718);
		tp.addTask(dummy, &Test::FiveArgReturnNo_Const, i++, 42, 'c', 3.14f, 2.718);

		// 5. Member function derived from base class (non-const)
		tp.addTask(*testptr, &ITest::derived, i);

		// 6. saving return values to check if it works
		tp.addTask(ArgYesReturnYes, i++, &returnValues[j++]);                     // Single argument, returns int
		tp.addTask(ArgNoReturnYes, &returnValues[j++]);                         // No arguments, returns int
		tp.addTask(TwoArgReturnYes, i++, 'c', &returnValues[j++]);                // Two arguments, returns int
		tp.addTask(ThreeArgReturnYes, i++, 42, 'c', &returnValues[j++]);          // Three arguments, returns int
		tp.addTask(FourArgReturnYes, i++, 42, 'c', 3.14f, &returnValues[j++]);    // Four arguments, returns int
		tp.addTask(FiveArgReturnYes, i++, 42, 'c', 3.14f, 2.718, &returnValues[j++]); // Five arguments, returns int

		tp.addTask(dummy, &Test::ArgYesReturnYes, i++, &returnValues[j++]);       // Single argument, returns int
		tp.addTask(dummy, &Test::ArgNoReturnYes, &returnValues[j++]);           // No arguments, returns int
		tp.addTask(dummy, &Test::TwoArgReturnYes, i++, 'c', &returnValues[j++]);  // Two arguments, returns int
		tp.addTask(dummy, &Test::ThreeArgReturnYes, i++, 42, 'c', &returnValues[j++]); // Three arguments, returns int
		tp.addTask(dummy, &Test::FourArgReturnYes, i++, 42, 'c', 3.14f, &returnValues[j++]); // Four arguments, returns int
		tp.addTask(dummy, &Test::FiveArgReturnYes, i++, 42, 'c', 3.14f, 2.718, &returnValues[j++]); // Five arguments, returns int

		tp.addTask(dummy, &Test::ArgYesReturnYes_Const, i++, &returnValues[j++]); // Single argument, returns int
		tp.addTask(dummy, &Test::ArgNoReturnYes_Const, &returnValues[j++]);     // No arguments, returns int
		tp.addTask(dummy, &Test::TwoArgReturnYes_Const, i++, 'c', &returnValues[j++]); // Two arguments, returns int
		tp.addTask(dummy, &Test::ThreeArgReturnYes_Const, i++, 42, 'c', &returnValues[j++]); // Three arguments, returns int
		tp.addTask(dummy, &Test::FourArgReturnYes_Const, i++, 42, 'c', 3.14f, &returnValues[j++]); // Four arguments, returns int
		tp.addTask(dummy, &Test::FiveArgReturnYes_Const, i++, 42, 'c', 3.14f, 2.718, &returnValues[j++]); // Five arguments, returns int

		lockWrite("										   finished inserting tasks");

	}	

	tp.waitForCompletion();

	pthread_mutex_lock(&globalLock);
	for (unsigned int i = 0; i < returnValues.size(); ++i)
	{
		std::cout << "vector(" << i << ") = " << returnValues[i] << std::endl;
	}
	std::cout << "tp has " << tp.threadCount() << " threads" << std::endl;
	pthread_mutex_unlock(&globalLock);



	tp.destroy(true);
	pthread_mutex_destroy(&globalLock);


	return (0);
}