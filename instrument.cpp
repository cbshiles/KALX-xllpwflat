// instrument.cpp - access to base class members
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmspwflat/instrument.h"

using namespace xll;
using namespace instrument;

static AddInX xai_fixed_income_instrument(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_fixed_income_instrument"), _T("PWFLAT.INSTRUMENT"))
	.Arg(XLL_DOUBLEX, _T("Settlement"), IS_SETTLEMENT)
	.Arg(XLL_FPX, _T("Times"), IS_TIMES)
	.Arg(XLL_FPX, _T("Amounts"), IS_AMOUNTS _T(" "))
);
HANDLEX WINAPI
xll_fixed_income_instrument(double set, const xfp* pt, const xfp* pc)
{
#pragma XLLEXPORT
	handle<fixed_income_instrument> h;
	
	try {
		ensure (size(*pt) == size(*pc));

		h = new fixed_income_instrument(datetime::date(set), size(*pt), pt->array, pc->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

	}

	return h.get();
}

static AddInX xai_instrument_settlement(
	FunctionX(XLL_DOUBLEX, _T("?xll_instrument_settlement"), _T("INSTRUMENT.SETTLEMENT"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the settlement date of the instrument corresponding to Handle. "))
	.Documentation(
		_T("The <newTerm>settlement date</newTerm> is the date at which the discount is 1. ")
	)
);
double WINAPI
xll_instrument_settlement(HANDLEX h)
{
#pragma XLLEXPORT
	double eff;

	try {
		handle<fixed_income_instrument> pi(h);

		ensure (pi);

		eff = pi->settlement().excel();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return eff;
}

static AddInX xai_instrument_effective(
	FunctionX(XLL_DOUBLEX, _T("?xll_instrument_effective"), _T("INSTRUMENT.EFFECTIVE"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the effective date of the instrument corresponding to Handle. "))
	.Documentation(
		_T("The <newTerm>effective date</newTerm> is the date on which the first cash flow occurs. ")
	)
);
double WINAPI
xll_instrument_effective(HANDLEX h)
{
#pragma XLLEXPORT
	double eff;

	try {
		handle<fixed_income_instrument> pi(h);

		ensure (pi);

		eff = pi->effective().excel();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return eff;
}

static AddInX xai_instrument_termination(
	FunctionX(XLL_DOUBLEX, _T("?xll_instrument_termination"), _T("INSTRUMENT.TERMINATION"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the termination date of the instrument corresponding to Handle. "))
	.Documentation(
		_T("The <newTerm>termination date</newTerm> is the date on which the last cash flow occurs. ")
	)
);
double WINAPI
xll_instrument_termination(HANDLEX h)
{
#pragma XLLEXPORT
	double eff;

	try {
		handle<fixed_income_instrument> pi(h);

		ensure (pi);

		eff = pi->termination().excel();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return eff;
}

static AddInX xai_instrument_time(
	FunctionX(XLL_FPX, _T("?xll_instrument_time"), _T("INSTRUMENT.TIME"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a one row array of the dates of the instrument cash flows corresponding to Handle. "))
	.Documentation(
	)
);
xfp* WINAPI
xll_instrument_time(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX t;

	try {
		handle<fixed_income_instrument> pi(h);

		ensure (pi);

		t.set(1, static_cast<xword>(pi->size()), pi->time());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return t.get();
}

static AddInX xai_instrument_cash(
	FunctionX(XLL_FPX, _T("?xll_instrument_cash"), _T("INSTRUMENT.CASH"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a one row array of the amounts of the instrument cash flows corresponding to Handle. "))
	.Documentation(
	)
);
xfp* WINAPI
xll_instrument_cash(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX c;

	try {
		handle<fixed_income_instrument> pi(h);

		ensure (pi);

		c.set(1, static_cast<xword>(pi->size()), pi->cash());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return c.get();
}

