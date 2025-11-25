#ifndef LOANCALCULATOR_H_INCLUDED 
#define LOANCALCULATOR_H_INCLUDED

#include <string>

class LoanCalculator
{
public:
  /**
   * @brief Constructor to initialize the calculator
   */
  LoanCalculator();

  /**
   * @brief Destructor
   */
  ~LoanCalculator() {}

  //
  // Setters and Getters
  //

  /**
   * @brief Set the total loan amount
   * @param A Loan amount (principal)
   */
  inline void setAmount(float A) { amount_ = A; amountSet_ = true; }

  /**
   * @brief Get the total loan amount
   * @return Loan amount
   */
  inline float getAmount() const { return amount_; }

  /**
   * @brief Set initial payment or down payment
   * @param initialA Initial payment
   */
  inline void setInitialPayment(float initialA)  { initialPayment_ = initialA; }

  /**
   * @brief Get initial payment
   * @return Initial payment
   */
  inline float getInitialPayment() const         { return initialPayment_; }

  /**
   * @brief Set yearly interest
   * @param i Interest rate in percentage (like 6.75)
   */
  void setInterest(float i) { interest_ = i; interestPeriodic_ = i/100.0/12.0; interestSet_ = true; }

  /**
   * @brief Get yearly interest
   * @return Interest in percentage
   */
  inline float getInterest() const         { return interest_; }

  /**
   * @brief Get monthly interest rate
   * @return Monthly interest as fraction
   */
  inline float getPeriodicInterest() const { return interestPeriodic_; }

  /**
   * @brief Set monthly payment amount
   * @param P Payment amount
   */
  void setPayment(float P)        { payment_ = P; paymentSet_ = true; }

  /**
   * @brief Get monthly payment
   * @return Payment amount
   */
  inline float getPayment() const { return payment_; }

  /**
   * @brief Set total number of periods
   * @param N Total periods in months
   */
  void setPeriodTotal(int N)        { periodTotal_ = N; periodTotalSet_ = true; }

  /**
   * @brief Get total number of periods
   * @return Total periods
   */
  inline int getPeriodTotal() const { return periodTotal_; }

  /**
   * @brief Set number of elapsed periods
   * @param n Number of periods passed
   */
  void setPeriodElapsed(int n)         { periodElapsed_ = n; periodElapsedSet_ = true; }

  /**
   * @brief Get elapsed periods
   * @return Elapsed periods
   */
  inline int getPeriodElapsed() const  { return periodElapsed_; }

  /**
   * @brief Set loan opening fee
   * @param fee Fee amount
   */
  inline void setOpeningFee(float fee) { openingFee_ = fee; }

  /**
   * @brief Get opening fee
   * @return Fee amount
   */
  inline float getOpeningFee() const   { return openingFee_; }

  /**
   * @brief Set opening fee percentage
   * @param percent Fee as percentage
   */
  inline void setOpeningPercent(float percent) { openingPercent_ = percent; }

  /**
   * @brief Get opening fee percentage
   * @return Fee percentage
   */
  inline float getOpeningPercent() const       { return openingPercent_; }

  /**
   * @brief Reset all calculator values to zero
   */
  inline void reset() {
    amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0;
    periodTotal_ = periodElapsed_ = 0;
    amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
  }

  //
  // The actual calculation methods
  //

  /**
   * @brief Calculate loan balance after some payments
   * @return Balance remaining
   */
  long double calculateLoanBalance();

  /**
   * @brief Calculate monthly payment
   * @return Payment amount
   */
  long double calculatePayment();

  /**
   * @brief Calculate number of payments needed
   * @return Number of payments
   */
  long double calculateNumberPayments();

  /**
   * @brief Calculate original loan amount
   * @return Loan amount
   */
  long double calculateLoanAmount();

  /**
   * @brief Calculate yearly interest rate
   * @return Interest percentage
   */
  long double calculateInterestRate();

  /**
   * @brief Calculate interest rate including fees
   * @return Effective interest percentage
   */
  long double calculateEffectiveInterestRate();

  /**
   * @brief Convert calculator values to string for printing
   * @return Formatted string with values
   */
  std::string toString();

private:
  float amount_;        // loan amount
  bool amountSet_;

  float initialPayment_;     // initial down payment

  float interest_;          // yearly interest
  float interestPeriodic_;  // monthly interest
  bool interestSet_;

  float payment_;       // monthly payment
  bool paymentSet_;

  int periodTotal_;     // total months
  bool periodTotalSet_;

  int periodElapsed_;   // months passed
  bool periodElapsedSet_;

  float openingFee_;        // extra fee
  float openingPercent_;    // extra fee %
};

#endif // LOANCALCULATOR_H_INCLUDED
