// Copyright (c) 2012-2019 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

import android.util.Log;
import org.json.JSONException;
import org.json.JSONObject;

public class SubWalletCallback {
    private long mInstance;
    private String mMasterWalletID;
    private String mSubWalletID;
    private ISubWalletListener mListener;
    private HttRequestETHSC jsonRPC;
    private String TAG = "SubWalletCallback";

    public long GetProxy() {
        return mInstance;
    }

    public SubWalletCallback(String masterWalletID, String subWalletID, String ethscRPC, String ethscApiMisc, ISubWalletListener listener) {
        mMasterWalletID = masterWalletID;
        mSubWalletID = subWalletID;
        mListener = listener;
        jsonRPC = new HttRequestETHSC(ethscRPC, ethscApiMisc);
        mInstance = InitSubWalletCallback();
    }

    public void setListener(ISubWalletListener listener) {
        mListener = listener;
    }

    public String GetWalletID() {
        return mMasterWalletID + ":" + mSubWalletID + " ";
    }

    /*
     * nNed to do GC of jni manually, because the finalization mechanism is Deprecated.
     */
    public void Dispose() {
        Log.w(TAG, GetWalletID() + "Dispose");
        DisposeNative(mInstance);
    }

    /**
     * Callback method fired when status of a transaction changed.
     *
     * @param txID     indicate hash of the transaction.
     * @param status   can be "Added", "Deleted" or "Updated".
     * @param desc     is an detail description of transaction status.
     * @param confirms is confirm count util this callback fired.
     */
    public void OnTransactionStatusChanged(String txID, String status, String desc, int confirms) {
//        Log.i(TAG, GetWalletID() + "[OnTransactionStatusChanged] " + txID + "," + status + "," + confirms);
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("txId", txID);
            jsonObject.put("status", status);
            jsonObject.put("desc", desc);
            jsonObject.put("confirms", confirms);
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnTransactionStatusChanged");

            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    /**
     * Callback method fired when best block chain height increased. This callback could be used to show progress.
     * @param progressInfo progress info contain detail as below:
     * {
     *     "Progress": 50,                    # 0% ~ 100%
     *     "BytesPerSecond": 12345678,        # 12.345678 MByte / s
     *     "LastBlockTime": 1573799697,       # timestamp of last block
     *     "DownloadPeer": "127.0.0.1"        # IP address of node
     * }
     */
    public void OnBlockSyncProgress(String progressInfo) {
//        Log.i(TAG, GetWalletID() + "[OnBlockSyncProgress] " + progressInfo);
        try {
            JSONObject jProgressInfo = new JSONObject(progressInfo);
            jProgressInfo.put("MasterWalletID", mMasterWalletID);
            jProgressInfo.put("ChainID", mSubWalletID);
            jProgressInfo.put("Action", "OnBlockSyncProgress");

            mListener.sendResultSuccess(jProgressInfo);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    public void OnBalanceChanged(String assetID, String balance) {
//       Log.i(TAG, GetWalletID() + "[OnBalanceChanged] " + assetID + " = " + balance);
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("Asset", assetID);
            jsonObject.put("Balance", balance);
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnBalanceChanged");

            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    public void OnTxPublished(String hash, String result) {
//       Log.i(TAG, GetWalletID() + "[OnTxPublished] " + hash + ", result: " + result);
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("hash", hash);
            jsonObject.put("result", result);
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnTxPublished");

            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    public void OnAssetRegistered(String asset, String info) {
//        Log.i(TAG, GetWalletID() + "[OnAssetRegistered] " + asset + ", info: " + info);
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("asset", asset);
            jsonObject.put("info", info);
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnAssetRegistered");
            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    public void OnConnectStatusChanged(String status) {
      // Log.i(TAG, GetWalletID() + "[OnConnectStatusChanged] status=" + status);
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("status", status);
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnConnectStatusChanged");
            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    public void OnETHSCEventHandled(String event) {
        JSONObject jsonObject = new JSONObject();
        // Log.i(TAG, GetWalletID() + "[OnETHSCEventHandled] event=" + event);
        try {
            jsonObject.put("event", new JSONObject(event));
            jsonObject.put("MasterWalletID", mMasterWalletID);
            jsonObject.put("ChainID", mSubWalletID);
            jsonObject.put("Action", "OnETHSCEventHandled");
            mListener.sendResultSuccess(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
            mListener.sendResultError(e.toString());
        }
    }

    // ETH sidechain callback: send result to spvsdk, not user
    public String GasPrice(int id) {
        // Log.d(TAG, "GasPrice id:" + id);
        return this.jsonRPC.GetPrice(id);
    }

    public String EstimateGas(String from, String to, String amount,
            String gasPrice, String data, int id) {
        // Log.d(TAG, "EstimateGas");
        return this.jsonRPC.EstimateGas(from, to, amount, gasPrice, data, id);
    }

    public String GetBalance(String address, int id) {
        // Log.d(TAG, "GetBalance address:" + address + " id:" + id);
        return this.jsonRPC.GetBalance(address, id);
    }

    public String SubmitTransaction(String tx, int id) {
        // Log.d(TAG, "SubmitTransaction：" + tx);
        return this.jsonRPC.SubmitTransaction(tx, id);
    }

     public String GetTransactions(String address, long begBlockNumber, long endBlockNumber, int id) {
        // Log.d(TAG, "GetTransactions address:" + address + " [" + begBlockNumber + " , " + endBlockNumber + " ] id:" + id);
        return this.jsonRPC.GetTransactions(address, begBlockNumber, endBlockNumber, id);
    }

    public String GetLogs(String contract, String address, String event, long begBlockNumber, long endBlockNumber, int id) {
        // Log.d(TAG, "GetLogs id:" + id);
        return this.jsonRPC.GetLogs(contract, address, event, begBlockNumber, endBlockNumber, id);
    }

    public String GetTokens(int id) {
        // Log.d(TAG, "GetTokens");
        return this.jsonRPC.GetTokens(id);
    }

    public String GetBlockNumber(int id) {
        // Log.d(TAG, "GetBlockNumber id:" + id);
        return this.jsonRPC.GetBlockNumber(id);
    }

    public String GetNonce(String address, int id) {
        // Log.d(TAG, "GetNonce address:" + address + " id:" + id);
        return this.jsonRPC.GetNonce(address, id);
    }

    private native long InitSubWalletCallback();

    private native void DisposeNative(long proxy);
}
