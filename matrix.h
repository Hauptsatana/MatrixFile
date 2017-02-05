#pragma once
#include <vector>
#include "elements.h"

using namespace std;

class Matrix {

private:
	int n;
	int m;
	vector<Nullable*> elements;
	string nullValue;

	vector<DoubleValue*> getDoubleValues();

public:
	Matrix(const int n, const int m, const string nullValue, const vector<Nullable*> elements);

	const int getN() { return n; }
	const int getM() { return m; }
	const string getNullValue() { return nullValue; }


	void multiply(const double modifier);

	const bool hasDoubleValues();

	Nullable* minElement();
	const double minValue();

	Nullable* maxElement();
	const double maxValue();

	Nullable* operator()(int row, int column);
};