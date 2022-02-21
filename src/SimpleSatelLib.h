#ifndef SATEL_INT_RS_LIB
#define SATEL_INT_RS_LIB

#include "command/OutputsStateCommand.h"
#include "command/ZonesViolationCommand.h"
#include "SimpleSatelFrame.h"

#define MAX_ANSWER_AWAIT 3000

typedef uint8_t (*Writer)(byte*, uint8_t);
typedef byte (*ReaderRead)();
typedef uint8_t (*ReaderAvailable)();

template<class T>
class Result {
	bool success;
	T data;
public:
	Result(T data) :
			success(true), data(data) {
	}
	Result(const Result<T> &toCopy) :
			success(toCopy.success), data(toCopy.data) {
	}
	Result() :
			success(false) {
	}
	inline bool isSuccess() {
		return success;
	}
	inline T getData() {
		return data;
	}
	void setData(T data)  {
		this->data = data;
		this->success = true;
	}
};

class SimpleSatelLibClass {

	Writer writeData;
	ReaderRead readData;
	ReaderAvailable isDataAvailable;

	bool readAnswer(byte *bytes, int &readCount);
	int readBytes(byte *bytes);
	bool processCommand(SSatel::Command command, SSatel::Answer &answer);

public:
	SimpleSatelLibClass() :
			writeData(NULL), readData(NULL), isDataAvailable(NULL) {
	}

	Result<SSatel::OutputsStateAnswer> readOutputsState();
	Result<SSatel::ZonesViolationAnswer> readZonesViolation();

	inline void setWriter(Writer writer) {
		this->writeData = writer;
	}
	inline void setReader(ReaderRead reader) {
		this->readData = reader;
	}
	inline void setChecker(ReaderAvailable checker) {
		this->isDataAvailable = checker;
	}
};

extern SimpleSatelLibClass SimpleSatel;

#endif /* SATEL_INT_RS_LIB */
