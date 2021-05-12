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

    public String CreateAddress() throws WalletException {
        return CreateAddress(mInstance);
    }

    public String GetAllAddress(int start, int count, boolean internal) throws WalletException {
        return GetAllAddress(mInstance, start, count, internal);
    }

    public String[] GetLastAddresses(boolean internal) throws WalletException {
        return GetLastAddresses(mInstance, internal);
    }

    public void UpdateUsedAddress(String[] usedAddresses) throws WalletException {
        UpdateUsedAddress(mInstance, usedAddresses);
    }

    public String GetAllPublicKeys(int start, int count) throws WalletException {
        return GetAllPublicKeys(mInstance, start, count);
    }

    public String CreateTransaction(String inputs, String outputs, String fee, String memo) throws WalletException {
        return CreateTransaction(mInstance, inputs, outputs, fee, memo);
    }

    public String SignTransaction(String rawTransaction, String payPassword) throws WalletException {
        return SignTransaction(mInstance, rawTransaction, payPassword);
    }

    public String GetTransactionSignedInfo(String tx) throws WalletException {
        return GetTransactionSignedInfo(mInstance, tx);
    }

    public String ConvertToRawTransaction(String rawTransaction) throws WalletException {
        return ConvertToRawTransaction(mInstance, rawTransaction);
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

    private native String CreateAddress(long subProxy);

    private native String GetAllAddress(long subProxy, int start, int count, boolean internal);

    private native String[] GetLastAddresses(long subProxy, boolean internal);

    private native void UpdateUsedAddress(long subProxy, String[] usedAddresses);

    private native String GetAllPublicKeys(long subProxy, int start, int count);

    private native String CreateTransaction(long subProxy, String inputs, String outputs, String fee, String memo);

    private native String SignTransaction(long subProxy, String rawTransaction, String payPassword);

    private native String GetTransactionSignedInfo(long subProxy, String rawTransaction);

    private native String ConvertToRawTransaction(long subProxy, String tx);

    private native String Sign(long subProxy, String message, String payPassword);

    private native boolean CheckSign(long subProxy, String publicKey, String message, String signature);

    private native String GetOwnerPublicKeyRing(long subProxy);
}
