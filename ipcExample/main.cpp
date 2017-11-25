/*
*
* ipcExample
* Copyright (C) 2015 Jean-Mathieu Vermosen
*
*/

// disable some warnings
#ifdef _MSC_VER
//#pragma warning( disable : XXXX )
#endif

#include <string>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include "functions/pipeExample.hpp"

int main(int argc, char** argv) {

	int exit;														// exit code

	try {

		bool end = false;											// main exit indicator 
		int test = 0;												// optional test

		for (int i = 1; i < argc; i++) {							// deals with optional arguments

			std::string arg(argv[i]);								// current std::string

			if (arg.substr(1, 4) == "test") {						// expects -test=n

				std::string str(arg.substr(6, arg.length() - 6));	// the value
				test = boost::lexical_cast<int>(str);				// runs the selected test automatically	
				end = true;											// for later use: only one attempt

			}
		}

		do {														// manual selection - loop over the choices

			int res = test;											// for manual definition

			if (test == 0) {										// no test defined in args

				std::cout											// message
					<< std::endl << "Please select an activity: "
					<< std::endl
					<< std::endl << "####### imports #######"
					<< std::endl << "1 - calculator"
					<< std::endl
					<< std::endl << "#### Miscellaneous ####"
					<< std::endl << "0 - exit"
					<< std::endl
					<< std::endl;

				std::cin >> res;									// user defined test

			}

			switch (res) {											// switch over the available tests

			case 1:

				pipeExample();
				break;

			case 0:

				end = true;											// stop the server
				break;

			default:												// unknown, invalid

				std::cout
					<< "invalid selection, please try again"
					<< std::endl;

				break;

			}

		} while (end == false);										// loop until exit

		exit = 0; system("pause");

	}
	catch (std::exception & e) {									// exception management

		std::cout
			<< "an error occured: "
			<< e.what()
			<< std::endl;

		exit = 1; system("pause");

	}
	catch (...) {													// unknown error

		std::cout
			<< "an unknown error occured..."
			<< std::endl;

		exit = 1; system("pause");

	}

	return exit;

}