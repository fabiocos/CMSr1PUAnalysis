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

#include "CalibCalorimetry/EcalTPGTools/interface/EcalTPGScale.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

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
  
private:
  
  edm::InputTag recoVertexCollection_;
  edm::InputTag ebRecHitCollection_; 
  edm::InputTag eeRecHitCollection_; 
  edm::InputTag tpCollection_;

  double tpEtTh_;
  double rhTEtTh_;

  int vtxSel_;
  const int numvtx;

  edm::LumiReWeighting * theLumiW_;
  edm::InputTag puSummaryCollection_;
  std::string dataPUFile,mcPUFile,dataPUHisto,mcPUHisto;

  bool rewe_;

  TH1F * vtxHisto_;

  TH1F * truePUorigHisto_;
  TH1F * truePUreweHisto_;
  TH1F * weightHisto_;

  std::vector< std::pair<float,float> > ebTowers;
  std::vector <std::pair<float,float> > eeTowers;

  TH2F * EBrhVSttM_;
  TH2F * EErhVSttM_;

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

};

using namespace edm;

EcalTPAnalysis::EcalTPAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  ebRecHitCollection_(iPSet.getParameter<edm::InputTag>("ebRecHitCollection")),
  eeRecHitCollection_(iPSet.getParameter<edm::InputTag>("eeRecHitCollection")),
  tpCollection_(iPSet.getParameter<edm::InputTag>("tpCollection")),
  tpEtTh_(iPSet.getParameter<double>("tpEtTh")),
  rhTEtTh_(iPSet.getParameter<double>("rhTEtTh")),
  vtxSel_(iPSet.getParameter<int>("vtxSel")),numvtx(60)
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

  EBrhVSttM_ = fs->make<TH2F>( "EBrhVStt", "EB RH tower vs TT Et matched", 20,0.,20.,20,0.,20.);
  EErhVSttM_ = fs->make<TH2F>( "EErhVStt", "EE RH tower vs TT Et matched", 20,0.,20.,20,0.,20.);

  EBrhEt_ = fs->make<TH1F>( "EBrhEt", "EB RH tower Et", 100,0.,100.);
  EBtpEt_ = fs->make<TH1F>( "EBtpEt", "EB TP tower Et", 100,0.,100.);
  EErhEt_ = fs->make<TH1F>( "EErhEt", "EE RH tower Et", 100,0.,100.);
  EEtpEt_ = fs->make<TH1F>( "EEtpEt", "EE TP tower Et", 100,0.,100.);

  EBrhEtM_ = fs->make<TH1F>( "EBrhEtM", "EB RH tower Et matched", 100,0.,100.);
  EBtpEtM_ = fs->make<TH1F>( "EBtpEtM", "EB TP tower Et matched", 100,0.,100.);
  EErhEtM_ = fs->make<TH1F>( "EErhEtM", "EE RH tower Et matched", 100,0.,100.);
  EEtpEtM_ = fs->make<TH1F>( "EEtpEtM", "EE TP tower Et matched", 100,0.,100.);

  EBrhEtSum_ = fs->make<TH1F>( "EBrhEtSum", "EB RH tower EtSum", 100,0.,100.);
  EBtpEtSum_ = fs->make<TH1F>( "EBtpEtSum", "EB TP tower EtSum", 100,0.,100.);
  EErhEtSum_ = fs->make<TH1F>( "EErhEtSum", "EE RH tower EtSum", 100,0.,100.);
  EEtpEtSum_ = fs->make<TH1F>( "EEtpEtSum", "EE TP tower EtSum", 100,0.,100.);

  EBrhEtSumM_ = fs->make<TH1F>( "EBrhEtSumM", "EB RH tower EtSum matched", 100,0.,100.);
  EBtpEtSumM_ = fs->make<TH1F>( "EBtpEtSumM", "EB TP tower EtSum matched", 100,0.,100.);
  EErhEtSumM_ = fs->make<TH1F>( "EErhEtSumM", "EE RH tower EtSum matched", 100,0.,100.);
  EEtpEtSumM_ = fs->make<TH1F>( "EEtpEtSumM", "EE TP tower EtSum matched", 100,0.,100.);

  EBrhEtSumVSvtx_ = fs->make<TProfile>( "EBrhEtSumVSvtx", "EB RH tower EtSum", numvtx, 0., (float)numvtx, 0., 100.);
  EBtpEtSumVSvtx_ = fs->make<TProfile>( "EBtpEtSumVSvtx", "EB TP tower EtSum", numvtx, 0., (float)numvtx, 0., 100.);
  EErhEtSumVSvtx_ = fs->make<TProfile>( "EErhEtSumVSvtx", "EE RH tower EtSum", numvtx, 0., (float)numvtx, 0., 100.);
  EEtpEtSumVSvtx_ = fs->make<TProfile>( "EEtpEtSumVSvtx", "EE TP tower EtSum", numvtx, 0., (float)numvtx, 0., 100.);

  EBrhEtSumMVSvtx_ = fs->make<TProfile>( "EBrhEtSumMVSvtx", "EB RH tower EtSum matched", numvtx, 0., (float)numvtx, 0., 100.);
  EBtpEtSumMVSvtx_ = fs->make<TProfile>( "EBtpEtSumMVSvtx", "EB TP tower EtSum matched", numvtx, 0., (float)numvtx, 0., 100.);
  EErhEtSumMVSvtx_ = fs->make<TProfile>( "EErhEtSumMVSvtx", "EE RH tower EtSum matched", numvtx, 0., (float)numvtx, 0., 100.);
  EEtpEtSumMVSvtx_ = fs->make<TProfile>( "EEtpEtSumMVSvtx", "EE TP tower EtSum matched", numvtx, 0., (float)numvtx, 0., 100.);

}

EcalTPAnalysis::~EcalTPAnalysis() {}

void EcalTPAnalysis::beginJob()
{
}

void EcalTPAnalysis::endJob(){return;}
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
  const EBRecHitCollection * EBRecHit = rechit_EB_col.product();

  edm::Handle<EcalRecHitCollection> rechit_EE_col;
  iEvent.getByLabel(eeRecHitCollection_,rechit_EE_col);
  const EERecHitCollection * EERecHit = rechit_EE_col.product();
  

  edm::ESHandle<CaloGeometry> theGeometry;
  edm::ESHandle<CaloSubdetectorGeometry> theBarrelGeometry_handle;
  edm::ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle;
  iSetup.get<CaloGeometryRecord>().get( theGeometry );
  iSetup.get<EcalEndcapGeometryRecord>().get("EcalEndcap",theEndcapGeometry_handle);
  iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);

  const CaloSubdetectorGeometry *theEndcapGeometry,*theBarrelGeometry;
  theEndcapGeometry = &(*theEndcapGeometry_handle);
  theBarrelGeometry = &(*theBarrelGeometry_handle);
  edm::ESHandle<EcalTrigTowerConstituentsMap> eTTmap_;
  iSetup.get<IdealGeometryRecord>().get(eTTmap_);

  edm::ESHandle<EcalChannelStatus> chanstat;
  iSetup.get<EcalChannelStatusRcd>().get(chanstat);
  const EcalChannelStatus* cstat=chanstat.product();      

  EcalTPGScale ecalScale ;
  ecalScale.setEventSetup(iSetup) ;

  ebTowers.clear();
  eeTowers.clear();

  unsigned int rhEBass = 0;
  unsigned int rhEEass = 0;

  for (unsigned int i=0;i<tp.product()->size();i++) {

    EcalTriggerPrimitiveDigi d=(*(tp.product()))[i];

    // Reject spikes in TP 
    if ( d.sFGVB() == 0 ) continue;

    int subdet=d.id().subDet();
    const EcalTrigTowerDetId TPtowid= d.id();

    float tpEt = ecalScale.getTPGInGeV(d.compressedEt(), TPtowid) ; 
    if (d.id().ietaAbs()==27 || d.id().ietaAbs()==28)    tpEt*=2;

    std::vector<DetId> recTow((*eTTmap_).constituentsOf(TPtowid));

    float rhEt = 0.;

    for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {

      if ( subdet == 1 ) {
        EBDetId myid(recTow[iRec]);
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
          if (is_spike || chanstatus!=0) continue;

          float  theta=theBarrelGeometry->getGeometry(myid)->getPosition().theta();
          //std::cout << (*myRecHit) << " " << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEt += myRecHit->energy()*std::sin(theta);
          rhEBass++;
        }
      }
      else if ( subdet == 2 ) {
        EEDetId myid(recTow[iRec]);
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
          if (is_spike || chanstatus!=0) continue;

          float  theta=theEndcapGeometry->getGeometry(myid)->getPosition().theta();
          //std::cout << (*myRecHit) << " " << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEt += myRecHit->energy()*std::sin(theta);
          rhEEass++;
        }
      }

    }

    std::pair<float,float> thisTower(tpEt,rhEt);
    if (subdet == 1 ) ebTowers.push_back(thisTower);
    if (subdet == 2 ) eeTowers.push_back(thisTower); 

    //    std::cout << "SubD = " << subdet << " RH size = " << recTow.size() << " TP Et = " << tpEt << " RH Et = " << rhEt << std::endl; 

  }

  float tpEBEtSum = 0.;
  float tpEEEtSum = 0.;
  float rhEBEtSum = 0.;
  float rhEEEtSum = 0.;

  float tpEBEtSumM = 0.;
  float tpEEEtSumM = 0.;
  float rhEBEtSumM = 0.;
  float rhEEEtSumM = 0.;

  for (unsigned int iPair=0; iPair < ebTowers.size(); iPair++) {
    tpEBEtSum += ebTowers[iPair].first;
    rhEBEtSum += ebTowers[iPair].second;
    if ( (int)nVtx == vtxSel_ ) {
      EBtpEt_->Fill(ebTowers[iPair].first,theWeight);
      EBrhEt_->Fill(ebTowers[iPair].second,theWeight);
    }
    if ( ebTowers[iPair].first > tpEtTh_ && ebTowers[iPair].second > rhTEtTh_ ) { 
      tpEBEtSumM += ebTowers[iPair].first;
      rhEBEtSumM += ebTowers[iPair].second;
      EBrhVSttM_->Fill(ebTowers[iPair].second,ebTowers[iPair].first,theWeight); 
      if ( (int)nVtx == vtxSel_ ) {
        EBtpEtM_->Fill(ebTowers[iPair].first,theWeight);
        EBrhEtM_->Fill(ebTowers[iPair].second,theWeight);
      }
    }
  }

  if ( (int)nVtx == vtxSel_ ) {
    EBtpEtSum_->Fill(tpEBEtSum,theWeight);
    EBrhEtSum_->Fill(rhEBEtSum,theWeight);
    EBtpEtSumM_->Fill(tpEEEtSumM,theWeight);
    EBrhEtSumM_->Fill(rhEEEtSumM,theWeight);
  }
  EBtpEtSumVSvtx_->Fill((int)nVtx,tpEBEtSum,theWeight);
  EBrhEtSumVSvtx_->Fill((int)nVtx,rhEBEtSum,theWeight);
  EBtpEtSumMVSvtx_->Fill((int)nVtx,tpEBEtSumM,theWeight);
  EBrhEtSumMVSvtx_->Fill((int)nVtx,rhEBEtSumM,theWeight);
 
  for (unsigned int iPair=0; iPair < eeTowers.size(); iPair++) {
    tpEEEtSum += eeTowers[iPair].first;
    rhEEEtSum += eeTowers[iPair].second;
    if ( (int)nVtx == vtxSel_ ) {
      EEtpEt_->Fill(eeTowers[iPair].first,theWeight);
      EErhEt_->Fill(eeTowers[iPair].second,theWeight);
    }
    if ( eeTowers[iPair].first > tpEtTh_ && eeTowers[iPair].second > rhTEtTh_ ) { 
      tpEEEtSumM += eeTowers[iPair].first;
      rhEEEtSumM += eeTowers[iPair].second;
      EErhVSttM_->Fill(eeTowers[iPair].second,eeTowers[iPair].first,theWeight); 
      if ( (int)nVtx == vtxSel_ ) {
        EEtpEt_->Fill(eeTowers[iPair].first,theWeight);
        EErhEtM_->Fill(eeTowers[iPair].second,theWeight);
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
  // }

  // std::cout << "\n" << std::endl;
  // std::cout << "EB: tot RH = " << rhEBTotEt << " TP Et sum = " << tpEBEtSum << " RH Et sum = " << rhEBEtSum << std::endl;
  // if ( rhEBnum != rhEBnumTT || rhEBnumTT != rhEBass )  std::cout << "EB rechit # = " << rhEBnum << " associated # = " << rhEBnumTT << " found in TT = " << rhEBass << std::endl;
  // std::cout << "\n" << std::endl;
  // std::cout << "EE: tot RH = " << rhEETotEt << " TP Et sum = " << tpEEEtSum << " RH Et sum = " << rhEEEtSum << std::endl;
  // if ( rhEEnum != rhEEnumTT || rhEEnumTT != rhEEass )  std::cout << "EE rechit # = " << rhEEnum << " associated # = " << rhEEnumTT << " found in TT = " << rhEEass << std::endl;
  // std::cout << "\n" << std::endl;

 
}//analyze


//define this as a plug-in
DEFINE_FWK_MODULE(EcalTPAnalysis);
