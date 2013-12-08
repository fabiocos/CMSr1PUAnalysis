import FWCore.ParameterSet.Config as cms

stdHcalAnaPset = cms.PSet(
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    hbheRecHitCollection = cms.InputTag("hbhereco"),
    vtxSel = cms.int32(20)                                     
)                                

hcalMinBiasAnalysis = cms.EDAnalyzer("HcalMinBiasAnalysis",
    stdHcalAnaPset
)                                     
