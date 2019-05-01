#pragma once

// Include librariea
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;

// Define token structure. This holds tokens and lexemes
struct tokens {
	string token;
	string lexeme;
	tokens(string tok, string lex) {
		token = tok, lexeme = lex;
	}
};

// function prototypes 
string strip_comments(string, ifstream&);
string format_separators(string);
bool is_separator(char);
bool is_operator(char);
bool is_keyword(string);
bool contains_period(string);
string lexer(string, vector<tokens>&);


// Function that removes comments from the input file 
// then returns the string following the delimiter '!'
string strip_comments(string input, ifstream& stream) {

	bool found = false;

	do {

		for (int i = 0; i < input.length(); i++) {
			if (input[i] == '!') {
				found = true;
				if (i == input.length() - 1) {
					input = "";
				}
				else {
					input = string(input.begin() + i + 1, input.end());
				}
				break;
			}
		}
		if (found) {
			break;
		}
	} while (getline(stream, input));

	if (!found) {
		input = "";
	}
	return input;
}

// Function that formats delimiters on the input file 
// returns the formatted string 
string format_separators(string input) {
	for (int i = 0; i < input.length(); i++) {
		if (is_separator(input[i])) {
			if (i == 0) {
				input = string(" ") + input[0] + string(" ") + string(input.begin() + 1, input.end());
			}
			else if (i == input.length() - 1) {
				input = string(input.begin(), input.begin() + i) + string(" ") + input[i] + string(" ");
			}
			else {
				string temp = string(input.begin() + i + 1, input.end());
				input = string(input.begin(), input.begin() + i) + string(" ") + input[i] + string(" ") + temp;
			}
			i += 2;
		}
	}
	return input;
}

// Function that determines if a char is a separator
bool is_separator(char input) {
	const int NUM_SEPARATORS = 11;
	char separators[NUM_SEPARATORS] = { '(', ')', ',', ':', ';', '[', ']', '{', '}', '\'' };

	for (int i = 0; i < NUM_SEPARATORS; i++) {
		if (input == separators[i]) {
			return true;
		}
	}
	return false;
}

// Function that determies if a char is an operator 
bool is_operator(char input) {
	const int NUM_SEPARATORS = 8;
	char ops[NUM_SEPARATORS] = { '*', '+', '-', '=', '/', '>', '<', '%' };
	for (int i = 0; i < NUM_SEPARATORS; i++) {
		if (input == ops[i]) {
			return true;
		}
	}
	return false;
}

// Function that determines if token is a keyword
bool is_keyword(string input) {
	const int SIZE = 15;
	string keywords[SIZE] = { "int", "float", "bool", "if", "else", "then", "do", "while", "whileend", "do", "doend", "for", "and", "or", "function" };
	for (int i = 0; i < SIZE; i++) {
		if (input == keywords[i])
			return true;
	}
	return false;

}

// Function checks if the input string contains a period 
bool contains_period(string input) {
	bool has_period = false;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '.')
			has_period = true;
	}
	return has_period;
}

// Function finds the next state based on the current input
string lexer(string input, vector<tokens>& token_vect) {

	// Declare the available states and inputs 
	enum identifier_inputs { id_letter, id_digit, id_dollar, id_other };
	enum identifier_states { id_one, id_two, id_three };
	enum real_inputs { r_plus, r_minus, r_digit, r_other, r_period };
	enum real_states { r_one, r_two, r_three, r_four, r_five };
	enum integer_inputs { int_plus, int_minus, int_digit, int_other };
	enum integer_states { int_one, int_two, int_three };

	// Initialize stae transition table  and variables 
	identifier_states id_table[3][4] = { id_two, id_three, id_three, id_three, id_two, id_two, id_two, id_three, id_three, id_three, id_three, id_three };
	real_states rs_table[5][5] = { r_two, r_two, r_two, r_five, r_five, r_five, r_five, r_two, r_five, r_three, r_five, r_five, r_four,
		r_five, r_five, r_five, r_five, r_four, r_five, r_five, r_five, r_five, r_five, r_five, r_five };
	integer_states int_table[3][4] = { int_two, int_two, int_two, int_three, int_three, int_three, int_two, int_three, int_three, int_three, int_three, int_three };
	string token_lexeme = "";
	identifier_states curr_state = id_one;
	identifier_inputs curr_input;

	// Get char of input string, figure out lexeme with state machine  
	for (int i = 0; i < input.size(); i++) {
		// Find current input 
		char test_char = input[0];
		if (isalpha(test_char))
			curr_input = id_letter;
		else if (isdigit(test_char))
			curr_input = id_digit;
		else if (test_char == '$')
			curr_input = id_dollar;
		else
			curr_input = id_other;
		// Look at transition table to get new state
		curr_state = id_table[curr_state][curr_input];
	}

	// Check for begining of identifier 
	if (curr_state == id_two) {
		token_lexeme = "IDENTIFIER               " + input + "\n";
		token_vect.push_back(tokens(input, "identifier"));
	}
	else {
		// Check for a real number 
		real_states r_curr_state = r_one;
		real_inputs r_curr_input;

		// Find the current input 
		for (int i = 0; i < input.length(); i++) {
			char test_char = input[i];
			if (test_char == '+')
				r_curr_input = r_plus;
			else if (test_char == '-')
				r_curr_input = r_minus;
			else if (isdigit(test_char))
				r_curr_input = r_digit;
			else if (test_char == '.')
				r_curr_input = r_other;
			// find the new state 
			r_curr_state = rs_table[r_curr_state][r_curr_input];
		}

		// Found a real nubmber (float)
		if (r_curr_state == r_four) {
			token_lexeme = "REAL NUMBER              " + input + "\n";
			token_vect.push_back(tokens(input, "real number"));
		}
		else {
			// Look for integer 
			integer_states i_curr_state = int_one;
			integer_inputs i_curr_input;
			// Find current input
			for (int i = 0; i < input.length(); i++) {
				char test_char = input[i];
				if (test_char == '+')
					i_curr_input = int_plus;
				else if (test_char == '-')
					i_curr_input = int_minus;
				else if (isdigit(test_char))
					i_curr_input = int_digit;
				else
					i_curr_input = int_other;
				// Move to next state 
				i_curr_state = int_table[i_curr_state][i_curr_input];
			}
			// Found an integer 
			if (i_curr_state == int_two) {
				token_lexeme = "INTEGER                  " + input + "\n";
				token_vect.push_back(tokens(input, "integer"));
			}
			else if (contains_period(input)) {
				token_lexeme = "SEPARATOR                " + input + "\n";
				token_vect.push_back(tokens(input, "separator"));
			}
		}
	}
	return token_lexeme;
}