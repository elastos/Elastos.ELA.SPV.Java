
package org.elastos.spvcore;


import org.json.JSONObject;

public interface ISubWalletListener {
    public void sendResultSuccess(JSONObject jsonObject);
    public void sendResultError(String error);
}
