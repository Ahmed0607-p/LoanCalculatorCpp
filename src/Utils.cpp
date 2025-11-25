#include "Utils.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/**
 * @brief Loads default loan settings from config.txt
 * If file is missing, function returns silently.
 * @param calc Reference to LoanCalculator object
 */
void loadConfig(LoanCalculator &calc) {
    ifstream file("config.txt");
    if (!file.is_open()) {
        //if config file doesn't fount , simply return 
        return;
    }

    string line;
    while (getline(file, line)) {
        string key;
        long double value;
        char equalSign;

        //key=value
        stringstream ss(line);

        if (ss >> key >> equalSign >> value) {

            if (key == "default_loan_amount")
                calc.setAmount(value);

            else if (key == "default_interest_rate")
                calc.setInterest(value);

            else if (key == "default_tenure_months")
                calc.setPeriodTotal((int)value);

            else if (key == "default_initial_Payment")
                calc.setInitialPayment(value);

            else if (key == "default_opening_Fee")
                calc.setOpeningFee(value);

            else if (key == "default_opening_Percent")
                calc.setOpeningPercent(value);
        }
    }
}
