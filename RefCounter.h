#pragma once

class RefCounter
{
private:
	CRITICAL_SECTION cs;
	int refCount;

public:
	RefCounter();

	~RefCounter();

	void Enter();

	void Leave();

	void AddRef();

	void Release();
};