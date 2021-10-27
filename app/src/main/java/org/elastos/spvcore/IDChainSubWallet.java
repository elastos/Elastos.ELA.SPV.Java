// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

/**
 * IDChainSubWallet jni
 */
public class IDChainSubWallet extends SidechainSubWallet {
    private long mInstance;

    public String CreateIDTransaction(String inputs, String payloadJson, String memo, String fee) throws WalletException {
        return CreateIDTransaction(mInstance, inputs, payloadJson, memo, fee);
    }

    public String GetDID(int start, int count, boolean internal) throws WalletException {
        return GetDID(mInstance, start, count, internal);
    }

    public String GetCID(int start, int count, boolean internal) throws WalletException {
        return GetCID(mInstance, start, count, internal);
    }

    public String Sign(String did, String message, String payPassword) throws WalletException {
        return Sign(mInstance, did, message, payPassword);
    }

    public boolean VerifySignature(String publicKey, String message, String signature) throws WalletException {
        return VerifySignature(mInstance, publicKey, message, signature);
    }

    public String GetPublicKeyDID(String publicKey) throws WalletException {
        return GetPublicKeyDID(mInstance, publicKey);
    }

    public String GetPublicKeyCID(String publicKey) throws WalletException {
        return GetPublicKeyCID(mInstance, publicKey);
    }

    public String SignDigest(String did, String digest, String payPassword) throws WalletException {
        return SignDigest(mInstance, did, digest, payPassword);
    }

    public IDChainSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    private native String CreateIDTransaction(long instance, String inputs, String payloadJSON, String memo, String fee);

    private native String GetDID(long instance, int start, int count, boolean internal);

    private native String GetCID(long instance, int start, int count, boolean internal);

    private native String Sign(long instance, String did, String message, String payPassword);

    private native boolean VerifySignature(long instance, String publicKey, String message, String signature);

    private native String GetPublicKeyDID(long instance, String publicKey);

    private native String GetPublicKeyCID(long instance, String publicKey);

    private native String SignDigest(long instance, String did, String digest, String payPassword);
}
