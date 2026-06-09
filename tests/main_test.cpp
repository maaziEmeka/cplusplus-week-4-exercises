#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "main.h"

// ---- Free function tests ----------------------------------------------------

TEST(BitcoinUtils, ParseTxid) {
    EXPECT_EQ(parse_txid("aabbccdd"),
              (std::vector<std::string>{"aa", "bb", "cc", "dd"}));
    EXPECT_EQ(parse_txid("deadbeef"),
              (std::vector<std::string>{"de", "ad", "be", "ef"}));
}

TEST(BitcoinUtils, ParseTxidEmpty) {
    EXPECT_EQ(parse_txid(""), std::vector<std::string>{});
}

TEST(BitcoinUtils, CreateUtxo) {
    UTXO utxo = create_utxo("tx1", 0, 1000.0);
    EXPECT_EQ(utxo.txid, "tx1");
    EXPECT_EQ(utxo.vout, 0u);
    EXPECT_DOUBLE_EQ(utxo.amount, 1000.0);
}

TEST(BitcoinUtils, UpdateUtxo) {
    UTXO utxo{"tx1", 0, 1000.0};
    update_utxo(utxo, 1500.0);
    EXPECT_DOUBLE_EQ(utxo.amount, 1500.0);
    EXPECT_EQ(utxo.txid, "tx1");
    EXPECT_EQ(utxo.vout, 0u);
}

TEST(BitcoinUtils, UnpackUtxoContainsTxid) {
    UTXO utxo{"abc", 1, 5000.0};
    std::string result = unpack_utxo(utxo);
    EXPECT_NE(result.find("TXID: abc"), std::string::npos);
}

TEST(BitcoinUtils, SwapAddresses) {
    auto [a, b] = swap_addresses("addr1", "addr2");
    EXPECT_EQ(a, "addr2");
    EXPECT_EQ(b, "addr1");
}

TEST(BitcoinUtils, UniqueAddresses) {
    std::vector<std::string> addresses = {"a", "b", "a"};
    auto unique = unique_addresses(addresses);
    EXPECT_EQ(unique, (std::unordered_set<std::string>{"a", "b"}));
}

TEST(BitcoinUtils, UniqueAddressesEmpty) {
    EXPECT_TRUE(unique_addresses({}).empty());
}

TEST(BitcoinUtils, BlockHeightGenerator) {
    EXPECT_EQ(block_height_generator(100, 103),
              (std::vector<uint32_t>{100, 101, 102}));
}

TEST(BitcoinUtils, BlockHeightGeneratorEmptyRange) {
    EXPECT_TRUE(block_height_generator(50, 50).empty());
}

// ---- BitcoinWallet tests ----------------------------------------------------

TEST(BitcoinWalletTest, EmptyBalance) {
    BitcoinWallet wallet;
    EXPECT_DOUBLE_EQ(wallet.get_balance(), 0.0);
}

TEST(BitcoinWalletTest, AddUtxosAndComputeBalance) {
    BitcoinWallet wallet;
    UTXO utxo1 = create_utxo("tx1", 0, 1.0);
    UTXO utxo2 = create_utxo("tx1", 1, 0.5);

    wallet.add_utxo(utxo1);
    EXPECT_DOUBLE_EQ(wallet.get_balance(), 1.0);

    wallet.add_utxo(utxo2);
    EXPECT_DOUBLE_EQ(wallet.get_balance(), 1.5);

    // Both UTXOs should be stored under distinct composite keys.
    EXPECT_EQ(wallet.utxos.size(), 2u);
}

// ---- TransactionPool tests --------------------------------------------------

TEST(TransactionPoolTest, AddAndDedup) {
    TransactionPool pool;
    EXPECT_EQ(pool.get_pool_size(), 0u);

    EXPECT_TRUE(pool.add_transaction("tx1"));
    EXPECT_EQ(pool.get_pool_size(), 1u);

    EXPECT_FALSE(pool.add_transaction("tx1"));  // duplicate
    EXPECT_EQ(pool.get_pool_size(), 1u);

    EXPECT_TRUE(pool.add_transaction("tx2"));
    EXPECT_EQ(pool.get_pool_size(), 2u);
}
