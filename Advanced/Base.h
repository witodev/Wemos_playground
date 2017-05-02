#pragma once
class Base
{
public:
	Base();
	virtual ~Base();

	virtual bool check() = 0;
	virtual void init() = 0;
	virtual void loop() = 0;
	bool OK;
};

