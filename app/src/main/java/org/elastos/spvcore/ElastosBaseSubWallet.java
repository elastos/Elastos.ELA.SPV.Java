// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

import android.util.Log;

/**
 * ElastosBaseSubWallet
 */
public class ElastosBaseSubWallet extends SubWallet {
    private long mInstance;
    private String TAG = "ElastosBaseSubWallet";

    public String CreateTransaction(String inputs, String outputs, String fee, String memo) throws WalletException {
        return CreateTransaction(mInstance, inputs, outputs, fee, memo);
    }

    public String GetTransactionSignedInfo(String tx) throws WalletException {
        return GetTransactionSignedInfo(mInstance, tx);
    }

    public String ConvertToRawTransaction(String rawTransaction) throws WalletException {
        return ConvertToRawTransaction(mInstance, rawTransaction);
    }

    public ElastosBaseSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    protected long GetProxy() {
        return mInstance;
    }

    private native String CreateTransaction(long subProxy, String inputs, String outputs, String fee, String memo);

    private native String GetTransactionSignedInfo(long subProxy, String rawTransaction);

    private native String ConvertToRawTransaction(long subProxy, String tx);
}
