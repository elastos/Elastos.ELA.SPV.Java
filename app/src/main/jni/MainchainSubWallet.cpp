// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "IMainchainSubWallet.h"
#include "nlohmann/json.hpp"

using namespace Elastos::ElaWallet;

#define JNI_CreateDepositTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateDepositTransaction(JNIEnv *env, jobject clazz, jlong instance,
                                                jstring jInputs,
                                                jstring jsideChainID,
                                                jstring jamount,
                                                jstring jsideChainAddress,
                                                jstring jlockAddress,
                                                jstring jfee,
                                                jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *sideChainID = env->GetStringUTFChars(jsideChainID, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    const char *sideChainAddress = env->GetStringUTFChars(jsideChainAddress, NULL);
    const char *lockAddress = env->GetStringUTFChars(jlockAddress, NULL);
    const char *fee = env->GetStringUTFChars(jfee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *wallet = (IMainchainSubWallet *) instance;
    jstring tx = NULL;

    try {
        nlohmann::json txJson = wallet->CreateDepositTransaction(nlohmann::json::parse(inputs),
                                                                  sideChainID, amount,
                                                                  sideChainAddress, lockAddress,
                                                                  fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jsideChainID, sideChainID);
    env->ReleaseStringUTFChars(jamount, amount);
    env->ReleaseStringUTFChars(jsideChainAddress, sideChainAddress);
    env->ReleaseStringUTFChars(jlockAddress, lockAddress);
    env->ReleaseStringUTFChars(jfee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_GenerateProducerPayload "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GenerateProducerPayload(JNIEnv *env, jobject clazz, jlong jProxy,
                                               jstring jPublicKey,
                                               jstring jNodePublicKey,
                                               jstring jNickName,
                                               jstring jURL,
                                               jstring jIPAddress,
                                               jlong location,
                                               jstring jPayPasswd) {
    bool exception = false;
    std::string msgException;
    jstring payload = NULL;

    const char *publicKey = env->GetStringUTFChars(jPublicKey, NULL);
    const char *nodePublicKey = env->GetStringUTFChars(jNodePublicKey, NULL);
    const char *nickName = env->GetStringUTFChars(jNickName, NULL);
    const char *url = env->GetStringUTFChars(jURL, NULL);
    const char *ipAddress = env->GetStringUTFChars(jIPAddress, NULL);
    const char *payPasswd = env->GetStringUTFChars(jPayPasswd, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json payloadJson = wallet->GenerateProducerPayload(publicKey, nodePublicKey,
                                                                     nickName, url, ipAddress,
                                                                     location, payPasswd);
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

#define JNI_GenerateCancelProducerPayload "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GenerateCancelProducerPayload(JNIEnv *env, jobject clazz, jlong jProxy,
                                                     jstring jPublicKey,
                                                     jstring jPayPasswd) {
    bool exception = false;
    std::string msgException;
    jstring payload = NULL;

    const char *publicKey = env->GetStringUTFChars(jPublicKey, NULL);
    const char *payPasswd = env->GetStringUTFChars(jPayPasswd, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
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

#define JNI_CreateRegisterProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateRegisterProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                         jstring jInputs,
                                                         jstring jPayloadJson,
                                                         jstring jamount,
                                                         jstring jFee,
                                                         jstring jMemo) {
    bool exception = false;
    std::string msgException;
    jstring tx = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jMemo, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateRegisterProducerTransaction(nlohmann::json::parse(inputs),
                                                                          nlohmann::json::parse(payloadJson),
                                                                          amount, fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
    env->ReleaseStringUTFChars(jamount, amount);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jMemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateUpdateProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateUpdateProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                       jstring jInputs,
                                                       jstring jPayloadJson,
                                                       jstring jFee,
                                                       jstring jMemo) {
    bool exception = false;
    std::string msgException;
    jstring tx = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jMemo, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateUpdateProducerTransaction(nlohmann::json::parse(inputs),
                                                                        nlohmann::json::parse(payloadJson),
                                                                        fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jMemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateCancelProducerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateCancelProducerTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                       jstring jInputs,
                                                       jstring jPayloadJson,
                                                       jstring jFee,
                                                       jstring jMemo) {
    bool exception = false;
    std::string msgException;
    jstring tx = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payloadJson = env->GetStringUTFChars(jPayloadJson, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jMemo, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateCancelProducerTransaction(nlohmann::json::parse(inputs),
                                                                        nlohmann::json::parse(payloadJson),
                                                                        fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jPayloadJson, payloadJson);
    env->ReleaseStringUTFChars(jMemo, memo);
    env->ReleaseStringUTFChars(jFee, fee);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateRetrieveDepositTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateRetrieveDepositTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                        jstring jInputs,
                                                        jstring jFee,
                                                        jstring jMemo) {
    bool exception = false;
    std::string msgException;
    jstring tx = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jMemo, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateRetrieveDepositTransaction(nlohmann::json::parse(inputs), fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jMemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_GetOwnerPublicKey "(J)Ljava/lang/String;"

static jstring JNICALL GetOwnerPublicKey(JNIEnv *env, jobject clazz, jlong jProxy) {
    bool exception = false;
    std::string msgException;
    jstring publicKey = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
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

#define JNI_GetOwnerAddress "(J)Ljava/lang/String;"

static jstring JNICALL GetOwnerAddress(JNIEnv *env, jobject clazz, jlong jSubWalletProxy) {
    bool exception = false;
    std::string msgException;
    jstring ownerAddress = NULL;

    try {
        IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;
        std::string address = subWallet->GetOwnerAddress();
        ownerAddress = env->NewStringUTF(address.c_str());

    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
    return ownerAddress;
}

#define JNI_GetOwnerDepositAddress "(J)Ljava/lang/String;"

static jstring JNICALL GetOwnerDepositAddress(JNIEnv *env, jobject clazz, jlong jSubWalletProxy) {
    bool exception = false;
    std::string msgException;
    jstring ownerDepositAddress = NULL;

    try {
        IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;
        std::string address = subWallet->GetOwnerDepositAddress();
        ownerDepositAddress = env->NewStringUTF(address.c_str());

    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
    return ownerDepositAddress;
}

#define JNI_GetCRDepositAddress "(J)Ljava/lang/String;"

static jstring JNICALL GetCRDepositAddress(JNIEnv *env, jobject clazz, jlong jSubWalletProxy) {
    bool exception = false;
    std::string msgException;
    jstring crDepositAddress = NULL;

    try {
        IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;
        std::string address = subWallet->GetCRDepositAddress();
        crDepositAddress = env->NewStringUTF(address.c_str());

    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
    return crDepositAddress;
}

#define JNI_GenerateCRInfoPayload "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)Ljava/lang/String;"

static jstring JNICALL GenerateCRInfoPayload(JNIEnv *env, jobject clazz, jlong jProxy,
                                             jstring jCRPublicKey,
                                             jstring jdid,
                                             jstring jNickName,
                                             jstring jURL,
                                             jlong location) {
    bool exception = false;
    std::string msgException;
    jstring payload = NULL;

    const char *publicKey = env->GetStringUTFChars(jCRPublicKey, NULL);
    const char *did = env->GetStringUTFChars(jdid, NULL);
    const char *nickName = env->GetStringUTFChars(jNickName, NULL);
    const char *url = env->GetStringUTFChars(jURL, NULL);

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json payloadJson = wallet->GenerateCRInfoPayload(publicKey, did, nickName, url,
                                                                   location);
        payload = env->NewStringUTF(payloadJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jCRPublicKey, publicKey);
    env->ReleaseStringUTFChars(jdid, did);
    env->ReleaseStringUTFChars(jNickName, nickName);
    env->ReleaseStringUTFChars(jURL, url);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return payload;
}

#define JNI_GenerateUnregisterCRPayload "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GenerateUnregisterCRPayload(JNIEnv *env, jobject clazz, jlong jProxy,
                                                   jstring jCID) {
    bool exception = false;
    std::string msgException;
    jstring payload = NULL;

    const char *cID = env->GetStringUTFChars(jCID, NULL);
    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json payloadJson = wallet->GenerateUnregisterCRPayload(cID);
        payload = env->NewStringUTF(payloadJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jCID, cID);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return payload;
}

#define JNI_CreateRegisterCRTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateRegisterCRTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                   jstring jInputs,
                                                   jstring jpayload,
                                                   jstring jamount,
                                                   jstring jfee,
                                                   jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *amount = env->GetStringUTFChars(jamount, NULL);
    const char *fee = env->GetStringUTFChars(jfee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    jstring tx = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateRegisterCRTransaction(nlohmann::json::parse(inputs),
                                                                    nlohmann::json::parse(payload),
                                                                    amount, fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jamount, amount);
    env->ReleaseStringUTFChars(jfee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateUpdateCRTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateUpdateCRTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                 jstring jInputs,
                                                 jstring jpayload,
                                                 jstring jFee,
                                                 jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    jstring tx = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateUpdateCRTransaction(nlohmann::json::parse(inputs),
                                                                  nlohmann::json::parse(payload),
                                                                  fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateUnregisterCRTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateUnregisterCRTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                     jstring jInputs,
                                                     jstring jpayload,
                                                     jstring jFee,
                                                     jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    jstring tx = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateUnregisterCRTransaction(nlohmann::json::parse(inputs),
                                                                      nlohmann::json::parse(payload),
                                                                      fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateRetrieveCRDepositTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateRetrieveCRDepositTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                                          jstring jInputs,
                                                          jstring jFee,
                                                          jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    jstring tx = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateRetrieveCRDepositTransaction(nlohmann::json::parse(inputs), fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateVoteTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateVoteTransaction(JNIEnv *env, jobject clazz, jlong jProxy,
                                            jstring jInputs,
                                            jstring jvoteContents,
                                            jstring jfee,
                                            jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *voteContents = env->GetStringUTFChars(jvoteContents, NULL);
    const char *fee = env->GetStringUTFChars(jfee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    LOGW("wallet", "CreateVoteTransaction start");

    jstring tx = NULL;

    try {
        IMainchainSubWallet *wallet = (IMainchainSubWallet *) jProxy;
        nlohmann::json txJson = wallet->CreateVoteTransaction(nlohmann::json::parse(inputs),
                                                              nlohmann::json::parse(voteContents),
                                                              fee, memo);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jvoteContents, voteContents);
    env->ReleaseStringUTFChars(jfee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CRCouncilMemberClaimNodeDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CRCouncilMemberClaimNodeDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                      jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->CRCouncilMemberClaimNodeDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateCRCouncilMemberClaimNodeTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateCRCouncilMemberClaimNodeTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                          jstring jInputs,
                                          jstring jpayload,
                                          jstring jFee,
                                          jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateCRCouncilMemberClaimNodeTransaction(nlohmann::json::parse(inputs),
                                                                                nlohmann::json::parse(payload),
                                                                                fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}
#define JNI_ProposalOwnerDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalOwnerDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                          jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalOwnerDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalCRCouncilMemberDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalCRCouncilMemberDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                    jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalCRCouncilMemberDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CalculateProposalHash "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CalculateProposalHash(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                            jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string hash = subWallet->CalculateProposalHash(nlohmann::json::parse(payload));
        result = env->NewStringUTF(hash.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateProposalTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateProposalTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                jstring jInputs,
                                                jstring jpayload,
                                                jstring jFee,
                                                jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateProposalTransaction(nlohmann::json::parse(inputs),
                                                                nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalReviewDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalReviewDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                            jstring jpayload) {

    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalReviewDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateProposalReviewTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateProposalReviewTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                      jstring jInputs,
                                                      jstring jpayload,
                                                      jstring jFee,
                                                      jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateProposalReviewTransaction(nlohmann::json::parse(inputs),
                                                                      nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalTrackingOwnerDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalTrackingOwnerDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                  jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalTrackingOwnerDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalTrackingNewOwnerDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalTrackingNewOwnerDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                      jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalTrackingNewOwnerDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalTrackingSecretaryDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL ProposalTrackingSecretaryDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                      jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalTrackingSecretaryDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateProposalTrackingTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateProposalTrackingTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                   jstring jInputs,
                                   jstring jpayload,
                                   jstring jFee,
                                   jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateProposalTrackingTransaction(nlohmann::json::parse(inputs),
                                                                        nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}
//////////////////////////////////////////////////
/*      Proposal Secretary General Election     */
//////////////////////////////////////////////////
#define JNI_ProposalSecretaryGeneralElectionDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
ProposalSecretaryGeneralElectionDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                       jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalSecretaryGeneralElectionDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalSecretaryGeneralElectionCRCouncilMemberDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
ProposalSecretaryGeneralElectionCRCouncilMemberDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                                       jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalSecretaryGeneralElectionCRCouncilMemberDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateSecretaryGeneralElectionTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateSecretaryGeneralElectionTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                          jstring jInputs,
                                          jstring jpayload,
                                          jstring jFee,
                                          jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateSecretaryGeneralElectionTransaction(nlohmann::json::parse(inputs),
                                                                                nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

//////////////////////////////////////////////////
/*             Proposal Change Owner            */
//////////////////////////////////////////////////

#define JNI_ProposalChangeOwnerDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
ProposalChangeOwnerDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                        jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalChangeOwnerDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_ProposalChangeOwnerCRCouncilMemberDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
ProposalChangeOwnerCRCouncilMemberDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                          jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalChangeOwnerCRCouncilMemberDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateProposalChangeOwnerTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateProposalChangeOwnerTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                    jstring jInputs,
                                    jstring jpayload,
                                    jstring jFee,
                                    jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateProposalChangeOwnerTransaction(nlohmann::json::parse(inputs),
                                                                          nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

//////////////////////////////////////////////////
/*           Proposal Terminate Proposal        */
//////////////////////////////////////////////////

#define JNI_TerminateProposalOwnerDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
TerminateProposalOwnerDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                            jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->TerminateProposalOwnerDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_TerminateProposalCRCouncilMemberDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
TerminateProposalCRCouncilMemberDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                      jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->TerminateProposalCRCouncilMemberDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateTerminateProposalTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateTerminateProposalTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                  jstring jInputs,
                                  jstring jpayload,
                                  jstring jFee,
                                  jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateTerminateProposalTransaction(nlohmann::json::parse(inputs),
                                                                        nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}
#define JNI_ProposalWithdrawDigest "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
ProposalWithdrawDigest(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                      jstring jpayload) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *payload = env->GetStringUTFChars(jpayload, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        std::string digest = subWallet->ProposalWithdrawDigest(nlohmann::json::parse(payload));
        result = env->NewStringUTF(digest.c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpayload, payload);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CreateProposalWithdrawTransaction "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL
CreateProposalWithdrawTransaction(JNIEnv *env, jobject clazz, jlong jSubWalletProxy,
                                  jstring jInputs,
                                  jstring jpayload,
                                  jstring jFee,
                                  jstring jmemo) {
    bool exception = false;
    std::string msgException;
    jstring result = NULL;

    const char *inputs = env->GetStringUTFChars(jInputs, NULL);
    const char *payload = env->GetStringUTFChars(jpayload, NULL);
    const char *fee = env->GetStringUTFChars(jFee, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    IMainchainSubWallet *subWallet = (IMainchainSubWallet *) jSubWalletProxy;

    try {
        nlohmann::json j = subWallet->CreateProposalWithdrawTransaction(nlohmann::json::parse(inputs),
                                                                        nlohmann::json::parse(payload), fee, memo);
        result = env->NewStringUTF(j.dump().c_str());
    } catch (const std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jInputs, inputs);
    env->ReleaseStringUTFChars(jpayload, payload);
    env->ReleaseStringUTFChars(jFee, fee);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(CreateDepositTransaction),
        REGISTER_METHOD(GenerateProducerPayload),
        REGISTER_METHOD(GenerateCancelProducerPayload),
        REGISTER_METHOD(CreateRegisterProducerTransaction),
        REGISTER_METHOD(CreateUpdateProducerTransaction),
        REGISTER_METHOD(CreateCancelProducerTransaction),
        REGISTER_METHOD(CreateRetrieveDepositTransaction),
        REGISTER_METHOD(GetOwnerPublicKey),
        REGISTER_METHOD(GetOwnerAddress),
        REGISTER_METHOD(GetOwnerDepositAddress),
        REGISTER_METHOD(GetCRDepositAddress),
        REGISTER_METHOD(GenerateCRInfoPayload),
        REGISTER_METHOD(GenerateUnregisterCRPayload),
        REGISTER_METHOD(CreateRegisterCRTransaction),
        REGISTER_METHOD(CreateUpdateCRTransaction),
        REGISTER_METHOD(CreateUnregisterCRTransaction),
        REGISTER_METHOD(CreateRetrieveCRDepositTransaction),
        REGISTER_METHOD(CreateVoteTransaction),
        REGISTER_METHOD(CRCouncilMemberClaimNodeDigest),
        REGISTER_METHOD(CreateCRCouncilMemberClaimNodeTransaction),
        REGISTER_METHOD(ProposalOwnerDigest),
        REGISTER_METHOD(ProposalCRCouncilMemberDigest),
        REGISTER_METHOD(CalculateProposalHash),
        REGISTER_METHOD(CreateProposalTransaction),
        REGISTER_METHOD(ProposalReviewDigest),
        REGISTER_METHOD(CreateProposalReviewTransaction),
        REGISTER_METHOD(ProposalTrackingOwnerDigest),
        REGISTER_METHOD(ProposalTrackingNewOwnerDigest),
        REGISTER_METHOD(ProposalTrackingSecretaryDigest),
        REGISTER_METHOD(CreateProposalTrackingTransaction),
        REGISTER_METHOD(ProposalSecretaryGeneralElectionDigest),
        REGISTER_METHOD(ProposalSecretaryGeneralElectionCRCouncilMemberDigest),
        REGISTER_METHOD(CreateSecretaryGeneralElectionTransaction),
        REGISTER_METHOD(ProposalChangeOwnerDigest),
        REGISTER_METHOD(ProposalChangeOwnerCRCouncilMemberDigest),
        REGISTER_METHOD(CreateProposalChangeOwnerTransaction),
        REGISTER_METHOD(TerminateProposalOwnerDigest),
        REGISTER_METHOD(TerminateProposalCRCouncilMemberDigest),
        REGISTER_METHOD(CreateTerminateProposalTransaction),
        REGISTER_METHOD(ProposalWithdrawDigest),
        REGISTER_METHOD(CreateProposalWithdrawTransaction),
};

jint RegisterMainchainSubWallet(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "MainchainSubWallet", methods, NELEM(methods));
}



