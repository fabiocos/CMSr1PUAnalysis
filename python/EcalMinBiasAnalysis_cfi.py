import FWCore.ParameterSet.Config as cms

stdEcalAnaPset = cms.PSet(
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    pixelClusterCollection = cms.InputTag("siPixelClusters"),
    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),                                     
    eeRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    eeDigiCollection = cms.InputTag("ecalDigis","eeDigis"),
    vtxSel = cms.int32(20)                                     
)                                

ecalMinBiasAnalysis = cms.EDAnalyzer("EcalMinBiasAnalysis",
    stdEcalAnaPset
)                                     
