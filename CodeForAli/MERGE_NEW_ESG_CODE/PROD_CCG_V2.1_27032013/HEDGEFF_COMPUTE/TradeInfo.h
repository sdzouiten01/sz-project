/******************************************************************************
* Class TradeInfo
*		This function stores infomations for purchasing options, futures, swaps
*		- t: when we buy/sell these equitities
*		- _amount: > 0, amount we hold; < 0, amount we short.
* 30-05-2010
******************************************************************************/
#pragma once
struct TradeInfo{
	TradeInfo( int t = -1, double amt = 0):_time(t), _amount(amt){}
	 int _time;
	double _amount;
};