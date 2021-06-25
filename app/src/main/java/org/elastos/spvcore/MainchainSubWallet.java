// Copyright (c) 2021 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.spvcore;

/**
 * MainchainSubWallet jni
 */
public class MainchainSubWallet extends SubWallet {
    private long mMainchainProxy;

    public MainchainSubWallet(long proxy) {
        super(proxy);
        mMainchainProxy = proxy;
    }

    public String CreateDepositTransaction(String inputs, String sideChainID, String amount, String sideChainAddress, String lockAddress, String fee, String memo) throws WalletException {
        return CreateDepositTransaction(mMainchainProxy, inputs, sideChainID, amount, sideChainAddress, lockAddress, fee, memo);
    }

    public String GetDepositAddress(String pubkey) throws WalletException {
        return GetDepositAddress(mMainchainProxy, pubkey);
    }

    public String CreateVoteTransaction(String inputs, String voteContents, String fee, String memo) throws WalletException {
        return CreateVoteTransaction(mMainchainProxy, inputs, voteContents, fee, memo);
    }

    public String GenerateProducerPayload(String publicKey, String nodePublicKey, String nickName, String url, String IPAddress, long location, String payPasswd) throws WalletException {
        return GenerateProducerPayload(mMainchainProxy, publicKey, nodePublicKey, nickName, url, IPAddress, location, payPasswd);
    }

    public String GenerateCancelProducerPayload(String publicKey, String payPasswd) throws WalletException {
        return GenerateCancelProducerPayload(mMainchainProxy, publicKey, payPasswd);
    }

    public String CreateRegisterProducerTransaction(String inputs, String payloadJson, String amount, String fee, String memo) throws WalletException {
        return CreateRegisterProducerTransaction(mMainchainProxy, inputs, payloadJson, amount, fee, memo);
    }

    public String CreateUpdateProducerTransaction(String inputs, String payloadJson, String fee, String memo) throws WalletException {
        return CreateUpdateProducerTransaction(mMainchainProxy, inputs, payloadJson, fee, memo);
    }

    public String CreateCancelProducerTransaction(String inputs, String payloadJson, String fee, String memo) throws WalletException {
        return CreateCancelProducerTransaction(mMainchainProxy, inputs, payloadJson, fee, memo);
    }

    public String CreateRetrieveDepositTransaction(String inputs, String amount, String fee, String memo) throws WalletException {
        return CreateRetrieveDepositTransaction(mMainchainProxy, inputs, amount, fee, memo);
    }

    public String GetOwnerPublicKey() throws WalletException {
        return GetOwnerPublicKey(mMainchainProxy);
    }

    public String GetOwnerAddress() throws WalletException {
        return GetOwnerAddress(mMainchainProxy);
    }

    public String GetOwnerDepositAddress() throws WalletException {
        return GetOwnerDepositAddress(mMainchainProxy);
    }

    public String GetCRDepositAddress() throws WalletException {
        return GetCRDepositAddress(mMainchainProxy);
    }

    public String GenerateCRInfoPayload(String crPublickey, String did, String nickName, String url,
                                        long location) throws WalletException {
        return GenerateCRInfoPayload(mMainchainProxy, crPublickey, did, nickName, url, location);
    }

    public String GenerateUnregisterCRPayload(String cID) throws WalletException {
        return GenerateUnregisterCRPayload(mMainchainProxy, cID);
    }

    public String CreateRegisterCRTransaction(String inputs, String payload, String amount, String fee, String memo) throws WalletException {
        return CreateRegisterCRTransaction(mMainchainProxy, inputs, payload, amount, fee, memo);
    }

    public String CreateUpdateCRTransaction(String inputs, String payload, String fee, String memo) throws WalletException {
        return CreateUpdateCRTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String CreateUnregisterCRTransaction(String inputs, String payload, String fee, String memo) throws WalletException {
        return CreateUnregisterCRTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String CreateRetrieveCRDepositTransaction(String inputs, String amount, String fee, String memo) throws WalletException {
        return CreateRetrieveCRDepositTransaction(mMainchainProxy, inputs, amount, fee, memo);
    }

    public String CRCouncilMemberClaimNodeDigest(String payload) throws WalletException {
        return CRCouncilMemberClaimNodeDigest(mMainchainProxy, payload);
    }

    public String CreateCRCouncilMemberClaimNodeTransaction(String inputs, String payload, String fee, String memo) throws WalletException {
        return CreateCRCouncilMemberClaimNodeTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String ProposalOwnerDigest(String payload) throws WalletException {
        return ProposalOwnerDigest(mMainchainProxy, payload);
    }

    public String ProposalCRCouncilMemberDigest(String payload) throws WalletException {
        return ProposalCRCouncilMemberDigest(mMainchainProxy, payload);
    }

    public String CalculateProposalHash(String payload) throws WalletException {
        return CalculateProposalHash(mMainchainProxy, payload);
    }

    public String CreateProposalTransaction(String inputs, String payload, String fee, String memo) throws WalletException {
        return CreateProposalTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String ProposalReviewDigest(String payload) throws WalletException {
        return ProposalReviewDigest(mMainchainProxy, payload);
    }

    public String CreateProposalReviewTransaction(String inputs, String payload, String fee, String memo) throws WalletException {
        return CreateProposalReviewTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String ProposalTrackingOwnerDigest(String payload) {
        return ProposalTrackingOwnerDigest(mMainchainProxy, payload);
    }

    public String ProposalTrackingNewOwnerDigest(String payload) {
        return ProposalTrackingNewOwnerDigest(mMainchainProxy,payload);
    }

    public String ProposalTrackingSecretaryDigest(String payload) {
        return ProposalTrackingSecretaryDigest(mMainchainProxy,payload);
    }

    public String CreateProposalTrackingTransaction(String inputs, String payload, String fee, String memo) {
        return CreateProposalTrackingTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    public String ProposalSecretaryGeneralElectionDigest(String payload) {
        return ProposalSecretaryGeneralElectionDigest(mMainchainProxy, payload);
    }
    public String ProposalSecretaryGeneralElectionCRCouncilMemberDigest(String payload) {
        return ProposalSecretaryGeneralElectionCRCouncilMemberDigest(mMainchainProxy, payload);
    }
    public String CreateSecretaryGeneralElectionTransaction(String inputs, String payload, String fee, String memo) {
        return CreateSecretaryGeneralElectionTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }
    public String ProposalChangeOwnerDigest(String payload) {
        return ProposalChangeOwnerDigest(mMainchainProxy, payload);
    }
    public String ProposalChangeOwnerCRCouncilMemberDigest(String payload) {
        return ProposalChangeOwnerCRCouncilMemberDigest(mMainchainProxy, payload);
    }
    public String CreateProposalChangeOwnerTransaction(String inputs, String payload, String fee, String memo) {
        return CreateProposalChangeOwnerTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }
    public String TerminateProposalOwnerDigest(String payload) {
        return TerminateProposalOwnerDigest(mMainchainProxy, payload);
    }
    public String TerminateProposalCRCouncilMemberDigest(String payload) {
        return TerminateProposalCRCouncilMemberDigest(mMainchainProxy, payload);
    }
    public String CreateTerminateProposalTransaction(String inputs, String payload, String fee, String memo) {
        return CreateTerminateProposalTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }
    public String ProposalWithdrawDigest(String payload) {
        return ProposalWithdrawDigest(mMainchainProxy, payload);
    }

    public String CreateProposalWithdrawTransaction(String inputs, String payload, String fee, String memo) {
        return CreateProposalWithdrawTransaction(mMainchainProxy, inputs, payload, fee, memo);
    }

    private native String CreateDepositTransaction(long proxy, String inputs, String sideChainID, String amount,
                                                   String sideChainAddress, String lockAddress, String fee, String memo);
    private native String GetDepositAddress(long Proxy, String pubkey);
    private native String CreateVoteTransaction(long Proxy, String inputs, String voteContents, String fee, String memo);
    private native String GenerateProducerPayload(long proxy, String publicKey, String nodePublicKey, String nickName,
                                                  String url, String IPAddress, long location, String payPasswd);
    private native String GenerateCancelProducerPayload(long proxy, String publicKey, String payPasswd);
    private native String CreateRegisterProducerTransaction(long proxy, String inputs, String payloadJson, String amount,
                                                            String fee, String memo);
    private native String CreateUpdateProducerTransaction(long proxy, String inputs, String payloadJson, String fee, String memo);
    private native String CreateCancelProducerTransaction(long proxy, String inputs, String payloadJson, String fee, String memo);
    private native String CreateRetrieveDepositTransaction(long proxy, String inputs, String amount, String fee, String memo);
    private native String GetOwnerPublicKey(long proxy);
    private native String GetOwnerAddress(long proxy);
    private native String GetOwnerDepositAddress(long proxy);
    private native String GetCRDepositAddress(long proxy);
    private native String GenerateCRInfoPayload(long proxy, String crPublickey, String did, String nickName, String url, long location);
    private native String GenerateUnregisterCRPayload(long proxy, String cID);
    private native String CreateRegisterCRTransaction(long proxy, String inputs, String payload, String amount, String fee, String memo);
    private native String CreateUpdateCRTransaction(long proxy, String inputs, String payload, String fee, String memo);
    private native String CreateUnregisterCRTransaction(long proxy, String inputs, String payload, String fee, String memo);
    private native String CreateRetrieveCRDepositTransaction(long Proxy, String inputs, String amount, String fee, String memo);
    private native String CRCouncilMemberClaimNodeDigest(long Proxy, String payload);
    private native String CreateCRCouncilMemberClaimNodeTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalOwnerDigest(long Proxy, String payload);
    private native String ProposalCRCouncilMemberDigest(long Proxy, String payload);
    private native String CalculateProposalHash(long Proxy, String payload);
    private native String CreateProposalTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalReviewDigest(long Proxy, String payload);
    private native String CreateProposalReviewTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalTrackingOwnerDigest(long Proxy, String payload);
    private native String ProposalTrackingNewOwnerDigest(long Proxy, String payload);
    private native String ProposalTrackingSecretaryDigest(long Proxy, String payload);
    private native String CreateProposalTrackingTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalSecretaryGeneralElectionDigest(long Proxy, String payload);
    private native String ProposalSecretaryGeneralElectionCRCouncilMemberDigest(long Proxy, String payload);
    private native String CreateSecretaryGeneralElectionTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalChangeOwnerDigest(long Proxy, String payload);
    private native String ProposalChangeOwnerCRCouncilMemberDigest(long Proxy, String payload);
    private native String CreateProposalChangeOwnerTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String TerminateProposalOwnerDigest(long Proxy, String payload);
    private native String TerminateProposalCRCouncilMemberDigest(long Proxy, String payload);
    private native String CreateTerminateProposalTransaction(long Proxy, String inputs, String payload, String fee, String memo);
    private native String ProposalWithdrawDigest(long Proxy, String payload);
    private native String CreateProposalWithdrawTransaction(long Proxy, String inputs, String payload, String fee, String memo);

}
