#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Implication {
    string sentence;
    unordered_set<string> premises;
    string conclusion;
    Implication(string s, unordered_set<string> p, string c) :
        sentence(s), premises(p), conclusion(c) {}
};

struct Clause {
	vector<string> symbols;
	bool operator==(const Clause &rhs) {
		return this->symbols == rhs.symbols;
	}
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

Clause getClause(string disjunction) {
	Clause clause;
	for (int i = 0; i < disjunction.size(); i++)
		if (disjunction[i] == '(' || disjunction[i] == ')' || disjunction[i] == 'v' || disjunction[i] == '^') 
			disjunction[i] = ' ';
	stringstream ss(disjunction);
	string symbol;
	while (ss >> symbol) {
		clause.symbols.push_back(symbol);
	}
	return clause;
}

vector<Clause> convertToClauses(ifstream &ifs, string query) {
	vector<Clause> clauses;
	string sentence;
	getline(ifs, sentence);
	if (query[0] == '~')
		query.erase(query.begin());
	else
		query = '~' + query;
	sentence += " ^ " + query + " ^";
	sentence = "^ " + sentence;
	int prevIndex = 0;
	for (int i = 1; i < sentence.size(); i++)
		if (sentence[i] == '^') {
			clauses.push_back(getClause(sentence.substr(prevIndex, i - prevIndex + 1)));
			prevIndex = i;
		}
	return clauses;
}

void printAClause(Clause clause) {
	if (clause.symbols.size() == 1) {
		cout << clause.symbols[0];
	} else {
		cout << "( ";
		for (int i = 0; i < clause.symbols.size(); i++)
			if (i != clause.symbols.size() - 1)
				cout << clause.symbols[i] << " v ";
			else	
				cout << clause.symbols[i];
		cout << " )";
	}
}

bool areComplementary(string symbol0, string symbol1) {
	if (symbol0[0] == '~' && symbol1[1] != '~' && symbol0.substr(1) == symbol1)
		return true;
	if (symbol0[0] != '~' && symbol1[0] == '~' && symbol1.substr(1) == symbol0)
		return true;
	return false;
}

Clause resolveTwoClauses(Clause clause0, Clause clause1) {
	sort(clause0.symbols.begin(), clause0.symbols.end());
	sort(clause0.symbols.begin(), clause0.symbols.end());
	if (clause0.symbols == clause1.symbols)
		return clause0;
	Clause newClause;
	if (clause0.symbols.empty() && clause1.symbols.empty())
		return newClause;
	if (clause0.symbols.empty() && !clause1.symbols.empty())
		return clause1;
	if (!clause0.symbols.empty() && clause1.symbols.empty())
		return clause0;
	int i = 0;
	int j = 0;
	if (clause0.symbols[i] <= clause1.symbols[j]) {
		newClause.symbols.push_back(clause0.symbols[i]);
		i++;
	} else {
		newClause.symbols.push_back(clause1.symbols[j]);
		j++;
	}
	int index = 0;
	while (i < clause0.symbols.size() && j < clause1.symbols.size()) {
		if (clause0.symbols[i] <= clause1.symbols[j]) {
			if (newClause.symbols[index] != clause0.symbols[i]) {
				newClause.symbols.push_back(clause0.symbols[i]);
				index++;
			}
			i++;
		} else {
			if (newClause.symbols[index] != clause1.symbols[j]) {
				newClause.symbols.push_back(clause1.symbols[j]);
				index++;
			}
			j++;
		}
	}
	while (i < clause0.symbols.size()) {
		if (newClause.symbols[index] != clause0.symbols[i]) {
			newClause.symbols.push_back(clause0.symbols[i]);
			index++;
		}
		i++;
	}
	while (j < clause1.symbols.size()) {
		if (newClause.symbols[index] != clause1.symbols[j]) {
			newClause.symbols.push_back(clause1.symbols[j]);
			index++;
		}
		j++;
	}
	return newClause;
}

vector<Clause> plResolve(Clause clause0, Clause clause1) {
	vector<Clause> resolvents;
	for (int i = 0; i < clause0.symbols.size(); i++)
		for (int j = 0; j < clause1.symbols.size(); j++) {
			string symbol0 = clause0.symbols[i];
			string symbol1 = clause1.symbols[j];
			if (areComplementary(symbol0, symbol1)) {
				cout << "resolve: ";
				printAClause(clause0);
				cout << " and ";
				printAClause(clause1);
				clause0.symbols.erase(clause0.symbols.begin() + i);
				clause1.symbols.erase(clause1.symbols.begin() + j);
				Clause newClause = resolveTwoClauses(clause0, clause1);
				if (newClause.symbols.empty()) {
					cout << " --> NULL" << endl;
					return resolvents;
				} else {
					resolvents.push_back(newClause);
					cout << " --> ";
					printAClause(newClause);
					cout << endl;
					return resolvents;
				}
			}
		}
	resolvents.push_back(clause0);
	resolvents.push_back(clause1);
	return resolvents;
}

bool hasClause(vector<Clause> &mergedClauses, Clause clause) {
	for (int i = 0; i < mergedClauses.size(); i++)
		if (mergedClauses[i].symbols == clause.symbols)
			return true;
	return false;
}

vector<Clause> mergeClauses(vector<Clause> clauses0, vector<Clause> clauses1) {
	for (int i = 0; i < clauses0.size(); i++)
		sort(clauses0[i].symbols.begin(), clauses0[i].symbols.end());
	for (int i = 0; i < clauses1.size(); i++)
		sort(clauses1[i].symbols.begin(), clauses1[i].symbols.end());
	vector<Clause> mergedClauses = clauses0;
	for (int i = 0; i < clauses1.size(); i++)
		if (!hasClause(mergedClauses, clauses1[i]))
			mergedClauses.push_back(clauses1[i]);
	return mergedClauses;
}

void printClauses(vector<Clause> clauses) {
	for (int i = 0; i < clauses.size(); i++) {
		printAClause(clauses[i]);
		cout << " ";
	}
	cout << endl;
}

void CNFCheck(ifstream &ifs, string query) {
	vector<Clause> clauses = convertToClauses(ifs, query);
	//printClauses(clauses);
	//return;
	vector<Clause> newClauses;
	while (true) {
		for (int i = 0; i < clauses.size(); i++)
			for (int j = i + 1; j < clauses.size(); j++) {
				vector<Clause> resolvents = plResolve(clauses[i], clauses[j]);
				if (resolvents.empty()) {
					cout << "--> true" << endl;
					return;
				}
				newClauses = mergeClauses(newClauses, resolvents);
				//cout << "newClauses: ";
				//printClauses(newClauses);
			}
		if (newClauses.size() <= clauses.size()) {
			cout << "--> false" << endl;
			return;
		}
		clauses = mergeClauses(clauses, newClauses);
	}
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
