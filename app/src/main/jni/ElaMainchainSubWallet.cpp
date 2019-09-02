// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ElaUtils.h"
#include "IMainchainSubWallet.h"
#include "nlohmann/json.hpp"

using namespace Elastos::ElaWallet;

#define SIG_nativeCreateDepositTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateDepositTransaction(JNIEnv *env, jobject clazz, jlong jMainSubWalletProxy,
		jstring jfromAddress,
		jstring jlockedAddress,
		jstring jamount,
		jstring jsideChainAddress,
		jstring jmemo,
		jboolean useVotedUTXO)
{
	bool exception = false;
	std::string msgException;

	const char *fromAddress = env->GetStringUTFChars(jfromAddress, NULL);
	const char *lockedAddress = env->GetStringUTFChars(jlockedAddress, NULL);
	const char *amount = env->GetStringUTFChars(jamount, NULL);
	const char *sideChainAddress = env->GetStringUTFChars(jsideChainAddress, NULL);
	const char *memo = env->GetStringUTFChars(jmemo, NULL);

	IMainchainSubWallet* wallet = (IMainchainSubWallet*)jMainSubWalletProxy;
	jstring tx = NULL;

	try {
		nlohmann::json txJson = wallet->CreateDepositTransaction(fromAddress, lockedAddress, amount, sideChainAddress, memo, useVotedUTXO);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jfromAddress, fromAddress);
	env->ReleaseStringUTFChars(jlockedAddress, lockedAddress);
	env->ReleaseStringUTFChars(jamount, amount);
	env->ReleaseStringUTFChars(jsideChainAddress, sideChainAddress);
	env->ReleaseStringUTFChars(jmemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeGenerateProducerPayload "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeGenerateProducerPayload(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jPublicKey,
		jstring jNodePublicKey,
		jstring jNickName,
		jstring jURL,
		jstring jIPAddress,
		jlong location,
		jstring jPayPasswd)
{
	bool exception = false;
	std::string msgException;
	jstring payload = NULL;

	const char *publicKey     = env->GetStringUTFChars(jPublicKey, NULL);
	const char *nodePublicKey = env->GetStringUTFChars(jNodePublicKey, NULL);
	const char *nickName      = env->GetStringUTFChars(jNickName, NULL);
	const char *url           = env->GetStringUTFChars(jURL, NULL);
	const char *ipAddress     = env->GetStringUTFChars(jIPAddress, NULL);
	const char *payPasswd     = env->GetStringUTFChars(jPayPasswd, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json payloadJson = wallet->GenerateProducerPayload(publicKey, nodePublicKey, nickName, url, ipAddress, location, payPasswd);
		payload = env->NewStringUTF(payloadJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jPublicKey, publicKey);
	env->ReleaseStringUTFChars(jNodePublicKey, nodePublicKey);
	env->ReleaseStringUTFChars(jNickName, nickName);
	env->ReleaseStringUTFChars(jURL, url);
	env->ReleaseStringUTFChars(jIPAddress, ipAddress);
	env->ReleaseStringUTFChars(jPayPasswd, payPasswd);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return payload;
}

#define SIG_nativeGenerateCancelProducerPayload "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeGenerateCancelProducerPayload(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jPublicKey,
		jstring jPayPasswd)
{
	bool exception = false;
	std::string msgException;
	jstring payload = NULL;

	const char *publicKey = env->GetStringUTFChars(jPublicKey, NULL);
	const char *payPasswd = env->GetStringUTFChars(jPayPasswd, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json payloadJson = wallet->GenerateCancelProducerPayload(publicKey, payPasswd);
		payload = env->NewStringUTF(payloadJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jPublicKey, publicKey);
	env->ReleaseStringUTFChars(jPayPasswd, payPasswd);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return payload;
}

#define SIG_nativeCreateRegisterProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateRegisterProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jFromAddress,
		jstring jPayloadJson,
		jstring jamount,
		jstring jMemo,
		jboolean useVotedUTXO)
{
	bool exception = false;
	std::string msgException;
	jstring tx = NULL;

	const char *fromAddress = env->GetStringUTFChars(jFromAddress, NULL);
	const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
	const char *amount = env->GetStringUTFChars(jamount, NULL);
	const char *memo        = env->GetStringUTFChars(jMemo, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json payload = nlohmann::json::parse(payloadJson);
		nlohmann::json txJson = wallet->CreateRegisterProducerTransaction(fromAddress, payload, amount, memo, useVotedUTXO);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jFromAddress, fromAddress);
	env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
	env->ReleaseStringUTFChars(jamount, amount);
	env->ReleaseStringUTFChars(jMemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeCreateUpdateProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateUpdateProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jFromAddress,
		jstring jPayloadJson,
		jstring jMemo,
		jboolean useVotedUTXO)
{
	bool exception = false;
	std::string msgException;
	jstring tx = NULL;

	const char *fromAddress = env->GetStringUTFChars(jFromAddress, NULL);
	const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
	const char *memo        = env->GetStringUTFChars(jMemo, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json payload = nlohmann::json::parse(payloadJson);
		nlohmann::json txJson = wallet->CreateUpdateProducerTransaction(fromAddress, payload, memo, useVotedUTXO);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jFromAddress, fromAddress);
	env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
	env->ReleaseStringUTFChars(jMemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeCreateCancelProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateCancelProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jFromAddress,
		jstring jPayloadJson,
		jstring jMemo,
		jboolean useVotedUTXO)
{
	bool exception = false;
	std::string msgException;
	jstring tx = NULL;

	const char *fromAddress = env->GetStringUTFChars(jFromAddress, NULL);
	const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
	const char *memo        = env->GetStringUTFChars(jMemo, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json payload = nlohmann::json::parse(payloadJson);
		nlohmann::json txJson = wallet->CreateCancelProducerTransaction(fromAddress, payload, memo, useVotedUTXO);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jFromAddress, fromAddress);
	env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
	env->ReleaseStringUTFChars(jMemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeCreateRetrieveDepositTransaction "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
static jstring JNICALL nativeCreateRetrieveDepositTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jamount,
		jstring jMemo)
{
	bool exception = false;
	std::string msgException;
	jstring tx = NULL;

	const char *amount = env->GetStringUTFChars(jamount, NULL);
	const char *memo   = env->GetStringUTFChars(jMemo, NULL);

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		nlohmann::json txJson = wallet->CreateRetrieveDepositTransaction(amount, memo);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jamount, amount);
	env->ReleaseStringUTFChars(jMemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeGetPublicKeyForVote "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetPublicKeyForVote(JNIEnv *env, jobject clazz, jlong jProxy)
{
	bool exception = false;
	std::string msgException;
	jstring publicKey = NULL;

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet *)jProxy;
		std::string pubKey = wallet->GetOwnerPublicKey();
		publicKey = env->NewStringUTF(pubKey.c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return publicKey;
}

#define SIG_nativeCreateVoteProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"
static jstring JNICALL nativeCreateVoteProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
		jstring jfromAddress,
		jstring jstake,
		jstring jPublicKeys,
		jstring jMemo,
		jboolean useVotedUTXO)
{

	bool exception = false;
	std::string msgException;

	const char *fromAddress = env->GetStringUTFChars(jfromAddress, NULL);
	const char *stake = env->GetStringUTFChars(jstake, NULL);
	const char *publicKeys  = env->GetStringUTFChars(jPublicKeys, NULL);
	const char *memo        = env->GetStringUTFChars(jMemo, NULL);

	jstring tx = NULL;

	try {
		IMainchainSubWallet *wallet = (IMainchainSubWallet*)jProxy;
		nlohmann::json txJson = wallet->CreateVoteProducerTransaction(fromAddress, stake, nlohmann::json::parse(publicKeys), memo, useVotedUTXO);
		tx = env->NewStringUTF(txJson.dump().c_str());
	} catch (std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	env->ReleaseStringUTFChars(jfromAddress, fromAddress);
	env->ReleaseStringUTFChars(jstake, stake);
	env->ReleaseStringUTFChars(jPublicKeys, publicKeys);
	env->ReleaseStringUTFChars(jMemo, memo);

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return tx;
}

#define SIG_nativeGetVotedProducerList "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetVotedProducerList(JNIEnv *env, jobject clazz, jlong jSubWalletProxy) {
	bool exception = false;
	std::string msgException;

	IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;
	jstring list = NULL;

	try {
		nlohmann::json listJson = subWallet->GetVotedProducerList();
		list = env->NewStringUTF(listJson.dump().c_str());
	} catch (const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return list;
}

#define SIG_nativeGetRegisteredProducerInfo "(J)Ljava/lang/String;"
static jstring JNICALL nativeGetRegisteredProducerInfo(JNIEnv *env, jobject clazz, jlong jSubWalletProxy) {
	bool exception = false;
	jstring info = NULL;
	std::string msgException;

	IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

	try {
		nlohmann::json infoJson = subWallet->GetRegisteredProducerInfo();
		info = env->NewStringUTF(infoJson.dump().c_str());
	} catch(const std::exception &e) {
		exception = true;
		msgException = e.what();
	}

	if (exception) {
		ThrowWalletException(env, msgException.c_str());
	}

	return info;
}

static const JNINativeMethod gMethods[] = {
	REGISTER_METHOD(nativeCreateDepositTransaction),
	REGISTER_METHOD(nativeGenerateProducerPayload),
	REGISTER_METHOD(nativeGenerateCancelProducerPayload),
	REGISTER_METHOD(nativeCreateRegisterProducerTransaction),
	REGISTER_METHOD(nativeCreateUpdateProducerTransaction),
	REGISTER_METHOD(nativeCreateCancelProducerTransaction),
	REGISTER_METHOD(nativeCreateRetrieveDepositTransaction),
	REGISTER_METHOD(nativeGetPublicKeyForVote),
	REGISTER_METHOD(nativeCreateVoteProducerTransaction),
	REGISTER_METHOD(nativeGetVotedProducerList),
	REGISTER_METHOD(nativeGetRegisteredProducerInfo),
};

jint register_elastos_spv_IMainchainSubWallet(JNIEnv *env)
{
	return jniRegisterNativeMethods(env,
			"org/elastos/spvcore/IMainchainSubWallet",
			gMethods, NELEM(gMethods));
}

