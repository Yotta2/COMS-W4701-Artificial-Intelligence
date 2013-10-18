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

    //switch (chooseAgent()) {
    for (int i = 1; i <= 5; i++)
    switch (i) {
    case 1:
        {
            BFSAgent agent(inputFilename, outputFilename);
            cout << "BFSearching..." << endl;
            agent.solve();
            break;
        }
    case 2:
        {
            DFSAgent agent(inputFilename, outputFilename);
            cout << "DFSearching..." << endl;
            agent.solve();
            break;
        }
    case 3:
        {
            UCSAgent agent(inputFilename, outputFilename);
            cout << "UCSearching..." << endl;
            agent.solve();
            break;
        }
    case 4:
        {
            GBFSAgent agent(inputFilename, outputFilename);
            cout << "GBFSearching..." << endl;
            agent.solve();
            break;
        }
    case 5:
        {
            ASTARSAgent agent(inputFilename, outputFilename);
            cout << "ASTARSearching..." << endl;
            agent.solve();
            break;
        }
    }
    return 0;
}
