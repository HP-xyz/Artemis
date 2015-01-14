#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "src/Artemis_Server.h"
int main(int argc, char* argv[])
{
	try
	{
		// Initialise the server.
		std::size_t num_threads = 10;
		Artemis::Artemis_Server s(num_threads);

		// Run the server until stopped.
		s.run();
		return 0;
	} catch (std::exception& e)
	{
		std::cerr << "Exception (MAIN): " << e.what() << "\n";
		return 1;
	}
}
