// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ELA_WALLET_ANDROID_SUBWALLETCALLBACK_H
#define ELA_WALLET_ANDROID_SUBWALLETCALLBACK_H

#include "ISubWalletCallback.h"
#include "Utils.h"
#include "ISubWallet.h"
#include "nlohmann/json.hpp"

namespace Elastos {
    namespace ElaWallet {

        class SubWalletCallback : public ISubWalletCallback {
        public:
            SubWalletCallback(JNIEnv *env, jobject jobj);

        public:
            virtual void OnTransactionStatusChanged(
                    const std::string &txid,
                    const std::string &status,
                    const nlohmann::json &desc,
                    uint32_t confirms);

            /**
             * Callback method fired when best block chain height increased. This callback could be used to show progress.
             * @param progressInfo progress info contain detail as below:
             * {
             *     "Progress": 50,                    # 0% ~ 100%
             *     "BytesPerSecond": 12345678,        # 12.345678 MBytes / s
             *     "LastBlockTime": 1573799697,       # timestamp of last block
             *     "DownloadPeer": "127.0.0.1"        # IP address of node
             * }
             */
            virtual void OnBlockSyncProgress(const nlohmann::json &progressInfo);

            virtual void OnBalanceChanged(const std::string &asset, const std::string &balance);

            virtual void OnTxPublished(const std::string &hash, const nlohmann::json &result);

            virtual void OnAssetRegistered(const std::string &asset, const nlohmann::json &info);

            virtual void OnConnectStatusChanged(const std::string &status);

            virtual void OnETHSCEventHandled(const nlohmann::json &event);

            // ETH sidechain callback: send result to spvsdk
            virtual nlohmann::json GasPrice(int id);

            virtual nlohmann::json EstimateGas(const std::string &from,
											   const std::string &to,
											   const std::string &amount,
											   const std::string &gasPrice,
											   const std::string &data,
											   int id);

            virtual nlohmann::json GetBalance(const std::string &address, int id);

            virtual nlohmann::json SubmitTransaction(const std::string &tx, int id);

            virtual nlohmann::json GetTransactions(const std::string &address,
												   uint64_t begBlockNumber,
												   uint64_t endBlockNumber,
												   int id);

            virtual nlohmann::json GetLogs(const std::string &contract,
										   const std::string &address,
										   const std::string &event,
										   uint64_t begBlockNumber,
										   uint64_t endBlockNumber,
										   int id);

            virtual nlohmann::json GetTokens(int id);

            virtual nlohmann::json GetBlockNumber(int id);

            virtual nlohmann::json GetNonce(const std::string &address, int id);

            virtual ~SubWalletCallback();

        private:
            JNIEnv *GetEnv();

        private:
            JavaVM *_jvm;
            jobject _obj;
        };

    }
}

#endif //ELA_WALLET_ANDROID_SUBWALLETCALLBACK_H
