import FWCore.ParameterSet.Config as cms

ecalTPAnalysis = cms.EDAnalyzer("EcalTPAnalysis",
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),                                     
    eeRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    tpCollection = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
    tpEtTh = cms.double(0.5),
    rhTEtTh = cms.double(1.),
    vtxSel = cms.int32(20)
)                                     
