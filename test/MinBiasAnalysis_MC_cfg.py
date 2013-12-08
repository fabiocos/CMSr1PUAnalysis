import FWCore.ParameterSet.Config as cms

# re-emulation of L1 trigger for run 202299

def patchGct(process) :

    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("L1GtTriggerMenuRcd"),
                 tag = cms.string("L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 ),
        cms.PSet(record = cms.string("L1GctChannelMaskRcd"),
                 tag = cms.string("L1GctChannelMask_AllEnergySumsMaskedFromHF_jetCentresToEta3Allowed_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 ),
        cms.PSet(record = cms.string("L1GctJetFinderParamsRcd"),
                 tag = cms.string("L1GctJetFinderParams_GCTPhysics_2012_04_27_JetSeedThresh5GeV_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 ),
        cms.PSet(record = cms.string("L1HfRingEtScaleRcd"),
                 tag = cms.string("L1HfRingEtScale_GCTPhysics_2012_04_27_JetSeedThresh5GeV_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 ),
        cms.PSet(record = cms.string("L1JetEtScaleRcd"),
                 tag = cms.string("L1JetEtScale_GCTPhysics_2012_04_27_JetSeedThresh5GeV_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 ),
        cms.PSet(record = cms.string("L1HtMissScaleRcd"),
                 tag = cms.string("L1HtMissScale_GCTPhysics_2012_04_27_JetSeedThresh5GeV_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_L1T")
                 )
        )

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

    # Starting from RAW (just suppressed digis)
    
#    process.hcalReEmulDigis.inputLabel = cms.VInputTag(cms.InputTag('hcalDigis'), cms.InputTag('hcalDigis'))

#    process.rctReEmulDigis.ecalDigis = cms.VInputTag( cms.InputTag( 'ecalDigis:EcalTriggerPrimitives' ) )
#    process.rctReEmulDigis.hcalDigis = cms.VInputTag( cms.InputTag( 'hcalReEmulDigis' ) )

# original from simulation ( if saved)
    
    process.rctReEmulDigis.ecalDigis = cms.VInputTag( cms.InputTag( 'simEcalTriggerPrimitiveDigis' ) )
    process.rctReEmulDigis.hcalDigis = cms.VInputTag( cms.InputTag( 'simHcalTriggerPrimitiveDigis' ) )

    process.gctReEmulDigis.inputLabel  = cms.InputTag("rctReEmulDigis")
    
    process.gtReEmulDigis.GmtInputTag  = cms.InputTag("gtDigis")
    process.gtReEmulDigis.GctInputTag  = cms.InputTag("gctReEmulDigis")

#     process.patchGct = cms.Sequence(
#         process.ecalDigis
#         + process.hcalDigis
#         + process.hcalReEmulDigis
#         + process.rctReEmulDigis
#         + process.gctReEmulDigis
#         + process.gtReEmulDigis
#         )

    process.patchGct = cms.Sequence(
        process.rctReEmulDigis
        + process.gctReEmulDigis
        + process.gtReEmulDigis
        )

process = cms.Process("MinBiasAnalysis")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V15A::All', '')

process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(200000)
    input = cms.untracked.int32(1000)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F0D7D604-0F87-E211-8E03-001E67397D73.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/EE03F268-F786-E211-AB87-001E67396DBA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F220EBA9-F686-E211-A56F-001E67397C33.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/EE2C6559-F786-E211-A789-003048D476AC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F040D46A-FC86-E211-9F36-003048D47704.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F05915E6-0887-E211-B3C0-001E67397D7D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F21A453C-F786-E211-BBE6-001E673967C5.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F236AF66-0587-E211-8E9F-001E67398D5E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F85F9312-0A87-E211-ADB5-002481E1501E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F2A7968F-1087-E211-8849-003048D47A0E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F44172EF-0B87-E211-92AD-001E6739730A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F45A0DE6-FC86-E211-92EB-003048D477B8.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FC401FE8-0987-E211-9C48-003048D476EE.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FC457186-F686-E211-8775-001E673982E6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FC5019C8-1A87-E211-92AF-002481E94262.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F4813491-0887-E211-8B46-0025B3E0651C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/F8EAA014-0187-E211-921B-001E67397747.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FA7C0330-1287-E211-BE56-003048D47A7E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FC18A8EF-0F87-E211-BA44-001E67397F35.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/FED05754-0087-E211-956D-001E6739665D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/ECB1E26B-FD86-E211-93F0-001E6739815B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/EC873F75-F886-E211-A35E-001E673970C1.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/EC4266D0-0687-E211-B1C4-002481E14E56.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/EC35C773-F686-E211-8A63-0025B3E05C2C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E84E5431-F686-E211-8E77-0025B3E05D82.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E84CB50E-0087-E211-82F7-001E67397035.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E6F9B506-1187-E211-ABE5-001E67398C1E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E4BE0CAD-F686-E211-8FFC-003048D476B4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E4B8B280-0F87-E211-B65E-003048D47A7C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E47ADDEC-FC86-E211-A77E-001E67396EAA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E45228EE-F686-E211-9E98-003048D45F50.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E42FB37D-0987-E211-8A8B-0025B3E063BA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E2E5A9F3-F786-E211-8969-001E67398D72.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E2BD00E3-F786-E211-8A1B-003048D4603E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E28AA076-0B87-E211-AF3A-002590200A30.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E2559035-F686-E211-8D11-003048673FFC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E2176B25-1487-E211-9C5E-003048D46008.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E0F43B9A-1B87-E211-9B4D-0025B3E0654E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/E0C65A01-0F87-E211-8450-003048D46018.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DEA3C06A-0587-E211-8ED2-001E67398C37.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DE703DAD-0087-E211-BC89-001E67396568.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DE02F1D5-F686-E211-AC04-001E6739672F.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DCC58C12-1287-E211-82AE-001E67398E6C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DC937ADE-F886-E211-BE8F-003048D479E4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DC23DC2C-0987-E211-9230-0025B3E06658.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DAE87B74-0987-E211-A2C3-003048674016.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/DA013DFA-0D87-E211-8ADC-003048D4773E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/D8E9B999-0B87-E211-9754-001E67398633.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/D851242D-0A87-E211-98AC-001E67397391.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/PU50nsExtM300ns_START53_V15A-v1/10000/D835F2A4-0387-E211-9166-001E67397E54.root')
)

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

from fabiocos.CMSr1PUAnalysis.EcalMinBiasAnalysis_cfi import stdEcalAnaPset

process.ecalMinBiasAnalysis = cms.EDAnalyzer("EcalMinBiasAnalysis",
    stdEcalAnaPset,
    PUrew = cms.PSet(
        puSummaryCollection = cms.untracked.InputTag("addPileupInfo","",""),
        dataPUFile = cms.untracked.string("202299_truePileUpHisto.root"),
        mcPUFile = cms.untracked.string("monitorPUSummaryInfo_histo.root"),
        dataPUHisto = cms.untracked.string("pileup"),
        mcPUHisto = cms.untracked.string("monitorPUSummaryInfo/nTruePU")
    )
)

from fabiocos.CMSr1PUAnalysis.HcalMinBiasAnalysis_cfi import stdHcalAnaPset

process.hcalMinBiasAnalysis = cms.EDAnalyzer("HcalMinBiasAnalysis",
    stdHcalAnaPset,
    PUrew = cms.PSet(
        puSummaryCollection = cms.untracked.InputTag("addPileupInfo","",""),
        dataPUFile = cms.untracked.string("202299_truePileUpHisto.root"),
        mcPUFile = cms.untracked.string("monitorPUSummaryInfo_histo.root"),
        dataPUHisto = cms.untracked.string("pileup"),
        mcPUHisto = cms.untracked.string("monitorPUSummaryInfo/nTruePU")
    )
)

from fabiocos.CMSr1PUAnalysis.CaloTowerAnalysis_cfi import stdCTAnaPset

process.caloTowerAnalysis = cms.EDAnalyzer("CaloTowerAnalysis",
    stdCTAnaPset,
    PUrew = cms.PSet(
        puSummaryCollection = cms.untracked.InputTag("addPileupInfo","",""),
        dataPUFile = cms.untracked.string("202299_truePileUpHisto.root"),
        mcPUFile = cms.untracked.string("monitorPUSummaryInfo_histo.root"),
        dataPUHisto = cms.untracked.string("pileup"),
        mcPUHisto = cms.untracked.string("monitorPUSummaryInfo/nTruePU")
    )
)

from fabiocos.CMSr1PUAnalysis.L1CaloAnalysis_cfi import stdl1CaloPset

process.l1CaloAnalysis = cms.EDAnalyzer("L1CaloAnalysis",
    stdl1CaloPset,
    PUrew = cms.PSet(
        puSummaryCollection = cms.untracked.InputTag("addPileupInfo","",""),
        dataPUFile = cms.untracked.string("202299_truePileUpHisto.root"),
        mcPUFile = cms.untracked.string("monitorPUSummaryInfo_histo.root"),
        dataPUHisto = cms.untracked.string("pileup"),
        mcPUHisto = cms.untracked.string("monitorPUSummaryInfo/nTruePU")
    )
)



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

process.TFileService = cms.Service("TFileService", fileName = cms.string("MinBiasAnalysis_MC_histo.root") )

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)


#process.p = cms.Path(process.level1Pattern * process.hltHighLevel *
#process.p = cms.Path(process.hltHighLevel *
process.p = cms.Path(
                     process.primaryVertexFilter * process.noscraping *
                     process.HBHENoiseFilter * process.eeBadScFilter *
#                     process.ecalDigis *
                     process.patchGct *
                     process.ecalMinBiasAnalysis * process.hcalMinBiasAnalysis * process.caloTowerAnalysis * process.l1CaloAnalysis)
# in MC I don't include the process.hltHighLevel in the sequence

