// function.cpp - Rename this file and replace this description.
#include "header.h"
#include "../../../fmspwflat/pwflat.h"
#include "../../../fmspwflat/forward_rate_agreement.h"

using namespace xll;
using pwflat::forward_curve;
using instrument::fixed_income_instrument;
using instrument::forward_rate_agreement;
using datetime::date;

static AddInX xai_pwflat_forward_rate(
	FunctionX(XLL_DOUBLEX, _T("?xll_pwflat_forward_rate"), _T("PWFLAT.FORWARD.RATE"))
	.Arg(XLL_HANDLEX, _T("Curve"), _T("is a handle to a pwflat yield curve."))
	.Arg(XLL_HANDLEX, _T("FRA"), _T("is a handle to a forward rate agreement."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the forward rate corresponding to the forward rate agreement."))
	.Documentation(_T("Optional documentation for function."))
);
double WINAPI
xll_pwflat_forward_rate(HANDLEX curve, HANDLEX fra)
{
#pragma XLLEXPORT
	double f = std::numeric_limits<double>::quiet_NaN(); // returns as #NUM!

	try {
		// from xll_forward_rate_agreement_valuation in forward_rate_agreement.cpp
		forward_rate_agreement *pfra(0);

		ensure (0 != (pfra = dynamic_cast<forward_rate_agreement*>(handle<fixed_income_instrument>(fra, false).ptr())));

		date set = pfra->settlement();
		date eff = pfra->effective();
		date ter = pfra->termination();

		double u = eff.diffyears(set);
		double v = ter.diffyears(set);
		double dcf = ter.diff_dcb(eff, pfra->dcb_);

		handle<forward_curve> hc(curve, false);
		ensure (hc); // same as ensure (hc != 0) since hc acts like a pointer

		f = (hc->discount(u)/hc->discount(v) - 1)/dcf;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return f;
}

// Bad form. Should go in separate file
#include "../../../xllbms/black.h"

static AddInX xai_pwflat_caplet_value(
	FunctionX(XLL_DOUBLEX, _T("?xll_pwflat_caplet_value"), _T("PWFLAT.CAPLET.VALUE"))
	.Arg(XLL_HANDLEX, _T("Curve"), _T("is a handle to a pwflat yield curve."))
	.Arg(XLL_HANDLEX, _T("FRA"), _T("is a handle to a forward rate agreement. "))
	.Arg(XLL_DOUBLEX, _T("Volatility"), _T("is the Black-Scholes volatility."))
	.Arg(XLL_DOUBLEX, _T("Strike"), _T("is the caplet strike. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the present value of a caplet corresponding to the forward rate agreement."))
	.Documentation(_T("Optional documentation for function."))
);
double WINAPI
xll_pwflat_caplet_value(HANDLEX curve, HANDLEX fra, double sigma, double k)
{
#pragma XLLEXPORT
	double value = std::numeric_limits<double>::quiet_NaN();

	try {
		double f = xll_pwflat_forward_rate(curve, fra);
		forward_rate_agreement *pfra(0);

		ensure (0 != (pfra = dynamic_cast<forward_rate_agreement*>(handle<fixed_income_instrument>(fra, false).ptr())));

		date set = pfra->settlement();
		date eff = pfra->effective();
		date ter = pfra->termination();

		double u = eff.diffyears(set);
		double v = ter.diffyears(set);
		double dcf = ter.diff_dcb(eff, pfra->dcb_);

		handle<forward_curve> hc(curve, false);
		ensure (hc); // same as ensure (hc != 0) since hc acts like a pointer

		value = hc->discount(v)*black::black(f, sigma, k, u)*dcf;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return value;
}
