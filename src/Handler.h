#ifndef __HANDLER_H__
#define __HANDLER_H__
#include <stdint.h>
#include <string.h>
#include <memory>

namespace bank{
	namespace loan {
		class Request;
		class Response;
		class Handler;
		
		using spRequest = std::shared_ptr<Request>;
		using spResponse = std::shared_ptr<Response>; 
		using spLoanHandler = std::shared_ptr<Handler>; 
		
		typedef enum {
			LOAN_BUSINESS = 1,
		} BUSINESS_TYPE;

		typedef enum {
			PASS = 0,
			NOT_PASS
		} BUSINESS_RESULT;

		typedef enum {
			LOAN_MIN = 5000,
			EMPLOYEE_LIMIT = 10000,
			BANK_MANAGER_LIMIT = 20000,
			CITY_BANK_MANAGER_LIMIT = 30000,
			PROVINCE_BANK_MANAGER_LIMIT = 40000,
			TOP_BANK_MANAGER_LIMIT = 50000,
			LOAN_MAX = 50000,
		}LOAN_LIMIT;

		class Request {
		public:
			static const spRequest buildRequest(BUSINESS_TYPE type, uint32_t loan) {
				return std::make_shared<Request>(type, loan);
			}
			
			Request(BUSINESS_TYPE type, uint32_t loan) : m_type(type), m_loan(loan) {}
			
			BUSINESS_TYPE getType() { return m_type; }
			uint32_t getLoan() { return m_loan; }
		private:
			BUSINESS_TYPE m_type;
			uint32_t m_loan;
		};
		
		class Response {
			static const int MAX_INFO_LEN = 100;
		public:
			static const spResponse buildResponse(BUSINESS_RESULT result, const char *info) {
				return std::make_shared<Response>(result, info);
			}

			Response(BUSINESS_RESULT result, const char *info): m_result(result) {
				if (info) {
					int _len = strlen(info);
					memcpy(m_info, info, _len < MAX_INFO_LEN ? _len : MAX_INFO_LEN);
					m_info[_len] = '\0';
				} else {
					static const char *_unknown_info = "null";
					int _len = strlen(_unknown_info);
					memcpy(m_info, _unknown_info, _len);
					m_info[_len] = '\0';
				}
			}
			
			BUSINESS_RESULT getResult() { return m_result; }
			const char *getInfo() { return m_info; }
		private:
			BUSINESS_RESULT m_result;
			char m_info[MAX_INFO_LEN + 1];
		};

		class Handler {
		public:
			Handler(uint32_t maxLoan) : m_maxLoan(maxLoan), m_nextHandler(NULL) {}
			virtual ~Handler() {}
			
			void setNext(const spLoanHandler &next) {
				m_nextHandler = next;
			}
			
			const spResponse handle(const spRequest &req) {
				if (!req) {
					return Response::buildResponse(NOT_PASS, "Request is null!");
				}
				
				if (hasPermission(req)) {
					return handleSelf(req);
				} else {
					return handleNext(req);
				}
			}	
		private:
			bool hasPermission(const spRequest &req) const {
				return ((req->getType() == LOAN_BUSINESS) && (req->getLoan() <= m_maxLoan));
			}
			
			// subclass must implement it.
			virtual const spResponse handleSelf(const spRequest &req) const = 0;
			
			const spResponse handleNext(const spRequest &req) const {
				if(m_nextHandler) {
					return m_nextHandler->handle(req);
				} else {
					return Response::buildResponse(NOT_PASS, "this is the last handler.\n");
				}
			}
		private:
			uint32_t m_maxLoan;
			spLoanHandler m_nextHandler;
		};

		class Employee : public Handler {
		public:
			Employee(uint32_t maxLoan) : Handler(maxLoan) {}
			virtual ~Employee() {}
		private:	
			const spResponse handleSelf(const spRequest &req) const {
				return Response::buildResponse(PASS, "Employee approved");
			}
		};

		class BankManager : public Handler {
		public:
			BankManager(uint32_t maxLoan) : Handler(maxLoan) {}
			virtual ~BankManager() {}
		private:
			const spResponse handleSelf(const spRequest &req) const {
				return Response::buildResponse(PASS, "BankManager approved");
			}
		};

		class CityBankManager : public Handler {
		public:
			CityBankManager(uint32_t maxLoan) : Handler(maxLoan) {}
			virtual ~CityBankManager() {}
		private:
			const spResponse handleSelf(const spRequest &req) const {
				return Response::buildResponse(PASS, "CityBankManager approved");
			}
		};

		class ProvinceBankManager : public Handler {
		public:
			ProvinceBankManager(uint32_t maxLoan) : Handler(maxLoan) {}
			virtual ~ProvinceBankManager() {}
		private:
			const spResponse handleSelf(const spRequest &req) const {
				return Response::buildResponse(PASS, "ProvinceBankManager approved");
			}
		};

		class TopBankManager : public Handler {
		public:
			TopBankManager(uint32_t maxLoan) : Handler(maxLoan) {}
			virtual ~TopBankManager() {}
		private:
			const spResponse handleSelf(const spRequest &req) const {
				return Response::buildResponse(PASS, "TopBankManager approved");
			}
		};
	}
}
#endif