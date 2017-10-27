// cash_deposit.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../xlldatetime/xlldatetime.h"
#include "../fmspwflat/cash_deposit.h"

using namespace fixed_income;
using namespace xll;

static AddInX xai_cash_deposit(
	FunctionX(XLL_HANDLEX, _T("?xll_cash_deposit"), FI_PREFIX _T("INSTRUMENT.CASH.DEPOSIT"))
	.Arg(XLL_LPOPERX, _T("Settle"), _T("is the number of days required to settle the cash deposit."),
		2)
	.Arg(XLL_LONGX, _T("Count"), IS_COUNT,
		3)
	.Arg(XLL_LONGX, _T("Unit"), IS_UNIT,
		_T("=UNIT_MONTHS()"))
	.Arg(XLL_LONGX, _T("Dcb"), IS_DCB,
		_T("=DCB_ACTUAL_360()"))
	.Arg(XLL_LONGX, _T("Roll"), IS_ROLL,
		_T("=ROLL_MODIFIED_FOLLOWING()"))
	.Arg(XLL_HANDLEX, _T("Calendar"), IS_CAL _T(" "),
		_T("=CALENDAR_NYB"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the handle to a cash deposit."))
	.Documentation(
		_T("The first argument can also be a five element array of the arguments. ")
	)
);
HANDLEX WINAPI
xll_cash_deposit(LPOPERX set, int count, time_unit unit, day_count_basis dcb, roll_convention roll, HANDLEX cal)
{
#pragma XLLEXPORT
	HANDLEX h(0);

	try {
		ensure (size<XLOPERX>(*set) == 1 || size<XLOPERX>(*set) == 6);

		if (size<XLOPERX>(*set) == 6) {
			count= static_cast<int>((*set)[1]);
			unit = static_cast<time_unit>(static_cast<int>((*set)[2]));
		    dcb = static_cast<day_count_basis>(static_cast<int>((*set)[3]));
			roll = static_cast<roll_convention>(static_cast<int>((*set)[4]));
			cal = (*set)[5];
		}
/*		else if (size<XLOPERX>(*count) == 1) {
			if (count->xltype == xltypeStr) {
				// ticker?
				LOPERX xEval;
				ensure (xEval = ExcelX(xlfEvaluate, ExcelX(xlfConcatenate, OPERX(_T("=CASH.DEPOSIT.")), safe_name(*count), OPERX(_T("()")))));

				return xEval.val.num;
			}
			handle<FPX> h_(cal);
			if (h_)
				cal_ = holiday_calendar(h_->size(), h_->array());
		}
*/		handle<instrument<double,datetime::date>> hcd(new cash_deposit<>(static_cast<int>((*set)[0]), count, unit, dcb, roll, HOLIDAY_CALENDAR(cal)));

		h = hcd.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

int
test_cash_deposit(void)
{
	try {
		cash_deposit<> cd0(2, 1, UNIT_WEEK, DCB_ACTUAL_360, ROLL_MODIFIED_FOLLOWING, CALENDAR_NONE);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_cash_deposit(test_cash_deposit);
#endif // _DEBUG
