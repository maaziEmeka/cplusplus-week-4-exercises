#include "main.h"

// ---- Supporting type member functions ---------------------------------------

bool UTXO::operator==(const UTXO& other) const {
    return txid == other.txid
        && vout == other.vout
        && amount == other.amount;
}

// ---- Free function exercises ------------------------------------------------

std::vector<std::string> parse_txid(const std::string& txid) {
    // TODO: split `txid` into 2-character chunks and return them as a vector
    //       of strings. Example: "aabbccdd" -> {"aa", "bb", "cc", "dd"}.
    (void)txid;
    std::vector<std::string> result;
    for (size_t i = 0; i < txid.size(); i += 2) {
        result.push_back(txid.substr(i, 2));
    }
    return result;
}

UTXO create_utxo(const std::string& txid, uint32_t vout, double amount) {
    // TODO: construct a UTXO from the three arguments.
    (void)txid;
    (void)vout;
    (void)amount;
    return UTXO{txid, vout, amount};
}

void update_utxo(UTXO& utxo, double new_amount) {
    // TODO: update `utxo.amount` in place to `new_amount`.
    (void)utxo;
    (void)new_amount;
    utxo.amount = new_amount;
}

std::string unpack_utxo(const UTXO& utxo) {
    // TODO: return a string of the form
    //       "TXID: <txid>, VOUT: <vout>, AMOUNT: <amount>"
    //       built from `utxo`'s fields.
    (void)utxo;
    return "TXID: " + utxo.txid + ", VOUT: " + std::to_string(utxo.vout) + ", AMOUNT: " + std::to_string(utxo.amount);
}

std::pair<std::string, std::string> swap_addresses(const std::string& a,
                                                   const std::string& b) {
    // TODO: return the two addresses swapped.
    (void)a;
    (void)b;
    return {b, a};
}

std::unordered_set<std::string> unique_addresses(
    const std::vector<std::string>& addresses) {
    // TODO: return a set containing each distinct address from `addresses`.
    (void)addresses;
    std::unordered_set<std::string> result;
    for (const auto& address : addresses) {
        result.insert(address);
    }
    return result;
}

std::vector<uint32_t> block_height_generator(uint32_t start, uint32_t end) {
    // TODO: return every height in the half-open range [start, end).
    (void)start;
    (void)end;
    std::vector<uint32_t> result;
    for (uint32_t i = start; i < end; ++i) {
        result.push_back(i);
    }
    return result;
}

// ---- BitcoinWallet ----------------------------------------------------------

void BitcoinWallet::add_utxo(const UTXO& utxo) {
    // TODO: store `utxo` in `utxos`, keyed by "<txid>:<vout>".
    //       Hint: std::to_string for the vout component.
    (void)utxo;
    utxos[utxo.txid + ":" + std::to_string(utxo.vout)] = utxo;
}

double BitcoinWallet::get_balance() const {
    // TODO: sum the `amount` of every UTXO in `utxos`.
    double balance = 0.0;
    for (const auto& pair : utxos) {
        balance += pair.second.amount;
    }
    return balance;
}

// ---- TransactionPool --------------------------------------------------------

bool TransactionPool::add_transaction(const std::string& txid) {
    // TODO: insert `txid` into `tx_pool`. Return true if it was newly
    //       inserted, false if it was already present.
    //       Hint: std::unordered_set::insert returns {iterator, bool}.
    (void)txid;
    auto result = tx_pool.insert(txid);

    return result.second;
}

std::size_t TransactionPool::get_pool_size() const {
    // TODO: return the number of transactions in `tx_pool`.
    return tx_pool.size();
}
