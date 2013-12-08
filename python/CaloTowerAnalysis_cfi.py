import FWCore.ParameterSet.Config as cms

stdCTAnaPset = cms.PSet(
    recoVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    caloTowerCollection = cms.InputTag("towerMaker"),
    vtxSel = cms.int32(20),
    cutEt = cms.bool(True),
    etTh = cms.double(1.),
    etEmTh = cms.double(-999.),
    etHadTh = cms.double(-999.)                                   
)

caloTowerAnalysis = cms.EDAnalyzer("CaloTowerAnalysis",
    stdCTAnaPset
)                                
