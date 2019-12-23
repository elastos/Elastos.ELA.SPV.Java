
package org.elastos.spvcore;

public interface ISubWalletCallback {
    /**
     * Callback method fired when status of a transaction changed.
     * @param txid indicate hash of the transaction.
     * @param status can be "Added", "Deleted" or "Updated".
     * @param desc is an detail description of transaction status.
     * @param confirms is confirm count util this callback fired.
     */
    public void OnTransactionStatusChanged(String txId, String status, String desc,int confirms);

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
    public void OnBlockSyncProgress(String progressInfo);

	public void OnBalanceChanged(String asset, String balance);

	/**
	 * @param result is json result
	 */
    public void OnTxPublished(String hash, String result);

    public void OnAssetRegistered(String asset, String info);

    public void OnConnectStatusChanged(String status);
}
