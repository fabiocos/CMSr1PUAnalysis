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

  const int numvtx;

  edm::LumiReWeighting * theLumiW_;
  edm::InputTag puSummaryCollection_;
  std::string dataPUFile,mcPUFile,dataPUHisto,mcPUHisto;

  TH1F * vtxHisto_;

  TH1F * truePUorigHisto_;
  TH1F * truePUreweHisto_;
  TH1F * weightHisto_;

  bool rewe_;

};

using namespace edm;

EcalTPAnalysis::EcalTPAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  ebRecHitCollection_(iPSet.getParameter<edm::InputTag>("ebRecHitCollection")),
  eeRecHitCollection_(iPSet.getParameter<edm::InputTag>("eeRecHitCollection")),
  tpCollection_(iPSet.getParameter<edm::InputTag>("tpCollection")),
  tpEtTh_(iPSet.getParameter<double>("tpEtTh")),
  numvtx(60)
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

  edm::Handle<EcalRecHitCollection> rechit_EE_col;
  iEvent.getByLabel(eeRecHitCollection_,rechit_EE_col);
  

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

  EcalTPGScale ecalScale ;
  ecalScale.setEventSetup(iSetup) ;

  for (unsigned int i=0;i<tp.product()->size();i++) {
    EcalTriggerPrimitiveDigi d=(*(tp.product()))[i];
    int subdet=d.id().subDet();
    const EcalTrigTowerDetId TPtowid= d.id();

    float Et = ecalScale.getTPGInGeV(d.compressedEt(), TPtowid) ; 
    if (d.id().ietaAbs()==27 || d.id().ietaAbs()==28)    Et*=2;


    if ( Et > tpEtTh_ ) { 

      std::vector<DetId> recTow((*eTTmap_).constituentsOf(TPtowid));

      float rhEtSum = 0.;

      for (unsigned int iRec=0; iRec < recTow.size(); iRec++) {

        if ( subdet == 1 ) {
          EBDetId myid(recTow[iRec]);
          const EBRecHitCollection * EBRecHit = rechit_EB_col.product();
          EcalRecHitCollection::const_iterator myRecHit = EBRecHit->find(myid);
          float  theta=theBarrelGeometry->getGeometry(myid)->getPosition().theta();
          //std::cout << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEtSum += myRecHit->energy()*std::sin(theta);
        }
        else if ( subdet == 2 ) {
          EEDetId myid(recTow[iRec]);
          const EERecHitCollection * EERecHit = rechit_EE_col.product();
          EcalRecHitCollection::const_iterator myRecHit = EERecHit->find(myid);
          float  theta=theEndcapGeometry->getGeometry(myid)->getPosition().theta();
          //std::cout << myRecHit->energy()*std::sin(theta) << std::endl;
          rhEtSum += myRecHit->energy()*std::sin(theta);
        }

        
      }
      
      std::cout << "SubD = " << subdet << " RH size = " << recTow.size() << " Et = " << Et << " RH Et = " << rhEtSum << std::endl; 

    }

  }
    
}//analyze


//define this as a plug-in
DEFINE_FWK_MODULE(EcalTPAnalysis);
