// interest_rate_swap.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmsdatetime/calendar.h"
#include "../fmspwflat/interest_rate_swap.h"

using namespace datetime;
using namespace instrument;
using namespace xll;

static AddInX xai_interest_rate_swap(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_interest_rate_swap"), _T("INTEREST.RATE.SWAP"))
	.Arg(XLL_LPOPERX,   _T("Count"), IS_COUNT)
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
xll_interest_rate_swap(LPOPERX count, time_unit unit, payment_frequency freq, day_count_basis dcb, roll_convention roll, HANDLEX cal)
{
#pragma XLLEXPORT
	handle<fixed_income_instrument> h;

	try {
		ensure (size<XLOPERX>(*count) == 1 || size<XLOPERX>(*count) == 6);

		holiday_calendar cal_;
	
		if (size<XLOPERX>(*count) == 6) {
			unit = static_cast<time_unit>(static_cast<int>((*count)[1]));
			freq = static_cast<payment_frequency>(static_cast<int>((*count)[2]));
		    dcb = static_cast<day_count_basis>(static_cast<int>((*count)[3]));
			roll = static_cast<roll_convention>(static_cast<int>((*count)[4]));
			handle<FPX> h_(static_cast<HANDLEX>((*count)[5]));
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
		else if (size<XLOPERX>(*count) == 1) {
			if (count->xltype == xltypeStr) {
				// ticker?
				LOPERX xEval;
				ensure (xEval = ExcelX(xlfEvaluate, ExcelX(xlfConcatenate, OPERX(_T("=INTEREST.RATE.SWAP.")), safe_name(*count), OPERX(_T("()")))));

				return xEval.val.num;
			}
			handle<FPX> h_(cal);
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
		
		h =  new instrument::interest_rate_swap(static_cast<int>((*count)[0]), unit, freq, dcb, roll, cal_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h.get();
}

static AddInX xai_interest_rate_swap_valuation(
	FunctionX(XLL_HANDLEX, _T("?xll_interest_rate_swap_valuation"), _T("INTEREST.RATE.SWAP.VALUATION"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an interest rate swap."))
	.Arg(XLL_DOUBLEX, _T("Settlement"), IS_SETTLEMENT)
	.Arg(XLL_DOUBLEX, _T("Effective"), IS_EFFECTIVE)
	.Arg(XLL_DOUBLEX, _T("Coupon"), IS_COUPON _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate cash flows and return the handle of the interest rate swap."))
	.Documentation(
		_T("")
	)
);
HANDLEX WINAPI
xll_interest_rate_swap_valuation(HANDLEX h, double set, double eff, double rate)
{
#pragma XLLEXPORT
	try {
		interest_rate_swap *pfra = dynamic_cast<interest_rate_swap*>(handle<fixed_income_instrument>(h).ptr());

		ensure (pfra);

		pfra->valuation(date(set), date(eff), rate);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}
