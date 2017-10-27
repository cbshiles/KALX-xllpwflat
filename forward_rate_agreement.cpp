// forward_rate_agreement.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmspwflat/forward_rate_agreement.h"

using namespace datetime;
using namespace instrument;
using namespace xll;

static AddInX xai_forward_rate_agreement(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_forward_rate_agreement"), _T("FORWARD.RATE.AGREEMENT"))
	.Arg(XLL_LPOPERX,   _T("Count"), IS_COUNT)
	.Arg(XLL_LONGX, _T("Unit"),  IS_UNIT)
	.Arg(XLL_LONGX, _T("Dcb"),   IS_DCB)
	.Arg(XLL_LONGX, _T("Roll"),  IS_ROLL)
	.Arg(XLL_HANDLEX, _T("Calendar"),   IS_CALENDAR _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle to a forward rate agreement."))
	.Documentation(
		_T("The first argument can also be a five element array of the arguments. ")
	)
);
HANDLEX WINAPI
xll_forward_rate_agreement(LPOPERX count, time_unit unit, day_count_basis dcb, roll_convention roll, HANDLEX cal)
{
#pragma XLLEXPORT
	handle<fixed_income_instrument> h;

	try {
		ensure (size<XLOPERX>(*count) == 1 || size<XLOPERX>(*count) == 5);

		holiday_calendar cal_;

		if (size<XLOPERX>(*count) == 5) {
			unit = static_cast<time_unit>(static_cast<int>((*count)[1]));
		    dcb = static_cast<day_count_basis>(static_cast<int>((*count)[2]));
			roll = static_cast<roll_convention>(static_cast<int>((*count)[3]));
			handle<FPX> h_(static_cast<HANDLEX>((*count)[4]));
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
		else if (size<XLOPERX>(*count) == 1) {
			handle<FPX> h_(cal);
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
		
		h = new instrument::forward_rate_agreement(static_cast<int>((*count)[0]), unit, dcb, roll, cal_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h.get();
}

static AddInX xai_forward_rate_agreement_valuation(
	FunctionX(XLL_HANDLEX, _T("?xll_forward_rate_agreement_valuation"), _T("FORWARD.RATE.AGREEMENT.VALUATION"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a forward rate agreement"))
	.Arg(XLL_DOUBLEX, _T("Settlement"), IS_SETTLEMENT)
	.Arg(XLL_DOUBLEX, _T("Effective"), IS_EFFECTIVE)
	.Arg(XLL_DOUBLEX, _T("Forward"), IS_FORWARD _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate cash flows and return the handle of the forward rate agreement. "))
	.Documentation(
		_T("Can be used for futures if convexity adjustment is taken into account.")
	)
);
HANDLEX WINAPI
xll_forward_rate_agreement_valuation(HANDLEX h, double set, double eff, double rate)
{
#pragma XLLEXPORT
	try {
		forward_rate_agreement *pfra(0);

		ensure (0 != (pfra = dynamic_cast<forward_rate_agreement*>(handle<fixed_income_instrument>(h).ptr())));

		pfra->valuation(date(set), date(eff), rate);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}
