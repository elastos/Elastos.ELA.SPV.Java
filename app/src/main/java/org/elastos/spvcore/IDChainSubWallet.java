package org.elastos.spvcore;

/**
 * IIDChainSubWallet jni
 */
public class IIDChainSubWallet extends ISidechainSubWallet{
    private long mIDchainProxy;


    public String CreateIDTransaction(String fromAddress, String payloadJson, String programJson, String memo) throws WalletException {
        return nativeCreateIdTransaction(mIDchainProxy, fromAddress, payloadJson, programJson, memo);
    }

    public IIDChainSubWallet(long proxy) {
        super(proxy);
        mIDchainProxy = proxy;
    }

    private native String nativeCreateIdTransaction(long proxy, String fromAddress,
            String payloadJson, String programJson, String memo);
}
