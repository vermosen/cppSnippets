/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vermosen
 *
 * Created on August 21, 2017, 5:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

/*
 * a simple example of a linux daemon
 */
class daemon
{
public:
    static int run()
    {
        if (daemon::daemonize() == 0)
        {
            while (1)
            {
                //TODO: Insert daemon code here.
                syslog(LOG_NOTICE, "First daemon started.");
                sleep(40);
                break;
            }

            syslog(LOG_NOTICE, "First daemon terminated.");
            closelog();

            return EXIT_SUCCESS;
        }
    }
    
private:
    static int daemonize()
    {
        pid_t pid;

        /* Fork off the parent process */
        pid = fork();

        /* An error occurred */
        if (pid < 0)
            exit(EXIT_FAILURE);

        /* Success: Let the parent terminate */
        if (pid > 0)
            exit(EXIT_SUCCESS);

        /* On success: The child process becomes session leader */
        if (setsid() < 0)
            exit(EXIT_FAILURE);

        /* Catch, ignore and handle signals */
        //TODO: Implement a working signal handler */
        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);

        /* Fork off for the second time*/
        pid = fork();

        /* An error occurred */
        if (pid < 0)
            exit(EXIT_FAILURE);

        /* Success: Let the parent terminate */
        if (pid > 0)
            exit(EXIT_SUCCESS);

        /* Set new file permissions */
        umask(0);

        /* Change the working directory to the root directory */
        /* or another appropriated directory */
        chdir("/");

        /* Close all open file descriptors */
        int x;
        for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
            close (x);
        }

        /* Open the log file */
        openlog ("first daemon", LOG_PID, LOG_DAEMON);
        
        return 0;
    }
};

int main(int argc, char ** argv)
{
    return daemon::run();
}