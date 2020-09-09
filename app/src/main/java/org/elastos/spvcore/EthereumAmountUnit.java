package org.elastos.spvcore;


public enum EthereumAmountUnit {
    TOKEN_DECIMAL(0),
    TOKEN_INTEGER(1),

    ETHER_WEI(0),
    ETHER_GWEI(3),
    ETHER_ETHER(6);

    private int  mState=0;
    EthereumAmountUnit(int value) {
        mState=value;
    }
    public int getState() {
        return mState;
    }
}