// forward_rate_agreement.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../xlldatetime/xlldatetime.h"
#include "../fmspwflat/forward_rate_agreement.h"

using namespace datetime;
using namespace fixed_income;
using namespace xll;

static AddInX xai_forward_rate_agreement(
	FunctionX(XLL_HANDLEX, _T("?xll_forward_rate_agreement"), FI_PREFIX _T("INSTRUMENT.FORWARD.RATE.AGREEMENT"))
	.Arg(XLL_LPOPERX, _T("Effective"), IS_EFFECTIVE)
	.Arg(XLL_LONGX, _T("Count"), IS_COUNT)
	.Arg(XLL_LONGX, _T("Unit"),  IS_UNIT)
	.Arg(XLL_LONGX, _T("Dcb"),   IS_DCB)
	.Arg(XLL_LONGX, _T("Roll"),  IS_ROLL)
	.Arg(XLL_HANDLEX, _T("Calendar"),   IS_CALENDAR _T(" "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle to a forward rate agreement."))
	.Documentation(
		_T("The first argument can also be a five element array of the arguments. ")
	)
);
HANDLEX WINAPI
xll_forward_rate_agreement(LPOPERX eff, LONG count, time_unit unit, day_count_basis dcb, roll_convention roll, HANDLEX cal)
{
#pragma XLLEXPORT
	HANDLEX h(0);

	try {
		ensure (size<XLOPERX>(*eff) == 1 || size<XLOPERX>(*eff) == 6);

		if (size<XLOPERX>(*eff) == 6) {
			count = static_cast<LONG>(eff[1]);
			unit = static_cast<time_unit>(static_cast<int>((*eff)[2]));
		    dcb = static_cast<day_count_basis>(static_cast<int>((*eff)[3]));
			roll = static_cast<roll_convention>(static_cast<int>((*eff)[4]));
			cal = (*eff)[5];
		}
		
		handle<instrument<double,date>> fra = new forward_rate_agreement<>(date((*eff)[0]), count, unit, dcb, roll, HOLIDAY_CALENDAR(cal));
		h = fra.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

int
test_forward_rate_agreement(void)
{
	try {
		OPERX ofra = ExcelX(xlfEvaluate, 
			OPERX(_T("=FI.INSTRUMENT.FORWARD.RATE.AGREEMENT(DATE(2013,1,1),3,UNIT_MONTH(),DCB_30U360(),ROLL_MODIFIED_FOLLOWING(),CALENDAY_NYB())")));
	}
	catch (std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_forward_rate_agreement(test_forward_rate_agreement);

#endif // _DEBUG