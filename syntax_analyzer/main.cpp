// Include libraries 
#include <iomanip>
using std::setw;
using std::right;
using std::left;
#include <sstream>
using std::stringstream;
#include <locale>
#include <ctype.h>


// Include header files, syntax.h includes lexer.h so it can be omitted here 
//#include "lexer.h"
#include "syntax.h"


// Main function definition 
int main() {

	// Define data and get file name  
	string file_name;
	vector<tokens> token_vect;
	cout << "File name: ";
	cin >> file_name;

	// Open input file
	ifstream input_file(file_name);
	ofstream output_file("output.txt");
	if (!input_file.is_open()) {
		cerr << "Could not open file " + file_name + "!" << endl;
		return -1;
	}

	// Print to output file 
	output_file << "--------------------------------" << endl;
	output_file << left << setw(15) << "TOKENS" << setw(10) << "" << "Lexemes" << endl;
	output_file << "--------------------------------" << endl << endl;

	// Get lines of text from input file 
	string token = "", line = "";
	while (std::getline(input_file, line)) {
		string temp = line;

		// Strip out comments 
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] == '!') {
				// Found beginig of comment 
				temp = strip_comments(string(temp.begin() + i + 1, temp.end()), input_file);
				break;
			}
		}

		// Format separators
		temp = format_separators(temp);

		// Parse line into tokens
		stringstream ss(temp);
		while (ss >> token) {
			if (token.length() == 1) {
				if (is_operator(token[0])) {
					output_file << left << setw(15) << "OPERATOR" << setw(10) << " " << token << endl;
					token_vect.push_back(tokens(token, "operator"));
				}
				else if (is_separator(token[0]) || token[0] == '.') {
					output_file << left << setw(15) << "SEPARATOR" << setw(10) << " " << token << endl;
					token_vect.push_back(tokens(token, "separator"));
				}
				else {
					output_file << lexer(token, token_vect);
				}
			}
			else if (is_keyword(token)) {
				output_file << left << setw(15) << "KEYWORD" << setw(10) << " " << token << endl;
				token_vect.push_back(tokens(token, "keyword"));
			}
			else {
				output_file << lexer(token, token_vect);
			}
		}
	}
	// Close the input file 
	input_file.close();

	// Display the contents of the tokens array to standard output 
	cout << "--------------------------------" << endl;
	cout << left << setw(15) << "TOKENS" << setw(10) << " " << "Lexemes" << endl;
	cout << "--------------------------------" << endl << endl;
	for (int i = 0; i < token_vect.size(); i++) {
		cout << left << setw(15) << token_vect[i].token << setw(10) << " " << token_vect[i].lexeme << endl;
	}
	// Check if the syntax is accepted
	if (!analyze_syntax(token_vect, 0, output_file)) {
		output_file << "ERROR: syntax error found in the source code" << endl;
	}

	// main end 
	return 0;
}