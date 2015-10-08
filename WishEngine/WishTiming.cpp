#include "stdafx.h"
#include "WishTiming.h"

void Wish::Wish_Timer_Start(wish_timer* pTimer) {
	QueryPerformanceFrequency(&pTimer->frequency);
	QueryPerformanceCounter(&pTimer->t1);
}

double Wish::Wish_Timer_Stop(wish_timer* pTimer) {
	QueryPerformanceCounter(&pTimer->t2);
	pTimer->elapsedTime = (double)(pTimer->t2.QuadPart - pTimer->t1.QuadPart) / (pTimer->frequency.QuadPart / 1000);
	return pTimer->elapsedTime;
}

double Wish::Wish_Timer_GetValue(wish_timer* pTimer) {
	return pTimer->elapsedTime;
}