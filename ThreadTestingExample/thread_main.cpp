// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread

void foo()
{
	// do stuff...

}

void bar(int x)
{
	// do stuff...
	while (true)
		std::cout << "bar...\n";

}

int main()
{
	std::thread first(foo);     // spawn new thread that calls foo()
	std::thread second(bar, 0);  // spawn new thread that calls bar(0)

	std::cout << "main, foo and bar now execute concurrently...\n";

	// synchronize threads:
	first.join();                // pauses until first finishes
	std::cout << "foo completed.\n";
	second.join();               // pauses until second finishes

	std::cout << "foo and bar completed.\n";

	while (true);

	return 0;
}