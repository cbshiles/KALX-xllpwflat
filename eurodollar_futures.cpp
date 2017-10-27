// eurodollar_futures.cpp - cash deposit
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"
#include "../fmspwflat/eurodollar_futures.h"

using namespace datetime;
using namespace instrument;
using namespace xll;
using namespace xml;

static AddInX xai_eurodollar_futures(
	FunctionX(XLL_HANDLEX XLL_UNCALCEDX, _T("?xll_eurodollar_futures"), _T("EURODOLLAR.FUTURES"))
	.Arg(XLL_WORDX,   _T("Nth"), _T("is the position of the contract "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle to an Eurodollar futures."))
	.Documentation(element()
		.content(element(_T("para")).content(_T("The value 1 for <codeInline>Nth</codeInline> indicates the front month contract. ")))
		.content(element(_T("para")).content(_T("The Chicago Mercantile Exchange uses the following definitions: ")))
		.content(element(_T("definitionTable"))
			.content(element(_T("definedTerm")).content(_T("Underlying Instrument")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("Eurodollar Time Deposit having a principal value of USD $1,000,000 with a three-month maturity."))))
			.content(element(_T("definedTerm")).content(_T("Price Quote")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("Quoted in IMM Three-Month LIBOR index points or 100 minus the rate on an annual basis over a 360 day year (e.g., a rate of 2.5% shall be quoted as 97.50). 1 basis point = .01 = $25."))))
			.content(element(_T("definedTerm")).content(_T("Tick Size")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("One-quarter of one basis point (0.0025 = $6.25 per contract) in the nearest expiring contract month; One-half of one basis point (0.005 = $12.50 per contract) in all other contract months. The \"new\" front-month contract begins trading in 0.0025 increments on the same Trade Date as the Last Trading Day of the expiring \"old\" front-month contract."))))
			.content(element(_T("definedTerm")).content(_T("Contract Months")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("Mar, Jun, Sep, Dec, extending out 10 years (total of 40 contracts) plus the four nearest serial expirations (months that are not in the March quarterly cycle). The new contract month terminating 10 years in the future is listed on the Tuesday following expiration of the front quarterly contract month."))))
			.content(element(_T("definedTerm")).content(_T("Last Trading Day")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("The second London bank business day prior to the third Wednesday of the contract expiry month. Trading in the expiring contract closes at 11:00 a.m. London Time on the last trading day."))))
			.content(element(_T("definedTerm")).content(_T("Final Settlement")))
			.content(element(_T("definition")).content(element(_T("para")).content(_T("Expiring contracts are cash settled to 100 minus the British Bankers' Association survey of 3-month U.S. Dollar LIBOR on the last trading day. Final settlement price will be rounded to four decimal places, equal to 1/10,000 of a percent, or $0.25 per contract."))))
		)
		,
		externalLink(_T("Chicago Mercantile Exchange"), _T("http://cme.com"))
	)
);
HANDLEX WINAPI
xll_eurodollar_futures(WORD ordinal)
{
#pragma XLLEXPORT
	handle<fixed_income_instrument> h;

	try {
		h = new instrument::eurodollar_futures(ordinal);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h.get();
}

static AddInX xai_eurodollar_futures_valuation(
	FunctionX(XLL_HANDLEX, _T("?xll_eurodollar_futures_valuation"), _T("EURODOLLAR.FUTURES.VALUATION"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle to an Eurodollar futures"))
	.Arg(XLL_DOUBLEX, _T("Settlement"), IS_SETTLEMENT)
	.Arg(XLL_DOUBLEX, _T("Price"), IS_PRICE _T(" "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Calculate cash flows and return the handle of the Eurodollar futures. "))
	.Documentation(
		_T("Price is 100 minus the rate in percent. ")
	)
);
HANDLEX WINAPI
xll_eurodollar_futures_valuation(HANDLEX h, double set, double price)
{
#pragma XLLEXPORT
	try {
		eurodollar_futures *pfra(0);

		ensure (0 != (pfra = dynamic_cast<eurodollar_futures*>(handle<fixed_income_instrument>(h).ptr())));

		pfra->valuation(date(set), price);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}
