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

string specifyInputFile() {
    string inputFilename = "in.txt";
    char ans;
    cout << "Do you want to specify the input filename\n(the default one is \"in.txt\")? (y/n): ";
    cin >> ans;
    if (ans == 'y')
        cin >> inputFilename;
    return inputFilename;
}

string specifyOutputFile() {
    string outputFilename = "out.txt";
    char ans;
    cout << "Do you want to specify the output filename\n(the default one is \"out.txt\")? (y/n): ";
    cin >> ans;
    if (ans == 'y')
        cin >> outputFilename;
    return outputFilename;
}

int main()
{
    string inputFilename = "in.txt";
    string outputFilename = "out.txt";
    bool needStat;
    char choice = 'y';
    cout << "Welcome to Sokoban!..." << endl;
    inputFilename = specifyInputFile();
    outputFilename = specifyOutputFile();

    //Ask the user if he/she wants the statistics
    cout << "Do you want the search statistics? (y/n): ";
    cin >> choice;
    needStat = (choice == 'y');

    switch (chooseAgent()) {
    //for (int i = 1; i <= 5; i++)
    //if (i != 2)
    //switch (i) {
    case 1:
        {
            BFSAgent agent(inputFilename, outputFilename, needStat);
            cout << "BFSearching..." << endl;
            agent.solve();
            break;
        }
    case 2:
        {
            DFSAgent agent(inputFilename, outputFilename, needStat);
            cout << "DFSearching..." << endl;
            agent.solve();
            break;
        }
    case 3:
        {
            UCSAgent agent(inputFilename, outputFilename, needStat);
            cout << "UCSearching..." << endl;
            agent.solve();
            break;
        }
    case 4:
        {
            GBFSAgent agent(inputFilename, outputFilename, needStat);
            cout << "GBFSearching..." << endl;
            agent.solve();
            break;
        }
    case 5:
        {
            ASTARSAgent agent(inputFilename, outputFilename, needStat);
            cout << "ASTARSearching..." << endl;
            agent.solve();
            break;
        }
    }
    return 0;
}
