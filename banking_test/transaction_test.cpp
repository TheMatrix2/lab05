#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>

TEST(Transaction, DataBase) {
    Account from(123, 1000);
    Account to(789, 100);
    Transaction transaction;
    transaction.set_fee(100);
    EXPECT_TRUE(transaction.Make(from, to, 500));
    EXPECT_FALSE(transaction.Make(from, to, 100000));
    EXPECT_EQ(from.GetBalance(), 400);
    EXPECT_EQ(to.GetBalance(), 600);
}

TEST(Transaction, Exceptions) {
    Account from(123, 1000);
    Account to(789, 100);
    Transaction transaction;
    transaction.set_fee(500);
    EXPECT_THROW(transaction.Make(from, from, 100), std::logic_error);
    EXPECT_THROW(transaction.Make(from, to, -100), std::invalid_argument);
    EXPECT_THROW(transaction.Make(from, to, 1), std::logic_error);
    EXPECT_FALSE(transaction.Make(from, to, 500));
}
