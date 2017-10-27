 // pwflat_forward.cpp - Piecewise flat forward curve.
#include "xllpwflat.h"
#include "../fmspwflat/pwflat.h"

using namespace xll;
using namespace pwflat;

class pwflat_forward : public forward_curve<> {
	std::vector<double> t_, f_;
public:
	pwflat_forward(double _f = 0)
		: forward_curve(_f)
	{ }
	pwflat_forward(size_t n, const double* t, const double* f, double _f_ = 0)
		: t_(t, t + n), f_(f, f + n)
	{
		set(n, &t_[0], &f_[0], _f_);
	}
	pwflat_forward(const pwflat_forward& F)
		: t_(F.t, F.t + F.n), f_(F.f, F.f + F.n)
	{
		set(n, &t_[0], &f_[0], F._f);
	}
	pwflat_forward& operator=(const pwflat_forward& F)
	{
		if (this != &F) {
			t_ = F.t_;
			f_ = F.f_;
		}

		set(n, &t_[0], &f_[0], F._f);

		return *this;
	}
	~pwflat_forward()
	{ }
};

#if 0

// construct curve
static AddInX xai_pwflat_forward_curve(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_pwflat_forward_curve"), _T("PWFLAT.CURVE"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Construct a handle to a yield curve with piecewise flat forwards."))
	.Documentation(
		_T("The root finder for the bootstrap algorithm uses <codeInline>Tolerance</codeInline> ")
		_T("as the absolute tolerance for convergence. ")
	)
);
HANDLEX WINAPI
xll_pwflat_forward_curve()
{
#pragma XLLEXPORT
	HANDLEX h(0);

	try {
		handle<fixed_income::pwflat::yield_curve<>> yc = new fixed_income::pwflat::yield_curve<>();
		h = yc.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
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
		handle<fixed_income::pwflat::yield_curve<>> h_(h);

		ensure (size(*pt) == size(*pc));
		ensure (h_);

		h_->add(size(*pt), pt->array, pc->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}

//#endif // _DEBUG

static AddInX xai_pwflat_forward_build(
	FunctionX(XLL_HANDLEX, _T("?xll_pwflat_forward_build"), _T("PWFLAT.BOOTSTRAP"))
	.Arg(XLL_FPX, _T("Instruments"), _T("is an array of handles to instruments "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Bootstrap a curve from a collection of fixed income Instruments."))
	.Documentation(
		_T("The <codeInline>Instruments</codeInline>s must have increasing maturites. ")
		,
		xml::element()
		.content(xml::xlink(_T("INSTRUMENT.CASH.DEPOSIT")))
		.content(xml::xlink(_T("INSTRUMENT.FORWARD.RATE.AGREEMENT")))
		.content(xml::xlink(_T("INSTRUMENT.INTEREST.RATE.SWAP")))
	)
);
HANDLEX WINAPI
xll_pwflat_forward_build(HANDLEX h, xfp* pi)
{
#pragma XLLEXPORT
	
	HANDLEX h(0);

	try {
		handle<yield_curve<>> h_(h);

		ensure (h_);

		h_->reset();
		for (xword i = 0; i < size(*pi); ++i) {
			handle<instrument::fixed_income_instrument> i_(pi->array[i]);
			ensure (i_);
			if (i_) {
				h_->add(i_->size(), i_->time(), i_->cash());
			}
		}

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}
#endif

static AddInX xai_pwflat_forward_curve(
	FunctionX(XLL_HANDLEX, _T("?xll_pwflat_forward_curve"), _T("PWFLAT.FORWARD.CURVE"))
	.Arg(XLL_FPX, _T("Times"), IS_TIMES,
		_T("={1,2,3}"))
	.Arg(XLL_FPX, _T("Rates"), IS_RATES,
		_T("={.1,.2,.3}"))
	.Arg(XLL_DOUBLEX, _T("Extrapolate"), _T("is an optional rate for extrapolating the curve"),
		.4)
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate the present value of CashFlows."))
	.Documentation(
		_T("The present value of each instrument used to build the curve should be zero. ")
	)
);
HANDLEX WINAPI
xll_pwflat_forward_curve(xfp* pt, xfp* pf, double _f)
{
#pragma XLLEXPORT
	HANDLEX h(0);

	try {
		ensure (size(*pt) == size(*pf));

		handle<forward_curve<>> f = new pwflat_forward(size(*pt), pt->array, pf->array, _f);
		ensure (f);

		h = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_pwflat_forward_value(
	FunctionX(XLL_FPX, _T("?xll_pwflat_forward"), _T("PWFLAT.FORWARD"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to a piecewise flat forward curve."))
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
		handle<forward_curve<>> h_(h);
		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = h_->value(pt->array[i]);
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
		handle<forward_curve<>> h_(h);
		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = spot(pt->array[i], *h_);
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
	.FunctionHelp(_T("Return the discount rate at Times in years."))
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
		handle<forward_curve<>> h_(h);
		ensure (h_);

		for (xword i = 0; i < size(*pt); ++i)
			pt->array[i] = discount(pt->array[i], *h_);
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
		OPERX oF = ExcelX(xlfEvaluate, OPERX(_T("=PWFLAT.FORWARD.CURVE({1,2,3},{.1,.2,.3})")));
		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.FORWARD")), oF, OPERX(1)) == .1); 
		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.FORWARD")), oF, OPERX(2)) == .2); 
		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.FORWARD")), oF, OPERX(3)) == .3); 
		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.FORWARD")), oF, OPERX(4)) == 0.); 

		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.SPOT")), oF, OPERX(2.5))); 
		ensure (ExcelX(xlUDF, OPERX(_T("PWFLAT.DISCOUNT")), oF, OPERX(2.5))); 
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_test_pwflat(test_pwflat);

#endif // _DEBUG