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
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json addressesJson = subWallet->GetLegacyAddresses(jIndex, jCount, jInternal);
        addresses = env->NewStringUTF(addressesJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addresses;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetLegacyAddresses),
};

jint RegisterBTCSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "BTCSubWallet", methods, NELEM(methods));
}