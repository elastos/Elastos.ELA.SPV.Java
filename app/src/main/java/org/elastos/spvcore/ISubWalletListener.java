
package org.elastos.spvcore;


import org.json.JSONObject;

public interface ISubWalletListener {

    void OnTransactionStatusChanged(JSONObject jsonObject);

    void OnBlockSyncProgress(JSONObject jsonObject);


    void OnBalanceChanged(JSONObject jsonObject);


    void OnTxPublished(JSONObject jsonObject);

    void OnAssetRegistered(JSONObject jsonObject);

    void OnConnectStatusChanged(JSONObject jsonObject);
}
