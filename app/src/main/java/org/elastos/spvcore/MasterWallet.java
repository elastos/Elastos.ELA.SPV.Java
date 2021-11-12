// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

import android.util.Log;

import java.util.ArrayList;

/**
 * MasterWallet
 */
public class MasterWallet {
    static public class CHAINID {
        public static final String MAIN     = "ELA";
        public static final String ID       = "IDChain";
        public static final String BTC      = "BTC";
    }

    static public String TAG = "IMasterWallet";

    private long mInstance;
    private ArrayList<SubWallet> mSubWallets = new ArrayList<SubWallet>();

    public MasterWallet(long instance) throws WalletException {
        mInstance = instance;
        Object[] allSubWallets = GetAllSubWallets(mInstance);
        for (int i = 0; i < allSubWallets.length; i++) {
            mSubWallets.add((SubWallet) allSubWallets[i]);
        }
    }

    public String GetID() {
        return GetID(mInstance);
    }

    public String GetBasicInfo() throws WalletException {
        return GetBasicInfo(mInstance);
    }

    public ArrayList<SubWallet> GetAllSubWallets() throws WalletException {
        Object[] allSubWallets = GetAllSubWallets(mInstance);

        for (int i = 0; i < allSubWallets.length; i++) {
            SubWallet subWallet = (SubWallet) allSubWallets[i];
            boolean found = false;
            for (int j = 0; j < mSubWallets.size(); ++j) {
                if (mSubWallets.get(j).GetChainID().equals(subWallet.GetChainID())) {
                    found = true;
                    break;
                }
            }
            if (!found)
                mSubWallets.add(subWallet);
        }

        return mSubWallets;
    }

    public SubWallet GetSubWallet(String chainID) throws WalletException {
        for (int i = 0; i < mSubWallets.size(); i++) {
            if (chainID.equals(mSubWallets.get(i).GetChainID())) {
                return mSubWallets.get(i);
            }
        }

        return null;
    }

    public SubWallet CreateSubWallet(String chainID) throws WalletException {
        switch (chainID) {
        case CHAINID.MAIN:
        case CHAINID.ID:
        case CHAINID.BTC:
            break;
        default:
            if (!chainID.startsWith("ETH")) {
              throw new WalletException("Not support " + chainID + " now.");
            }
            break;
        }

        for (int i = 0; i < mSubWallets.size(); ++i) {
            if (mSubWallets.get(i).GetChainID().equals(chainID)) {
                return mSubWallets.get(i);
            }
        }

        long subProxy = CreateSubWallet(mInstance, chainID);
        if (subProxy == 0) {
            Log.e(TAG, "Native create subwallet fail: subProxy is 0");
            throw new WalletException("Native create subwallet fail");
        }

        SubWallet subWallet = null;
        switch (chainID){
            case CHAINID.MAIN:
                subWallet = new MainchainSubWallet(subProxy);
                break;
            case CHAINID.ID:
                subWallet = new IDChainSubWallet(subProxy);
                break;
            case CHAINID.BTC:
                subWallet = new BTCSubWallet(subProxy);
                break;
            default:
                if (chainID.startsWith("ETH")) {
                    subWallet = new EthSidechainSubWallet(subProxy);
                } else {
                    throw new WalletException("Unsupport chainID: " + chainID);
                }
                break;
        }

        mSubWallets.add(subWallet);

        return subWallet;
    }

    public void DestroyWallet(SubWallet wallet) throws WalletException {
        for (int i = 0; i < mSubWallets.size(); ++i) {
            if (mSubWallets.get(i).GetChainID().equals(wallet.GetChainID())) {
                mSubWallets.remove(i);
                break;
            }
        }
        DestroyWallet(mInstance, wallet.GetChainID());
    }

    public String GetOwnerPublicKeyRing() throws WalletException {
        return GetOwnerPublicKeyRing(mInstance);
    }

    public String GetPublicKeyRing() throws WalletException {
        return GetPublicKeyRing(mInstance);
    }

    public boolean IsAddressValid(String address) throws WalletException {
        return IsAddressValid(mInstance, address);
    }

    public boolean IsSubWalletAddressValid(String chainID, String address) throws WalletException {
        return IsSubWalletAddressValid(mInstance, chainID, address);
    }

    public String[] GetSupportedChains() throws WalletException {
        return GetSupportedChains(mInstance);
    }

    public long GetInstance() {
        return mInstance;
    }

    public void ChangePassword(String oldPassword, String newPassword) throws WalletException {
        ChangePassword(mInstance, oldPassword, newPassword);
    }

    public void ResetPassword(String mnemonic, String passPhrase, String newPassword) throws WalletException {
        ResetPassword(mInstance, mnemonic, passPhrase, newPassword);
    }

    public String GetPubKeyInfo() throws WalletException {
        return GetPubKeyInfo(mInstance);
    }

    public boolean VerifyPrivateKey(String mnemonic, String passphrase) throws WalletException {
        return VerifyPrivateKey(mInstance, mnemonic, passphrase);
    }

    public boolean VerifyPassPhrase(String passPhrase, String payPasswd) throws WalletException {
        return VerifyPassPhrase(mInstance, passPhrase, payPasswd);
    }

    public boolean VerifyPayPassword(String payPasswd) throws WalletException {
        return VerifyPayPassword(mInstance, payPasswd);
    }

    public String ExportKeystore(String backPasswd, String payPasswd) throws WalletException {
        return ExportKeystore(mInstance, backPasswd, payPasswd);
    }

    public String ExportMnemonic(String payPasswd) throws WalletException {
        return ExportMnemonic(mInstance, payPasswd);
    }

    public String ExportPrivateKey(String payPasswd) throws WalletException {
        return ExportPrivateKey(mInstance, payPasswd);
    }

    public String ExportMasterPublicKey() throws WalletException {
        return ExportMasterPublicKey(mInstance);
    }

    private native String GetID(long instance);

    private native String GetBasicInfo(long instance);

    private native Object[] GetAllSubWallets(long instance);

    private native long GetSubWallet(long instance, String chainID);

    private native long CreateSubWallet(long instance, String chainID);

    private native String GetOwnerPublicKeyRing(long instance);

    private native String GetPublicKeyRing(long instance);

    private native void DestroyWallet(long instance, String chainID);

    private native boolean IsAddressValid(long instance, String address);

    private native boolean IsSubWalletAddressValid(long instance, String chainID, String address);

    private native String[] GetSupportedChains(long instance);

    private native void ChangePassword(long instance, String oldPassword, String newPassword);

    private native void ResetPassword(long instance, String mnemonic, String passphrase, String newPassword);

    private native String GetPubKeyInfo(long instance);

    private native boolean VerifyPrivateKey(long instance, String mnemonic, String passphrase);

    private native boolean VerifyPassPhrase(long instance, String passPhrase, String payPasswd);

    private native boolean VerifyPayPassword(long instance, String payPasswd);

    private native String ExportKeystore(long instance, String backPasswd, String payPasswd);

    private native String ExportMnemonic(long instance, String payPasswd);

    private native String ExportPrivateKey(long instance, String payPasswd);

    private native String ExportMasterPublicKey(long instance);
}
