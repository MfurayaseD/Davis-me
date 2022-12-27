#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

fstream sys;
ifstream file;
string line;
string text;
string key;
int append = 0;

vector<vector<char>> transpose(vector<vector<char>>& A) {
	int rows = A.size();
	if (rows == 0) return { {} };
	int cols = A[0].size();
	vector<vector<char>> r(cols, vector<char>(rows));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			r[j][i] = A[i][j];
		}
	}

	return r;
}

void StoreUnchanged() {
	file.open("user.txt");
	sys.open("system.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			sys << line;
		}
	}
	else {
		cout << "File does not exist in the directory!" << endl;
	}
	file.close();
	sys.close();
}

void censor() {
	string word;
	string input;
	vector <string> censored_words;

	cout << "Enter a list of words to censor separated by a comma: ";
	getline(cin >> ws,input);	
	
	stringstream ss(input);
	while (getline(ss, word, ',')) {
		word.erase(0, word.find_first_not_of(" \t\n\r\f\v"));
		word.erase(word.find_last_not_of(" \t\n\r\f\v") + 1);
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		censored_words.push_back(word);
	}

	file.open("user.txt");
	sys.open("system.txt");

	if (file.is_open()) {
		while (getline(file, line)) {
			stringstream ss(line);
			while (ss >> word) {
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				bool censored = false;
				for (const auto& censored_word : censored_words) {
					if (word == censored_word) {
						censored = true;
						break;
					}
				}
				if (censored) {
					string asterisk(word.size(), '*');
					sys << asterisk << " ";
				}
				else {
					sys << word << " ";
				}
			}
			sys << endl;
		}
	}
	else {
		cout << "File does not exist in the directory!" << endl;
	}
	file.close();
	sys.close();
}


void encrypt() {
	cout << "Enter the encryption key: ";
	cin >> key;
	int column = key.size();

	file.open("user.txt");
	sys.open("system.txt");
	if (file.is_open()) {
		while (getline(file, text)) {
			text = key + text;
			if (text.size() % column != 0) {
				append = column - (text.size() % column);
				for (int count = 0; count < append; count++) {
					text = text + "-";
				}
			}
			int row = text.size() / column;

			vector<vector<char>> vec(row);
			int index = 0;
			char* char_arr;
			char_arr = &text[0];
			for (int i = 0; i < row; i++) {
				int col = column;
				vec[i] = vector<char>(col);
				for (int j = 0; j < col; j++) {
					if (char_arr[index] != ' ') {
						vec[i][j] = char_arr[index];
						index++;
					}
					else {
						vec[i][j] = '-';
						index++;
					}

				}
			}
			vector<vector<char>> cipher;
			cipher = transpose(vec);
			int idx = 0;
			sort(cipher.begin(), cipher.end(), [idx](const vector<char>& a, const vector<char>& b) {
				return a.at(idx) < b.at(idx);
				});
			int columnIndex = 0;
			for_each(cipher.begin(), cipher.end(),
				[&](vector<char>& row) {
					row.erase(next(row.begin(), columnIndex));
				});
			for (int i = 0; i < cipher.size(); i++) {
				for (int j = 0; j < cipher[i].size(); j++) {
					sys << cipher[i][j];
				}
			}
		}
		file.close();
		sys.close();
		cout << "File encrypted! " << endl;
	}
	else {
		cout << "File not found! " << endl;
	}
}

void decrypt() {
	// decrypt

}


int main() {
	int option = 0;
	string line;
	cout << "Enter a number 1-4 for the options below: " << endl;
	cout << "1. Copy the contents of the user file into the system file." << endl;
	cout << "2. Censor certain words in the user file and store the result in system file." << endl;
	cout << "3. Encrypt the user file and store the result in system file." << endl;
	cout << "4. Decrypt the system file." << endl;
	cout << "Answer: ";
	cin >> option;

	switch (option)
	{
	case 1:
		StoreUnchanged();
		break;
	case 2:
		censor();
		break;
	case 3:
		encrypt();
		break;
	case 4:
		decrypt();
		break;
	default:
		cout << "Invalid option. Please enter a number between 1 and 4!";
		break;
	}

}

