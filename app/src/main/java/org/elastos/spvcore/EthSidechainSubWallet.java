// Copyright (c) 2012-2019 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;


/**
 * EthSidechainSubWallet jni
 */
public class EthSidechainSubWallet extends SubWallet {
    private long mInstance;

    public String CreateTransfer(String targetAddress, String amount, int amountUnit) throws WalletException {
        return CreateTransfer(mInstance, targetAddress, amount, amountUnit);
    }

    public String CreateTransferGeneric(String targetAddress, String amount, int amountUnit,
            String gasPrice, int gasPriceUnit, String gasLimit, String data) throws WalletException {
        return CreateTransferGeneric(mInstance, targetAddress, amount, amountUnit, gasPrice, gasPriceUnit, gasLimit, data);
    }

    public void DeleteTransfer(String tx) throws WalletException {
        DeleteTransfer(mInstance, tx);
    }

    public EthSidechainSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    private native String CreateTransfer(long instance, String targetAddress, String amount, int amountUnit);

    private native String CreateTransferGeneric(long instance, String targetAddress, String amount, int amountUnit,
                                                String gasPrice, int gasPriceUnit, String gasLimit, String data);

    private native void DeleteTransfer(long instance, String tx);
}
