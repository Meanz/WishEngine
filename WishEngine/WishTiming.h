#pragma once

#include <Windows.h>

namespace Wish
{
	struct wish_timer
	{
		LARGE_INTEGER t1, t2, frequency;
		double elapsedTime;
	};

	void Wish_Timer_Start(wish_timer* pTimer);
	double Wish_Timer_Stop(wish_timer* pTimer);
	double Wish_Timer_GetValue(wish_timer* pTimer);
}

