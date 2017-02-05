
#include <string>
#include "ioops.h"
#include <cstdio>

using namespace std;

void test(string readFrom, string writeTo) {
	try {
		Matrix* matr = loadMatrix(readFrom);
		if (matr->hasDoubleValues()) {
			printf("Min: %f, Max: %f\n", matr->minValue(), matr->maxValue());
		}
		matr->multiply(10);
		storeMatrix(*matr, writeTo);
	} catch (int e) {
		switch (e) {
			case MF_INVALID_HEADER_EXCEPTION:
				printf("File has invalid header. Try another file.\n");
				break;
			case MF_INVALID_CONTENT_EXCEPTION:
				printf("File has invalid content. Try another file.\n");
				break;
			case MF_UNEXPECTED_CONTENT_LENGTH_EXCEPTION:
				printf("Matrix size does not match file content length.\n");
				break;
			default:
				printf("Undefined exception.\n");
				break;
		}
	}
}

const string fileDir = "E:\\Git\\projects\\MatrixFile\\Debug\\";

void main(int argc, char** argv) {
	test(fileDir + "test.txt", fileDir + "1.txt");
	test(fileDir + "1.txt", fileDir + "2.txt");
}