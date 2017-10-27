// xllpwflat.h - Piecewise flat forward curve.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
//#define EXCEL12
#include "xll/xll.h"
#include "../fmspwflat/pwflat.h"

#ifndef CATEGORY
#define CATEGORY _T("PiecewiseFlat")
#endif

typedef xll::traits<XLOPERX>::xfp xfp;
typedef xll::traits<XLOPERX>::xword xword;

#define IS_EFFECTIVE _T("is the date at which the first cash flow occurs")
#define IS_SETTLEMENT _T("is the date at which the yield curve discount is 1")
#define IS_VALUATION _T("is the date at which the quote is made")
#define IS_PRICE _T("is the 100 based price")
#define IS_RATE  _T("is the interest rate")
#define IS_FORWARD  _T("is the forward rate")
#define IS_COUPON  _T("is the par coupon")
#define IS_COUNT _T("is the number of Units")
#define IS_UNIT  _T("is the UNIT_* enumeration corresponding to Count")
#define IS_FREQ  _T("is the FREQ_* enumeration corresponding to the payment frequency of the fixed leg")
#define IS_DCB   _T("is the DCB_* enumeration daycount basis")
#define IS_ROLL  _T("is the ROLL_* enumeration date rolling convention")
#define IS_CALENDAR   _T("is the CALENDAR_* enumeration holiday calendar")
#define IS_TIMES	_T("is an array of cash flow times")
#define IS_AMOUNTS	_T("is an array of cash flow amounts ")

inline LOPERX 
safe_name(const OPERX& o) 
{ 
	return xll::ExcelX(xlfSubstitute, o, OPERX(_T("/")), OPERX(_T("_")));
}

#define INSTRUMENT_HANDLE(type, ticker, instrument) \
static AddInX xai_instrument_##ticker( \
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T(XLL_STRZ_(_xll_instrument_ ## ticker ## @0)), _T(XLL_STRZ_(type ## . ## ticker))) \
	.Category(_T("BBG")).FunctionHelp("Indicative data from Bloomberg ticker. ").Documentation()); \
extern "C" __declspec(dllexport) HANDLEX WINAPI xll_instrument_ ## ticker(void) \
{ return handle<fixed_income_instrument>(new instrument).get(); }
