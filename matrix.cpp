#include "matrix.h"
#include <iterator>
#include <algorithm>

using namespace std;

Matrix::Matrix(const int n, const int m, const string nullValue, const vector<Nullable*> elements) {
	this->n = n;
	this->m = m;
	this->nullValue = nullValue;
	this->elements = vector<Nullable*>(elements);
}

void Matrix::multiply(const double modifier) {
	for_each(elements.begin(), elements.end(), [modifier](Nullable* &x) {
		switch (x->getType()) {
		case ElementType::Null:
			return;
		case ElementType::Double:
			DoubleValue* dx = (DoubleValue*)x;
			dx->setValue(dx->getValue() * modifier);
			return;
		}
	});
}

Nullable* Matrix::operator()(int row, int column) {
	int index = row * m + column;
	if (index >= n * m) {
		throw new invalid_argument("Row and column indexes exceed actual matrix size");
	}
	return elements.at(index);
}

// Double values content

vector<DoubleValue*> Matrix::getDoubleValues() {
	vector<Nullable*> filtered;
	copy_if(elements.begin(), elements.end(), back_inserter(filtered),
		[](Nullable* x) { return x->getType() == ElementType::Double; });

	vector<DoubleValue*> res;
	transform(filtered.begin(), filtered.end(), back_inserter(res),
		[](Nullable* x) -> DoubleValue* {return static_cast<DoubleValue*>(x); });
	return res;
}

const bool Matrix::hasDoubleValues() {
	return getDoubleValues().size() != 0;
}

const double getDoubleValue(Nullable* val) {
	double res = -1;
	if (val->getType() == ElementType::Double) {
		res = ((DoubleValue*)val)->getValue();
	}
	return res;
}

// Min/Max

Nullable* Matrix::minElement() {
	vector<DoubleValue*> values = getDoubleValues();
	if (values.size() == 0) {
		return NullValue::getInstance();
	}
	return *min_element(values.begin(), values.end(),
		[](DoubleValue* x, DoubleValue* y) { return x->getValue() < y->getValue(); }
	);
}

Nullable* Matrix::maxElement() {
	vector<DoubleValue*> values = getDoubleValues();
	if (values.size() == 0) {
		return NullValue::getInstance();
	}
	return *min_element(values.begin(), values.end(),
		[](DoubleValue* x, DoubleValue* y) { return x->getValue() > y->getValue(); }
	);
}


const double Matrix::minValue() {
	return getDoubleValue(minElement());
}

const double Matrix::maxValue() {
	return getDoubleValue(maxElement());
}




