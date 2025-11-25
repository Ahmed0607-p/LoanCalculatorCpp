#include<stdlib.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <QApplication>

#include <LoanCalcQtMainWindow.h>
#include <CmdLineParser.h>
#include <LoanCalculator.h>
#include "Utils.h"

using namespace std;

/// Types of calculations we can do
enum CALC_TYPE
{
  CALC_UNKNOWN=0,    ///< Unknown calculation type
  CALC_BALANCE=100,  ///< Calculate remaining loan balance
  CALC_PAYMENT,      ///< Calculate monthly payment
  CALC_NUMPAYMENTS,  ///< Calculate number of payments
  CALC_AMOUNT,       ///< Calculate initial loan amount
  CALC_INTEREST      ///< Calculate yearly interest rate
};

// Command line options
const string ARG_CALC_BALANCE      = "-cb";
const string ARG_CALC_PAYMENT      = "-cp";
const string ARG_CALC_NUMPAYMENTS  = "-cn";
const string ARG_CALC_AMOUNT       = "-ca";
const string ARG_CALC_INTEREST     = "-ci";

const string ARG_PAYMENT           = "-p";
const string ARG_PERIOD_TOTAL      = "-N";
const string ARG_PERIOD_ELAPSED    = "-n";
const string ARG_AMOUNT            = "-a";
const string ARG_INITIAL_PAYMENT   = "-ai";
const string ARG_INTEREST          = "-i";
const string ARG_OPENFEE           = "-of";
const string ARG_OPENPERCENT       = "-op";

/// Set up command line options
/**
 * @brief Initializes the command line parser with all options
 * @param clp Reference to the CmdLineParser object
 */
void loadCmdLine(CmdLineParser &clp)
{
  clp.setMainHelpText("A simple loan calculator");
  clp.setMainHelpTextEnd("With no options set, a GUI will be launched");

  // Calculation types
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_BALANCE,
         "Calculate the loan balance after making several payments, given:\n"
         "\t\t loan amount, interest, monthly payment and number of monthly payments made so far",
         false, CALC_BALANCE));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_PAYMENT,
         "Calculate the monthly loan payment, given: loan amount, loan period, and interest",
         false, CALC_PAYMENT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_NUMPAYMENTS,
         "Calculate the number of payments needed to pay a loan, given: loan amount, monthly payment, interest",
         false, CALC_NUMPAYMENTS));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_AMOUNT,
         "Calculate the initial loan amount, given: monthly payment, loan period, and interest",
         false, CALC_AMOUNT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_INTEREST,
         "Calculate the loan interest, given: loan amount, loan period, and monthly payment",
         false, CALC_INTEREST));
  clp.setMutExclUsageText("Calculations");

  // Different values
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_PAYMENT, "Set the monthly loan payment. Ej: 325.67"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_TOTAL, "Set the total loan period in months. Ej: 60"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_ELAPSED, "Set the elapsed period in months. Ej: 32"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_AMOUNT, "Set the initial amount. Ej: 19300"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INITIAL_PAYMENT,
         "Set the initial payment, loan will be for (initial amount - initial payment) Ej: 1000, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INTEREST, "Set the yearly interest rate. Ej: 6.75"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENFEE, "Set fees for opening the loan. Ej: 100, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENPERCENT,
         "Set fees for opening the loan, charged as a percentage. Ej: 2.75%, Default 0.0%"));
  
  clp.setMinNumberArgs(3);
}

//
// Simple Command line parser
//
/**
 * @brief Reads command line arguments and sets values in the calculator
 * @param argc Argument count
 * @param argv Argument values
 * @param clp Command line parser object
 * @param calculator LoanCalculator object
 * @return Type of calculation to perform (CALC_TYPE)
 */
CALC_TYPE parseCommandLine(int argc, char **argv, CmdLineParser &clp, LoanCalculator &calculator)
{
  CALC_TYPE ct(CALC_UNKNOWN);

  if(!clp.parseCmdLine(argc, argv))
  {
    clp.printUsage();
    return ct;
  }

  // Set all calculator values from command line options
  calculator.setAmount(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_AMOUNT))->getValue());
  calculator.setInitialPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INITIAL_PAYMENT))->getValue());
  calculator.setInterest(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INTEREST))->getValue());
  calculator.setPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_PAYMENT))->getValue());
  calculator.setPeriodTotal(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_TOTAL))->getValue());
  calculator.setPeriodElapsed(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_ELAPSED))->getValue());
  calculator.setOpeningFee(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENFEE))->getValue());
  calculator.setOpeningPercent(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENPERCENT))->getValue());

  CmdLineOption *option(clp.getMutExclOption());
  if(option != NULL) ///< If a calculation option was selected
  {
    ct = (CALC_TYPE) ((CmdLineOptionFlag*) option)->getValueKey();
  }

  return ct;
}

//
// Main program
//
/**
 * @brief Main function - program starts here
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return Exit code
 */
int main(int argc, char **argv)
{
  LoanCalculator calculator;
  loadConfig(calculator);

  // If no arguments are given, then launch the GUI
  if(argc == 1)
  {
    QApplication app(argc, argv);

    LoanCalcQtMainWindow mainWindow(&calculator);
    mainWindow.show();

    return app.exec();
  }

  //
  // Parse the command line arguments
  //
  CmdLineParser clp;
  loadCmdLine(clp);
  CALC_TYPE ct = parseCommandLine(argc, argv, clp, calculator);

  // Validation/checks
  if (calculator.getAmount() <= 0) {
      std::cout << "Error! Loan amount can't be negative\n";
      return 1;
  }
  if (calculator.getInterest() <= 0) {
      std::cout << "Error! Interest rate can't be negative\n";
      return 1;
  }
  if (calculator.getPeriodTotal() <= 0) {
      std::cout << "Error! Total loan period must be positive\n";
      return 1;
  }
  if (calculator.getPeriodElapsed() < 0) {
      std::cout << "Error! Elapsed period cant be negative\n";
      return 1;
  }  

  try
  {
    std::cout << "\n";

    // Perform calculation based on selected type
    if(ct == CALC_BALANCE)
    {
        std::cout << "Loan Balance = " << (float) calculator.calculateLoanBalance() << "\n";
    }
    else if(ct == CALC_PAYMENT)
    {
      float payment = calculator.calculatePayment();
      std::cout << "Monthly Payment    = " << payment << "\n"
           << "Total amt paid     = " << (float) (payment*calculator.getPeriodTotal())
           << "\n";

      if(calculator.getOpeningPercent() != 0.0 || calculator.getOpeningFee() != 0.0)
      {
        std::cout << "Interest with fees = "
             << (float) calculator.calculateEffectiveInterestRate()
             << "%"
             << "\n";
      }
    }
    else if(ct == CALC_NUMPAYMENTS)
    {
      std::cout << "Number of payments = " << (float) calculator.calculateNumberPayments() << "\n";
    }
    else if(ct == CALC_AMOUNT)
    {
      std::cout << "Initial Loan amount = " << (float) calculator.calculateLoanAmount() << "\n";
    }
    else if(ct == CALC_INTEREST)
    {
      std::cout << "Yearly Interest Rate = " << (float) calculator.calculateInterestRate() << "%\n";
    }
    else if(ct == CALC_UNKNOWN)
    {
      // Most likely help was requested
      return 1;
    }
    else
    {
      std::cerr << "Unrecognized calculation type, exiting\n";
      return 0;
    }

    // Print all values set in the calculator
    std::cout << calculator.toString() << "\n";
  }
  catch(const exception &e)
  {
    std::cerr << "Error executing loan calculator: " << + e.what() << "\n";
  }

  std::cout << "\n";
  return 0; 
}
