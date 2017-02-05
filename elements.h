#pragma once

const enum class ElementType {
	Null = 0,
	Double = 1
};

class Nullable {

public:
	const virtual ElementType getType() = 0;
};

class NullValue : public Nullable {

private:
	static NullValue* instance;

	NullValue() { }
	NullValue(const NullValue& x) { }
	NullValue(NullValue&& x) { }

public:
	static NullValue* getInstance();
	const ElementType getType();

};

class DoubleValue : public Nullable {

private:
	double value = 0;

public:
	DoubleValue() { }
	DoubleValue(const double value);

	const double getValue();
	void setValue(const double value);
	
	const ElementType getType();
};
