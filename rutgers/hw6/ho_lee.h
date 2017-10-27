// ho_lee.h - constant volatility Ho-Lee model
// Uncomment the following line to use features for Excel2007 and above.
//#define EXCEL12
#include <tuple>
#include "xll/xll.h"
#include "../../../xllbms/black.h"

#ifndef CATEGORY
#define CATEGORY _T("Rutgers")
#endif

typedef xll::traits<XLOPERX>::xfp xfp; // FP data type

namespace ho_lee {

	// E[log D(u, v)] where Du = D(0, u) and Dv = D(0,v).
	// log D(u, v) = - int_u^v phi(s) ds + sigma^2 (v - u)^3/6 - sigma(v - u) B_u
	// log D(0, t) = - int_0^t phi(s) + sigma^2 t^3/6
	inline double ElogD(double u, double v, double Du, double Dv, double sigma)
	{
		return log(Dv/Du) - sigma*sigma*u*v*(v - u)/2;
	}
	// Cov(log D(t, u), log D(t, v))
	inline double CovlogD(double t, double u, double v, double sigma)
	{
		return sigma*sigma*(v - t)*(u - t)*t;
	}
	// Var(log D(u, v))
	inline double VarlogD(double u, double v, double sigma)
	{
		return CovlogD(u, u, v, sigma);
	}

	// Cov(log D(t, u), log D_t)
	inline double CovlogD(double t, double u, double sigma)
	{
		return sigma*sigma*(u - t)*(u - t)*t*t/2;
	}

	// sum_j e^N_j ~= e^N, E[N] = mu, Var(N) = sigma^2
	// Match mean and variance.
	template<class E, class Cov>
	inline std::tuple<double,double> 
	lognormalsum(int n, E e, Cov c)
	{
		double EexpN = 0, VarexpN = 0;

		for (int i = 0; i < n ; ++i) {
			EexpN += e(i);
			for (int j = 0; j < n; ++j) {
				VarexpN += c(i, j);
			}
		}

		// VarexpN = EexpN^2 (exp(sigma2) - 1)
		double sigma2 = log(VarexpN/(EexpN*EexpN) + 1);
		
		// EexpN = exp(mu + sigma2/2)
		double mu = log(EexpN) - sigma2/2;

		return std::make_tuple(mu, sigma2);
	}

	// E[D(t, u) e^gamma(t, u)], gamma = Cov(log D(t, u), log D_t).
	inline double ED_(double t, double u, double Dt, double Du, double sigma)
	{
		// E[e^N] = exp(E[N] + Var(N)/2)
		return exp(ElogD(t, u, Dt, Du, sigma) + VarlogD(t, u, sigma)/2 + CovlogD(t, u, sigma));
	}
	// Cov(D(t,u) e^gamma(t, u), D(t, v) e^gamma(t, v)
	inline double CovD_(double t, double u, double v, double Dt, double Du, double Dv, double sigma)
	{
		// Var(e^N) = E[e^N]^2 (e^Var(N) - 1)
		double Ee_Nu = ED_(t, u, Dt, Du, sigma);
		double Ee_Nv = ED_(t, v, Dt, Dv, sigma);

		return Ee_Nu*Ee_Nv*(exp(CovlogD(t, u, v, sigma)) - 1);
	}

	// forward value of swaption
	// E[c sum_0^n-1 D(t_0, t_j+1) + D(t_0,t_n) - 1)^+ D_t]/D(0,t_0)
	// = E[c sum_0^n-1 D(t_0, t_j+1)e^gamma_j + D(t_0,t_n)e^gamma_n - 1)^+]
	inline double
	swaption_forward_value(double sigma, double c, int n, const double* t, const double* D, const double* dcf = 0)
	{
		// Really std::function<double, int>, but auto is easier.
		auto ED = [=](int i) -> double { 
			double EDi = ED_(t[0], t[i], D[0], D[i], sigma);

			if (i < n - 1)
				EDi *= c*(dcf ? dcf[i] : t[i + 1] - t[i]);

			return EDi;
		};

		// Use get<0>(ms2) to get the mean, get<1>(ms2) to get the variance.
		std::tuple<double,double> ms2 = lognormalsum(n - 1, 
			[=](int i) { return ED(i + 1); },
			[=](int i, int j) { return CovD_(t[0], t[i + 1], t[j + 1], D[0], ED(i + 1), ED(j + 1), sigma); }
		);

		// Define appropriate values for these.
		double F = exp(std::get<0>(ms2) + std::get<1>(ms2)/2);
		double S = sqrt(std::get<1>(ms2)/t[0]);
		double K = 1;
		double T = t[0];

		return black::value(F, S, K, T);
	}

} // namespace ho_lee
