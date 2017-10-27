// instrument.cpp - access to base class members
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmsdatetime/datetime.h"
#include "../fmspwflat/bootstrap.h"

using namespace xll;
using namespace fixed_income;

class fixed_income_instrument : public instrument<double,datetime::date> {
	std::vector<double> t_;
	std::vector<double> c_;
public:
	fixed_income_instrument(size_t n, const double* t, const double* c)
	{
		set(n, t, c);
	}
};

static AddInX xai_fixed_income_instrument(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_fixed_income_instrument"), FI_PREFIX _T("INSTRUMENT"))
	.Arg(XLL_FPX, _T("Times"), IS_TIMES,
		_T("={1,2,3}"))
	.Arg(XLL_FPX, _T("Flows"), IS_FLOWS _T(" "),
		_T("={.01,.02,.03}"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Create an arbitrary fixed income instrument."))
	.Documentation()
);
HANDLEX WINAPI
xll_fixed_income_instrument(xfp* pt, xfp* pc)
{
#pragma XLLEXPORT
	HANDLEX h(0);
	
	try {
		if (in_function_wizard())
			return 0;

		ensure (size(*pt) == size(*pc));

		handle<fixed_income::instrument<double,datetime::date>> hib(new fixed_income_instrument(size(*pt), pt->array, pc->array));

		h = hib.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

	}

	return h;
}

static AddInX xai_instrument_fix(
	FunctionX(XLL_HANDLEX, _T("?xll_instrument_fix"), FI_PREFIX _T("INSTRUMENT.FIX"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to and instrument"))
	.Arg(XLL_DATEX, _T("Date"), _T("is the date associated with the Coupon"))
	.Arg(XLL_DOUBLEX, _T("Coupon"), _T("is the Coupon corresponding to Date"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Fix cash flows for an instrument based on Date and Coupon."))
	.Documentation(
		_T("For cash deposits, Date is the valuation date of the Coupon. ")
		_T("For forward rate agreements and swaps, Date is the effective date. ")
	)
);
HANDLEX WINAPI
xll_instrument_fix(HANDLEX h, double d, double c)
{
#pragma XLLEXPORT
	try {
		handle<fixed_income::instrument<double,datetime::date>> hib(h);
		ensure (hib);

		hib->fix(datetime::date(d), c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_instrument_time(
	FunctionX(XLL_FPX, _T("?xll_instrument_time"), FI_PREFIX _T("INSTRUMENT.TIME"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a one row array of the time in years of the instrument cash flows corresponding to Handle."))
	.Documentation(
	)
);
xfp* WINAPI
xll_instrument_time(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX t;

	try {
		handle<fixed_income::instrument<double,datetime::date>> hib(h);
		ensure (hib);
		
		t.set(1, static_cast<xword>(hib->n), hib->t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return t.get();
}

static AddInX xai_instrument_cash(
	FunctionX(XLL_FPX, _T("?xll_instrument_cash"), FI_PREFIX _T("INSTRUMENT.FLOW"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an instrument "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a one row array of the amounts of the instrument cash flows corresponding to Handle."))
	.Documentation(
	)
);
xfp* WINAPI
xll_instrument_cash(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX c;

	try {
		handle<fixed_income::instrument<double,datetime::date>> hib(h);
		ensure (hib);

		c.set(1, static_cast<xword>(hib->n), hib->c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return c.get();
}
