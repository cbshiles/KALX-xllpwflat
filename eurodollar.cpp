// eurodollar.cpp - Eurodollar related functions.
// Copyright (c) 2011 KALX, LLC. All rights reserved.
#include "xllpwflat.h"
#include "../fmspwflat/eurodollar.h"

using namespace xll;

static AddInX xai_eurodollar_first_contract(
	FunctionX(XLL_LONGX, _T("?xll_eurodollar_first_contract"), _T("EURODOLLAR.FIRST.CONTRACT"))
	.Arg(XLL_DOUBLEX, _T("Date"), _T("is a date"))
	.Arg(XLL_WORDX, _T("Days"), _T("is the number of days after date to not trade "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the ordinal of the front Eurodollar contract at least Days past Date. "))
	.Documentation()
);
LONG WINAPI
xll_eurodollar_first_contract(double d, WORD days)
{
#pragma XLLEXPORT

	return eurodollar_first_contract(datetime::date(d), days);
}

#ifdef _DEBUG

int
test_eurodollar_first_contract(void)
{
	try {
		datetime::date imm(2011, 9, 1);
		
		imm.imm(3, DAY_WED);
		ensure (imm.day() == 21);

		datetime::date d(imm);
		d.incr(-30, UNIT_DAYS);

		int ordinal;
		ordinal = eurodollar_first_contract(d, 30);
		ensure (ordinal == 1);
		ordinal = eurodollar_first_contract(d, 31);
		ensure (ordinal == 2);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_eurodollar_first_contract(test_eurodollar_first_contract);

#endif // _DEBUG