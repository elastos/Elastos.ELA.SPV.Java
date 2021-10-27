// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

import android.util.Log;

/**
 * SubWallet
 */
public class SubWallet {
    private long mInstance;
    private String TAG = "SubWallet";

    public String GetChainID() throws WalletException {
        return GetChainID(mInstance);
    }

    public String GetBasicInfo() throws WalletException {
        return GetBasicInfo(mInstance);
    }

    public String GetAddresses(int start, int count, boolean internal) throws WalletException {
        return GetAddresses(mInstance, start, count, internal);
    }

    public String GetPublicKeys(int start, int count, boolean internal) throws WalletException {
        return GetPublicKeys(mInstance, start, count, internal);
    }

    public String SignTransaction(String rawTransaction, String payPassword) throws WalletException {
        return SignTransaction(mInstance, rawTransaction, payPassword);
    }

    public String Sign(String message, String payPassword) throws WalletException {
        return Sign(mInstance, message, payPassword);
    }

    public boolean CheckSign(String publicKey, String message, String signature) throws WalletException {
        return CheckSign(mInstance, publicKey, message, signature);
    }

    public String GetOwnerPublicKeyRing() throws WalletException {
        return GetOwnerPublicKeyRing(mInstance);
    }


    public SubWallet(long instance) {
        mInstance = instance;
    }

    protected long GetProxy() {
        return mInstance;
    }

    private native String GetChainID(long subProxy);

    private native String GetBasicInfo(long subProxy);

    private native String GetAddresses(long subProxy, int start, int count, boolean internal);

    private native String GetPublicKeys(long subProxy, int start, int count, boolean internal);

    private native String SignTransaction(long subProxy, String rawTransaction, String payPassword);

    private native String Sign(long subProxy, String message, String payPassword);

    private native boolean CheckSign(long subProxy, String publicKey, String message, String signature);

    private native String GetOwnerPublicKeyRing(long subProxy);
}
