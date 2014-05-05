import FWCore.ParameterSet.Config as cms

genMinBiasAnalysis = cms.EDAnalyzer("GenMinBiasAnalysis",
    hepmcCollection = cms.InputTag("generator"),
    verbose = cms.untracked.bool(False)                                    
)                                     
