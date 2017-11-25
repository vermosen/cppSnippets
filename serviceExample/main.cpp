#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <fstream>
#include <exception>

#include <boost/thread.hpp>

#include "serviceImpl.hpp"

#ifdef _WIN32
#include <WinSock.h>
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char ** argv)
{
	try
	{
		dtcc::serviceImpl srv(3);

		if (!dtcc::serviceImpl::run(srv))
		{
			throw std::exception();
		}

		return 0;
	}
	catch (std::exception & e)
	{
		return 1;
	}
}

#endif