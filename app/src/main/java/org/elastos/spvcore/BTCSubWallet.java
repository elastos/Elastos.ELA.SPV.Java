// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

import android.util.Log;

/**
 * SubWallet
 */
public class BTCSubWallet extends SubWallet {
    private long mInstance;
    private String TAG = "BTCSubWallet";

    public String GetLegacyAddresses(int index, int count, boolean internal) throws WalletException {
        return GetLegacyAddresses(mInstance, index, count, internal);
    }

    public BTCSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    protected long GetProxy() {
        return mInstance;
    }

    private native String GetLegacyAddresses(long subProxy, int index, int count, boolean internal);
}
