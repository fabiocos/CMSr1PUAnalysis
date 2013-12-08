import FWCore.ParameterSet.Config as cms

stdl1CaloPset = cms.PSet(
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    caloRegionCollection = cms.InputTag("rctReEmulDigis"),
    ettCollection = cms.InputTag("gctReEmulDigis"),
    httCollection = cms.InputTag("gctReEmulDigis"),
    etmCollection = cms.InputTag("gctReEmulDigis"),
    l1TrigCollection = cms.InputTag("gtReEmulDigis"),
    caloTowerCollection = cms.InputTag("towerMaker"),
    vtxSel = cms.int32(20),
    etTh = cms.double(0.),
    cutCTEt = cms.bool(True),
    etCTTh = cms.double(1.)
)

l1CaloAnalysis = cms.EDAnalyzer("L1CaloAnalysis",
    stdl1CaloPset
)                                
