#pragma once

#include <string>
#include <vector>
#include "elements.h"

using namespace std;

const enum class LineType {
	Empty,
	Size,
	NullValue,
	Elements,
	Undefined
};

class FileLine {

protected:
	string original;

public:
	FileLine(const string original) {
		this->original = original;
	}

	const string getOriginal() {
		return string(original);
	}

	const virtual LineType getType() = 0;

};

class EmptyLine : public FileLine {

public:
	EmptyLine(const string original) : FileLine(original) {}

	const LineType getType() {
		return LineType::Empty;
	}
};

class SizeLine : public FileLine {

private:
	int n;
	int m;

public:
	SizeLine(const string original, const int n, const int m) : FileLine(original) {
		this->n = n;
		this->m = m;
	}

	const int getN() { return n; }
	const int getM() { return m; }

	const LineType getType() {
		return LineType::Size;
	}
};

class NullValueLine : public FileLine {

private:
	string nullValue;

public:
	NullValueLine(const string original, const string nullValue) : FileLine(original) {
		this->nullValue = nullValue;
	}

	const string getNullValue() {
		return string(nullValue);
	}

	const LineType getType() {
		return LineType::NullValue;
	}

};

class ElementsLine : public FileLine {

private:
	vector<Nullable*> elements;

public:
	ElementsLine(const string original, const vector<Nullable*> elements) : FileLine(original) {
		this->elements = vector<Nullable*>(elements);
	}

	const vector<Nullable*> getElements() {
		return vector<Nullable*>(elements);
	}

	const LineType getType() {
		return LineType::Elements;
	}

};

class UndefinedLine : public FileLine {

public:
	UndefinedLine(const string original) : FileLine(original) { }

	const LineType getType() {
		return LineType::Undefined;
	}

};