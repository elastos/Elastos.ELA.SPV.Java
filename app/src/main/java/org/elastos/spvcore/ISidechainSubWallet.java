package org.elastos.spvcore;

/**
 * ISidechainSubWallet jni
 */
public class ISidechainSubWallet extends ISubWallet {
    private long mSidechainProxy;

    public String CreateWithdrawTransaction(String fromAddress, String amount, String mainChainAddress, String memo) throws WalletException {
        return nativeCreateWithdrawTransaction(mSidechainProxy, fromAddress, amount, mainChainAddress, memo);
    }

    public String GetGenesisAddress() throws WalletException {
        return nativeGetGenesisAddress(mSidechainProxy);
    }

    public ISidechainSubWallet(long proxy) {
        super(proxy);
        mSidechainProxy = proxy;
    }

    private native String nativeCreateWithdrawTransaction(long proxy, String fromAddress, String amount, String mainChainAddress, String memo);

    private native String nativeGetGenesisAddress(long proxy);
}
