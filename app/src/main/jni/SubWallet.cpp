// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include "Utils.h"
#include "ISubWallet.h"
#include "nlohmann/json.hpp"

#define TAG "SubWallet"

using namespace Elastos::ElaWallet;

#define JNI_GetChainID "(J)Ljava/lang/String;"

static jstring JNICALL GetChainID(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring chainId = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string result = subWallet->GetChainID();
        chainId = env->NewStringUTF(result.c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return chainId;
}

#define JNI_GetBasicInfo "(J)Ljava/lang/String;"

static jstring JNICALL GetBasicInfo(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring info = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json result = subWallet->GetBasicInfo();
        info = env->NewStringUTF(result.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return info;
}

#define JNI_CreateAddress "(J)Ljava/lang/String;"

static jstring JNICALL CreateAddress(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring addr = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string result = subWallet->CreateAddress();
        addr = env->NewStringUTF(result.c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addr;
}

#define JNI_GetAllAddress "(JIIZ)Ljava/lang/String;"

static jstring JNICALL GetAllAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                     jint jStart,
                                     jint jCount,
                                     jboolean jInternal) {
    jstring addresses = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json addressesJson = subWallet->GetAllAddress(jStart, jCount, jInternal);
        addresses = env->NewStringUTF(addressesJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addresses;
}

#define JNI_GetLastAddresses "(JZ)[Ljava/lang/String;"

static jobjectArray JNICALL GetLastAddresses(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                     jboolean jInternal) {
    std::vector<std::string> addresses;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        addresses = subWallet->GetLastAddresses(jInternal);
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    jclass objClass = env->FindClass("java/lang/String");
    jobjectArray objArray = env->NewObjectArray(addresses.size(), objClass, 0);
    for (int i = 0; i < addresses.size(); ++i) {
        env->SetObjectArrayElement(objArray, i, env->NewStringUTF(addresses[i].c_str()));
    }

    return objArray;
}

#define JNI_UpdateUsedAddress "(J[Ljava/lang/String;)V"

static void JNICALL UpdateUsedAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                      jobjectArray jusedAddresses) {
    std::vector<std::string> usedAddresses;

    jint len = env->GetArrayLength(jusedAddresses);
    for (int i = 0; i < len; ++i) {
        jstring jAddress = static_cast<jstring>(env->GetObjectArrayElement(jusedAddresses, i));
        const char *address = env->GetStringUTFChars(jAddress, NULL);
        usedAddresses.push_back(address);
    }

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        subWallet->UpdateUsedAddress(usedAddresses);
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }
}

#define JNI_GetAllPublicKeys "(JII)Ljava/lang/String;"

static jstring JNICALL GetAllPublicKeys(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                        jint jStart,
                                        jint jCount) {
    jstring publicKeys = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json pubKeyJson = subWallet->GetAllPublicKeys(jStart, jCount);
        publicKeys = env->NewStringUTF(pubKeyJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return publicKeys;
}

#define JNI_CreateTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                         jstring jInputs,
                                         jstring jOutputs,
                                         jstring jFee,
                                         jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *outputs = env->GetStringUTFChars(jOutputs, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json result = subWallet->CreateTransaction(nlohmann::json::parse(inputs),
                                                            nlohmann::json::parse(outputs),
                                                            fee, memo);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jOutputs, outputs);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_SignTransaction "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL SignTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                       jstring jRawTransaction,
                                       jstring jPayPassword) {
    bool exception = false;
    std::string msgException;

    const char *rawTransaction = env->GetStringUTFChars(jRawTransaction, NULL);
    const char *payPassword = env->GetStringUTFChars(jPayPassword, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json result = subWallet->SignTransaction(nlohmann::json::parse(rawTransaction),
                                                           payPassword);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (const std::exception &e) {
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

#define JNI_GetTransactionSignedInfo "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GetTransactionSignedInfo(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                                jstring jTransactionJson) {
    bool exception = false;
    std::string msgException;

    const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);
    jstring result = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json signers = subWallet->GetTransactionSignedInfo(
                nlohmann::json::parse(transactionJson));
        result = env->NewStringUTF(signers.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ConvertToRawTransaction "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ConvertToRawTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                                jstring jTransactionJson) {
    bool exception = false;
    std::string msgException;

    const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);
    jstring result = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string tx = subWallet->ConvertToRawTransaction(nlohmann::json::parse(transactionJson));
        result = env->NewStringUTF(tx.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetChainID),
        REGISTER_METHOD(GetBasicInfo),
        REGISTER_METHOD(CreateAddress),
        REGISTER_METHOD(GetAllAddress),
        REGISTER_METHOD(GetLastAddresses),
        REGISTER_METHOD(UpdateUsedAddress),
        REGISTER_METHOD(GetAllPublicKeys),
        REGISTER_METHOD(CreateTransaction),
        REGISTER_METHOD(SignTransaction),
        REGISTER_METHOD(GetTransactionSignedInfo),
        REGISTER_METHOD(ConvertToRawTransaction),
};

jint RegisterSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "SubWallet", methods, NELEM(methods));
}