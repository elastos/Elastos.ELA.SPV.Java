// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

/**
 * ISidechainSubWallet jni
 */
public class SidechainSubWallet extends ElastosBaseSubWallet {
    private long mInstance;

    public String CreateWithdrawTransaction(String inputs, String amount, String mainChainAddress, String fee, String memo) throws WalletException {
        return CreateWithdrawTransaction(mInstance, inputs, amount, mainChainAddress, fee, memo);
    }

    public SidechainSubWallet(long proxy) {
        super(proxy);
        mInstance = proxy;
    }

    private native String CreateWithdrawTransaction(long proxy, String inputs, String amount, String mainChainAddress, String fee, String memo);
}
