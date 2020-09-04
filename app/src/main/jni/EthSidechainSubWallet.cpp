// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "IEthSidechainSubWallet.h"
#include "nlohmann/json.hpp"

using namespace Elastos::ElaWallet;

#define JNI_CreateTransfer "(JLjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;"

static jstring JNICALL CreateTransfer(JNIEnv *env, jobject clazz, jlong instance,
                                        jstring jtargetAddress, jstring jamount, jint amountUnit) {
    bool exception = false;
    std::string msgException;

    IEthSidechainSubWallet *wallet = (IEthSidechainSubWallet *) instance;
    const char *targetAddress = env->GetStringUTFChars(jtargetAddress, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    jstring tx = NULL;

    try {
        nlohmann::json txJson = wallet->CreateTransfer(targetAddress, amount, amountUnit);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jtargetAddress, targetAddress);
    env->ReleaseStringUTFChars(jamount, amount);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateTransferGeneric "(JLjava/lang/String;Ljava/lang/String;ILjava/lang/String;ILjava/lang/String;Ljava/lang/String)Ljava/lang/String;"

static jstring JNICALL CreateTransferGeneric(JNIEnv *env, jobject clazz, jlong instance,
                                        jstring jtargetAddress, jstring jamount, jint amountUnit,
                                        jstring jgasPrice, jint gasPriceUnit, jstring jgasLimit, jstring jdata) {
    bool exception = false;
    std::string msgException;

    IEthSidechainSubWallet *wallet = (IEthSidechainSubWallet *) instance;
    const char *targetAddress = env->GetStringUTFChars(jtargetAddress, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    const char *gasPrice = env->GetStringUTFChars(jgasPrice, NULL);
    const char *gasLimit = env->GetStringUTFChars(jgasLimit, NULL);
    const char *data = env->GetStringUTFChars(jdata, NULL);
    jstring tx = NULL;

    try {
        nlohmann::json txJson = wallet->CreateTransferGeneric(targetAddress, amount, amountUnit,
                gasPrice, gasPriceUnit, gasLimit, data);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jtargetAddress, targetAddress);
    env->ReleaseStringUTFChars(jamount, amount);
    env->ReleaseStringUTFChars(jgasPrice, gasPrice);
    env->ReleaseStringUTFChars(jgasLimit, gasLimit);
    env->ReleaseStringUTFChars(jdata, data);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

static const JNINativeMethod methods[] = {
    REGISTER_METHOD(CreateTransfer),
    // REGISTER_METHOD(CreateTransferGeneric),
};

jint RegisterEthSidechainSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "EthSidechainSubWallet", methods, NELEM(methods));
}
