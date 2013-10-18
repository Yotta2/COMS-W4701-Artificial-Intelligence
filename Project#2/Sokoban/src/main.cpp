#include <iostream>
#include <ctime>
#include <sys/time.h>
#include "../include/BFSAgent.h"
#include "../include/DFSAgent.h"
#include "../include/UCSAgent.h"
#include "../include/GBFSAgent.h"
#include "../include/ASTARSAgent.h"

using namespace std;

int chooseAgent() {
    cout << "1) Breadth first search" << endl;
    cout << "2) Depth first search" << endl;
    cout << "3) Uniform cost search" << endl;
    cout << "4) Greedy best first search" << endl;
    cout << "5) A* search" << endl;
    cout << "Please choose a search strategy: ";
    int choice;
    cin >> choice;
    return choice;
}

int main()
{
    string inputFilename = "in.txt";
    string outputFilename = "out.txt";

    switch (chooseAgent()) {
    case 1:
        {
            BFSAgent agent(inputFilename, outputFilename);
            cout << "Searching..." << endl;
            struct timeval start, end;
            long mtime, seconds, useconds;
            gettimeofday(&start, NULL);
            agent.solve();
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;

            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            cout << "Used time(ms): " << mtime << endl;
            break;
        }

    case 2:
        {
            DFSAgent agent(inputFilename, outputFilename);
            cout << "Searching..." << endl;
            struct timeval start, end;
            long mtime, seconds, useconds;
            gettimeofday(&start, NULL);
            agent.solve();
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;

            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            cout << "Used time(ms): " << mtime << endl;
            break;
        }
    case 3:
        {
            UCSAgent agent(inputFilename, outputFilename);
            cout << "Searching..." << endl;
            struct timeval start, end;
            long mtime, seconds, useconds;
            gettimeofday(&start, NULL);
            agent.solve();
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;

            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            cout << "Used time(ms): " << mtime << endl;
            break;
        }
    case 4:
        {
            GBFSAgent agent(inputFilename, outputFilename);
            cout << "Searching..." << endl;
            struct timeval start, end;
            long mtime, seconds, useconds;
            gettimeofday(&start, NULL);
            agent.solve();
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;

            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            cout << "Used time(ms): " << mtime << endl;
            break;
        }
    case 5:
        {
            ASTARSAgent agent(inputFilename, outputFilename);
            cout << "Searching..." << endl;
            struct timeval start, end;
            long mtime, seconds, useconds;
            gettimeofday(&start, NULL);
            agent.solve();
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;

            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            cout << "Used time(ms): " << mtime << endl;
            break;
        }
    }
    return 0;
}
