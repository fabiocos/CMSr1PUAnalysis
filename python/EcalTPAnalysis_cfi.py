import FWCore.ParameterSet.Config as cms

stdEcalTPPset = cms.PSet(
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),                                     
    eeRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    tpCollection = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
    tpEtTh = cms.double(0.5),
    rhTEtTh = cms.double(0.5),
    vtxSel = cms.int32(20),
    effTh = cms.double(40.)
)

ecalTPAnalysis = cms.EDAnalyzer("EcalTPAnalysis",
    stdEcalTPPset                                
)                                     
