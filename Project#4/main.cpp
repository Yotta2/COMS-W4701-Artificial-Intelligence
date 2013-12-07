#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

struct Implication {
    string sentence;
    unordered_set<string> premises;
    string conclusion;
    Implication(string s, unordered_set<string> p, string c) :
        sentence(s), premises(p), conclusion(c) {}
};

void removeTailingZero(string &line) {
    for (int i = line.size() - 1; i >= 0; i--)
        if (line[i] != ' ') {
            line.resize(i + 1);
            break;
        }
}

bool isImplication(string line) {
    for (int i = 0; i < line.size(); i++)
        if (line[i] == '>')
            return true;
    return false;
}

Implication convertToImplication(string line) {
	unordered_set<string> premises;
	string sentence = line;
	string conclusion = "F";
	for (int i = 0; i < line.size(); i++)
		if (line[i] == 'v')
			line[i] = ' ';
	stringstream ss(line);
	string premise;
	while (ss >> premise) {
		if (premise[0] != '~') {
			conclusion = premise;
		} else {
			premise.erase(premise.begin());
			premises.insert(premise);
		}
	}
	sentence += "(";
	for (unordered_set<string>::iterator itr = premises.begin(); itr != premises.end(); itr++)
		sentence += *itr + " ^ ";
	sentence[sentence.size() - 2] = '=';
	sentence[sentence.size() - 1] = '>';
	sentence += " " + conclusion + ")";
	Implication implication(sentence, premises, conclusion);
	return implication;
}

bool isFact(string line) {
	for (int i = 0; i < line.size(); i++)
		if (line[i] == 'v' || line[i] == '=' || line[i] == '^' || line[i] == '>')
			return false;
	return true;
}

void parseFile(ifstream &ifs, vector<string> &initSymbols, vector<Implication> &implications) {
    string line;
    while (getline(ifs, line)) {
        if (!isImplication(line)) {
			removeTailingZero(line);
			if (isFact(line))
           	 	initSymbols.push_back(line);
			else
				implications.push_back(convertToImplication(line));
        } else {
            unordered_set<string> premises;
            string sentence = line;
            string conclusion;
            removeTailingZero(line);
            for (int i = line.size() - 1; i >= 0; i--)
                if (line[i] == ' ') {
                    conclusion = line.substr(i + 1);
                    line.resize(i);
					break;
                }
            removeTailingZero(line);
            for (int i = 0; i < line.size(); i++)
                if (line[i] == '^' || line[i] == '=' || line[i] == '>')
                    line[i] = ' ';
            stringstream ss(line);
            string premise;
            while (ss >> premise) {
                premises.insert(premise);
            }
            Implication implication(sentence, premises, conclusion);
            implications.push_back(implication);
        }
    }
}

void forwardChainingCheck(ifstream &ifs, string query) {
    vector<string> initSymbols;
    vector<Implication> implications;
    parseFile(ifs, initSymbols, implications);
    queue<string> agenda;
    unordered_set<string> inferred;
    for (int i = 0; i < initSymbols.size(); i++) {
        cout << initSymbols[i] << endl;
        if (initSymbols[i] == query)
            return;
        agenda.push(initSymbols[i]);
    }
    while (!agenda.empty()) {
        string p = agenda.front();
        agenda.pop();
        if (p == query) {
            cout << "--> true" << endl;
            return;
        }
        if (inferred.find(p) == inferred.end()) {
            inferred.insert(p);
            for (int i = 0; i < implications.size(); i++)
                if (implications[i].premises.find(p) != implications[i].premises.end()) {
                    implications[i].premises.erase(p);
                    if (implications[i].premises.empty()) {
                        cout << implications[i].sentence << endl;
                        agenda.push(implications[i].conclusion);
                    }
                }
        }
    }
    cout << "--> false" << endl;
}

void dp(string p, vector<Implication> &implications, vector<string> &initSymbols,
		unordered_set<string> &checkingSymbols, unordered_map<string, bool> &checkedSymbols,
		stack<string> &outputStack) {
	if (checkedSymbols.find(p) != checkedSymbols.end())
		return;
	for (int i = 0; i < initSymbols.size(); i++)
		if (initSymbols[i] == p) {
			outputStack.push(p);
			checkedSymbols[p] = true;
			return;
		}
	for (int i = 0; i < implications.size(); i++)
		if (implications[i].conclusion == p) {
			unordered_set<string>::iterator itr = implications[i].premises.begin();
			for (; itr != implications[i].premises.end(); itr++) {
				if (checkingSymbols.find(*itr) != checkingSymbols.end())
					break;
				checkingSymbols.insert(*itr);
				dp(*itr, implications, initSymbols, checkingSymbols, checkedSymbols, outputStack);
				checkingSymbols.erase(*itr);
			}
			itr = implications[i].premises.begin();
			bool flag = true;
			for (; itr != implications[i].premises.end(); itr++) {
				if (checkedSymbols[*itr] == false) {
					flag = false;
					break;
				}
			}	
			if (flag) {
				checkedSymbols[p] = true;
				//cout << implications[i].sentence << endl;
				outputStack.push(implications[i].sentence);
				return;
			}
		}
	checkedSymbols[p] = false;
}

void backwardChainingCheck(ifstream &ifs, string query) {
	vector<string> initSymbols;
    vector<Implication> implications;
    parseFile(ifs, initSymbols, implications);
	unordered_map<string, bool> checkedSymbols;
	unordered_set<string> checkingSymbols;
	stack<string> outputStack;
	dp(query, implications, initSymbols, checkingSymbols, checkedSymbols, outputStack);
	if (checkedSymbols[query]) {
		while (!outputStack.empty()) {
			cout << outputStack.top() << endl;
			outputStack.pop();
		}
		cout << "--> true" << endl;
	} else {
		cout << "--> false" << endl;
	}
}

void CNFCheck(ifstream &ifs, string query) {
}

int main(int argc, const char* argv[]) {
    string mode(argv[1]);
    ifstream ifs(argv[2]);
    string query(argv[3]);
    if (mode == "forward")
        forwardChainingCheck(ifs, query);
    else if (mode == "backward")
        backwardChainingCheck(ifs, query);
    else if (mode == "CNF")
        CNFCheck(ifs, query);
    return 0;
}
