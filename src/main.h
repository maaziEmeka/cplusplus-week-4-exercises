#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// ---- UTXO -------------------------------------------------------------------

struct UTXO {
    std::string txid;
    uint32_t vout = 0;
    double amount = 0.0;

    bool operator==(const UTXO& other) const;
};

// ---- Free functions ---------------------------------------------------------

// Split `txid` into 2-character segments. Example:
//   "deadbeef" -> {"de", "ad", "be", "ef"}
std::vector<std::string> parse_txid(const std::string& txid);

// Build a UTXO from its three fields.
UTXO create_utxo(const std::string& txid, uint32_t vout, double amount);

// Mutate `utxo` in place, setting its amount to `new_amount`.
void update_utxo(UTXO& utxo, double new_amount);

// Return a human-readable string of the form:
//   "TXID: <txid>, VOUT: <vout>, AMOUNT: <amount>"
std::string unpack_utxo(const UTXO& utxo);

// Return {b, a} (i.e. the two addresses swapped).
std::pair<std::string, std::string> swap_addresses(const std::string& a,
                                                   const std::string& b);

// Return the set of distinct addresses from the input list.
std::unordered_set<std::string> unique_addresses(
    const std::vector<std::string>& addresses);

// Return the block heights in the half-open range [start, end).
std::vector<uint32_t> block_height_generator(uint32_t start, uint32_t end);

// ---- BitcoinWallet ----------------------------------------------------------

class BitcoinWallet {
public:
    // Key is "<txid>:<vout>".
    std::unordered_map<std::string, UTXO> utxos;

    // Store `utxo` under the composite key.
    void add_utxo(const UTXO& utxo);

    // Sum the amounts of every stored UTXO.
    double get_balance() const;
};

// ---- TransactionPool --------------------------------------------------------

class TransactionPool {
public:
    std::unordered_set<std::string> tx_pool;

    // Insert `txid`. Returns true if it was newly inserted, false if `txid`
    // was already present.
    bool add_transaction(const std::string& txid);

    std::size_t get_pool_size() const;
};
