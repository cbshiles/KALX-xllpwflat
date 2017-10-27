// cash_deposit.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmsdatetime/calendar.h"
#include "../fmspwflat/cash_deposit.h"

using namespace instrument;
using namespace xll;

static AddInX xai_cash_deposit(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_cash_deposit"), _T("CASH.DEPOSIT"))
	.Arg(XLL_LPOPERX, _T("Count"), IS_COUNT)
	.Arg(XLL_LONGX, _T("Unit"), IS_UNIT)
	.Arg(XLL_LONGX, _T("Dcb"), IS_DCB)
	.Arg(XLL_LONGX, _T("Roll"), IS_ROLL)
	.Arg(XLL_HANDLEX, _T("Calendar"), IS_CALENDAR _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the handle to a cash deposit."))
	.Documentation(
		_T("The first argument can also be a five element array of the arguments. ")
	)
);
HANDLEX WINAPI
xll_cash_deposit(LPOPERX count, time_unit unit, day_count_basis dcb, roll_convention roll, HANDLEX cal)
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

		h = new cash_deposit(static_cast<int>((*count)[0]), unit, dcb, roll, cal_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h.get();
}

static AddInX xai_cash_deposit_valuation(
	FunctionX(XLL_HANDLEX, _T("?xll_cash_deposit_valuation"), _T("CASH.DEPOSIT.VALUATION"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a cash deposit"))
	.Arg(XLL_DOUBLEX, _T("Settlement"), IS_SETTLEMENT)
	.Arg(XLL_DOUBLEX, _T("Effective"), IS_EFFECTIVE)
	.Arg(XLL_DOUBLEX, _T("Rate"), IS_RATE _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate cash flows and return the handle of the cash deposit."))
	.Documentation(
		_T("Typically the settlement date equals the effective date for cash deposits. ")
	)
);
HANDLEX WINAPI
xll_cash_deposit_valuation(HANDLEX h, double set, double eff, double rate)
{
#pragma XLLEXPORT
	try {
		cash_deposit *pcd(0);

		ensure (0 != (pcd = dynamic_cast<cash_deposit*>(handle<fixed_income_instrument>(h).ptr())));

		pcd->valuation(date(set), date(eff), rate);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}


#ifdef _DEBUG

int
test_cash_deposit(void)
{
	try {
		cash_deposit cd0(1, UNIT_WEEK, DCB_ACTUAL_360, ROLL_MODIFIED_FOLLOWING, CALENDAR_NONE);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_cash_deposit(test_cash_deposit);

#endif // _DEBUG