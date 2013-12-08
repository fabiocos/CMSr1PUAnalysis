/*class HcalMinBiasAnalysis
 *  
 *  Class to study ECAL response in Minimum Bias events
 *
 *  $Date: 2013/05/29 09:44:55 $
 *  $Revision: 1.3 $
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
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/HcalObjects/interface/HcalChannelStatus.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

class HcalMinBiasAnalysis : public edm::EDAnalyzer
{

public:
  explicit HcalMinBiasAnalysis(const edm::ParameterSet&);
  virtual ~HcalMinBiasAnalysis();
  virtual void beginJob();
  virtual void endJob();  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  
private:
  
  edm::InputTag recoVertexCollection_;
  edm::InputTag hbheRecHitCollection_; 

  int vtxSel_;
  
  TH1F * hvtxHisto_;
  TH1F * hemultiHisto_;
  TH1F * hehitEneAllHisto_;
  TH1F * hehit5EtaHisto_;
  TH1F * hehit5EneHisto_;
  TH1F * hehit5ETHisto_;
  TH1F * hehit5SumETHisto_;
  TH1F * hehit5TimeHisto_;

  TH2F * he5TimeVSEneHisto_;

  std::vector<TH1F*> he5SumETvtxHisto_;

  TProfile* he5SumEVSeta_;
  TProfile* he5SumETVSeta_;

  TProfile* he5SumETVSvtx_;

  const float lowEta;
  const float highEta;
  const int nEta;
  const int numvtx;

  edm::LumiReWeighting * theLumiW_;
  edm::InputTag puSummaryCollection_;
  std::string dataPUFile,mcPUFile,dataPUHisto,mcPUHisto;

  TH1F * truePUorigHisto_;
  TH1F * truePUreweHisto_;
  TH1F * weightHisto_;

  bool rewe_;

};

using namespace edm;

HcalMinBiasAnalysis::HcalMinBiasAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  hbheRecHitCollection_(iPSet.getParameter<edm::InputTag>("hbheRecHitCollection")),
  vtxSel_(iPSet.getParameter<int>("vtxSel")),lowEta(1.4),highEta(3.),nEta(18),numvtx(60)
{    

  edm::Service<TFileService> fs;

  hvtxHisto_ = fs->make<TH1F>( "vtx", "vertex ndof.gt.4 multiplicity", 100, 0., 100. ) ;
  hemultiHisto_ = fs->make<TH1F>( "hemulti", "HE hits multiplicity", 60, 0., 3000. ); 
  hehitEneAllHisto_ = fs->make<TH1F>( "hehitEneAll", "HE hits energy", 1000, 0., 100. ); 
  hehit5EtaHisto_ = fs->make<TH1F>( "hehit5Eta", "HE hits abs eta .gt. 0.5", nEta, lowEta, highEta ); 
  hehit5EneHisto_ = fs->make<TH1F>( "hehit5Ene", "HE hits energy .gt. 0.5", 1000, 0., 100. ); 
  hehit5ETHisto_ = fs->make<TH1F>( "hehit5ET", "HE hits e_T .gt. 0.5", 500, 0., 50. ); 
  hehit5SumETHisto_ = fs->make<TH1F>( "hehit5SumET", "HE hits sum e_T .gt. 0.5", 500, 0., 500. ); 
  hehit5TimeHisto_ = fs->make<TH1F>( "hehit5Time", "HE hits time .gt. 0.5", 200, -100., 100. ); 

  he5TimeVSEneHisto_ = fs->make<TH2F>( "he5TimeVSEne", "HE hits .gt. 0.5 Time VS Ene", 50, 0., 100., 100, -100., 100. ); 

  he5SumEVSeta_ = fs->make<TProfile>( "he5SumEVSeta", "HE hits .gt. 0.5 SumE VS Eta", nEta, lowEta, highEta, 0., 500.); 
  he5SumETVSeta_ = fs->make<TProfile>( "he5SumETVSeta", "HE hits .gt. 0.5 SumET VS Eta", nEta, lowEta, highEta, 0., 500.); 

  he5SumETVSvtx_ = fs->make<TProfile>( "he5SumETVSvtx", "HE hits .gt. 0.5 SumET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  Char_t histo1[200];
  Char_t histo2[200];

  for ( int iVtx = 0; iVtx <= numvtx; iVtx++ ) {
    sprintf (histo1, "he5SumETvtx%02d", iVtx) ;
    sprintf (histo2, "HE hits sum e_T .gt. 0.5 vtx %02d", iVtx) ;
    he5SumETvtxHisto_.push_back(fs->make<TH1F>( histo1, histo2, 500, 0., 500. )); 
  }

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

HcalMinBiasAnalysis::~HcalMinBiasAnalysis() {}

void HcalMinBiasAnalysis::beginJob()
{
}

void HcalMinBiasAnalysis::endJob(){return;}
void HcalMinBiasAnalysis::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
}
void HcalMinBiasAnalysis::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void HcalMinBiasAnalysis::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
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

  // Start HCAL rec hit analysis

  edm::Handle<reco::VertexCollection> vtxCollection;
  iEvent.getByLabel(recoVertexCollection_, vtxCollection );
  
  unsigned int nVtx = 0;
  for ( unsigned int iloop = 0; iloop < (*vtxCollection).size(); iloop++ ) {
    if ( (*vtxCollection)[iloop].ndof() > 4 ) { nVtx++; }
  }

  hvtxHisto_->Fill((float)nVtx,theWeight);

  unsigned int nhe = 0;
    
  edm::Handle< HBHERecHitCollection > hbheRHCollection;
  iEvent.getByLabel(hbheRecHitCollection_, hbheRHCollection );
  
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);     

  edm::ESHandle<HcalChannelQuality> chanstat;
  iSetup.get<HcalChannelQualityRcd>().get( chanstat );
  const HcalChannelQuality* cstat=chanstat.product();      

  edm::ESHandle<HcalSeverityLevelComputer> hcalSevLvlComputerHndl;
  iSetup.get<HcalSeverityLevelComputerRcd>().get(hcalSevLvlComputerHndl);
  const HcalSeverityLevelComputer* hcalSevLvlComputer = hcalSevLvlComputerHndl.product();    

  std::vector<float> sumh5ET(numvtx+1,0.);
  
  std::vector<float> sum5Eeta(nEta,0.);
  std::vector<float> sum5ETeta(nEta,0.);

  for (HBHERecHitCollection::const_iterator myRecHit = hbheRHCollection->begin(); myRecHit != hbheRHCollection->end() ; ++myRecHit) {

    // select only HE hits

    if ( myRecHit->id().subdet() != 2 ) { continue; }

    // Quality selection on HCAL rec hits

    const uint32_t recHitFlag = myRecHit->flags();
    const uint32_t dbStatusFlag = cstat->getValues(myRecHit->id())->getValue();
    int severityLevel = hcalSevLvlComputer->getSeverityLevel(myRecHit->id(), recHitFlag, dbStatusFlag); 

    if ( severityLevel > 9 ) { continue; }

    nhe++;

    HcalDetId myHEiD(myRecHit->id());
    const CaloCellGeometry* cellGeometry = pG->getSubdetectorGeometry(myHEiD)->getGeometry(myHEiD) ;    
    double cellEta = std::fabs(cellGeometry->getPosition().eta());
    double tgThetaH = std::exp(-1.*cellEta);
    double sinTheta = 2.*tgThetaH/(1.+tgThetaH*tgThetaH);
    double hE = myRecHit->energy();
    double hET = hE*sinTheta;

    if ( (int)nVtx <= numvtx ) {
      if ( hET > 0.5 ) {sumh5ET[nVtx] += hET; }
    }
    
    if ( (int)nVtx == vtxSel_ ) {
      
      // ENDCAP
      
      double hTime = myRecHit->time();

      int ietaBin = (int)((float)nEta/(highEta-lowEta)*(cellEta-lowEta));
      ietaBin = (ietaBin < nEta) ? ietaBin : nEta-1; 

      hehitEneAllHisto_->Fill(hE,theWeight);

      if ( hET > 0.5 ) {
        
        hehit5EtaHisto_->Fill(cellEta,theWeight);
        hehit5EneHisto_->Fill(hE,theWeight);
        hehit5ETHisto_->Fill(hET,theWeight);
        hehit5TimeHisto_->Fill(hTime,theWeight);
        
        he5TimeVSEneHisto_->Fill(hE,hTime,theWeight);
     
        sum5Eeta[ietaBin] += hE;
        sum5ETeta[ietaBin] += hET;
        
      }
        
    }
    
  }

  if ( (int)nVtx == vtxSel_ ) {
      
    hemultiHisto_->Fill((float)nhe,theWeight);
    
    hehit5SumETHisto_->Fill(sumh5ET[vtxSel_],theWeight);
    
    for ( int ibin = 0; ibin < nEta; ibin++ ) {
      float eta = lowEta+(float)ibin*(highEta-lowEta)/(float)nEta;
      he5SumEVSeta_->Fill(eta,sum5Eeta[ibin],theWeight);
      he5SumETVSeta_->Fill(eta,sum5ETeta[ibin],theWeight);
    }
  }

  if ( (int)nVtx <= numvtx ) {
    he5SumETVSvtx_->Fill((float)nVtx,sumh5ET[nVtx],theWeight);
    he5SumETvtxHisto_[nVtx]->Fill(sumh5ET[nVtx],theWeight);
  }


}//analyze


//define this as a plug-in
DEFINE_FWK_MODULE(HcalMinBiasAnalysis);
