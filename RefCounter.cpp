#include "stdafx.h"
#include "RefCounter.h"

RefCounter::RefCounter()
{
	::InitializeCriticalSection(&cs);
	refCount = 0;
}

RefCounter::~RefCounter()
{
	::DeleteCriticalSection(&cs);
}

void RefCounter::Enter()
{
	::EnterCriticalSection(&cs);
}

void RefCounter::Leave()
{
	::LeaveCriticalSection(&cs);
}

void RefCounter::AddRef()
{
	::EnterCriticalSection(&cs);

	if (refCount == 0)
	{
		//FPDF_InitLibrary();
	}

	refCount++;

	::LeaveCriticalSection(&cs);
}

void RefCounter::Release()
{
	::EnterCriticalSection(&cs);

	refCount--;

	if (refCount == 0)
	{
		//FPDF_DestroyLibrary();
	}
	::LeaveCriticalSection(&cs);
}