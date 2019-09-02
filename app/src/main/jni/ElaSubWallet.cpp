// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include "ElaUtils.h"
#include "ISubWallet.h"
#include "nlohmann/json.hpp"

using namespace Elastos::ElaWallet;

#define  CLASS_SUBWALLET   "org/elastos/spvcore/ISubWallet"
#define  FIELD_SUBWALLET   "mSubProxy"

#define SIG_nativeGetChainID "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetChainID(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	jstring chainId = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		std::string result = subWallet->GetChainID();
		chainId = env->NewStringUTF(result.c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return chainId;
}

#define SIG_nativeGetBasicInfo "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetBasicInfo(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	jstring info = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json result = subWallet->GetBasicInfo();
		info = env->NewStringUTF(result.dump().c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return info;
}

#define SIG_nativeGetBalanceInfo "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetBalanceInfo(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	jstring info = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json result = subWallet->GetBalanceInfo();
		info = env->NewStringUTF(result.dump().c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return info;
}

#define SIG_nativeGetBalance "(JI)Ljava/lang/String;"
static jstring JNICALL nativeGetBalance(JNIEnv *env, jobject clazz, jlong jSubProxy, jint balanceType)
{
	jstring balance = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
        std::string result = subWallet->GetBalance(BalanceType(balanceType));
        balance = env->NewStringUTF(result.c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return balance;
}

#define SIG_nativeCreateAddress "(J)Ljava/lang/String;"
static jstring JNICALL nativeCreateAddress(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	jstring addr = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		std::string result = subWallet->CreateAddress();
		addr = env->NewStringUTF(result.c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return addr;
}

#define SIG_nativeGetAllAddress "(JII)Ljava/lang/String;"
static jstring JNICALL nativeGetAllAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jint jStart,
		jint jCount)
{
	jstring addresses = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json addressesJson = subWallet->GetAllAddress(jStart, jCount);
		addresses = env->NewStringUTF(addressesJson.dump().c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return addresses;
}

#define SIG_nativeGetBalanceWithAddress "(JLjava/lang/String;I)Ljava/lang/String;"
static jstring JNICALL nativeGetBalanceWithAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jaddress,
		jint balanceType)
{
	bool exception = false;
	std::string msgException;

	const char* address = env->GetStringUTFChars(jaddress, NULL);
	jstring balance = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
        std::string result = subWallet->GetBalanceWithAddress(address, BalanceType(balanceType));
        balance = env->NewStringUTF(result.c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jaddress, address);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return (jstring)balance;
}

class ElaSubWalletCallback: public ISubWalletCallback
{
	public:
		virtual void OnTransactionStatusChanged(
				const std::string &txid,
				const std::string &status,
				const nlohmann::json &desc,
				uint32_t confirms);

		virtual void OnBlockSyncStarted();

		/**
		 * Callback method fired when best block chain height increased. This callback could be used to show progress.
		 * @param currentBlockHeight is the of current block when callback fired.
		 * @param progress is current progress when block height increased.
		 */
		virtual void OnBlockSyncProgress(uint32_t currentBlockHeight, uint32_t estimatedHeight, time_t lastBlockTime);

		/**
		 * Callback method fired when block end synchronizing with a peer. This callback could be used to show progress.
		 */
		virtual void OnBlockSyncStopped();

		virtual void OnBalanceChanged(const std::string &asset, const std::string &balance);

		virtual void OnTxPublished(const std::string &hash, const nlohmann::json &result);

		virtual void OnAssetRegistered(const std::string &asset, const nlohmann::json &info);

		virtual void OnConnectStatusChanged(const std::string &status);

		ElaSubWalletCallback(
				/* [in] */ JNIEnv* env,
				/* [in] */ jobject jobj);

		~ElaSubWalletCallback();

	private:
		JNIEnv* GetEnv();
		void Detach();

	private:
		JavaVM* mVM;
		jobject mObj;
};


static std::map<jlong, ElaSubWalletCallback*> sSubCallbackMap;
#define SIG_nativeAddCallback "(JLorg/elastos/spvcore/ISubWalletCallback;)V"
static void JNICALL nativeAddCallback(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jobject jsubCallback)
{
	try {
		if (sSubCallbackMap.find(jSubProxy) == sSubCallbackMap.end()) {
			ElaSubWalletCallback *subCallback = new ElaSubWalletCallback(env, jsubCallback);
			ISubWallet *subWallet = (ISubWallet *)jSubProxy;
			subWallet->AddCallback(subCallback);
			sSubCallbackMap[jSubProxy] = subCallback;
			LOGI("Subwallet add callback %ld", jSubProxy);
		} else {
			LOGE("Sub wallet callback %ld already exist", jSubProxy);
		}
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}
}

#define SIG_nativeRemoveCallback "(J)V"
static void JNICALL nativeRemoveCallback(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		if (sSubCallbackMap.find(jSubProxy) != sSubCallbackMap.end()) {
			LOGI("Subwallet remove callback %ld", jSubProxy);
			ElaSubWalletCallback *callback = sSubCallbackMap[jSubProxy];
			//subWallet->RemoveCallback(callback);
			delete callback;
			sSubCallbackMap.erase(jSubProxy);
		} else {
			LOGW("Sub wallet callback already removed");
		}
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}
}

#define SIG_nativeCreateTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jfromAddress,
		jstring jtoAddress,
		jstring jamount,
		jstring jmemo,
		jboolean useVotedUTXO)
{
	bool exception = false;
	std::string msgException;

	const char* fromAddress = env->GetStringUTFChars(jfromAddress, NULL);
	const char* toAddress = env->GetStringUTFChars(jtoAddress, NULL);
	const char *amount = env->GetStringUTFChars(jamount, NULL);
	const char* memo = env->GetStringUTFChars(jmemo, NULL);

	ISubWallet* subWallet = (ISubWallet*)jSubProxy;

	jstring tx = NULL;
	try {
		nlohmann::json result = subWallet->CreateTransaction(fromAddress, toAddress, amount, memo, useVotedUTXO);
		tx = env->NewStringUTF(result.dump().c_str());
	} catch (std::exception& e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jfromAddress, fromAddress);
	env->ReleaseStringUTFChars(jtoAddress, toAddress);
	env->ReleaseStringUTFChars(jamount, amount);
	env->ReleaseStringUTFChars(jmemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}


#define SIG_nativeSignTransaction "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeSignTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jRawTransaction,
		jstring jPayPassword)
{
	bool exception = false;
	std::string msgException;

	const char* rawTransaction = env->GetStringUTFChars(jRawTransaction, NULL);
	const char* payPassword = env->GetStringUTFChars(jPayPassword, NULL);

	ISubWallet* subWallet = (ISubWallet*)jSubProxy;
	jstring tx = NULL;

	try {
		nlohmann::json result = subWallet->SignTransaction(nlohmann::json::parse(rawTransaction), payPassword);
		tx = env->NewStringUTF(result.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jRawTransaction, rawTransaction);
	env->ReleaseStringUTFChars(jPayPassword, payPassword);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeGetTransactionSignedInfo "(JLjava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeGetTransactionSignedInfo(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jTransactionJson)
{
	bool exception = false;
	std::string msgException;

	const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);
	jstring result = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json signers = subWallet->GetTransactionSignedInfo(nlohmann::json::parse(transactionJson));
		result = env->NewStringUTF(signers.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return result;
}

#define SIG_nativePublishTransaction "(JLjava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativePublishTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jTransactionJson)
{
	bool exception = false;
	std::string msgException;

	const char* transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);

	jstring result = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json r = subWallet->PublishTransaction(nlohmann::json::parse(transactionJson));
		result = env->NewStringUTF(r.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return result;
}

#define SIG_nativeGetAllTransaction "(JIILjava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeGetAllTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jint start,
		jint count,
		jstring jaddressOrTxid)
{
	bool exception = false;
	std::string msgException;

	const char* addressOrTxid = env->GetStringUTFChars(jaddressOrTxid, NULL);
	jstring tx = NULL;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		nlohmann::json result = subWallet->GetAllTransaction(start, count, addressOrTxid);
		tx = env->NewStringUTF(result.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jaddressOrTxid, addressOrTxid);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}


#define SIG_nativeSign "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeSign(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jmessage,
		jstring jpayPassword)
{
	bool exception = false;
	std::string msgException;

	const char* message = env->GetStringUTFChars(jmessage, NULL);
	const char* payPassword = env->GetStringUTFChars(jpayPassword, NULL);

	ISubWallet* subWallet = (ISubWallet*)jSubProxy;
	jstring result = NULL;

	try {
		std::string r = subWallet->Sign(message, payPassword);
		result = env->NewStringUTF(r.c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jmessage, message);
	env->ReleaseStringUTFChars(jpayPassword, payPassword);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return result;
}

#define SIG_nativeCheckSign "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z"
static jboolean JNICALL nativeCheckSign(JNIEnv *env, jobject clazz, jlong jSubProxy,
		jstring jPublicKey,
		jstring jMessage,
		jstring jSignature)
{
	bool exception = false;
	std::string msgException;

	const char* publicKey = env->GetStringUTFChars(jPublicKey, NULL);
	const char* message = env->GetStringUTFChars(jMessage, NULL);
	const char* signature = env->GetStringUTFChars(jSignature, NULL);

	jboolean result = false;

	try {
		ISubWallet* subWallet = (ISubWallet*)jSubProxy;
		result = subWallet->CheckSign(publicKey, message, signature);
	} catch (std::exception& e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jPublicKey, publicKey);
	env->ReleaseStringUTFChars(jMessage, message);
	env->ReleaseStringUTFChars(jSignature, signature);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return result;
}

#define SIG_nativeGetPublicKey "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetPublicKey(JNIEnv *env, jobject clazz, jlong jSubProxy)
{
	bool exception = false;
	std::string msgException;

	jstring key = NULL;

	try {
		ISubWallet *subWallet = (ISubWallet *)jSubProxy;
		std::string result = subWallet->GetPublicKey();
		key = env->NewStringUTF(result.c_str());
	} catch (std::exception &e) {
		ThrowWalletException(env, e.what());
	}

	return key;
}

static const JNINativeMethod gMethods[] = {
	REGISTER_METHOD(nativeGetChainID),
	REGISTER_METHOD(nativeGetBasicInfo),
	REGISTER_METHOD(nativeGetBalanceInfo),
	REGISTER_METHOD(nativeGetBalance),
	REGISTER_METHOD(nativeCreateAddress),
	REGISTER_METHOD(nativeGetAllAddress),
	REGISTER_METHOD(nativeGetBalanceWithAddress),
	REGISTER_METHOD(nativeAddCallback),
	REGISTER_METHOD(nativeRemoveCallback),
	REGISTER_METHOD(nativeCreateTransaction),
	REGISTER_METHOD(nativeSignTransaction),
	REGISTER_METHOD(nativeGetTransactionSignedInfo),
	REGISTER_METHOD(nativePublishTransaction),
	REGISTER_METHOD(nativeGetAllTransaction),
	REGISTER_METHOD(nativeSign),
	REGISTER_METHOD(nativeCheckSign),
	REGISTER_METHOD(nativeGetPublicKey),
};

jint register_elastos_spv_ISubWallet(JNIEnv *env)
{
	return jniRegisterNativeMethods(env, "org/elastos/spvcore/ISubWallet",
			gMethods, NELEM(gMethods));
}

ElaSubWalletCallback::ElaSubWalletCallback(
		/* [in] */ JNIEnv* env,
		/* [in] */ jobject jobj)
{
	mObj = env->NewGlobalRef(jobj);
	env->GetJavaVM(&mVM);
}

ElaSubWalletCallback::~ElaSubWalletCallback()
{
	if (mObj) {
		GetEnv()->DeleteGlobalRef(mObj);
	}
}

JNIEnv* ElaSubWalletCallback::GetEnv()
{
	JNIEnv* env;
	assert(mVM != NULL);
	mVM->AttachCurrentThread(&env, NULL);
	return env;
}

void ElaSubWalletCallback::Detach()
{
	assert(mVM != NULL);
	mVM->DetachCurrentThread();
}

void ElaSubWalletCallback::OnTransactionStatusChanged(const std::string &txid, const std::string &status,
		const nlohmann::json &desc, uint32_t confirms)
{
	JNIEnv* env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnTransactionStatusChanged","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
	jstring jtxid = env->NewStringUTF(txid.c_str());
	jstring jstatus = env->NewStringUTF(status.c_str());
	jstring jdesc = env->NewStringUTF(desc.dump().c_str());

	env->CallVoidMethod(mObj, methodId, jtxid, jstatus, jdesc, confirms);

	Detach();
}

void ElaSubWalletCallback::OnBlockSyncStarted()
{
	JNIEnv* env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStarted","()V");
	env->CallVoidMethod(mObj, methodId);

	Detach();
}

void ElaSubWalletCallback::OnBlockSyncProgress(uint32_t currentBlockHeight, uint32_t estimatedHeight, time_t lastBlockTime)
{
	JNIEnv* env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncProgress", "(IIJ)V");
	env->CallVoidMethod(mObj, methodId, currentBlockHeight, estimatedHeight, lastBlockTime);

	Detach();
}

void ElaSubWalletCallback::OnBlockSyncStopped()
{
	JNIEnv* env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStopped", "()V");
	env->CallVoidMethod(mObj, methodId);

	Detach();
}

void ElaSubWalletCallback::OnBalanceChanged(const std::string &asset, const std::string &balance)
{
	JNIEnv *env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnBalanceChanged", "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring jasset = env->NewStringUTF(asset.c_str());
	jstring jbalance = env->NewStringUTF(balance.c_str());
	env->CallVoidMethod(mObj, methodId, jasset, jbalance);

	Detach();
}

void ElaSubWalletCallback::OnTxPublished(const std::string &hash, const nlohmann::json &result)
{
	JNIEnv *env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnTxPublished", "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring jhash = env->NewStringUTF(hash.c_str());
	jstring jresult = env->NewStringUTF(result.dump().c_str());
	env->CallVoidMethod(mObj, methodId, jhash, jresult);

	Detach();
}

void ElaSubWalletCallback::OnAssetRegistered(const std::string &asset, const nlohmann::json &info)
{
	JNIEnv *env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnAssetRegistered", "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring jasset = env->NewStringUTF(asset.c_str());
	jstring jinfo = env->NewStringUTF(info.dump().c_str());
	env->CallVoidMethod(mObj, methodId, jasset, jinfo);

	Detach();
}

void ElaSubWalletCallback::OnConnectStatusChanged(const std::string &status)
{
	JNIEnv *env = GetEnv();

	jclass clazz = env->GetObjectClass(mObj);
	jmethodID methodId = env->GetMethodID(clazz, "OnConnectStatusChanged", "(Ljava/lang/String;)V");
	jstring jstatus = env->NewStringUTF(status.c_str());
	env->CallVoidMethod(mObj, methodId, jstatus);

	Detach();
}
