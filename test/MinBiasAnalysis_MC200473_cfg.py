import FWCore.ParameterSet.Config as cms

# re-emulation of L1 trigger for run 200473

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
    input = cms.untracked.int32(500000)
#    input = cms.untracked.int32(10000)
#    input = cms.untracked.int32(1000)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/005597C1-57CA-E211-B61D-00261894387A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/005857A9-57CA-E211-85CF-0026189438EB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/00903666-A1CA-E211-B55A-0026189438EB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/009130B5-57CA-E211-8B68-003048679244.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/02B028E6-58CA-E211-8AAF-00261894393D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0427E658-78CA-E211-B090-002618943843.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/044BE259-5CCA-E211-9CEA-0026189438E1.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0665EA8E-8ACA-E211-9EF7-002618943898.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/068DDFCE-73CA-E211-839C-00304867908C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/08306AC5-57CA-E211-AF04-002618943829.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/08AB8A7A-5CCA-E211-ABF6-003048678A6C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/08E68E08-58CA-E211-A9C2-003048FFCBB0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0A9C54DB-57CA-E211-89E8-002618FDA279.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0AA830F3-57CA-E211-AAD5-00304867BFB0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0AD1F0EA-53CA-E211-8460-0030486790A6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0C9F6952-85CA-E211-955F-003048FFCB74.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0CEA810A-60CA-E211-BDDC-002590593872.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0E003CCB-59CA-E211-B947-002618943800.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0E4F4B41-58CA-E211-A08D-002618943957.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0E5962CB-57CA-E211-BD45-002618943945.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0EA44820-59CA-E211-8CDA-002618943970.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0EB7C31A-58CA-E211-B043-0026189438AF.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0EBCDAF2-63CA-E211-BD79-002618FDA287.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/0ED8816F-60CA-E211-8DE3-00261894387C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/10708652-52CA-E211-ABCB-002618943866.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1200EC9E-63CA-E211-89B1-003048FFD752.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/122834B1-59CA-E211-8672-0026189438C4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/12E39304-5ACA-E211-B4A8-002618943935.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/12F3ADA7-5FCA-E211-A2DE-002618943921.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/162DB92D-59CA-E211-A320-00261894391F.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/16455EFE-57CA-E211-A4BE-002618943943.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/16621DE4-59CA-E211-BE39-00261894387B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/18106232-58CA-E211-B540-002618943911.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1AC7D81B-62CA-E211-B0F8-0030486790A0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1C1FD066-C5CA-E211-842C-003048678FF4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1C32C614-5FCA-E211-913F-0026189438A7.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1C4FD538-5ACA-E211-AFDD-0025905938D4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1CF4A3C5-59CA-E211-A9E8-0026189438B8.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1CFEE538-5ECA-E211-9DEA-003048678B8E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1E65C8F9-5BCA-E211-BB60-002618943950.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/1EF5521A-58CA-E211-9ADA-0030486790A0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2205DFAB-58CA-E211-8991-0026189438C0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2241B3BC-7DCA-E211-BB37-003048678E8A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/244B0C9A-6DCA-E211-B771-003048FFD752.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/26C27F48-5ACA-E211-805F-00304867D836.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2810C58C-58CA-E211-BE4F-00261894387E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/285182D0-52CA-E211-BA67-002618943874.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/28D832A5-5ACA-E211-A59D-00304867C1BC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/28DF5B26-59CA-E211-8CDC-00261894390E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/28DF8B37-5ECA-E211-B87A-0026189437EB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/28E24D49-5ACA-E211-9EF8-003048678FE4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2A1CFA11-59CA-E211-9DD2-003048678FEA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2A47231B-5BCA-E211-9A57-003048FFD76E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2AA1476F-60CA-E211-8F12-002618943858.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2AC07B65-58CA-E211-848B-0026189438A9.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2AD7823B-64CA-E211-819C-003048678B08.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C1029D0-57CA-E211-8E02-00261894380A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C14420D-58CA-E211-A88F-0026189437F0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C3F36DC-5ACA-E211-8D86-0026189438A2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C46868D-57CA-E211-9586-00261894394F.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C7D4AAE-59CA-E211-871F-003048678FA0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2C98F749-5ACA-E211-9334-003048678F26.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2E4F51EB-64CA-E211-99AA-00304867BEC0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2E66E9A1-86CA-E211-9667-0030486791F2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2ED2CDE7-77CA-E211-B6A6-0025905938B4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/2EFEF088-A5CA-E211-A10C-00261894387C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3042496B-7FCA-E211-89CC-002590593872.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3070C433-86CA-E211-8075-002618943961.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/30EFDF67-67CA-E211-80DA-003048FFD756.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/32A1ED34-60CA-E211-B3D8-0026189438C0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/32A70C0C-78CA-E211-9EE5-0026189437F0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/32A78CA7-57CA-E211-B303-002618943834.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/346427F8-57CA-E211-9D2F-00261894398A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3635788D-67CA-E211-A71B-00259059642E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3661C14E-6DCA-E211-84D0-003048FFCBFC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/36922612-8CCA-E211-AD03-0026189438D3.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/36CE89F5-59CA-E211-9451-0025905938B4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/385972FF-57CA-E211-903A-0026189438A2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3896A171-60CA-E211-8681-0026189437F0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/38E455FF-57CA-E211-ABD8-002618943919.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3C34FB4A-77CA-E211-A3F2-0025905964A6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3CEB9159-7FCA-E211-8B9D-0025905964A6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/3CFCBD4C-7CCA-E211-B13A-0026189438E1.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/40E31FF7-57CA-E211-A77D-002590596498.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/40E8FE12-58CA-E211-8009-002618943880.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/42046B2D-B8CA-E211-BA52-0025905964BA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/42E416CE-59CA-E211-8D4B-00261894396F.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4460638B-59CA-E211-BCBA-002618943811.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44688CD0-74CA-E211-AC79-0025905964BC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44A28FAB-57CA-E211-B3E5-002618943916.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44CB0E62-58CA-E211-8FF3-002618943899.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44DD6797-58CA-E211-B3A6-002618943868.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44E4CD99-57CA-E211-84AA-003048FFD744.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44F13D33-5ACA-E211-8099-0030486792B4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/44F98852-60CA-E211-936A-003048FFD760.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/460406E0-57CA-E211-A98A-003048678BEA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/46333AD9-5ACA-E211-AF16-0026189437ED.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4686FDDC-63CA-E211-AC52-002618FDA26D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/46D827FA-5BCA-E211-A9F3-00248C0BE014.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/46E9C547-5ACA-E211-A307-002618943940.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/48226F61-57CA-E211-BDEA-00261894385D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/48B4D7AB-67CA-E211-A2C8-002618943920.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/48D40C00-55CA-E211-9E9D-00261894392B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4A4CA2B1-57CA-E211-A0CE-00261894392C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4A4CE2DB-65CA-E211-9033-00304867D836.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4AE1BD94-82CA-E211-B53C-003048FFD754.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4C1F80ED-59CA-E211-AF42-003048FFD732.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4C2D0E16-59CA-E211-961C-003048678E6E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4C594B54-98CA-E211-A376-0030486790C0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4EA1A863-B4CA-E211-B4BB-0026189438BF.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4EAD3D63-5FCA-E211-B7C9-00261894386C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/4ED2A7F9-57CA-E211-A90D-003048678AE4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/50467401-72CA-E211-A875-00261894392D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/50787D04-5CCA-E211-8C59-003048678F74.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/521D321E-64CA-E211-9300-002618943919.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/525A064E-5ACA-E211-8464-0030486792F0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/526A90F7-57CA-E211-BCD6-003048FFCB84.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/527A52EB-91CA-E211-8AD1-00261894386B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5693A316-58CA-E211-8871-0026189438E2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/58F7F340-59CA-E211-94D7-002618FDA28E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5A097808-59CA-E211-AC7F-003048678BEA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5A0D4C70-82CA-E211-BC70-00304867924A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5ABC725B-76CA-E211-980A-002590593872.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5ADBD56C-6ACA-E211-8946-003048FFCC0A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5AE2EC0A-59CA-E211-95E2-002618943976.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5C787A87-70CA-E211-B601-0026189438C2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5CB2E6DE-8ACB-E211-BDAD-0025905964A2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5CE23DA2-5FCA-E211-A090-002618943894.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5E1B139F-A2CA-E211-9C21-0026189438FE.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5E786E01-D3CA-E211-A2CF-003048679296.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/5EC31EBE-89CA-E211-A6EF-00261894389C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6034D022-5BCA-E211-A255-00261894390A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/66800D62-5ACA-E211-9829-003048678B92.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/66C48BFC-57CA-E211-B343-002590596498.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/66FD9F30-59CA-E211-B569-002618943866.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/680A86E6-58CA-E211-942A-002618FDA216.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6879742E-72CA-E211-937D-002618943857.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/68837570-70CA-E211-9DE3-0026189437EB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/68FB5B32-5ACA-E211-A3D5-003048FFD736.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6A091EBB-59CA-E211-9563-002618943980.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6A3B6E25-6DCA-E211-976A-0030486791DC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6CD66605-5CCA-E211-8E2D-003048678B18.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6E2DBE06-8CCA-E211-BCA5-0026189438D4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6E5D72A7-93CA-E211-B863-00248C0BE005.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/6EB6CFB2-59CA-E211-A929-0026189438BC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/702C514A-73CA-E211-A858-002618943963.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7054C226-94CA-E211-8AE3-00261894383E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/70CF1681-B5CA-E211-9D48-0030486791F2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/70FC3AC9-58CA-E211-BC4E-0026189438DF.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/723AA7A3-59CA-E211-812A-0026189437F8.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/72B3B2EB-64CA-E211-8FE1-003048FFD752.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/744F7EBF-59CA-E211-B04A-00304867BFB0.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/749B6958-52CA-E211-8D2F-002618FDA265.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/74BFEAD2-5ACA-E211-A9FA-003048D15DB6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/74E4DF4B-59CA-E211-892A-003048FFD7C2.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/74F83879-64CA-E211-8604-002618FDA287.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/76925572-58CA-E211-8396-0026189438B3.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/76B63CAE-61CA-E211-BBDF-003048D3C010.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/76F5FBCD-57CA-E211-A155-00261894396D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/78100515-59CA-E211-93F5-002618943977.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/789E9AFE-5FCA-E211-B41D-00261894398C.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/789F7C8D-57CA-E211-86FA-002618943983.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7A3BA841-5CCA-E211-AC34-003048FFCBA8.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7A640B35-5ACA-E211-BD2B-002618943933.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7C5FE6EB-7ACA-E211-8316-0026189437F8.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7CADC53A-5ECA-E211-872D-00261894388D.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/7CD1ABDC-57CA-E211-B8A5-00304867903E.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/801396C8-59CA-E211-A09C-0025905964B4.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/803742F0-7ACA-E211-96B0-0026189438DB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/803D8785-5ECA-E211-85FB-003048678B08.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/807F26BD-59CA-E211-9B53-002618943926.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/84832321-5ACA-E211-A8DF-002618943869.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/84D8D5A3-57CA-E211-9059-003048678E24.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/84F05D33-5ACA-E211-9BCD-003048D15DB6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/8606DC8E-59CA-E211-A9FE-003048678B84.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/881656FE-58CA-E211-8172-0026189438AE.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/88825811-78CA-E211-8450-002618943962.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/88A66E4B-59CA-E211-B0DC-002618943843.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/8A1EEDF5-79CA-E211-89F2-003048FFCB74.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/8A5B7A65-6DCA-E211-8249-002618943885.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/8AE8BA9B-78CA-E211-A4C6-002618943976.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/8E42AFDD-89CA-E211-B38C-00261894388B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/90649CB8-52CA-E211-B6FE-00261894389F.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/90738A25-61CA-E211-85C2-002618943976.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/90AE2140-ABCA-E211-A255-0026189438BC.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/921297F4-57CA-E211-A377-003048678D86.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/9217403D-5CCA-E211-84DD-002618943913.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/9278E96D-58CA-E211-95AE-0026189438AB.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/9286A35A-58CA-E211-8DFF-0026189438BA.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/94C0819A-58CA-E211-A2A7-00261894380A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/94E24301-59CA-E211-88B9-002618943898.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/962FD8F4-59CA-E211-BD07-003048FFD732.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/966C0A51-5CCA-E211-841A-002618943821.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/968B17E7-58CA-E211-A609-00261894390B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/96A7524A-99CA-E211-82C9-002618943937.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/96DD5843-5ACA-E211-B8A5-00261894398B.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/96E101A3-61CA-E211-B619-00304867BFC6.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/96EBCAE5-5FCA-E211-A6AA-00261894398A.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/982CE7E7-58CA-E211-B3A6-003048678B12.root',
        '/store/mc/Summer12/Neutrino_Pt_2to20_gun/RAWRECOSIMHLT/ShoLibRD202299_PU50nsExtM300nsTP_START53_V7N-v1/20000/98B7360C-5ACA-E211-972A-0026189438A9.root')
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
        dataPUFile = cms.untracked.string("200473_truePileUpHisto.root"),
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
        dataPUFile = cms.untracked.string("200473_truePileUpHisto.root"),
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
        dataPUFile = cms.untracked.string("200473_truePileUpHisto.root"),
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
        dataPUFile = cms.untracked.string("200473_truePileUpHisto.root"),
        mcPUFile = cms.untracked.string("monitorPUSummaryInfo_histo.root"),
        dataPUHisto = cms.untracked.string("pileup"),
        mcPUHisto = cms.untracked.string("monitorPUSummaryInfo/nTruePU")
    )
)

from fabiocos.CMSr1PUAnalysis.EcalTPAnalysis_cfi import stdEcalTPPset

process.ecalTPAnalysis = cms.EDAnalyzer("EcalTPAnalysis",
    stdEcalTPPset,
    PUrew = cms.PSet(
        puSummaryCollection = cms.untracked.InputTag("addPileupInfo","",""),
        dataPUFile = cms.untracked.string("200473_truePileUpHisto.root"),
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

process.caloTowerAnalysis.cutEt = cms.bool(False)
process.caloTowerAnalysis.etTh = cms.double(1.)
process.caloTowerAnalysis.etEmTh = cms.double(-999.)
process.caloTowerAnalysis.etHadTh = cms.double(-999.)
process.l1CaloAnalysis.etTh = cms.double(0.)
process.l1CaloAnalysis.cutCTEt = cms.bool(False)
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
                     process.ecalDigis *
                     process.patchGct *
                     process.ecalTPAnalysis * 
                     process.ecalMinBiasAnalysis * process.hcalMinBiasAnalysis * process.caloTowerAnalysis * process.l1CaloAnalysis)
# in MC I don't include the process.hltHighLevel in the sequence

