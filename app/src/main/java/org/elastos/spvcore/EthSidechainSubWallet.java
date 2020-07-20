// Copyright (c) 2012-2019 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

/**
 * EthSidechainSubWallet jni
 */
public class EthSidechainSubWallet extends SidechainSubWallet {
    private long mInstance;

    public EthSidechainSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }
}
