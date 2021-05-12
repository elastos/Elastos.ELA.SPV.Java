// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "ISidechainSubWallet.h"
#include "nlohmann/json.hpp"

using namespace Elastos::ElaWallet;

#define JNI_CreateWithdrawTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateWithdrawTransaction(JNIEnv *env, jobject clazz, jlong jSideSubWalletProxy,
                          jstring jinputs,
                          jstring jamount,
                          jstring jmainChainAddress,
                          jstring jfee,
                          jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jinputs, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    const char *mainChainAddress = env->GetStringUTFChars(jmainChainAddress, NULL);
    const char *fee = env->GetStringUTFChars(jfee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    ISidechainSubWallet *wallet = (ISidechainSubWallet *) jSideSubWalletProxy;
    jstring tx = NULL;

    try {
        nlohmann::json txJson = wallet->CreateWithdrawTransaction(nlohmann::json::parse(inputs), amount,
                                                                  mainChainAddress, fee, memo);

        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jinputs, inputs);
    env->ReleaseStringUTFChars(jamount, amount);
    env->ReleaseStringUTFChars(jmainChainAddress, mainChainAddress);
    env->ReleaseStringUTFChars(jfee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}


static const JNINativeMethod methods[] = {
        REGISTER_METHOD(CreateWithdrawTransaction),
};

jint RegisterSidechainSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "SidechainSubWallet", methods, NELEM(methods));
}
