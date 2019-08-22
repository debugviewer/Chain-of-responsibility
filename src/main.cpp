#include "Handler.h"
#include <stdio.h>

using namespace bank::loan;

spLoanHandler initLoanBusinessChain() {
	auto _employee = std::make_shared<Employee>(EMPLOYEE_LIMIT);
	auto _bank_manager = std::make_shared<BankManager>(BANK_MANAGER_LIMIT);
	auto _city_bank_manager = std::make_shared<CityBankManager>(CITY_BANK_MANAGER_LIMIT);
	auto _province_bank_manager = std::make_shared<ProvinceBankManager>(PROVINCE_BANK_MANAGER_LIMIT);
	auto _top_bank_manager = std::make_shared<TopBankManager>(TOP_BANK_MANAGER_LIMIT);
	
	// set the chain of responsibility
	// Employee --> BankManager --> CityBankManager --> ProvinceBankManager --> TopBankManager
	_employee->setNext(_bank_manager);
	_bank_manager->setNext(_city_bank_manager);
	_city_bank_manager->setNext(_province_bank_manager);
	_province_bank_manager->setNext(_top_bank_manager);
	
	return _employee;
}

int main() {
	spLoanHandler _loan_handler = initLoanBusinessChain();
	
	uint32_t _loan = 0;
	printf("please input the amount of loan(%d~%d):\n", LOAN_MIN, LOAN_MAX);
	scanf("%d", &_loan);
	if (_loan < LOAN_MIN || _loan > LOAN_MAX) {
		printf("The amount of loan is out of range, the range is %d~%d\n", LOAN_MIN, LOAN_MAX);
		return -1;
	}
	
	const spRequest _req = Request::buildRequest(LOAN_BUSINESS, _loan);
	const spResponse _res = _loan_handler->handle(_req);
	
	if (_res) {
		printf("loan amount is %d, result is %d, %s\n", _loan, _res->getResult(), _res->getInfo());
	}
	
	return 0;
}