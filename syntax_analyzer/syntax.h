#pragma once

// Include needed header file  
#include "lexer.h"

// function prototypes 
bool S(vector<tokens>, int, ofstream&);
bool E(vector<tokens>, int, ofstream&);
bool Q(vector<tokens>, int, ofstream&);
bool T(vector<tokens>, int, ofstream&);
bool R(vector<tokens>, int, ofstream&);
bool F(vector<tokens>, int, ofstream&);
bool analyze_syntax(vector<tokens>, int, ofstream&);

// Function is a production rule that checks for assignment operation 
bool S(vector<tokens> token_vect, int index, ofstream& output_file) {

	if (token_vect[index].lexeme == "identifier") {
		if (token_vect.size() > index + 1 && token_vect[index + 1].token == "=") {
			output_file << " <statement> -> <assign>\n <assign> -> <identifier>";
			if (E(token_vect, index + 2, output_file)) {
				output_file << " <expression>\n";
				return true;
			}
		}
	}
	return false;
}

// Second production rule 
bool E(vector<tokens> token_vect, int index, ofstream& output_file) {
	if (T(token_vect, index, output_file)) {
		output_file << "<expression> -> <term>";
		if (Q(token_vect, index + 1, output_file)) {
			output_file << "<expression_prime>\n";
			return true;
		}
	}
	return false;
}

// Third production rule 
bool Q(vector<tokens> token_vect, int index, ofstream& output_file) {
	if (token_vect[index].token == "+") {
		output_file << "+";
		if (index + 1 < token_vect.size() && T(token_vect, index + 1, output_file)) {
			output_file << "<term>";
			if (index + 2 < token_vect.size() && Q(token_vect, index + 2, output_file)) {
				output_file << "<expression prime>";
				return true;
			}
		}
	}
	else if (token_vect[index].token == "-") {
		output_file << "-";
		if (index + 1 < token_vect.size() && T(token_vect, index + 1, output_file)) {
			output_file << "<term>";
			if (index + 2 < token_vect.size() && Q(token_vect, index + 2, output_file)) {
				output_file << "<expression prime>";
				return true;
			}
		}
	}
	else {
		if (index == token_vect.size() - 1) {
			output_file << "epsilon\n";
			return true;
		}
	}
	return false;
}

// Fourth production rule 
bool T(vector<tokens> token_vect, int index, ofstream& output_file) {
	if (F(token_vect, index, output_file)) {
		if (index + 1 < token_vect.size() && R(token_vect, index + 1, output_file)) {
			output_file << "<term> -> <factor> <term prime>\n";
			return true;
		}
	}
	return false;
}

// Fifth production rule 
bool R(vector<tokens> token_vec, int index, ofstream& output_file) {
	if (token_vec[index].token == "*") {
		output_file << "*";
		if (index + 1 < token_vec.size() && F(token_vec, index + 1, output_file)) {
			output_file << "<factor>";
			if (index + 2 < token_vec.size() && R(token_vec, index + 2, output_file)) {
				output_file << "<term prime>\n";
				return true;
			}
		}
	}
	else if (token_vec[index].token == "/") {
		if (index + 1 < token_vec.size() && F(token_vec, index + 1, output_file)) {
			output_file << "<factor>";
			if (index + 2 < token_vec.size() && R(token_vec, index + 2, output_file)) {
				output_file << "<term prime>\n";
				return true;
			}
		}
	}
	else if (index + 1 == token_vec.size()) {
		output_file << "epsilon\n";
		return true;
	}
	else {
		return false;
	}
}

// Sixth production rule
bool F(vector<tokens> token_vec, int index, ofstream& output_file) {
	if (token_vec[index].token == "(") {
		if (index + 1 < token_vec.size() && E(token_vec, index + 1, output_file)) {
			if (index + 2 < token_vec.size() && E(token_vec, index + 2, output_file)) {
				output_file << "( <expression> )\n";
				return true;
			}
		}
	}
	else if (token_vec[index].lexeme == "identifier") {
		output_file << "<identifier>\n";
		return true;
	}
	return false;
}

// function that calls other functions to analyze the syntax
bool analyze_syntax(vector<tokens> token_vect, int index, ofstream& output_file) {
	return S(token_vect, index, output_file)
		|| E(token_vect, index, output_file)
		|| Q(token_vect, index, output_file)
		|| T(token_vect, index, output_file)
		|| R(token_vect, index, output_file)
		|| F(token_vect, index, output_file);
}
