// Copyright (c) 2009-2010 No
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include "headers.h"

void assertTrue(bool value, string message)
{
    if (!value)
        throw runtime_error(message);
}

void test_coinbase()
{
    CTransaction tx;
    tx.SetNull();
    assertTrue(tx.IsNull(), "IsNull");
    assertTrue(tx.IsFinal(), "IsFinal");
    assertTrue(!tx.IsCoinBase(), "!IsCoinBase");
    cout << tx.ToString() << "\n";
    tx.vin.resize(1);
    tx.vin[0].prevout.SetNull();
    tx.vout.resize(1);
    tx.vout[0].nValue = 50 * COIN;
    vector<unsigned char> key;
    key.resize(32);
    for (int ind = 0 ; ind < 32 ; ind++)
        key[ind] = ind;
    tx.vout[0].scriptPubKey << key << OP_CHECKSIG;
    tx.vin[0].scriptSig = CScript() << 0x23 << CBigNum(0xcafe);
    assertTrue(tx.IsCoinBase(), "IsCoinBase");
    assertTrue(tx.GetSigOpCount() == 1, "GetSigOpCount");
    assertTrue(tx.GetValueOut() == 50 * COIN, "GetValueOut");
    assertTrue(tx.GetHash().GetHex() == "77574aa2d3a665735b276e54bc28d058ca3da616104118a18555dad7ea0ad498", "GetHash");
    assertTrue(tx.CheckTransaction(), "CheckTransaction");
    cout << tx.ToString() << "\n";
}

void test_transaction()
{
    CTransaction tx;
    tx.SetNull();
    tx.vin.resize(1);
    uint256 prev = uint256(0x6666) << 240;
    tx.vin[0].prevout = COutPoint(prev, 0);
    tx.vout.resize(1);
    tx.vout[0].nValue = 20 * COIN;
    vector<unsigned char> key;
    key.resize(32);
    for (int ind = 0 ; ind < 32 ; ind++)
        key[ind] = ind;
    tx.vout[0].scriptPubKey << key << OP_CHECKSIG;
    tx.vin[0].scriptSig = CScript() << 0x23 << CBigNum(0xcafe);
    assertTrue(!tx.IsCoinBase(), "IsCoinBase");
    assertTrue(tx.CheckTransaction(), "CheckTransaction");
    cout << tx.ToString() << "\n";
}

bool RunTests(int argc, char* argv[])
{
    test_coinbase();
    test_transaction();
    return true;
}

void Shutdown(void* parg)
{
    exit(0);
}

int main(int argc, char* argv[])
{
    bool fRet = false;
    fRet = RunTests(argc, argv);

    if (fRet) {
        cout << "all good\n";
        exit(0);
    }
    else {
        cout << "failures\n";
        exit(1);
    }
}

