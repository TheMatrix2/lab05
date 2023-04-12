#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class Account_mock : public Account {
public:
    Account_mock(int id, int balance) : Account(id, balance) {}

    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

class Transaction_mock : public Transaction {
public:
    MOCK_METHOD3(Make, bool(Account & from, Account & to, int sum));
    MOCK_METHOD1(Debit, void(int diff));
    MOCK_METHOD1(Credit, void(int diff));
};

TEST(Account, Mock) {
    Account_mock acc(1, 100);
    EXPECT_CALL(acc, GetBalance()).Times(1);
    EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(2);
    EXPECT_CALL(acc, Lock()).Times(2);
    EXPECT_CALL(acc, Unlock()).Times(1);
    acc.GetBalance();
    acc.ChangeBalance(100); // throw
    acc.Lock();
    acc.ChangeBalance(100);
    acc.Lock(); // throw
    acc.Unlock();
}

TEST(Account, Test) {
    Account acc(1, 100);
    EXPECT_EQ(acc.id(), 1);
    EXPECT_EQ(acc.GetBalance(), 100);
    EXPECT_NO_THROW(acc.Lock());
    acc.ChangeBalance(100);
    EXPECT_EQ(acc.GetBalance(), 200);
}

TEST(Transaction, Mock) {
    Transaction_mock tr;
    Account acc1(1, 50);
    Account acc2(2, 500);
    EXPECT_CALL(tr, Make(testing::_, testing::_, testing::_)).Times(6);
    tr.set_fee(100);
    tr.Make(acc1, acc2, 199);
    tr.Make(acc2, acc1, 500);
    tr.Make(acc2, acc1, 300);
    tr.Make(acc1, acc1, 0);
    tr.Make(acc1, acc2, -1);
    tr.Make(acc1, acc2, 99);
}

TEST(Transaction, Test) {
    Transaction tr;
    Account ac1(1, 50);
    Account ac2(2, 500);
    tr.set_fee(100);
    EXPECT_EQ(tr.fee(), 100);
    EXPECT_THROW(tr.Make(ac1, ac1, 0), std::logic_error);
    EXPECT_THROW(tr.Make(ac1, ac2, -1), std::invalid_argument);
    EXPECT_THROW(tr.Make(ac1, ac2, 99), std::logic_error);
    EXPECT_FALSE(tr.Make(ac1, ac2, 199));
    EXPECT_FALSE(tr.Make(ac2, ac1, 500));
    EXPECT_FALSE(tr.Make(ac2, ac1, 300));
}