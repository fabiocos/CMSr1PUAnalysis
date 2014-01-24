import FWCore.ParameterSet.Config as cms

# re-emulation of L1 trigger for run 200473

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
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V43::All', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_53_V21::All', '')

process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(50000)
#    input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/02174E40-CC10-E311-91DB-E0CB4E5536D5.root' 
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/06FA1BA1-CF10-E311-B680-20CF3027A60B.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/0A029E04-CA10-E311-B08B-E0CB4E19F965.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/0AE3488B-D010-E311-8085-001EC9D26F89.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/12D0E56E-C910-E311-83B6-20CF30561701.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/141F5388-CE10-E311-A849-00259074AE40.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/14D1E564-CB10-E311-8E3E-485B39800B93.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/184AEB1E-CB10-E311-8ED9-485B39800BBE.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/1A093578-D010-E311-83F4-E0CB4E29C4F5.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/28512EFB-CB10-E311-90F9-001EC9D8D993.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/328F08E4-CF10-E311-A214-90E6BAE8CC21.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/381391F4-D410-E311-BA5F-00259074AEC2.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/3835C692-D510-E311-B5F7-00259073E4D6.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/3C981A95-D110-E311-A47E-00259073E30E.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/40BBAD7B-C710-E311-8DD0-485B39800BD2.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/44C4B32B-D210-E311-981D-001EC9D8B15D.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/46892CE1-C810-E311-8154-20CF30561701.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/46A256C3-D010-E311-A546-E0CB4EA0A929.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/46A7ADDB-CA10-E311-A905-485B39800BBB.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/4C0D8C32-D410-E311-AE2F-002590747E1C.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/5603E5DB-CC10-E311-BADC-001EC9D7FA18.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/56669D4D-CC10-E311-887C-485B39800B93.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/583D2842-CE10-E311-AFA4-00259073E3D4.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/5C9D1D40-CC10-E311-9E99-90E6BA0D0988.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/60BF3287-CC10-E311-9E28-20CF300E9EBD.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/62E29158-D110-E311-8844-00259073E38A.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/6479731C-CF10-E311-9169-00259073E3D4.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/66A8D6D4-D210-E311-B371-001EC9D8B15D.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/6A039BCB-D610-E311-BEED-0025907750A0.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/70F7AD33-D010-E311-8115-002590747DD8.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/72F8A6E2-CF10-E311-8923-001EC9D26F89.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/7873D8EB-CD10-E311-A56A-20CF3027A5B0.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/78B4946C-CD10-E311-B9D8-20CF300E9EB4.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/7A9DB568-CF10-E311-BD6D-00261834B586.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/7ABB4FBB-C910-E311-A04A-485B39800B65.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/7E79E362-D110-E311-B083-20CF3027A5B9.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/803D8F20-CF10-E311-9E64-00259073E442.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/80CAE9FA-CB10-E311-BFA0-E0CB4E19F983.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8419FC38-D810-E311-A8A1-00259073E42E.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8803E707-CC10-E311-BC7B-BCAEC53F6D57.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8AADE856-CF10-E311-AA70-BCAEC50971F9.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8E270C8D-CA10-E311-A4F9-E0CB4E1A114E.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8EBAF2C1-D010-E311-930D-90E6BAE8CC21.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/8EFEE461-CD10-E311-AFFE-485B39800BBB.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/90A40394-CA10-E311-A2C6-00259073E3D4.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/944844DD-CA10-E311-B73E-20CF3019DF0F.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/94CC3718-CD10-E311-B1F4-485B39800BB0.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/9C13C46B-CB10-E311-9E0A-90E6BA19A252.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/9C9D50AA-CB10-E311-85C4-485B39800C32.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/A0C0D334-CE10-E311-9A8E-E0CB4E19F9B8.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/A4F99F24-CB10-E311-8910-485B39800C2D.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/A688F19D-D310-E311-BFB6-002590747E1C.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/A6897EF2-CD10-E311-8313-00261834B5B1.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/B0CBA848-CA10-E311-8A88-485B39800C32.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/B61CAF4D-CC10-E311-B0CE-20CF3027A5DC.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/BAD837CB-CE10-E311-9EE8-90E6BA0D0987.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/BE530284-CC10-E311-BE0A-485B39800BC7.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/C0739006-D110-E311-AFB3-001EC9D8B15D.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/C0D08BF9-CB10-E311-9FCB-485B39800BC7.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/C4CFCB0C-2011-E311-91E5-485B39800BD5.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/CC22EBB4-CB10-E311-B189-485B39800C37.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D0E5670C-CC10-E311-83E0-20CF300E9EB4.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D44B3B4E-CA10-E311-85F5-E0CB4E1A1192.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D60C6170-C910-E311-9675-485B39800BC8.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D6E9EBD2-CC10-E311-A67E-BCAEC54B302A.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D6ED8187-CE10-E311-8D33-001EC9D83165.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D8129FA3-CD10-E311-BD2A-485B39800BB0.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/D84B99A8-D110-E311-9925-001EC9D8B15D.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/DAA9C687-CE10-E311-9F57-00259073E4CA.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/DC6D11D8-CC10-E311-AFBE-20CF3027A5DC.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/E0941A12-C810-E311-84FC-20CF3027A628.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/E0D3E318-CD10-E311-94E1-00259073E526.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/E889D3B2-CB10-E311-8000-20CF305B058C.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/E8EF78A3-CF10-E311-8A00-002590747DD8.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/ECA7554E-C810-E311-8989-E0CB4E1A11A1.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/ECD9AA6E-CB10-E311-9B52-20CF30561724.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/F429F0FF-C910-E311-B194-00259073E532.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/F64415C8-CE10-E311-A013-E0CB4EA0A91E.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/FC1A8FB1-CB10-E311-BAE6-20CF305B0572.root'
    ,'/store/data/Run2012C/MinimumBias/RAW-RECO/28Aug2013-v1/20000/FEDA2128-C910-E311-ACEC-001EC9D81D4C.root')
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '200473.json').getVLuminosityBlockRange()

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

process.load("fabiocos.CMSr1PUAnalysis.EcalTPAnalysis_cfi")

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

process.caloTowerAnalysis.cutEt = cms.bool(False)
process.caloTowerAnalysis.etTh = cms.double(1.)
process.caloTowerAnalysis.etEmTh = cms.double(-999.)
process.caloTowerAnalysis.etHadTh = cms.double(-999.)
process.l1CaloAnalysis.etTh = cms.double(0.)
process.l1CaloAnalysis.cutCTEt = cms.bool(False)
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
                     process.ecalTPAnalysis * 
                     process.ecalMinBiasAnalysis * process.hcalMinBiasAnalysis * process.caloTowerAnalysis * process.l1CaloAnalysis )
# in MC I don't include the process.hltHighLevel in the sequence




