// ho_lee.h - constant volatility Ho-Lee model
// Uncomment the following line to use features for Excel2007 and above.
//#define EXCEL12
#include "xll/xll.h"

#ifndef CATEGORY
#define CATEGORY _T("Rutgers")
#endif

namespace ho_lee {

	// E[log D(u, v)] where Du = D(0, u) and Dv = D(0,v).
	// log D(u, v) = - int_u^v phi(s) ds + sigma^2 (v - u)^3/6 - sigma(v - u) B_u
	// log D(0, t) = - int_0^t phi(s) + sigma^2 t^3/6
	inline double ElogD(double u, double v, double Du, double Dv, double sigma)
	{
		return log(Dv/Du) - sigma*sigma*u*v*(v - u)/2;
	}
	// Var(log D(u, v))
	inline double VarlogD(double u, double v, double Du, double Dv, double sigma)
	{
		return sigma*sigma*(v - u)*(v - u)*u;
	}
	// Cov(log D(t, u), log D(t, v))
	inline double CovlogD(double t, double u, double v, double sigma)
	{
		return sigma*sigma*(v - t)*(u - t)*t;
	}

} // namespace ho_lee
