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

#define JNI_GetAddresses "(JIIZ)Ljava/lang/String;"

static jstring JNICALL GetAddresses(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                     jint jStart, jint jCount, jboolean jInternal) {
    jstring addresses = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json addressesJson = subWallet->GetAddresses(jStart, jCount, jInternal);
        addresses = env->NewStringUTF(addressesJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addresses;
}

#define JNI_GetPublicKeys "(JIIZ)Ljava/lang/String;"

static jstring JNICALL GetPublicKeys(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                        jint jIndex, jint jCount, jboolean jInternal) {
    jstring publicKeys = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json pubKeyJson = subWallet->GetPublicKeys(jIndex, jCount, jInternal);
        publicKeys = env->NewStringUTF(pubKeyJson.dump().c_str());
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return publicKeys;
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


#define JNI_SignDigest "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL SignDigest(JNIEnv *env, jobject clazz, jlong instance,
                                  jstring jaddress,
                                  jstring jdigest,
                                  jstring jpayPasswd) {
    bool exception = false;
    std::string msgException;
    jstring signature = NULL;

    const char *address = env->GetStringUTFChars(jaddress, NULL);
    const char *digest = env->GetStringUTFChars(jdigest, NULL);
    const char *payPasswd = env->GetStringUTFChars(jpayPasswd, NULL);

    try {
        ISubWallet *wallet = (ISubWallet *) instance;
        std::string sig = wallet->SignDigest(address, digest, payPasswd);
        signature = env->NewStringUTF(sig.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jaddress, address);
    env->ReleaseStringUTFChars(jdigest, digest);
    env->ReleaseStringUTFChars(jpayPasswd, payPasswd);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
    return signature;
}

#define JNI_VerifyDigest "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z"

static jboolean JNICALL VerifyDigest(JNIEnv *env, jobject clazz, jlong instance,
                                  jstring jpublicKey,
                                  jstring jdigest,
                                  jstring jsignature) {
    bool exception = false;
    std::string msgException;
    bool valid = false;

    const char *publicKey = env->GetStringUTFChars(jpublicKey, NULL);
    const char *digest = env->GetStringUTFChars(jdigest, NULL);
    const char *signature = env->GetStringUTFChars(jsignature, NULL);

    try {
        ISubWallet *wallet = (ISubWallet *) instance;
        valid = wallet->VerifyDigest(publicKey, digest, signature);
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpublicKey, publicKey);
    env->ReleaseStringUTFChars(jdigest, digest);
    env->ReleaseStringUTFChars(jsignature, signature);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
    return (jboolean) valid;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetChainID),
        REGISTER_METHOD(GetBasicInfo),
        REGISTER_METHOD(GetAddresses),
        REGISTER_METHOD(GetPublicKeys),
        REGISTER_METHOD(SignTransaction),
        REGISTER_METHOD(SignDigest),
        REGISTER_METHOD(VerifyDigest),
};

jint RegisterSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "SubWallet", methods, NELEM(methods));
}