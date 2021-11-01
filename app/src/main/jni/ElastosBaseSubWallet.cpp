// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include "Utils.h"
#include "IElastosBaseSubWallet.h"
#include "nlohmann/json.hpp"

#define TAG "ElastosBaseSubWallet"

using namespace Elastos::ElaWallet;


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

    IElastosBaseSubWallet *subWallet = (IElastosBaseSubWallet *) jSubProxy;
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

#define JNI_GetTransactionSignedInfo "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GetTransactionSignedInfo(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                                jstring jTransactionJson) {
    bool exception = false;
    std::string msgException;

    const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);
    jstring result = NULL;

    try {
        IElastosBaseSubWallet *subWallet = (IElastosBaseSubWallet *) jSubProxy;
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
        IElastosBaseSubWallet *subWallet = (IElastosBaseSubWallet *) jSubProxy;
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
        REGISTER_METHOD(CreateTransaction),
        REGISTER_METHOD(GetTransactionSignedInfo),
        REGISTER_METHOD(ConvertToRawTransaction),
};

jint RegisterElastosBaseSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "ElastosBaseSubWallet", methods, NELEM(methods));
}
