#pragma once

#include <iostream>

#include <sys/types.h>

#ifndef WIN32
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#else
#include <Windows.h>
#endif

// simple example using pipes on the unix side 
// and ShellExecute on the windows sidec
void pipeExample() {

#ifndef WIN32
	pid_t  pid;
	int    i;
	char   buf[BUF_SIZE];

	fork();
	pid = getpid();
	for (i = 1; i <= MAX_COUNT; i++) {
		sprintf(buf, "This line is from pid %d, value = %d\n", pid, i);
		write(1, buf, strlen(buf));

	}
#else

	wchar_t * szPath = L"C:\\WINDOWS\\system32\\Calc.exe";

	HINSTANCE hRet = ShellExecute(
		HWND_DESKTOP,								//Parent window
		L"open",									//Operation to perform
		szPath,										//Path to program
		NULL,										//Parameters
		NULL,										//Default directory
		SW_SHOW);									//How to open

	/*
	The function returns a HINSTANCE (not really useful in this case)
	So therefore, to test its result, we cast it to a LONG.
	Any value over 32 represents success!
	*/
	if ((LONG)hRet <= 32) 
		std::cout << "Unable to start program" << std::endl;

#endif

}