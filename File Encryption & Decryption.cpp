#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

fstream sys;  // File stream for system file
ifstream file; // File stream for user file
string line;   // String to store each line read from file
string text;   // String to store text read from file
string key;    // String to store encryption/decryption key
int append = 0; // Variable to store the number of characters appended for padding during encryption

// Function to transpose a 2D vector
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

// Function to copy the contents of the user file into the system file
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

// Function to censor certain words in the user file and store the result in the system file
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

// Function to encrypt the user file and store the result in the system file
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

// Function to decrypt the system file
void decrypt() {
    string decryptedText;

    cout << "Enter the decryption key: ";
    cin >> key;

    file.open("system.txt");
    if (file.is_open()) {
        while (getline(file, text)) {
            int column = key.size();
            int row = text.size() / column;

            vector<vector<char>> cipher(row, vector<char>(column));
		
            int idx = 0;
            for (int i = 0; i < column; ++i) {
                for (int j = 0; j < row; ++j) {
                    cipher[j][i] = text[idx++];
                }
            }

            // Reverse sorting process
            sort(cipher.begin(), cipher.end());

            // Transpose
            cipher = transpose(cipher);

            // Remove padding characters
            for (int i = 0; i < cipher.size(); ++i) {
                cipher[i].erase(cipher[i].begin() + cipher[i].size() - append, cipher[i].end());
            }

            // Construct decrypted text
            for (int i = 0; i < cipher.size(); ++i) {
                for (int j = 0; j < cipher[i].size(); ++j) {
                    decryptedText += cipher[i][j];
                }
            }
        }
        file.close();

        // Write decrypted text to console or file
        //cout << "Decrypted text: " << decryptedText << endl;
        // Or write to a file if needed
	ofstream decryptedFile("decrypted.txt");
        decryptedFile << decryptedText;
        decryptedFile.close();
    } else {
        cout << "System file not found!" << endl;
    }
}

int main() {
	int option = 0; // Variable to store user's option
    	string line;    // String to store each line read from user input

	// Display menu options
	cout << "Enter a number 1-4 for the options below: " << endl;
	cout << "1. Copy the contents of the user file into the system file." << endl;
	cout << "2. Censor certain words in the user file and store the result in system file." << endl;
	cout << "3. Encrypt the user file and store the result in system file." << endl;
	cout << "4. Decrypt the system file." << endl;
	cout << "Answer: ";
	cin >> option;

	// Perform actions based on user's option
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

