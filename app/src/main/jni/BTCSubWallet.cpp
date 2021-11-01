// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include "Utils.h"
#include "IBTCSubWallet.h"
#include "nlohmann/json.hpp"

#define TAG "BTCSubWallet"

using namespace Elastos::ElaWallet;

#define JNI_GetLegacyAddresses "(JIIZ)Ljava/lang/String;"

static jstring JNICALL GetLegacyAddresses(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                     jint jIndex, jint jCount, jboolean jInternal) {
    jstring addresses = NULL;

    try {
        IBTCSubWallet *subWallet = (IBTCSubWallet *) jSubProxy;
        nlohmann::json addressesJson = subWallet->GetLegacyAddresses(jIndex, jCount, jInternal);
        addresses = env->NewStringUTF(addressesJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addresses;
}


#define JNI_CreateTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                         jstring jInputs,
                                         jstring jOutputs,
                                         jstring jChangeAddress,
                                         jstring jFeePerKB) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *outputs = env->GetStringUTFChars(jOutputs, NULL);
    const char *changeAddress = env->GetStringUTFChars(jChangeAddress, NULL);
    const char *feePerKB = env->GetStringUTFChars(jFeePerKB, NULL);

    IBTCSubWallet *subWallet = (IBTCSubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json result = subWallet->CreateTransaction(nlohmann::json::parse(inputs),
                                                            nlohmann::json::parse(outputs),
                                                            changeAddress, feePerKB);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jOutputs, outputs);
    env->ReleaseStringUTFChars(jChangeAddress, changeAddress);
    env->ReleaseStringUTFChars(jFeePerKB, feePerKB);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetLegacyAddresses),
        REGISTER_METHOD(CreateTransaction),
};

jint RegisterBTCSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "BTCSubWallet", methods, NELEM(methods));
}
