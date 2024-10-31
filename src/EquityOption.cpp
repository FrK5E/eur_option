/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*!
 Copyright (C) 2005, 2006, 2007, 2009 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include "WEHalf.hpp"
#include <ql/qldefines.hpp>
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#    include <ql/auto_link.hpp>
#endif
#include <ql/instruments/vanillaoption.hpp>
#include <ql/math/integrals/tanhsinhintegral.hpp>
#include <ql/pricingengines/vanilla/analyticeuropeanengine.hpp>
#include <ql/pricingengines/vanilla/analyticeuropeanvasicekengine.hpp>
#include <ql/pricingengines/vanilla/analytichestonengine.hpp>
#include <ql/pricingengines/vanilla/baroneadesiwhaleyengine.hpp>
#include <ql/pricingengines/vanilla/batesengine.hpp>
#include <ql/pricingengines/vanilla/binomialengine.hpp>
#include <ql/pricingengines/vanilla/bjerksundstenslandengine.hpp>
#include <ql/pricingengines/vanilla/fdblackscholesvanillaengine.hpp>
#include <ql/pricingengines/vanilla/integralengine.hpp>
#include <ql/pricingengines/vanilla/mcamericanengine.hpp>
#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>
#include <ql/pricingengines/vanilla/qdfpamericanengine.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/utilities/dataformatters.hpp>
#include <iomanip>
#include <iostream>

using namespace QuantLib;

int main(int, char*[]) {

    std::cout << std::endl;

    // set up dates
    Calendar calendar = TARGET();
    Date todaysDate(15, May, 1998);
    Date settlementDate(17, May, 1998);
    Settings::instance().evaluationDate() = todaysDate;

    // our options
    Option::Type type(Option::Put);
    Real underlying = 36;
    Real strike = 40;
    Spread dividendYield = 0.00;
    Rate riskFreeRate = 0.06;
    Volatility volatility = 0.20;
    Date maturity(17, May, 1999);
    DayCounter dayCounter = Actual365Fixed();
    DayCounter wehalf = WEHalf(); 

    std::cout << "Option type = " << type << std::endl;
    std::cout << "Maturity = " << maturity << std::endl;
    std::cout << "Underlying price = " << underlying << std::endl;
    std::cout << "Strike = " << strike << std::endl;
    std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate) << std::endl;
    std::cout << "Dividend yield = " << io::rate(dividendYield) << std::endl;
    std::cout << "Volatility = " << io::volatility(volatility) << std::endl;
    std::cout << std::endl;
    std::string method;
    std::cout << std::endl;

    // write column headings
    Size widths[] = {35, 14, 14, 14};
    std::cout << std::setw(widths[0]) << std::left << "Method" << std::setw(widths[1]) << std::left
              << "European" << std::setw(widths[2]) << std::left << "Bermudan"
              << std::setw(widths[3]) << std::left << "American" << std::endl;

    auto europeanExercise = ext::make_shared<EuropeanExercise>(maturity);


    auto underlyingH = makeQuoteHandle(underlying);

    // bootstrap the yield/dividend/vol curves
    Handle<YieldTermStructure> interestRateTermStructure(
        ext::make_shared<FlatForward>(settlementDate, riskFreeRate, dayCounter));
    Handle<YieldTermStructure> dividendTS(
        ext::make_shared<FlatForward>(settlementDate, dividendYield, dayCounter));
    Handle<BlackVolTermStructure> volatilityTS(
        ext::make_shared<BlackConstantVol>(settlementDate, calendar, volatility, wehalf));
    auto payoff = ext::make_shared<PlainVanillaPayoff>(type, strike);
    auto process = ext::make_shared<BlackScholesMertonProcess>(underlyingH, dividendTS,
                                                                  interestRateTermStructure, volatilityTS);

    // options
    VanillaOption europeanOption(payoff, europeanExercise);


    // Black-Scholes for European
    method = "Black-Scholes";
    europeanOption.setPricingEngine(ext::make_shared<AnalyticEuropeanEngine>(process, interestRateTermStructure));
    auto npv = europeanOption.NPV();

    std::cout << std::setw(widths[0]) << std::left << method << std::fixed << std::setw(widths[1])
              << std::left << npv << std::setw(widths[2]) << std::left << "N/A"
              << std::setw(widths[3]) << std::left << "N/A" << std::endl;
}
