/* 
 * class EcalTPAnalysis
 *  
 *  Class to study ECAL Trigger Primitives response 
 *
 */

// framework & common header files
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "CalibCalorimetry/EcalTPGTools/interface/EcalTPGScale.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/Common/interface/SortedCollection.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

class TTower
{

public:

  typedef EcalTrigTowerDetId key_type;

  TTower( const EcalTrigTowerDetId myId, std::pair<float,float> myValue ):
    theId_(myId),theValue_(myValue)
  {}

  virtual ~TTower(){};

  EcalTrigTowerDetId id() { return theId_; }
  std::pair<float,float> value() { return theValue_; }

private:

  EcalTrigTowerDetId theId_;
  std::pair<float,float> theValue_;

}; 


class EcalTPAnalysis : public edm::EDAnalyzer
{

public:
  explicit EcalTPAnalysis(const edm::ParameterSet&);
  virtual ~EcalTPAnalysis();
  virtual void beginJob();
  virtual void endJob();  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);

  const EcalRecHit * giveGoodHit( const DetId & theId ) const;  
  
private:
  
  edm::InputTag recoVertexCollection_;
  edm::InputTag ebRecHitCollection_; 
  edm::InputTag eeRecHitCollection_; 
  edm::InputTag tpCollection_;

  double tpEtTh_;
  double rhTEtTh_;

  int vtxSel_;
  
  double effTh_;

  const int numvtx;
  const int nrankTh_;
  const float minRankTh_;

  edm::LumiReWeighting * theLumiW_;
  edm::InputTag puSummaryCollection_;
  std::string dataPUFile,mcPUFile,dataPUHisto,mcPUHisto;

  bool rewe_;

  TH1F * vtxHisto_;

  TH1F * truePUorigHisto_;
  TH1F * truePUreweHisto_;
  TH1F * weightHisto_;

  //  std::vector< std::pair<float,float> > ebTowers;
  //  std::vector <std::pair<float,float> > eeTowers;

  TH2F * EBtpVSrh_;
  TH2F * EEtpVSrh_;

  TProfile * EBdiffVSrh_; 
  TProfile * EEdiffVSrh_; 

  TProfile * EBdiffVSrhM_; 
  TProfile * EEdiffVSrhM_; 

  TH1F * EBrhEtNOtp_;
  TH1F * EErhEtNOtp_;

  TH1F * EBrh_;
  TH1F * EBtp_;
  TH1F * EErh_;
  TH1F * EEtp_;
  TH1F * EBrhM_;
  TH1F * EErhM_;

  TH1F * EBrhEt_;
  TH1F * EBtpEt_;
  TH1F * EErhEt_;
  TH1F * EEtpEt_;

  TH1F * EBrhEtM_;
  TH1F * EBtpEtM_;
  TH1F * EErhEtM_;
  TH1F * EEtpEtM_;

  TH1F * EBrhEtSum_;
  TH1F * EBtpEtSum_;
  TH1F * EErhEtSum_;
  TH1F * EEtpEtSum_;

  TH1F * EBrhEtSumM_;
  TH1F * EBtpEtSumM_;
  TH1F * EErhEtSumM_;
  TH1F * EEtpEtSumM_;

  TProfile * EBrhEtSumVSvtx_;
  TProfile * EBtpEtSumVSvtx_;
  TProfile * EErhEtSumVSvtx_;
  TProfile * EEtpEtSumVSvtx_;

  TProfile * EBrhEtSumMVSvtx_;
  TProfile * EBtpEtSumMVSvtx_;
  TProfile * EErhEtSumMVSvtx_;
  TProfile * EEtpEtSumMVSvtx_;

  std::vector<double> Wvtx;
  std::vector<double> rhOWvtx;
  std::vector<double> tpOWvtx;
  std::vector<double> rhOMWvtx;
  std::vector<double> tpOMWvtx;

  TH1F * rhSum_;
  TH1F * tpSum_;
  TH1F * rhSumM_;
  TH1F * tpSumM_;

  TProfile * rhSumVSvtx_;
  TProfile * tpSumVSvtx_;
  TProfile * rhSumMVSvtx_;
  TProfile * tpSumMVSvtx_;

  TH1F * rhSumOthEff_;
  TH1F * tpSumOthEff_;
  TH1F * rhSumOthMEff_;
  TH1F * tpSumOthMEff_;

  TProfile * EBMatchEffVSvtx_;
  TProfile * EEMatchEffVSvtx_;

  TH1F * EBhitEtM_;
  TH1F * EEhitEtM_;
  TH1F * EBhitEtnoM_;
  TH1F * EEhitEtnoM_;

  TH1F * EBhitTimeM_;
  TH1F * EEhitTimeM_;
  TH1F * EBhitTimenoM_;
  TH1F * EEhitTimenoM_;

  TH1F * rhRank_;
  TH1F * rhEff_;

  TH1F * tpRank_;
  TH1F * tpEff_;

  TH1F * normaRank_;

  const EBRecHitCollection * EBRecHit; 
  const EERecHitCollection * EERecHit; 
  const EcalChannelStatus* cstat;
  const CaloSubdetectorGeometry *theEndcapGeometry,*theBarrelGeometry;
  const EcalElectronicsMapping* theMapping_;

};

using namespace edm;

EcalTPAnalysis::EcalTPAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  ebRecHitCollection_(iPSet.getParameter<edm::InputTag>("ebRecHitCollection")),
  eeRecHitCollection_(iPSet.getParameter<edm::InputTag>("eeRecHitCollection")),
  tpCollection_(iPSet.getParameter<edm::InputTag>("tpCollection")),
  tpEtTh_(iPSet.getParameter<double>("tpEtTh")),
  rhTEtTh_(iPSet.getParameter<double>("rhTEtTh")),
  vtxSel_(iPSet.getParameter<int>("vtxSel")),
  effTh_(iPSet.getParameter<double>("effTh")),
  numvtx(60),nrankTh_(100),minRankTh_(0.)
{    

  edm::Service<TFileService> fs;

  vtxHisto_ = fs->make<TH1F>( "vtx", "vertex ndof.gt.4 multiplicity", 100, 0., 100. ) ;

  // Luminosity reweighting for simulation

  truePUorigHisto_ = fs->make<TH1F>( "truePUorig", "True PU distribution original", 100, 0., 100. );
  truePUreweHisto_ = fs->make<TH1F>( "truePUrewe", "True PU distribution reweighted", 100, 0., 100. );
  weightHisto_ = fs->make<TH1F>( "weight", "PU weight distribution", 100, 0., 10. );

  rewe_ = false;
  if ( iPSet.exists( "PUrew" ) ) {
    puSummaryCollection_ = (iPSet.getParameter<edm::ParameterSet>("PUrew")).getUntrackedParameter<edm::InputTag>("puSummaryCollection");
    dataPUFile = (iPSet.getParameter<edm::ParameterSet>("PUrew")).getUntrackedParameter<std::string>("dataPUFile");
    mcPUFile = (iPSet.getParameter<edm::ParameterSet>("PUrew")).getUntrackedParameter<std::string>("mcPUFile");
    dataPUHisto = (iPSet.getParameter<edm::ParameterSet>("PUrew")).getUntrackedParameter<std::string>("dataPUHisto");
    mcPUHisto = (iPSet.getParameter<edm::ParameterSet>("PUrew")).getUntrackedParameter<std::string>("mcPUHisto");
    theLumiW_ = new LumiReWeighting(mcPUFile,dataPUFile,mcPUHisto,dataPUHisto);
    rewe_ = true;
  }

  EBtpVSrh_ = fs->make<TH2F>( "EBrhVStt", "EB RH tower vs RH Et", 40,0.,20.,40,0.,20.);
  EEtpVSrh_ = fs->make<TH2F>( "EErhVStt", "EE RH tower vs RH Et", 40,0.,20.,40,0.,20.);

  EBdiffVSrh_ = fs->make<TProfile>( "EBdiffVSrh", "EB TP-RH tower vs RH Et", 40,0.,20.,-20.,20.);
  EEdiffVSrh_ = fs->make<TProfile>( "EEdiffVSrh", "EE TP-RH tower vs RH Et", 40,0.,20.,-20.,20.);

  EBdiffVSrhM_ = fs->make<TProfile>( "EBdiffVSrhM", "EB TP-RH tower vs RH Et matched", 40,0.,20.,-20.,20.);
  EEdiffVSrhM_ = fs->make<TProfile>( "EEdiffVSrhM", "EE TP-RH tower vs RH Et matched", 40,0.,20.,-20.,20.);

  EBrhEtNOtp_ = fs->make<TH1F>( "EBrhEtNOtp", "EB RH tower Et tp=0", 50,0.,5.);
  EErhEtNOtp_ = fs->make<TH1F>( "EErhEtNOtp", "EE RH tower Et tp=0", 50,0.,5.);

  EBrh_ = fs->make<TH1F>( "EBrh", "EB RH tower multiplicity", 100,0.,100.);
  EBtp_ = fs->make<TH1F>( "EBtp", "EB TP tower multiplicity", 100,0.,100.);
  EErh_ = fs->make<TH1F>( "EErh", "EE RH tower multiplicity", 100,0.,100.);
  EEtp_ = fs->make<TH1F>( "EEtp", "EE TP tower multiplicity", 100,0.,100.);
  EBrhM_ = fs->make<TH1F>( "EBrhM", "EB RH tower multiplicity matched", 100,0.,100.);
  EErhM_ = fs->make<TH1F>( "EErhM", "EE RH tower multiplicity matched", 100,0.,100.);

  EBrhEt_ = fs->make<TH1F>( "EBrhEt", "EB RH tower Et", 100,0.,50.);
  EBtpEt_ = fs->make<TH1F>( "EBtpEt", "EB TP tower Et", 100,0.,50.);
  EErhEt_ = fs->make<TH1F>( "EErhEt", "EE RH tower Et", 100,0.,50.);
  EEtpEt_ = fs->make<TH1F>( "EEtpEt", "EE TP tower Et", 100,0.,50.);

  EBrhEtM_ = fs->make<TH1F>( "EBrhEtM", "EB RH tower Et matched", 100,0.,50.);
  EBtpEtM_ = fs->make<TH1F>( "EBtpEtM", "EB TP tower Et matched", 100,0.,50.);
  EErhEtM_ = fs->make<TH1F>( "EErhEtM", "EE RH tower Et matched", 100,0.,50.);
  EEtpEtM_ = fs->make<TH1F>( "EEtpEtM", "EE TP tower Et matched", 100,0.,50.);

  EBrhEtSum_ = fs->make<TH1F>( "EBrhEtSum", "EB RH tower EtSum", 100,0.,100.);
  EBtpEtSum_ = fs->make<TH1F>( "EBtpEtSum", "EB TP tower EtSum", 100,0.,100.);
  EErhEtSum_ = fs->make<TH1F>( "EErhEtSum", "EE RH tower EtSum", 100,0.,100.);
  EEtpEtSum_ = fs->make<TH1F>( "EEtpEtSum", "EE TP tower EtSum", 100,0.,100.);

  EBrhEtSumM_ = fs->make<TH1F>( "EBrhEtSumM", "EB RH tower EtSum matched", 100,0.,100.);
  EBtpEtSumM_ = fs->make<TH1F>( "EBtpEtSumM", "EB TP tower EtSum matched", 100,0.,100.);
  EErhEtSumM_ = fs->make<TH1F>( "EErhEtSumM", "EE RH tower EtSum matched", 100,0.,100.);
  EEtpEtSumM_ = fs->make<TH1F>( "EEtpEtSumM", "EE TP tower EtSum matched", 100,0.,100.);

  EBrhEtSumVSvtx_ = fs->make<TProfile>( "EBrhEtSumVSvtx", "EB RH tower EtSum", numvtx, 0., (float)numvtx, 0., 500.);
  EBtpEtSumVSvtx_ = fs->make<TProfile>( "EBtpEtSumVSvtx", "EB TP tower EtSum", numvtx, 0., (float)numvtx, 0., 500.);
  EErhEtSumVSvtx_ = fs->make<TProfile>( "EErhEtSumVSvtx", "EE RH tower EtSum", numvtx, 0., (float)numvtx, 0., 500.);
  EEtpEtSumVSvtx_ = fs->make<TProfile>( "EEtpEtSumVSvtx", "EE TP tower EtSum", numvtx, 0., (float)numvtx, 0., 500.);

  EBrhEtSumMVSvtx_ = fs->make<TProfile>( "EBrhEtSumMVSvtx", "EB RH tower EtSum matched", numvtx, 0., (float)numvtx, 0., 500.);
  EBtpEtSumMVSvtx_ = fs->make<TProfile>( "EBtpEtSumMVSvtx", "EB TP tower EtSum matched", numvtx, 0., (float)numvtx, 0., 500.);
  EErhEtSumMVSvtx_ = fs->make<TProfile>( "EErhEtSumMVSvtx", "EE RH tower EtSum matched", numvtx, 0., (float)numvtx, 0., 500.);
  EEtpEtSumMVSvtx_ = fs->make<TProfile>( "EEtpEtSumMVSvtx", "EE TP tower EtSum matched", numvtx, 0., (float)numvtx, 0., 500.);

  rhSum_ = fs->make<TH1F>( "rhSum", "RH tower EtSum", 100,0.,100.);
  tpSum_ = fs->make<TH1F>( "tpSum", "TP tower EtSum", 100,0.,100.);
  rhSumM_ = fs->make<TH1F>( "rhSumM", "RH tower EtSum matched", 100,0.,100.);
  tpSumM_ = fs->make<TH1F>( "tpSumM", "TP tower EtSum matched", 100,0.,100.);

  rhSumVSvtx_ = fs->make<TProfile>( "rhSumVSvtx", "RH tower EtSum vs vtx", numvtx, 0., (float)numvtx, 0., 500.);
  tpSumVSvtx_ = fs->make<TProfile>( "tpSumVSvtx", "TP tower EtSum vs vtx", numvtx, 0., (float)numvtx, 0., 500.);
  rhSumMVSvtx_ = fs->make<TProfile>( "rhSumMVSvtx", "RH tower EtSum matched vs vtx", numvtx, 0., (float)numvtx, 0., 500.);
  tpSumMVSvtx_ = fs->make<TProfile>( "tpSumMVSvtx", "TP tower EtSum matched vs vtx", numvtx, 0., (float)numvtx, 0., 500.);

  rhSumOthEff_ = fs->make<TH1F>( "rhSumOthEff", "RH tower sum over thresold eff vs vtx", numvtx, 0., (float)numvtx);
  tpSumOthEff_ = fs->make<TH1F>( "tpSumOthEff", "TP tower sum over thresold eff vs vtx", numvtx, 0., (float)numvtx);
  rhSumOthMEff_ = fs->make<TH1F>( "rhSumOthMEff", "RH tower sum matched over thresold eff vs vtx", numvtx, 0., (float)numvtx);
  tpSumOthMEff_ = fs->make<TH1F>( "tpSumOthMEff", "TP tower sum matched over thresold eff vs vtx", numvtx, 0., (float)numvtx);

  EBMatchEffVSvtx_ = fs->make<TProfile>( "EBMatchEffVSvtx", "EB RH tower sum match eff vs vtx", numvtx, 0., (float)numvtx, 0., 1.);
  EEMatchEffVSvtx_ = fs->make<TProfile>( "EEMatchEffVSvtx", "EE RH tower sum match eff vs vtx", numvtx, 0., (float)numvtx, 0., 1.);

  EBhitEtM_ = fs->make<TH1F>( "EBhitEtM", "EB matched rechit Et", 100, 0., 50.); 
  EEhitEtM_ = fs->make<TH1F>( "EEhitEtM", "EE matched rechit Et", 100, 0., 50.); 
  EBhitEtnoM_ = fs->make<TH1F>( "EBhitEtnoM", "EB unmatched rechit Et", 100, 0., 50.); 
  EEhitEtnoM_ = fs->make<TH1F>( "EEhitEtnoM", "EE unmatched rechit Et", 100, 0., 50.); 

  EBhitTimeM_ = fs->make<TH1F>( "EBhitTimeM", "EB matched rechit Time", 120, -60., 60.); 
  EEhitTimeM_ = fs->make<TH1F>( "EEhitTimeM", "EE matched rechit Time", 120, -60., 60.); 
  EBhitTimenoM_ = fs->make<TH1F>( "EBhitTimenoM", "EB unmatched rechit Time", 120, -60., 60.); 
  EEhitTimenoM_ = fs->make<TH1F>( "EEhitTimenoM", "EE unmatched rechit Time", 120, -60., 60.); 

  for ( int ibin = 0; ibin < numvtx; ibin++ ) {
    Wvtx.push_back(0.);
    rhOWvtx.push_back(0.);
    tpOWvtx.push_back(0.);
    rhOMWvtx.push_back(0.);
    tpOMWvtx.push_back(0.);
  }

  float maxRankTh_ = minRankTh_+(float)nrankTh_ ;

  rhRank_ = fs->make<TH1F>( "rhRank", "number of events passing rh threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  rhRank_->Sumw2();
  rhEff_ = fs->make<TH1F>( "rhEff", "efficiency passing rh threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  rhEff_->Sumw2();

  tpRank_ = fs->make<TH1F>( "tpRank", "number of events passing tp threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  tpRank_->Sumw2();
  tpEff_ = fs->make<TH1F>( "tpEff", "efficiency passing tp threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  tpEff_->Sumw2();

  normaRank_ = fs->make<TH1F>( "normaRank", "number of events with at least 1 vtx", nrankTh_, minRankTh_, maxRankTh_ ); 
  normaRank_->Sumw2();

}

EcalTPAnalysis::~EcalTPAnalysis() {}

void EcalTPAnalysis::beginJob()
{
}

void EcalTPAnalysis::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
}
void EcalTPAnalysis::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void EcalTPAnalysis::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 

  // if simulation, perform pileup reweighting

  float theWeight = 1.;
  if ( ! iEvent.isRealData() && rewe_ ) {
    edm::Handle<std::vector< PileupSummaryInfo> > puSummary;
   iEvent.getByLabel(puSummaryCollection_, puSummary );
    
    std::vector<PileupSummaryInfo>::const_iterator PVI;

    float Tnpv = -1;
    for(PVI = puSummary->begin(); PVI != puSummary->end(); ++PVI) {

      int BX = PVI->getBunchCrossing();

      if(BX == 0) { 
        Tnpv = PVI->getTrueNumInteractions();
        continue;
      }

    }

    theWeight = theLumiW_->weight( Tnpv ); 

    truePUorigHisto_->Fill(Tnpv);
    truePUreweHisto_->Fill(Tnpv,theWeight);
    weightHisto_->Fill(theWeight);

  }

  // Start ECAL rec hit analysis

  edm::Handle<reco::VertexCollection> vtxCollection;
  iEvent.getByLabel(recoVertexCollection_, vtxCollection );
  
  unsigned int nVtx = 0;
  for ( unsigned int iloop = 0; iloop < (*vtxCollection).size(); iloop++ ) {
    if ( (*vtxCollection)[iloop].ndof() > 4 ) { nVtx++; }
  }

  vtxHisto_->Fill((float)nVtx,theWeight);

  edm::Handle<EcalTrigPrimDigiCollection> tp;
  iEvent.getByLabel(tpCollection_,tp);

  edm::Handle<EcalRecHitCollection> rechit_EB_col;
  iEvent.getByLabel(ebRecHitCollection_,rechit_EB_col);
  EBRecHit = rechit_EB_col.product();

  edm::Handle<EcalRecHitCollection> rechit_EE_col;
  iEvent.getByLabel(eeRecHitCollection_,rechit_EE_col);
  EERecHit = rechit_EE_col.product();
  

  edm::ESHandle<CaloGeometry> theGeometry;
  edm::ESHandle<CaloSubdetectorGeometry> theBarrelGeometry_handle;
  edm::ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle;
  iSetup.get<CaloGeometryRecord>().get( theGeometry );
  iSetup.get<EcalEndcapGeometryRecord>().get("EcalEndcap",theEndcapGeometry_handle);
  iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);

  theEndcapGeometry = &(*theEndcapGeometry_handle);
  theBarrelGeometry = &(*theBarrelGeometry_handle);
  edm::ESHandle<EcalTrigTowerConstituentsMap> eTTmap_;
  iSetup.get<IdealGeometryRecord>().get(eTTmap_);

  edm::ESHandle<EcalChannelStatus> chanstat;
  iSetup.get<EcalChannelStatusRcd>().get(chanstat);
  cstat=chanstat.product();      

  EcalTPGScale ecalScale ;
  ecalScale.setEventSetup(iSetup) ;

  edm::ESHandle< EcalElectronicsMapping > ecalmapping;
  iSetup.get< EcalMappingRcd >().get(ecalmapping);
  theMapping_ = ecalmapping.product();

  //  ebTowers.clear();
  //  eeTowers.clear();

  edm::SortedCollection< TTower > ebTowers;
  edm::SortedCollection< TTower > eeTowers;

  unsigned int rhEBass = 0;
  unsigned int rhEEass = 0;

  for (unsigned int i=0;i<tp.product()->size();i++) {

    EcalTriggerPrimitiveDigi d=(*(tp.product()))[i];

    int subdet=d.id().subDet();
    const EcalTrigTowerDetId TPtowid= d.id();

    // Reject spikes in TP 
    //    if ( subdet == 1 && d.sFGVB() == 0 ) continue;

    float tpEt = ecalScale.getTPGInGeV(d.compressedEt(), TPtowid) ; 
    if (d.id().ietaAbs()==27 || d.id().ietaAbs()==28)    tpEt*=2;

    std::vector<DetId> recTow((*eTTmap_).constituentsOf(TPtowid));

    float rhEt = 0.;

    for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {

      if ( subdet == 1 ) {

        EBDetId myid(recTow[iRec]);
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );

        if ( myRecHit != 0 ) {

          rhEBass++;

          float  theta=theBarrelGeometry->getGeometry(myid)->getPosition().theta();
          //std::cout << (*myRecHit) << " " << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEt += myRecHit->energy()*std::sin(theta);

        }
      }
      else if ( subdet == 2 ) {

        EEDetId myid(recTow[iRec]);
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );
        if ( myRecHit != 0 ) {

          rhEEass++;

          float  theta=theEndcapGeometry->getGeometry(myid)->getPosition().theta();
          //          std::cout << (*myRecHit) << " " << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEt += myRecHit->energy()*std::sin(theta);

        }
      }

    }

    std::pair<float,float> thisTower(tpEt,rhEt);
    TTower myTower(TPtowid,thisTower);
    if (subdet == 1 ) ebTowers.push_back(myTower);
    if (subdet == 2 ) eeTowers.push_back(myTower);

    //    std::cout << "SubD = " << subdet << " RH size = " << recTow.size() << " TP Et = " << tpEt << " RH Et = " << rhEt << std::endl; 

  }

  if ( nVtx < 1 ) return;

  unsigned int nebrh = 0;
  unsigned int nebtp = 0;
  unsigned int neerh = 0;
  unsigned int neetp = 0;
  unsigned int nebrhm = 0;
  unsigned int neerhm = 0;

  float tpEBEtSum = 0.;
  float tpEEEtSum = 0.;
  float rhEBEtSum = 0.;
  float rhEEEtSum = 0.;

  float tpEBEtSumM = 0.;
  float tpEEEtSumM = 0.;
  float rhEBEtSumM = 0.;
  float rhEEEtSumM = 0.;

  for (unsigned int iPair=0; iPair < ebTowers.size(); iPair++) {
    if ( ebTowers[iPair].value().first == 0. ) EBrhEtNOtp_->Fill(ebTowers[iPair].value().second,theWeight); 
    EBtpVSrh_->Fill(ebTowers[iPair].value().second,ebTowers[iPair].value().first,theWeight); 
    EBdiffVSrh_->Fill(ebTowers[iPair].value().second,ebTowers[iPair].value().first-ebTowers[iPair].value().second,theWeight); 
    if ( ebTowers[iPair].value().first > tpEtTh_ ) { nebtp++; tpEBEtSum += ebTowers[iPair].value().second; }
    if ( ebTowers[iPair].value().second > rhTEtTh_ ) { nebrh++; rhEBEtSum += ebTowers[iPair].value().second; }
    if ( (int)nVtx == vtxSel_ ) {
      if ( ebTowers[iPair].value().first > tpEtTh_ ) EBtpEt_->Fill(ebTowers[iPair].value().first,theWeight);
      if ( ebTowers[iPair].value().second > rhTEtTh_ ) EBrhEt_->Fill(ebTowers[iPair].value().second,theWeight);
    }

    std::vector<DetId> recTow((*eTTmap_).constituentsOf(ebTowers[iPair].id()));

    if ( ebTowers[iPair].value().first > tpEtTh_ && ebTowers[iPair].value().second > rhTEtTh_ ) { 
      EBdiffVSrhM_->Fill(ebTowers[iPair].value().second,ebTowers[iPair].value().first-ebTowers[iPair].value().second,theWeight); 
      nebrhm++;
      tpEBEtSumM += ebTowers[iPair].value().first;
      rhEBEtSumM += ebTowers[iPair].value().second;
      if ( (int)nVtx == vtxSel_ ) {
        EBtpEtM_->Fill(ebTowers[iPair].value().first,theWeight);
        EBrhEtM_->Fill(ebTowers[iPair].value().second,theWeight);
      }
      
      for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );
        if ( myRecHit != 0 ) {
          float et = myRecHit->energy()*std::sin(theBarrelGeometry->getGeometry(EBDetId(recTow[iRec]))->getPosition().theta());
          float time = myRecHit->time();
          EBhitEtM_->Fill(et,theWeight);
          EBhitTimeM_->Fill(time,theWeight);
        }
      }
      //      std::cout << "EB TT size = " << recTow.size() << std::endl;

    }
    else if ( ebTowers[iPair].value().second > rhTEtTh_ ) {
      
      for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );
        if ( myRecHit != 0 ) {
          float et = myRecHit->energy()*std::sin(theBarrelGeometry->getGeometry(EBDetId(recTow[iRec]))->getPosition().theta());
          float time = myRecHit->time();
          EBhitEtnoM_->Fill(et,theWeight);
          EBhitTimenoM_->Fill(time,theWeight);
        }
      }
    }

  }

  if ( (int)nVtx == vtxSel_ ) {
    EBtpEtSum_->Fill(tpEBEtSum,theWeight);
    EBrhEtSum_->Fill(rhEBEtSum,theWeight);
    EBtpEtSumM_->Fill(tpEBEtSumM,theWeight);
    EBrhEtSumM_->Fill(rhEBEtSumM,theWeight);
  }
  EBtpEtSumVSvtx_->Fill((int)nVtx,tpEBEtSum,theWeight);
  EBrhEtSumVSvtx_->Fill((int)nVtx,rhEBEtSum,theWeight);
  EBtpEtSumMVSvtx_->Fill((int)nVtx,tpEBEtSumM,theWeight);
  EBrhEtSumMVSvtx_->Fill((int)nVtx,rhEBEtSumM,theWeight);

  if ( nebrh > 0 ) EBMatchEffVSvtx_->Fill((int)nVtx,(float)nebrhm/(float)nebrh, theWeight);
 
  for (unsigned int iPair=0; iPair < eeTowers.size(); iPair++) {
    if ( eeTowers[iPair].value().first == 0. ) EErhEtNOtp_->Fill(eeTowers[iPair].value().second,theWeight); 
    EEtpVSrh_->Fill(eeTowers[iPair].value().second,eeTowers[iPair].value().first,theWeight); 
    EEdiffVSrh_->Fill(eeTowers[iPair].value().second,eeTowers[iPair].value().first-eeTowers[iPair].value().second,theWeight); 
    if ( eeTowers[iPair].value().first > tpEtTh_ ) { neetp++; tpEEEtSum += eeTowers[iPair].value().first; }
    if ( eeTowers[iPair].value().second > rhTEtTh_ ) { neerh++; rhEEEtSum += eeTowers[iPair].value().second; }
    if ( (int)nVtx == vtxSel_ ) {
      if ( eeTowers[iPair].value().first > tpEtTh_ ) EEtpEt_->Fill(eeTowers[iPair].value().first,theWeight);
      if ( eeTowers[iPair].value().first > tpEtTh_ ) EErhEt_->Fill(eeTowers[iPair].value().second,theWeight);
    }

    std::vector<DetId> recTow((*eTTmap_).constituentsOf(eeTowers[iPair].id()));

    if ( eeTowers[iPair].value().first > tpEtTh_ && eeTowers[iPair].value().second > rhTEtTh_ ) { 
      EEdiffVSrhM_->Fill(eeTowers[iPair].value().second,eeTowers[iPair].value().first-eeTowers[iPair].value().second,theWeight); 
      
      // if ( std::fabs(eeTowers[iPair].value().first-eeTowers[iPair].value().second) > 2. && eeTowers[iPair].value().first > 6. ) {
      //   std::cout << "Interesting difference TT = " << eeTowers[iPair].id() 
      //             << " TP = " << eeTowers[iPair].value().first 
      //             << " RH = " << eeTowers[iPair].value().second << std::endl;
      // }
      // std::cout << "EE TT size = " << recTow.size() << std::endl;
      
      neerhm++;
      tpEEEtSumM += eeTowers[iPair].value().first;
      rhEEEtSumM += eeTowers[iPair].value().second;
      if ( (int)nVtx == vtxSel_ ) {
        EEtpEtM_->Fill(eeTowers[iPair].value().first,theWeight);
        EErhEtM_->Fill(eeTowers[iPair].value().second,theWeight);
      }
      
      for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );
        if ( myRecHit != 0 ) {
          float et = myRecHit->energy()*std::sin(theEndcapGeometry->getGeometry(EEDetId(recTow[iRec]))->getPosition().theta());
          float time = myRecHit->time();
          EEhitEtM_->Fill(et,theWeight);
          EEhitTimeM_->Fill(time,theWeight);

          // EEDetId thisDetId(myRecHit->detid()());
          // std::cout << "TT = " << eeTowers[iPair].id() << " iRec = " << iRec << " detId = " << thisDetId << " Et = " << et << std::endl;
          // EcalElectronicsId eleId(theMapping_->getElectronicsId(thisDetId));
          // std::cout << "Electronics Id = " << eleId << std::endl;
        }
      }

    }
    else if ( ebTowers[iPair].value().second > rhTEtTh_ ) {
      
      for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {
        const EcalRecHit * myRecHit = giveGoodHit( recTow[iRec] );
        if ( myRecHit != 0 ) {
          float et = myRecHit->energy()*std::sin(theEndcapGeometry->getGeometry(EEDetId(recTow[iRec]))->getPosition().theta());
          float time = myRecHit->time();
          EEhitEtnoM_->Fill(et,theWeight);
          EEhitTimenoM_->Fill(time,theWeight);
        }
      }

    }
  }

  if ( (int)nVtx == vtxSel_ ) {
    EEtpEtSum_->Fill(tpEEEtSum,theWeight);
    EErhEtSum_->Fill(rhEEEtSum,theWeight);
    EEtpEtSumM_->Fill(tpEEEtSumM,theWeight);
    EErhEtSumM_->Fill(rhEEEtSumM,theWeight);
  }
  EEtpEtSumVSvtx_->Fill((int)nVtx,tpEEEtSum,theWeight);
  EErhEtSumVSvtx_->Fill((int)nVtx,rhEEEtSum,theWeight);
  EEtpEtSumMVSvtx_->Fill((int)nVtx,tpEEEtSumM,theWeight);
  EErhEtSumMVSvtx_->Fill((int)nVtx,rhEEEtSumM,theWeight);

  if ( neerh > 0 ) EEMatchEffVSvtx_->Fill((int)nVtx,(float)neerhm/(float)neerh, theWeight);
  
  EBrh_->Fill(nebrh,theWeight);
  EBtp_->Fill(nebtp,theWeight);
  EErh_->Fill(neerh,theWeight);
  EEtp_->Fill(neetp,theWeight);
  EBrhM_->Fill(nebrhm,theWeight);
  EErhM_->Fill(neerhm,theWeight);

  for ( int irank = 0; irank < nrankTh_; irank++ ) {
    float threshold = minRankTh_+(float)irank;
    if ( rhEBEtSum+rhEEEtSum > threshold ) { rhRank_->Fill(threshold+0.5,theWeight); }
    if ( tpEBEtSum+tpEEEtSum > threshold ) { tpRank_->Fill(threshold+0.5,theWeight); }
    normaRank_->Fill(threshold+0.5,theWeight); 
  }

  Wvtx[nVtx] += theWeight;
  if ( rhEBEtSum+rhEEEtSum > effTh_ ) rhOWvtx[nVtx] += theWeight;
  if ( tpEBEtSum+tpEEEtSum > effTh_ ) tpOWvtx[nVtx] += theWeight;
  if ( rhEBEtSumM+rhEEEtSumM > effTh_ ) rhOMWvtx[nVtx] += theWeight;
  if ( tpEBEtSumM+tpEEEtSumM > effTh_ ) tpOMWvtx[nVtx] += theWeight;

  if ( (int)nVtx == vtxSel_ ) {
    rhSum_->Fill( rhEBEtSum+rhEEEtSum, theWeight );
    tpSum_->Fill( tpEBEtSum+tpEEEtSum, theWeight );
    rhSumM_->Fill( rhEBEtSumM+rhEEEtSumM, theWeight );
    tpSumM_->Fill( tpEBEtSumM+tpEEEtSumM, theWeight );
  }

  rhSumVSvtx_->Fill( (int)nVtx, rhEBEtSum+rhEEEtSum, theWeight );
  tpSumVSvtx_->Fill( (int)nVtx, tpEBEtSum+tpEEEtSum, theWeight );
  rhSumMVSvtx_->Fill( (int)nVtx, rhEBEtSumM+rhEEEtSumM, theWeight );
  tpSumMVSvtx_->Fill( (int)nVtx, tpEBEtSumM+tpEEEtSumM, theWeight );

  // check for rechits presence independent on TP one

  // unsigned int rhEBnum = 0;
  // unsigned int rhEEnum = 0;
  // unsigned int rhEBnumTT = 0;
  // unsigned int rhEEnumTT = 0;
  // float rhEBTotEt = 0.;
  // float rhEETotEt = 0.;

  // for ( EcalRecHitCollection::const_iterator myRecHit = EBRecHit->begin(); myRecHit != EBRecHit->end(); ++myRecHit ) {
  //   rhEBnum++;
  //   EBDetId myid(myRecHit->id());
  //   float  theta=theBarrelGeometry->getGeometry(myid)->getPosition().theta();
  //   rhEBTotEt += myRecHit->energy()*std::sin(theta);
  //   EcalTrigTowerDetId towid = (*eTTmap_).towerOf(myid);
  //   if ( tp.product()->find(towid) != tp.product()->end() ) rhEBnumTT++;
  // }
  // for ( EcalRecHitCollection::const_iterator myRecHit = EERecHit->begin(); myRecHit != EERecHit->end(); ++myRecHit ) {
  //   rhEEnum++;
  //   EEDetId myid(myRecHit->id());
  //   float  theta=theEndcapGeometry->getGeometry(myid)->getPosition().theta();
  //   rhEETotEt += myRecHit->energy()*std::sin(theta);
  //   EcalTrigTowerDetId towid = (*eTTmap_).towerOf(myid);
  //   if ( tp.product()->find(towid) != tp.product()->end() ) rhEEnumTT++;
  //   bool isTT( tp.product()->find(towid) != tp.product()->end() && (tp.product()->find(towid))->compressedEt() > 0 );
  //   std::cout << "EE RH # " << rhEEnum << " detId = " << myid << " isGood = "  << giveGoodHit(myid) << " isTT = " << isTT  << " Et = " << myRecHit->energy()*std::sin(theta) << std::endl;
  // }

  // std::cout << "\n" << std::endl;
  // std::cout << "EB: tot RH = " << rhEBTotEt << " TP Et sum = " << tpEBEtSum << " RH Et sum = " << rhEBEtSum << std::endl;
  // //   if ( rhEBnum != rhEBnumTT || rhEBnumTT != rhEBass )  std::cout << "EB rechit # = " << rhEBnum << " associated # = " << rhEBnumTT << " found in TT = " << rhEBass << std::endl;
  // std::cout << "EB rechit # = " << rhEBnum << " associated # = " << rhEBnumTT << " found in TT = " << rhEBass << std::endl;
  // std::cout << "\n" << std::endl;
  // std::cout << "EE: tot RH = " << rhEETotEt << " TP Et sum = " << tpEEEtSum << " RH Et sum = " << rhEEEtSum << std::endl;
  // //   if ( rhEEnum != rhEEnumTT || rhEEnumTT != rhEEass )  std::cout << "EE rechit # = " << rhEEnum << " associated # = " << rhEEnumTT << " found in TT = " << rhEEass << std::endl;
  // std::cout << "EE rechit # = " << rhEEnum << " associated # = " << rhEEnumTT << " found in TT = " << rhEEass << std::endl;
  // std::cout << "\n" << std::endl;

 
}//analyze

void EcalTPAnalysis::endJob(){

  double eff = 0.;
  double erreff = 0.;
  for ( int ivtx = 0; ivtx < numvtx; ivtx++ ) {

    if ( Wvtx[ivtx] > 0. ) { 
      eff = rhOWvtx[ivtx]/Wvtx[ivtx]; 
      erreff = std::sqrt(rhOWvtx[ivtx]*(1.-rhOWvtx[ivtx]/Wvtx[ivtx]))/Wvtx[ivtx];
      rhSumOthEff_->SetBinContent((Int_t)ivtx, eff);
      rhSumOthEff_->SetBinError((Int_t)ivtx, erreff);
      
      //      std::cout << "iVtx = " << ivtx << " W = " << Wvtx[ivtx] << " RHW = " << rhOWvtx[ivtx] << " eff = " << eff << " +/- " << erreff << std::endl;

      eff = tpOWvtx[ivtx]/Wvtx[ivtx]; 
      erreff = std::sqrt(tpOWvtx[ivtx]*(1.-tpOWvtx[ivtx]/Wvtx[ivtx]))/Wvtx[ivtx];
      tpSumOthEff_->SetBinContent((Int_t)ivtx, eff);
      tpSumOthEff_->SetBinError((Int_t)ivtx, erreff);

      eff = rhOMWvtx[ivtx]/Wvtx[ivtx]; 
      erreff = std::sqrt(rhOMWvtx[ivtx]*(1.-rhOMWvtx[ivtx]/Wvtx[ivtx]))/Wvtx[ivtx];
      rhSumOthMEff_->SetBinContent((Int_t)ivtx, eff);
      rhSumOthMEff_->SetBinError((Int_t)ivtx, erreff);

      eff = tpOMWvtx[ivtx]/Wvtx[ivtx]; 
      erreff = std::sqrt(tpOMWvtx[ivtx]*(1.-tpOMWvtx[ivtx]/Wvtx[ivtx]))/Wvtx[ivtx];
      tpSumOthMEff_->SetBinContent((Int_t)ivtx, eff);
      tpSumOthMEff_->SetBinError((Int_t)ivtx, erreff);

    }
    
  }

  rhEff_->Divide(rhRank_,normaRank_,1.,1.,"B");
  tpEff_->Divide(tpRank_,normaRank_,1.,1.,"B");

}

const EcalRecHit * EcalTPAnalysis::giveGoodHit( const DetId & theId ) const {

  int subdet(theId.subdetId());

  const EcalRecHit * thisHit(0);

  //  std::cout << "SubDet " << subdet << std::endl;
  
  if ( subdet == 1 ) {

    EBDetId myid(theId);
    EcalRecHitCollection::const_iterator myRecHit = EBRecHit->find(myid);

    if ( myRecHit != EBRecHit->end() ) {
      
      // Quality selection on ECAL rec hits
      Bool_t is_spike=false;
      if (myRecHit->checkFlag(EcalRecHit::kWeird) || myRecHit->checkFlag(EcalRecHit::kDiWeird)) is_spike=true;
      int chanstatus=-1;
      EcalChannelStatus::const_iterator chIt = cstat->find( myRecHit->id() );
      if ( chIt != cstat->end() ) {
        chanstatus = chIt->getStatusCode() & 0x1F;
      }
      if (!is_spike && chanstatus==0) thisHit = &(*myRecHit);      
    }

  } else if ( subdet ==2 ) {

    EEDetId myid(theId);
    EcalRecHitCollection::const_iterator myRecHit = EERecHit->find(myid);

    if ( myRecHit != EERecHit->end() ) {
      
      // Quality selection on ECAL rec hits
      Bool_t is_spike=false;
      if (myRecHit->checkFlag(EcalRecHit::kWeird) || myRecHit->checkFlag(EcalRecHit::kDiWeird)) is_spike=true;
      int chanstatus=-1;
      EcalChannelStatus::const_iterator chIt = cstat->find( myRecHit->id() );
      if ( chIt != cstat->end() ) {
        chanstatus = chIt->getStatusCode() & 0x1F;
      }
      if (!is_spike && chanstatus==0) thisHit = &(*myRecHit);      
      
    }
    
  }
  
  return thisHit;

}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalTPAnalysis);
