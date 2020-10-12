package org.elastos.spvcore;

import android.util.Log;

import com.fasterxml.jackson.core.JsonEncoding;
import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

public class HttRequestETHSC {
    private String ethscRPC;
    private String ethscApiMisc;
    private String getTransactionsUrlPrefix;
    private String getTokensUrlPrefix;
    private String TAG = "HttRequestETHSC";

    HttRequestETHSC(String ethscRPC, String ethscApiMisc) {
        this.ethscRPC = ethscRPC;
        this.ethscApiMisc = ethscApiMisc;
        this.getTransactionsUrlPrefix = this.ethscApiMisc + "/api/1/eth/history?address=";
        this.getTokensUrlPrefix = this.ethscApiMisc + "/api/1/eth/erc20/list";
    }

    public String GetPrice(int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
//            Log.d(TAG, "GetPrice");
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_gasPrice");
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "GetPrice result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String EstimateGas(String from, String to, String amount, String gasPrice, String data, int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
//            Log.d(TAG, "EstimateGas");
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_estimateGas");
            generator.writeFieldName("params");
            generator.writeStartArray();
            generator.writeStartObject();
            generator.writeStringField("from", from);
            generator.writeStringField("to", to);
            generator.writeStringField("amount", amount);
            generator.writeStringField("gasPrice", gasPrice);
            generator.writeStringField("data", data);
            generator.writeEndObject();
            generator.writeEndArray();
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "EstimateGas result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetBalance(String address, int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
//            Log.d(TAG, "GetBalance address:" + address);
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_getBalance");
            generator.writeFieldName("params");
            generator.writeStartArray();
            generator.writeString(address);
            generator.writeString( "latest");
            generator.writeEndArray();
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "GetBalance address:" + address + " result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String SubmitTransaction(String tx, int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
            Log.d(TAG, "SubmitTransaction tx:" + tx);
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_sendRawTransaction");
            generator.writeFieldName("params");
            generator.writeStartArray();
            generator.writeString(tx);
            generator.writeEndArray();
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "SubmitTransaction result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetTransactions(String address, long begBlockNumber, long endBlockNumber, int id) {
        String rpcUrl = this.getTransactionsUrlPrefix + address + "&begBlockNumber=" + begBlockNumber
                + "&endBlockNumber=" + endBlockNumber + "&sort=desc";
        String result = null;
        Log.d(TAG, "GetTransactions address:" + address);
        try {
            URL url = new URL(rpcUrl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setConnectTimeout(10000);
            conn.setRequestMethod("GET");
            String originResult = getResponce(conn);
            if (originResult != null) {
                JSONObject resultObj = new JSONObject(originResult);
                resultObj.put("id", id);

                JSONArray nameList = resultObj.getJSONArray("result");
                int length = nameList.length();
                for(int i = 0; i < length; i++) {
                    JSONObject jsonObject = nameList.getJSONObject(i);
                    transformJSONObjKey(jsonObject, "contractAddress", "contract");
                    transformJSONObjKey(jsonObject, "value", "amount");
                    transformJSONObjKey(jsonObject, "input", "data");
                    transformJSONObjKey(jsonObject, "confirmations", "blockConfirmations");
                    transformJSONObjKey(jsonObject, "transactionIndex", "blockTransactionIndex");
                    transformJSONObjKey(jsonObject, "timeStamp", "blockTimestamp");
                    jsonObject.put("gasLimit", "5012644");
                }

                result = resultObj.toString();
            }
        } catch (IOException | JSONException e) {
            e.printStackTrace();
        }

        return result;
    }

    public String GetLogs(String contract, String address1, String event, long begBlockNumber, long endBlockNumber, int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
            //TODO: temp, spvsdk will fix it
            String address = address1.replaceAll("000000000000000000000000", "");
            Log.d(TAG, "GetLogs address:" + address);
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_getLogs");
            generator.writeFieldName("params");
            generator.writeStartArray();
            generator.writeStartObject();
            generator.writeStringField("address", address);
            generator.writeStringField( "fromBlock", "0x" + Long.toString(begBlockNumber, 16));
            generator.writeStringField( "toBlock", "0x" + Long.toString(endBlockNumber, 16));
            generator.writeEndObject();
            generator.writeEndArray();
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetBlockNumber(int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
//             Log.d(TAG, "GetBlockNumber");
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_blockNumber");
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "GetBlockNumber result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetNonce(String address, int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
//             Log.d(TAG, "GetNonce address:" + address);
            connection = getConnection();
            OutputStream os = connection.getOutputStream();
            JsonFactory factory = new JsonFactory();
            JsonGenerator generator = factory.createGenerator(os, JsonEncoding.UTF8);
            generator.writeStartObject();
            generator.writeNumberField("id", id);
            generator.writeStringField("method", "eth_getTransactionCount");
            generator.writeFieldName("params");
            generator.writeStartArray();
            generator.writeString(address);
            generator.writeString( "latest");
            generator.writeEndArray();
            generator.writeEndObject();
            generator.close();
            os.close();

            result = getResponce(connection);
            Log.d(TAG, "GetNonce address:" + address + " result:" + result);
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetTokens(int id) {
        String result = null;
        // Log.d(TAG, "GetTokens");
        try {
            URL url = new URL(this.getTokensUrlPrefix);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setConnectTimeout(10000);
            conn.setRequestMethod("GET");
            String originResult = getResponce(conn);
            if (originResult != null) {
                JSONObject resultObj = new JSONObject(originResult);
                resultObj.put("id", id);
                result = resultObj.toString();
            }
        } catch (IOException | JSONException e) {
            e.printStackTrace();
        }

        return result;
        // JSONObject tokenObj = new JSONObject();
        // // TestNet
        // try {
        //     tokenObj.put("address", "0xfdce7fb4050cd43c654c6cecead950343990ce75");
        //     tokenObj.put("symbol", "TTECH");
        //     tokenObj.put("name", "TTECH");
        //     tokenObj.put("description", "TTECH");
        //     tokenObj.put("decimals", 0);
        //     tokenObj.put("defaultGasLimit", "");
        //     tokenObj.put("defaultGasPrice", "");

        //     JSONArray tokenArray = new JSONArray();
        //     tokenArray.put(tokenObj);

        //     JSONObject resultObj = new JSONObject();
        //     resultObj.put("id", id);
        //     resultObj.put("result", tokenArray);
        //     Log.d(TAG, "GetTokens result:" + resultObj.toString());
        //     return resultObj.toString();
        // } catch (JSONException e) {
        //     e.printStackTrace();
        //     Log.d(TAG, "GetTokens exception:" + e.getMessage());
        // }

        // return "{}";
    }

    private HttpURLConnection getConnection() throws IOException {
        URL url = new URL(this.ethscRPC);
        HttpURLConnection connection = (HttpURLConnection)url.openConnection();
        connection.setRequestMethod("POST");
        connection.setRequestProperty("User-Agent",
                "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.95 Safari/537.11");
        connection.setRequestProperty("Content-Type", "application/json");
        connection.setRequestProperty("Accept", "application/json");
        connection.setDoOutput(true);
        connection.connect();

        return connection;
    }

    private String getResponce(HttpURLConnection connection) {
        String result = null;
        try {
            int code = connection.getResponseCode();
            if (code == 200) {
                InputStream is = connection.getInputStream();
                result = readStream(is);
                return result;
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.d(TAG, "httprequest error result:" + result);
        return result;
    }

    private String readStream(InputStream in) {
        try {
            ByteArrayOutputStream baoStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int len = -1;
            while ((len = in.read(buffer)) != -1) {
                baoStream.write(buffer, 0, len);
            }
            String content = baoStream.toString();
            in.close();
            baoStream.close();
            return content;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private void transformJSONObjKey(JSONObject jsonObj, String originKey, String newKey) {
        String value = null;
        try {
            value = jsonObj.getString(originKey);
            jsonObj.put(newKey, value);
            jsonObj.remove(originKey);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
