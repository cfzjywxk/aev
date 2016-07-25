#ifndef __SAMPLECLASS_H_
#define __SAMPLECLASS_H_

class sampleClass {
public:
	sampleClass() :
			value_(0)
	{
	}
	sampleClass(int var) :
			value_(var)
	{
	}
	int getPlusOne();
private:
	int value_;
};

#endif
