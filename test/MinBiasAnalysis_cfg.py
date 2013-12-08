import FWCore.ParameterSet.Config as cms

# re-emulation of L1 trigger for run 202299

def patchGct(process) :

    process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

    process.load("SimCalorimetry.HcalSimProducers.hcalUnsuppressedDigis_cfi")
    process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

    from L1Trigger.RegionalCaloTrigger.rctDigis_cfi import rctDigis
    from L1Trigger.GlobalCaloTrigger.gctDigis_cfi import gctDigis
    from L1Trigger.GlobalTrigger.gtDigis_cfi import gtDigis    

    process.hcalReEmulDigis = process.simHcalTriggerPrimitiveDigis.clone()
    process.rctReEmulDigis  = rctDigis.clone()
    process.gctReEmulDigis  = gctDigis.clone()
    process.gtReEmulDigis   = gtDigis.clone()
    
    process.hcalReEmulDigis.inputLabel = cms.VInputTag(cms.InputTag('hcalDigis'), cms.InputTag('hcalDigis'))

    process.rctReEmulDigis.ecalDigis = cms.VInputTag( cms.InputTag( 'ecalDigis:EcalTriggerPrimitives' ) )
    process.rctReEmulDigis.hcalDigis = cms.VInputTag( cms.InputTag( 'hcalReEmulDigis' ) )
    
    process.gctReEmulDigis.inputLabel  = cms.InputTag("rctReEmulDigis")
    
    process.gtReEmulDigis.GmtInputTag  = cms.InputTag("gtDigis")
    process.gtReEmulDigis.GctInputTag  = cms.InputTag("gctReEmulDigis")

    process.patchGct = cms.Sequence(
        process.ecalDigis
        + process.hcalDigis
        + process.hcalReEmulDigis
        + process.rctReEmulDigis
        + process.gctReEmulDigis
        + process.gtReEmulDigis
        )

process = cms.Process("MinBiasAnalysis")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V43::All', '')

process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(50000)
#    input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/EC98CE5C-AA7F-E211-ABC2-485B39800B98.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/F092B0CE-B27F-E211-9025-00259073E42E.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/F0A16146-AD7F-E211-9672-20CF3056171D.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/EA4084CC-A77F-E211-9CBB-00259073E4EA.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/0274B626-B27F-E211-A7F4-20CF305B04F5.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/0ABA7C26-B27F-E211-B550-00259073E42E.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/2084B5CB-B27F-E211-949F-485B3989724C.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/282B806F-AE7F-E211-9BE1-001EC9D2577D.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/06B7E803-B17F-E211-BE6E-00259073E42E.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/109DFFD4-AB7F-E211-8B89-485B39800C16.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/30FC9227-B07F-E211-ADE0-E0CB4E19F9AF.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/24AB7A8E-B47F-E211-965B-00259073E536.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/3C776A70-AE7F-E211-9B7F-485B39800B86.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/3E86A2D5-B47F-E211-BA0C-00259073E446.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/3EB504FA-B37F-E211-876A-20CF3027A5B9.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/40C957FF-C27F-E211-B1C5-E0CB4E1A1147.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/268515B7-B07F-E211-9676-485B39897256.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/4EC0DB5D-B87F-E211-9ED6-00259073E388.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/50539DCF-A97F-E211-B09A-90E6BA442F3C.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/30C47562-AC7F-E211-8821-90E6BA442F29.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/30C7A422-B57F-E211-9D04-20CF305B058C.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/5E0F32AD-AE7F-E211-B764-BCAEC54B3067.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/345D4294-B17F-E211-8ADC-00259073E42E.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/62A43681-BA7F-E211-B67A-00259073E504.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/64BF3188-B47F-E211-8E46-20CF3027A5B9.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/6C72D7A5-B37F-E211-AFA7-20CF300E9ECD.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/6EAB25F3-AA7F-E211-A75E-20CF3019DEE8.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/44EE5CC8-B77F-E211-A902-00259022277E.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/7E563943-B47F-E211-90A7-00259073E4BE.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/843B1903-B17F-E211-930A-20CF3027A577.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/8677FB9B-B67F-E211-83BD-90E6BA19A213.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/8C47AD1F-B07F-E211-BD39-20CF3056170B.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/520007E1-B17F-E211-8FC4-00259073E4AC.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/ACA67E67-B37F-E211-89C5-00259073E452.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/5A1120DD-B57F-E211-9FB3-20CF3027A631.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/B4F7B617-B37F-E211-88AA-20CF300E9ECD.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/62632879-A87F-E211-9662-20CF305B04F0.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/7AFF9D4B-B17F-E211-99F4-485B39800BA4.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/CEB8565A-AF7F-E211-BD34-E0CB4E4408DE.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/A6793961-AA7F-E211-AC24-E0CB4E19F983.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/B451914E-BB7F-E211-975C-20CF3019DF11.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/C6240ABA-B07F-E211-8D54-002618FDA194.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/C8E9CDA6-AC7F-E211-A479-485B3989723B.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/D2C08EBA-B57F-E211-BED1-20CF305B058C.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/D4CB5E00-B67F-E211-93BD-E0CB4E1A1147.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/E64AE49A-AF7F-E211-B6B2-20CF305B052D.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/D6B9AE91-AD7F-E211-B03E-BCAEC5364CBE.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/DC317A1A-AE7F-E211-9BFE-BCAEC53F6D3A.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/E42C3AEA-B17F-E211-B6DE-20CF3027A5F3.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/E4B6E8B0-AD7F-E211-9121-001EC9D2577D.root',
    '/store/data/Run2012C/MinimumBias/RAW-RECO/25Feb2013-v1/10000/E879E84C-AF7F-E211-875E-20CF305B052D.root')
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '202299.json').getVLuminosityBlockRange()

process.level1Pattern = cms.EDFilter('HLTLevel1Pattern',
                                     L1GtReadoutRecordTag = cms.InputTag('gtDigis'),
                                     triggerBit = cms.string('L1Tech_BPTX_plus_AND_minus.v0'),
                                     daqPartitions = cms.uint32(1),
                                     ignoreL1Mask = cms.bool(True),
                                     invert = cms.bool(False),
                                     throw = cms.bool(True),
                                     bunchCrossings = cms.vint32( -2, -1,  0,  1,  2 ),
                                     triggerPattern = cms.vint32(  0,  0,  1,  0,  1 )
#                                     triggerPattern = cms.vint32(  1,  0,  1,  0,  1 )
#                                     triggerPattern = cms.vint32(  1,  0,  1,  0,  0 )
)

process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.HLTPaths = cms.vstring("HLT_ZeroBias_v*")
#process.hltHighLevel.HLTPaths = cms.vstring("HLT_Physics_v*")
#process.hltHighLevel.HLTPaths = cms.vstring("HLT_L1Tech53_MB_2_v*")
#for LP_MinBias2 datasample

process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
process.load('RecoMET.METFilters.eeBadScFilter_cfi')

process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
)


process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection =
                                           cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24),
                                           maxd0 = cms.double(2)
)

# Unpacking of RAW, only of present 
process.load('SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cff')
process.load('EventFilter.EcalRawToDigi.EcalUnpackerData_cfi')
process.ecalDigis = process.ecalEBunpacker.clone()
process.ecalDigis.InputLabel = cms.InputTag('rawDataCollector')

patchGct(process)

process.load("fabiocos.CMSr1PUAnalysis.EcalMinBiasAnalysis_cfi")
process.load("fabiocos.CMSr1PUAnalysis.HcalMinBiasAnalysis_cfi")
process.load("fabiocos.CMSr1PUAnalysis.CaloTowerAnalysis_cfi")
process.load("fabiocos.CMSr1PUAnalysis.L1CaloAnalysis_cfi")

process.MessageLogger.categories=cms.untracked.vstring('FwkJob'
                                                      ,'FwkReport'
                                                      ,'FwkSummary'
                                                      ,'Root_NoDictionary'
                                                      ,'EcalMinimumBiasInfo'
                                                      ,'HcalMinimumBiasInfo'
)
    

process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.EcalMinimumBiasInfo = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.HcalMinimumBiasInfo = cms.untracked.PSet(limit = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.caloTowerAnalysis.cutEt = cms.bool(True)
process.caloTowerAnalysis.etTh = cms.double(1.)
process.caloTowerAnalysis.etEmTh = cms.double(-999.)
process.caloTowerAnalysis.etHadTh = cms.double(-999.)
process.l1CaloAnalysis.etTh = cms.double(0.)
process.l1CaloAnalysis.cutCTEt = cms.bool(True)
process.l1CaloAnalysis.etCTTh = cms.double(1.)

process.TFileService = cms.Service("TFileService", fileName = cms.string("MinBiasAnalysis_histo.root") )

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#process.p = cms.Path(process.level1Pattern * process.hltHighLevel *
process.p = cms.Path(process.hltHighLevel *
                     process.primaryVertexFilter * process.noscraping *
                     process.HBHENoiseFilter * process.eeBadScFilter *
#                     process.ecalDigis *
                     process.patchGct *
                     process.ecalMinBiasAnalysis * process.hcalMinBiasAnalysis * process.caloTowerAnalysis * process.l1CaloAnalysis )
# in MC I don't include the process.hltHighLevel in the sequence




