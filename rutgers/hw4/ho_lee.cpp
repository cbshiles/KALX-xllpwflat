// function.cpp - Rename this file and replace this description.
#include "ho_lee.h"

using namespace xll;

static AddInX xai_ElogD(
	FunctionX(XLL_DOUBLEX, _T("?xll_ElogD"), _T("HO.LEE.E.LOG.D"))
	.Arg(XLL_DOUBLEX, _T("u"), _T("is the effective date of the zero coupon bond."))
	.Arg(XLL_DOUBLEX, _T("v"), _T("is the termination date of the zero coupon bond."))
	.Arg(XLL_DOUBLEX, _T("Du"), _T("is discount to the effective date."))
	.Arg(XLL_DOUBLEX, _T("Dv"), _T("is discount to th termination date."))
	.Arg(XLL_DOUBLEX, _T("sigma"), _T("is the Ho-Lee constant volatility. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the expected value of log D(u, v) in the Ho-Lee model."))
	.Documentation(_T(""))
);
double WINAPI
xll_ElogD(double u, double v, double Du, double Dv, double sigma)
{
#pragma XLLEXPORT
	double result(std::numeric_limits<double>::quiet_NaN());

	try {
		result = ho_lee::ElogD(u, v, Du, Dv, sigma);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return result;
}

static AddInX xai_VarlogD(
	FunctionX(XLL_DOUBLEX, _T("?xll_VarlogD"), _T("HO.LEE.VAR.LOG.D"))
	.Arg(XLL_DOUBLEX, _T("u"), _T("is the effective date of the zero coupon bond."))
	.Arg(XLL_DOUBLEX, _T("v"), _T("is the termination date of the zero coupon bond."))
	.Arg(XLL_DOUBLEX, _T("Du"), _T("is discount to the effective date."))
	.Arg(XLL_DOUBLEX, _T("Dv"), _T("is discount to th termination date."))
	.Arg(XLL_DOUBLEX, _T("sigma"), _T("is the Ho-Lee constant volatility. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the variance value of log D(u, v) in the Ho-Lee model."))
	.Documentation(_T(""))
);
double WINAPI
xll_VarlogD(double u, double v, double Du, double Dv, double sigma)
{
#pragma XLLEXPORT
	double result(std::numeric_limits<double>::quiet_NaN());

	try {
		result = ho_lee::VarlogD(u, v, Du, Dv, sigma);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

static AddInX xai_CovlogD(
	FunctionX(XLL_DOUBLEX, _T("?xll_CovlogD"), _T("HO.LEE.COV.LOG.D"))
	.Arg(XLL_DOUBLEX, _T("t"), _T("is the effective date of the zero coupon bond."))
	.Arg(XLL_DOUBLEX, _T("u"), _T("is the termination date of the zero coupon bond 1."))
	.Arg(XLL_DOUBLEX, _T("v"), _T("is the termination date of the zero coupon bond 2."))
	.Arg(XLL_DOUBLEX, _T("sigma"), _T("is the Ho-Lee constant volatility. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the convariance value of log D(t, u) and D(t, v) in the Ho-Lee model."))
	.Documentation(_T(""))
);
double WINAPI
xll_CovlogD(double t, double u, double v, double sigma)
{
#pragma XLLEXPORT
	double result(std::numeric_limits<double>::quiet_NaN());

	try {
		result = ho_lee::CovlogD(t, u, v, sigma);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	
	return result;
}