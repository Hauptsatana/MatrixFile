#include "elements.h"

// NullValue

NullValue* NullValue::instance = new NullValue();

NullValue* NullValue::getInstance() {
	return instance;
}

const ElementType NullValue::getType() {
	return ElementType::Null;
}

// DoubleValue

DoubleValue::DoubleValue(const double value) {
	this->value = value;
}

const double DoubleValue::getValue() {
	return value;
}

void DoubleValue::setValue(const double value) {
	this->value = value;
}

const ElementType DoubleValue::getType() {
	return ElementType::Double;
}