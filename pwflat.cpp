// pwflat.cpp - Piecewise flat forward curve.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty made.
#include "xllpwflat.h"

using namespace xll;

static AddInX xai_pwflat(
	ArgsX(CATEGORY)
	.Documentation(
		xml::Conceptual(_T("Bootstrap yield curve that uses piecewise flat continuously compounded forwards."))
		.section(_T("Introduction"),
			_T("A yield curve tells you the cost today of obtaining one unit of some instrument in the future. ")
			_T("Typically the instrument is a currency, but it could also be any commodity. ")
			_T("The market gives you quotes on a stream of fixed amounts at given times. ")
			_T("Building a curve means ")
			_T("finding a discount function that reprices the market. ")
			_T("This is a highly underdetermined problem. ")
		)
		.section(_T("Theory"),
			_T("The <newTerm>discount</newTerm> function, <math>D</math>(<math>t</math>) is ")
			_T("the value now of one unit at time <math>t</math>. ")
			_T("The stream of cash flows ")
			_T("(<math>t<subscript>j</subscript></math>, <math>c<subscript>j</subscript></math>) ")
			_T("having quoted price <math>p</math> is <newTerm>repriced</newTerm> by the curve if ")
			_T("<quote><math>p = " ENT_Sigma "<subscript>j</subscript> c<subscript>j</subscript> ")
			_T("D(t<subscript>j</subscript></math>).</quote> ")
			_T("Every <newTerm>fixed income instrument</newTerm> is simply a stream of fixed cash flows. ")
			_T("The right hand side of the above equation is called the <newTerm>present value</newTerm> ")
			_T("of the instrument. ")
			_T("</para><para>")
			_T("The <newTerm>continuously compounded spot rate</newTerm>, <math>r</math>(<math>t</math>) ")
			_T("is defined in terms of the discount by ")
			_T("<quote><math>D</math>(<math>t</math>) = exp(" ENT_minus "<math>tr</math>(<math>t</math>))</quote> ")
			_T("and the <newTerm>instantaneous forward rate</newTerm>, <math>f</math>(<math>t</math>) ")
			_T("is defined in terms of the discount by ")
			_T("<quote><math>D</math>(<math>t</math>) = exp(" ENT_minus ENT_int "<markup><sub>0</sub></markup>")
			_T("<math><markup><sup>t</sup></markup> f</math>(<math>s</math>)<math> ds</math>).</quote> ")
			_T("Note that <math>f</math>(<math>t</math>) = <math>r</math>(<math>t</math>) + ") 
			_T("<math>tr'</math>(<math>t</math>).")
			_T("</para><para>")
			_T("Any one of the three functions determines the other two, but it is particularly ")
			_T("convenient to use the forward rate. ")
			_T("For example, it it not obvious that a given spot curve might imply negative forward rates. ")
			_T("Also note that the discount is obtained by an integral of the forward curve. ")
			_T("This helps smooth out any kinks in the forward parameterization. ")
		)
		.section(_T("Bootstrap"),
			_T("The <newTerm>bootstrap</newTerm> method for constructing a curve uses a piecewise constant ")
			_T("instantaneous forward curve. ") 
			_T("As fixed income instruments of increasing maturity are added to the the curve, the forward from the ")
			_T("maturity of the last instrument added is the constant value that reprices the new instrument .")
			_T("The values of the previously added instruments will not be affected by this. ")
		)
		.section(_T("Conventions"),
			_T("The <newTerm>valuation date</newTerm> of a curve is the date at which the quotes ")
			_T("for the instruments that make up the curve are valid. ")
			_T("The quotes for all of the instruments should be at the same valuation date. ")
			_T("The <newTerm>settlement date</newTerm> of a curve is the date at which the discount is 1. ")
			_T("Typically the date of the first cash flow of each cash deposit is equal to the settlement date. ")
			_T("Every instrument has an <newTerm>effective date</newTerm> and a <newTerm>termination date</newTerm>. ")
			_T("The effective date is the date of the first cash flow associated with the instrument and ")
			_T("the termination date is the date of the last cash flow. ")
			_T("</para><para>")
			_T("It is an error to query the curve past the maximum of the termination dates of the instruments ")
			_T("used to build the curve. If you wish to extrapolate then you can add a synthetic instrument ")
			_T("having an arbitrarily large termination date. ")
		)
	)
);

// Recalculate on open to hook up handles???