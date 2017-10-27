// interest_rate_swap.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../xlldatetime/xlldatetime.h"
#include "../fmspwflat/interest_rate_swap.h"

using namespace datetime;
using namespace fixed_income;
using namespace xll;

static AddInX xai_interest_rate_swap(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_interest_rate_swap"), _T("INSTRUMENT.INTEREST.RATE.SWAP"))
	.Arg(XLL_LPOPERX, _T("Effective"), IS_EFFECTIVE)
	.Arg(XLL_LONGX,   _T("Count"), IS_COUNT)
	.Arg(XLL_LONGX, _T("Unit"),  IS_UNIT)
	.Arg(XLL_LONGX, _T("Freq"),  IS_FREQ)
	.Arg(XLL_LONGX, _T("Dcb"),   IS_DCB)
	.Arg(XLL_LONGX, _T("Roll"),  IS_ROLL)
	.Arg(XLL_HANDLEX, _T("Calendar"), IS_CALENDAR _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the handle to an interest rate swap."))
	.Documentation(
		_T("The first argument can also be a six element array of the arguments. ")
	)
);
HANDLEX WINAPI
xll_interest_rate_swap(LPOPERX eff, LONG count, time_unit unit, payment_frequency freq, day_count_basis dcb, roll_convention roll, HANDLEX cal)
{
#pragma XLLEXPORT
	HANDLEX h(0);

	try {
		ensure (size<XLOPERX>(*eff) == 1 || size<XLOPERX>(*eff) == 7);

		if (size<XLOPERX>(*eff) == 7) {
			count = static_cast<LONG>((*eff)[1]);
			unit = static_cast<time_unit>(static_cast<int>((*eff)[2]));
			freq = static_cast<payment_frequency>(static_cast<int>((*eff)[3]));
		    dcb = static_cast<day_count_basis>(static_cast<int>((*eff)[4]));
			roll = static_cast<roll_convention>(static_cast<int>((*eff)[5]));
			cal = static_cast<HANDLEX>((*eff)[6]);
		}
/*		else if (size<XLOPERX>(*eff) == 1) {
			if (eff->xltype == xltypeStr) {
				// ticker?
				LOPERX xEval;
				ensure (xEval = ExcelX(xlfEvaluate, ExcelX(xlfConcatenate, OPERX(_T("=INTEREST.RATE.SWAP.")), safe_name(*eff), OPERX(_T("()")))));

				return xEval.val.num;
			}
			handle<FPX> h_(cal);
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
*/		
		handle<instrument<double,date>> swap =  new interest_rate_swap<>(date((*eff)[0]), count, unit, freq, dcb, roll, HOLIDAY_CALENDAR(cal));
		h = swap.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

