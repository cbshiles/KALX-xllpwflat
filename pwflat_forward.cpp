// pwflat_forward.cpp - bootstrap piecewise flat yield curve
#include "xllpwflat.h"
#include "../fmspwflat/instrument.h"

using namespace xll;
using namespace pwflat;

#ifdef _DEBUG

// construct curve
static AddInX xai_pwflat_yield_curve(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_pwflat_yield_curve"), _T("PWFLAT.YIELD.CURVE"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Construct a handle to a yield curve with piecewise flat forwards."))
	.Documentation(
	)
);
HANDLEX WINAPI
xll_pwflat_yield_curve()
{
#pragma XLLEXPORT
	HANDLEX yc(0);

	try {
		handle<yield_curve<>> h = new yield_curve<>();
		yc = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		yc = 0;
	}

	return yc;
}


static AddInX xai_pwflate_forward_add(
	FunctionX(XLL_HANDLEX, _T("?xll_pwflat_forward_add"), _T("PWFLAT.ADD"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat forward curve"))
	.Arg(XLL_FPX, _T("Times"), IS_TIMES)
	.Arg(XLL_FPX, _T("Amounts"), IS_AMOUNTS)
	.Category(CATEGORY)
	.FunctionHelp(_T("Add an arbitrary fixed income instrument to a piecewise flat forward curve."))
	.Documentation(
		_T("The first row of the <codeInline>CashFlows</codeInline> array is the time in years ")
		_T("corresponding to cash flow amounts in the second row. ")
		_T("The bootstrap algorithm ensures the present value of the cash flows is zero. ")
		_T("Often this means the first entry of the first row is 0 and the first entry of ")
		_T("the second row is the negative of the price. ")
		_T("</para><para>")
		_T("This function does not play nice with Excel. It is better to use <codeInline>PWFLAT.BUILD</codeInline>. ")
		,
		xml::xlink(_T("PWFLAT.BUILD"))
	)
);
HANDLEX WINAPI
xll_pwflat_forward_add(HANDLEX h, xfp* pt, xfp* pc)
{
#pragma XLLEXPORT
	try {
		ensure (size(*pt) == size(*pc));
		handle<yield_curve<>> h_;
		ensure (0 != (h_ = handle<yield_curve<>>(h)));

		h_->add(pt->array, pt->array + size(*pt), pc->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}

#endif // _DEBUG

static AddInX xai_pwflate_forward_build(
	FunctionX(XLL_HANDLEX, _T("?xll_pwflat_forward_build"), _T("PWFLAT.BUILD"))
	.Arg(XLL_FPX, _T("Instruments"), _T("is an array of handles to instruments "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Bootstrap a curve from a collection of fixed income Instruments."))
	.Documentation(
		_T("The <codeInline>Instruments</codeInline>s must have increasing maturites. ")
		,
		xml::element()
		.content(xml::xlink(_T("PWFLAT.ADD")))
	)
);
HANDLEX WINAPI
xll_pwflat_forward_build(xfp* pi)
{
#pragma XLLEXPORT
	HANDLEX yc(0);

	try {
		handle<yield_curve<>> h_ = new yield_curve<>();
	
		ensure (h_);

		h_->reset();
		for (xword i = 0; i < size(*pi); ++i) {
			handle<instrument::fixed_income_instrument> i_(pi->array[i]);
			ensure (i_);
			if (i_) {
				h_->add(i_->time(), i_->time() + i_->size(), i_->cash());
			}
		}

		yc = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return yc;
}

static AddInX xai_pwflate_forward_pv(
	FunctionX(XLL_HANDLEX, _T("?xll_pwflat_forward_pv"), _T("PWFLAT.PV"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat forward curve"))
	.Arg(XLL_FPX, _T("Times"), IS_TIMES)
	.Arg(XLL_FPX, _T("Amounts"), IS_AMOUNTS)
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate the present value of CashFlows."))
	.Documentation(
		_T("The present value of each instrument used to build the curve should be zero. ")
	)
);
HANDLEX WINAPI
xll_pwflat_forward_pv(HANDLEX h, xfp* pt, xfp* pc)
{
#pragma XLLEXPORT
	double pv(std::numeric_limits<double>::quiet_NaN());

	try {
		handle<yield_curve<>> h_(h);

		ensure (size(*pt) == size(*pc));
		ensure (h_);

		pv = h_->forward().pv(pt->array, pt->array + size(*pt), pc->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pv;
}

static AddInX xai_pwflat_forward(
	FunctionX(XLL_FPX, _T("?xll_pwflat_forward"), _T("PWFLAT.FORWARD"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat yield curve."))
	.Arg(XLL_FPX, _T("Times"), _T("is an array of times in years "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the instantaneous forward rate at Times in years."))
	.Documentation(
		_T("The discount, <math>D</math>, is related to the instantaneous forward, <math>f</math>, by <math>D</math>(<math>t</math>) = exp(")
		ENT_minus ENT_int _T("<math>_0^t f</math>(<math>s</math>) <math>ds</math>). ")
		,
		xml::element()
		.content(xml::xlink(_T("PWFLAT.DISCOUNT")))
		.content(xml::xlink(_T("PWFLAT.SPOT")))
	)
);
xfp* WINAPI
xll_pwflat_forward(HANDLEX h, xfp* pt)
{
#pragma XLLEXPORT
	try {
		handle<yield_curve<>> h_(h);

		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = h_->forward()(pt->array[i]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return pt;
}

static AddInX xai_pwflat_spot(
	FunctionX(XLL_FPX, _T("?xll_pwflat_spot"), _T("PWFLAT.SPOT"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat yield curve"))
	.Arg(XLL_FPX, _T("Times"), _T("is an array of times in years "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the continuously compounded spot rate at Times in years."))
	.Documentation(
		_T("The discount, <math>D</math>, is related to the continuously compounded spot rate, <math>r</math>, by <math>D</math>(<math>t</math>) = exp(")
		ENT_minus _T("<math>tr</math>(<math>t</math>)). ")
		,
		xml::element()
		.content(xml::xlink(_T("PWFLAT.DISCOUNT")))
		.content(xml::xlink(_T("PWFLAT.FORWARD")))
	)
);
xfp* WINAPI
xll_pwflat_spot(HANDLEX h, xfp* pt)
{
#pragma XLLEXPORT
	try {
		handle<yield_curve<>> h_(h);

		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = h_->forward().spot(pt->array[i]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return pt;
}

static AddInX xai_pwflat_discount(
	FunctionX(XLL_FPX, _T("?xll_pwflat_discount"), _T("PWFLAT.DISCOUNT"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat yield curve"))
	.Arg(XLL_FPX, _T("Times"), _T("is an array of times in years "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the instantaneous discount rate at Times in years."))
	.Documentation(
		_T("The discount, <math>D</math>(<math>t</math>), is the value of a zero coupon bond paying amount 1 at time <math>t</math>. ")
		_T("It  is related to the instantaneous forward, <math>f</math>, by <math>D</math>(<math>t</math>) = exp(")
		ENT_minus ENT_int _T("<math>_0^t f</math>(<math>s</math>) <math>ds</math>) ")
		_T("and to the continuously compounded spot rate, <math>r</math>, by <math>D</math>(<math>t</math>) = exp(")
		ENT_minus _T("<math>tr</math>(<math>t</math>)). ")
		,
		xml::element()
		.content(xml::xlink(_T("PWFLAT.FORWARD")))
		.content(xml::xlink(_T("PWFLAT.SPOT")))
	)
);
xfp* WINAPI
xll_pwflat_discount(HANDLEX h, xfp* pt)
{
#pragma XLLEXPORT
	try {
		handle<yield_curve<>> h_(h);

		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = h_->forward().discount(pt->array[i]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return pt;
}

#ifdef _DEBUG

int
test_pwflat(void)
{
	try {
		yield_curve<> yc; 
		double f = 0.04;
		// pv of (0, -1), (1, e), ..., (n, 1+e) is 0 for f(t) = f.
		double e = exp(f) - 1;
		double t[10], c[10];

		t[0] = 0;
		c[0] = -1;
		t[1] = 1;
		c[1] = 1 + e;
		yc.add(t, t + 2, c);
		for (int i = 2; i < 10; ++i ) {
			c[i - 1] -= 1;
			t[i] = i;
			c[i] = 1 + e;
			yc.add(t, t + i + 1, c, 1e-13);
		}

		for (double u = 0; u < 9; u += 0.01) {
			ensure (fabs(yc.forward()(u) - f) < 2e-16);
			ensure (fabs(yc.forward()(u) - yc.forward().spot(u)) < 2e-16);
			ensure (fabs(yc.forward().discount(u) - exp(-u*yc.forward().spot(u))) < 2e-16);
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_test_pwflat(test_pwflat);

#endif // _DEBUG