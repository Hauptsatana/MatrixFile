#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "matrix.h"
#include "lines.h"

using namespace std;

// input/output operations

#define MF_INVALID_HEADER_EXCEPTION 100
#define MF_INVALID_CONTENT_EXCEPTION 200
#define MF_UNEXPECTED_CONTENT_LENGTH_EXCEPTION 300

const char* SIZE_TOKEN = "size";
const char* NULL_VALUE_TOKEN = "null";

// Whole string is integer
bool tryParseInt(const char* str, int &res) {
	if (strlen(str) < 1 || str[0] < '0' || str[0] > '9') {
		return false;
	}
	size_t next;
	try {
		res = stoi(string(str), &next);
	}
	catch (exception e) {
		return false;
	}
	if (next == strlen(str)) {
		return true;
	}
	return false;
}

// Whole string is dot-/comma-separated double
bool tryParseDouble(const char* str, double &res) {

	if (strlen(str) < 1 || !(str[0] >= '0' && str[0] <= '9' || str[0] == '.' || str[0] == ',')) {
		return false;
	}

	string copy(str);
	replace(copy.begin(), copy.end(), ',', '.');
	size_t next;
	try {
		res = stod(copy, &next);
	} catch (exception e) { // can be thrown sometimes from stod when string is invalid
		return false;
	}
	if (next == copy.size()) {
		return true;
	}
	return false;
}

// !!! memory release issue
// returns EmptyLine, SizeLine, NullValueLine or UndefinedLine
FileLine* parseHeaderLine(const string line) {
	char* cline = new char[line.length()];
	strcpy(cline, line.data());
	
	const char* delim = " \t";
	char* token;
	token = strtok(cline, delim);

	if (token == NULL) {
		return new EmptyLine(line);
	}
	
	if (strcmp(token, SIZE_TOKEN) == 0) {
		// read and check N and M
		char* ntok = strtok(NULL, delim);
		char* mtok = strtok(NULL, delim);
		char* rest = strtok(NULL, delim);
		int n;
		int m;
		bool ok = false;
		if (ntok != NULL && mtok != NULL 
				&& tryParseInt(ntok, n) && tryParseInt(mtok, m) 
				&& rest == NULL) {
			return new SizeLine(line, n, m);
		} else {
			return new UndefinedLine(line);
		}
	}

	if (strcmp(token, NULL_VALUE_TOKEN) == 0) {
		// read nullValue;
		char* nulltok = strtok(NULL, delim);
		char* rest = strtok(NULL, delim);
		bool ok = false;
		if (nulltok != NULL && rest == NULL) {
			return new NullValueLine(line, string(nulltok));
		} else {
			return new UndefinedLine(line);
		}
	}
	return new UndefinedLine(line);
}

// return EmptyLine, ElementsLine or Undefined
FileLine* parseContentLine(const string line, string nullValue) {
	char* cline = new char[line.length()];
	strcpy(cline, line.data());

	const char* delim = " \t";
	char* token;
	token = strtok(cline, delim);

	if (token == NULL) {
		return new EmptyLine(line);
	}

	// parse elements before the end
	vector<Nullable*> elements;
	while (token != NULL) {
		double elem;
		if (tryParseDouble(token, elem)) {
			elements.push_back(new DoubleValue(elem));
		} else if (strcmp(token, nullValue.data()) == 0) {
			elements.push_back(NullValue::getInstance());
		} else {
			return new UndefinedLine(line);
		}
		token = strtok(NULL, delim);
	}

	return new ElementsLine(line, elements);
}

// read matrix from valid file, throw exception unless invalid
Matrix* loadMatrix(string filename) {
	ifstream input(filename);
	string line;
	
	// read header
	bool sizeRead = false;
	bool nullRead = false;
	int n = 0;
	int m = 0;
	string nullValue;
	while((!sizeRead || !nullRead) && getline(input, line)) {
		FileLine* parsed = parseHeaderLine(line);
		switch (parsed->getType()) {
			case LineType::Empty: 
				break;
			case LineType::Undefined:
				throw MF_INVALID_HEADER_EXCEPTION;
			case LineType::Size:
				if (sizeRead) {
					throw MF_INVALID_HEADER_EXCEPTION;
				}
				sizeRead = true;
				n = ((SizeLine*)parsed)->getN();
				m = ((SizeLine*)parsed)->getM();
				break;
			case LineType::NullValue:
				if (nullRead) {
					throw MF_INVALID_HEADER_EXCEPTION;
				}
				nullRead = true;
				nullValue = ((NullValueLine*)parsed)->getNullValue();
				break;
		}
	}

	// check header
	if (!sizeRead || !nullRead) {
		throw MF_INVALID_HEADER_EXCEPTION;
	}

	// read content
	vector<Nullable*> elements;
	while (getline(input, line)) {
		FileLine* parsed = parseContentLine(line, nullValue);
		switch (parsed->getType()) {
			case LineType::Empty:
				break;
			case LineType::Undefined:
				throw MF_INVALID_CONTENT_EXCEPTION;
			case LineType::Elements:
				vector<Nullable*> parsedElements = ((ElementsLine*)parsed)->getElements();
				elements.insert(elements.end(), parsedElements.begin(), parsedElements.end());
		}
	}
	input.close();

	// check content length
	if (elements.size() != n * m) {
		throw MF_UNEXPECTED_CONTENT_LENGTH_EXCEPTION;
	}

	return new Matrix(n, m, nullValue, elements);
}

void storeMatrix(Matrix& matr, string filename) {
	int n = matr.getN();
	int m = matr.getM();
	string nullValue = matr.getNullValue();
	ofstream output(filename);
	output << SIZE_TOKEN << " " << n << " " << m << "\n";
	output << NULL_VALUE_TOKEN << " " << nullValue << "\n";
	output << "\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Nullable* val = matr(i, j);
			switch (val->getType())
			{
				case ElementType::Null:
					output << nullValue << " ";
					break;
				case ElementType::Double:
					output << ((DoubleValue*)val)->getValue() << " ";
					break;
			}
		}
		output << "\n";
	}
	output.close();
}