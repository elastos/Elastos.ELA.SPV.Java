package org.elastos.spvcore;

import android.util.Log;

import com.fasterxml.jackson.core.JsonEncoding;
import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

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
    private String TAG = "HttRequestETHSC";

    HttRequestETHSC(String ethscRPC, String ethscApiMisc) {
        this.ethscRPC = ethscRPC;
        this.ethscApiMisc = ethscApiMisc;
        this.getTransactionsUrlPrefix = this.ethscApiMisc + "/api/1/eth/history?address=";
    }

    public String GetPrice(int id) {
        HttpURLConnection connection = null;
        String result = null;
        try {
            Log.d(TAG, "GetPrice");
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
            Log.d(TAG, "EstimateGas");
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
            Log.d(TAG, "GetBalance address:" + address);
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
        Log.d(TAG, "GetTransactions rpcUrl:" + rpcUrl);
        try {
            URL url = new URL(rpcUrl);
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
            Log.d(TAG, "GetBlockNumber");
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
            Log.d(TAG, "GetNonce address:" + address);
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
        } catch (MalformedURLException | ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (connection != null) connection.disconnect();
        return result;
    }

    public String GetTokens(int id) {
        return "{}";
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
                int totalBytes = connection.getContentLength();
                Log.d(TAG, "httprequest totalBytes:" + totalBytes);
                InputStream in = connection.getInputStream();
                ByteArrayOutputStream outStream = new ByteArrayOutputStream();
                byte[] buffer = new byte[totalBytes];
                int len = 0;
                while((len = in.read(buffer)) != -1) {
                    outStream.write(buffer,0, len);
                }
                in.close();
                byte[] data =  outStream.toByteArray();
                result = new String(data, "UTF-8");
                Log.d(TAG, "httprequest result:" + result);
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
}
