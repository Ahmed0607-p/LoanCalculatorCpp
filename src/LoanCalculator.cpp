#include<iostream>
#include <math.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <math.h>

#include "LoanCalculator.h"

using namespace std;

/**
 * @brief Constructor for LoanCalculator
 * Initializes all flags and values to defaults.
 */
LoanCalculator::LoanCalculator() :
  amountSet_(false),
  initialPayment_(0.0),
  interestSet_(false),
  paymentSet_(false),
  periodTotalSet_(false),
  periodElapsedSet_(false),
  openingFee_(0.0),
  openingPercent_(0.0)
{
}

//
// The actual calculation methods
//

/**
 * @brief Calculates the loan balance after some payments
 * @return Remaining loan balance
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculateLoanBalance()
{
  if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and elapsed period for this calculation" );
  }

  return (amount_*powl((1+interestPeriodic_), periodElapsed_)) -
         (payment_/interestPeriodic_)*(powl((1+interestPeriodic_), periodElapsed_)-1);
}

/**
 * @brief Calculates the monthly payment for the loan
 * @return Payment amount
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculatePayment()
{
  if(!amountSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and total period for this calculation" );
  }

  long double totalAmount = amount_ - initialPayment_;
  totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0));

  return (interestPeriodic_*totalAmount) /
         (1 - powl((1+interestPeriodic_), (-1*periodTotal_)));
}

/**
 * @brief Calculates number of payments needed to repay the loan
 * @return Number of payments
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculateNumberPayments()
{
  if(!amountSet_ || !interestSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and payment for this calculation" );
  }

  return (-1.0*log1p(1.0-(interestPeriodic_*amount_/payment_))) /
         log1p(1.0 + interestPeriodic_);
}

/**
 * @brief Calculates original loan amount based on payment and interest
 * @return Loan amount
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculateLoanAmount()
{
  if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set payment, interest, and total period for this calculation" );
  }

  return (payment_/interestPeriodic_) *
         (1 - powl((1+interestPeriodic_), (-1*periodTotal_)));
}

/**
 * @brief Calculates yearly interest rate approximately
 * @return Yearly interest rate in percent
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculateInterestRate()
{
  if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount, payment, and total period for this calculation" );
  }

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = powl((powl((1.0 + payment_/amount_), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
}

/**
 * @brief Calculates effective interest rate including fees
 * @return Effective yearly interest rate in percent
 * @throws invalid_argument if necessary values are not set
 */
long double LoanCalculator::calculateEffectiveInterestRate()
{
  if(!amountSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount and total period for this calculation" );
  }

  float payment = calculatePayment();
  float totalAmount = amount_ - initialPayment_;

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = powl((powl((1.0 + payment/totalAmount), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
}

/**
 * @brief Returns a summary of all set values
 * @return String containing loan details
 */
std::string LoanCalculator::toString()
{
  stringstream ss;

  //ss << "LoanCalculator set values:\n";

  if(amountSet_)
  {
    ss << "Initial Amount:      " << amount_ << "\n";
  }

  if(initialPayment_ != 0.0)
  {
    ss << "Initial Payment:     " << initialPayment_ << "\n";
    ss << "Actual Loan Amount:  " << (amount_ - initialPayment_) << "\n";
  }

  if(interestSet_)
  {
    ss << "Yearly Interest:     " << interest_ << "%\n";
    //ss << "Yearly Interest:     " << interest_
    //   << "\nMonthly Interest:    " << interestPeriodic_ << "\n";
  }

  if(paymentSet_)
  {
    ss << "Monthly payment:     " << payment_ << "\n";
  }

  if(periodTotalSet_)
  {
    ss << "Loan Period:         " << periodTotal_ << " months\n";
  }

  if(periodElapsedSet_)
  {
    ss << "Elapsed Period:      " << periodElapsed_ << " months\n";
  }

  if(openingFee_ != 0.0)
  {
      ss << "Opening Fee:       " << openingFee_ << "\n";
  }

  if(openingPercent_ != 0.0)
  {
    ss << "Opening Fee %:       " << openingPercent_ << "% = "
       << openingPercent_/100*(amount_ - initialPayment_) << "\n";
  }

  return ss.str();
}
