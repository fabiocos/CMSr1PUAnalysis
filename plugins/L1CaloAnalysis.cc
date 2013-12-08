/*class L1CaloAnalysis
 *  
 *  Class to study L1 Calo triggers
 *
 *  $Date: 2013/05/29 16:22:41 $
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

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

class L1CaloAnalysis : public edm::EDAnalyzer
{

public:
  explicit L1CaloAnalysis(const edm::ParameterSet&);
  virtual ~L1CaloAnalysis();
  virtual void beginJob();
  virtual void endJob();  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  
private:
  
  edm::InputTag recoVertexCollection_;
  edm::InputTag caloRegionCollection_; 
  edm::InputTag ettCollection_; 
  edm::InputTag httCollection_; 
  edm::InputTag etmCollection_; 
  edm::InputTag l1TrigCollection_;
  edm::InputTag caloTowerCollection_; 

  int vtxSel_;
  double etTh_;
  bool   cutCTEt_;
  double etCTTh_;
  
  TH1F * hvtxHisto_;

  // Barrel

  TH1F * CRBmultiHisto_;
  TH1F * CRBtotETHisto_;
  TH1F * CRBtotSumETHisto_;
  TProfile* CRBtotSumETVSvtx_;

  // Endcap

  TH1F * CREmultiHisto_;
  TH1F * CREtotETHisto_;
  TH1F * CREtotSumETHisto_;
  TProfile* CREtotSumETVSvtx_;

  // Forward

  TH1F * CRFmultiHisto_;
  TH1F * CRFtotETHisto_;
  TH1F * CRFtotSumETHisto_;
  TProfile* CRFtotSumETVSvtx_;

  // Total 

  TH1F * L1ETTHisto_;
  TH1F * L1HTTHisto_;
  TH1F * L1ETMHisto_;
  TProfile* L1ETTVSvtx_;
  TProfile* L1HTTVSvtx_;
  TProfile* L1ETMVSvtx_;

  TProfile* L1CRsumVSvtx_;

  // Trigger bits

  TH1F * L1TrigHisto_;
  TH1F * L1TrigRateHisto_;
  TH1F * L1TrigNorma_;

  TH1F* CRsumRank_;
  TH1F* CRsumEff_;

  TH1F * ETTRank_;
  TH1F * ETTEff_;

  TH1F * HTTRank_;
  TH1F * HTTEff_;

  TH1F * normaRank_;

  TProfile * ETTCTdiffVSvtx_;
  TProfile * ETTBarrelCTdiffVSvtx_;
  TProfile * ETTEndcapCTdiffVSvtx_;
  TProfile * ETTForwardCTdiffVSvtx_;
  TH2F *     ETTvsCT_;

  const int numvtx;
  const int nrankTh_;
  const float minRankTh_;

  edm::LumiReWeighting * theLumiW_;
  edm::InputTag puSummaryCollection_;
  std::string dataPUFile,mcPUFile,dataPUHisto,mcPUHisto;

  TH1F * truePUorigHisto_;
  TH1F * truePUreweHisto_;
  TH1F * weightHisto_;

  bool rewe_;

  bool first_;

};

using namespace edm;

L1CaloAnalysis::L1CaloAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  caloRegionCollection_(iPSet.getParameter<edm::InputTag>("caloRegionCollection")),
  ettCollection_(iPSet.getParameter<edm::InputTag>("ettCollection")),
  httCollection_(iPSet.getParameter<edm::InputTag>("httCollection")),
  etmCollection_(iPSet.getParameter<edm::InputTag>("etmCollection")),
  l1TrigCollection_(iPSet.getParameter<edm::InputTag>("l1TrigCollection")),
  caloTowerCollection_(iPSet.getParameter<edm::InputTag>("caloTowerCollection")),
  vtxSel_(iPSet.getParameter<int>("vtxSel")),
  etTh_(iPSet.getParameter<double>("etTh")),
  cutCTEt_(iPSet.getParameter<bool>("cutCTEt")),
  etCTTh_(iPSet.getParameter<double>("etCTTh")),
  numvtx(60),nrankTh_(300),minRankTh_(10.),first_(true)
{    

  edm::Service<TFileService> fs;

  hvtxHisto_ = fs->make<TH1F>( "vtx", "vertex ndof.gt.4 multiplicity", 100, 0., 100. ) ;

  CRBmultiHisto_ = fs->make<TH1F>( "CRBmulti", "L1 CaloRegion barrel multiplicity", 50, 0., 200. ); 

  CRBtotETHisto_ = fs->make<TH1F>( "CRBtotET", "L1 CaloRegion barrel tot E_T", 500, 0., 50. ); 
  CRBtotSumETHisto_ = fs->make<TH1F>( "CRBtotSumET", "L1 CaloRegion barrel tot sum E_T", 500, 0., 500. ); 
  CRBtotSumETVSvtx_ = fs->make<TProfile>( "CRBtotSumETVSvtx", "L1 CaloRegion barrel tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  CREmultiHisto_ = fs->make<TH1F>( "CREmulti", "L1 CaloRegion endcap multiplicity", 50, 0., 200. ); 

  CREtotETHisto_ = fs->make<TH1F>( "CREtotET", "L1 CaloRegion endcap tot E_T", 500, 0., 50. ); 
  CREtotSumETHisto_ = fs->make<TH1F>( "CREtotSumET", "L1 CaloRegion endcap tot sum E_T", 500, 0., 500. ); 
  CREtotSumETVSvtx_ = fs->make<TProfile>( "CREtotSumETVSvtx", "L1 CaloRegion endcap tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  CRFmultiHisto_ = fs->make<TH1F>( "CRFmulti", "L1 CaloRegion forward multiplicity", 50, 0., 200. ); 

  CRFtotETHisto_ = fs->make<TH1F>( "CRFtotET", "L1 CaloRegion forward tot E_T", 500, 0., 50. ); 
  CRFtotSumETHisto_ = fs->make<TH1F>( "CRFtotSumET", "L1 CaloRegion forward tot sum E_T", 500, 0., 500. ); 
  CRFtotSumETVSvtx_ = fs->make<TProfile>( "CRFtotSumETVSvtx", "L1 CaloRegion forward tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  L1ETTHisto_ = fs->make<TH1F>( "L1ETT", "L1 ETT", 500, 0., 1000. ); 
  L1HTTHisto_ = fs->make<TH1F>( "L1HTT", "L1 HTT", 500, 0., 1000. ); 
  L1ETMHisto_ = fs->make<TH1F>( "L1ETM", "L1 ETM", 500, 0., 1000. ); 

  L1ETTVSvtx_ = fs->make<TProfile>( "L1ETTVSvtx", "L1 ETT VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 
  L1HTTVSvtx_ = fs->make<TProfile>( "L1HTTVSvtx", "L1 HTT VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 
  L1ETMVSvtx_ = fs->make<TProfile>( "L1ETMVSvtx", "L1 ETM VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 

  L1CRsumVSvtx_ = fs->make<TProfile>( "L1CRsumVSvtx", "L1 CaloRegion sum VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 

  L1TrigHisto_ = fs->make<TH1F>( "L1Trig", "L1 Trigger bits", 128, -.5, 127.5);
  L1TrigHisto_->Sumw2();
  L1TrigRateHisto_ = fs->make<TH1F>( "L1TrigRate", "L1 Trigger bits rate", 128, -.5, 127.5);
  L1TrigRateHisto_->Sumw2();
  L1TrigNorma_ = fs->make<TH1F>( "L1TrigNorma", "L1 Trigger bits normalization", 128, -.5, 127.5);
  L1TrigNorma_->Sumw2();

  float maxRankTh_ = minRankTh_+(float)nrankTh_ ;

  CRsumRank_ = fs->make<TH1F>( "CRsumRank", "number of events passing CRsum threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  CRsumRank_->Sumw2();
  CRsumEff_ = fs->make<TH1F>( "CRsumEff", "efficiency passing CRsum threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  CRsumEff_->Sumw2();

  ETTRank_ = fs->make<TH1F>( "ETTRank", "number of events passing ETT threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  ETTRank_->Sumw2();
  ETTEff_ = fs->make<TH1F>( "ETTEff", "efficiency passing ETT threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  ETTEff_->Sumw2();

  HTTRank_ = fs->make<TH1F>( "HTTRank", "number of events passing HTT threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  HTTRank_->Sumw2();
  HTTEff_ = fs->make<TH1F>( "HTTEff", "efficiency passing HTT threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  HTTEff_->Sumw2();

  normaRank_ = fs->make<TH1F>( "normaRank", "number of events with at least 1 vtx", nrankTh_, minRankTh_, maxRankTh_ ); 
  normaRank_->Sumw2();

  // Cross check with CaloTowers

  ETTCTdiffVSvtx_ = fs->make<TProfile>( "ETTCTdiffVSvtx", "difference ETT - sumET CaloTowers VS Vtx ", numvtx, 0., (float)numvtx, -200., 100.);
  ETTBarrelCTdiffVSvtx_ = fs->make<TProfile>( "ETTBarrelCTdiffVSvtx", "difference ETT - sumET CaloTowers Barrel VS Vtx ", numvtx, 0., (float)numvtx, -200., 100.);
  ETTEndcapCTdiffVSvtx_ = fs->make<TProfile>( "ETTEndcapCTdiffVSvtx", "difference ETT - sumET CaloTowers Endcap VS Vtx ", numvtx, 0., (float)numvtx, -200., 100.);
  ETTForwardCTdiffVSvtx_ = fs->make<TProfile>( "ETTForwardCTdiffVSvtx", "difference ETT - sumET CaloTowers Forward VS Vtx ", numvtx, 0., (float)numvtx, -200., 100.);
  ETTvsCT_ = fs->make<TH2F>( "ETTvsCT", "ETT vs sumET CaloTowers", 40, 0., 400., 40, 0., 400. ); 

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

L1CaloAnalysis::~L1CaloAnalysis() {}

void L1CaloAnalysis::beginJob()
{
}

void L1CaloAnalysis::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
}
void L1CaloAnalysis::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void L1CaloAnalysis::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
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

  // Start L1Calo analysis

  edm::Handle<reco::VertexCollection> vtxCollection;
  iEvent.getByLabel(recoVertexCollection_, vtxCollection );
  
  unsigned int nVtx = 0;
  for ( unsigned int iloop = 0; iloop < (*vtxCollection).size(); iloop++ ) {
    if ( (*vtxCollection)[iloop].ndof() > 4 ) { nVtx++; }
  }

  hvtxHisto_->Fill((float)nVtx,theWeight);

  edm::Handle<L1CaloRegionCollection> towers;
  iEvent.getByLabel(caloRegionCollection_,towers);

  std::vector<float> sumBCR(numvtx+1,0.);
  std::vector<float> sumECR(numvtx+1,0.);
  std::vector<float> sumFCR(numvtx+1,0.);

  double totSumBCR = 0.;
  double totSumECR = 0.;
  double totSumFCR = 0.;

  double totSumCR = 0.;
  double totBASumCR = 0.;
  double totENSumCR = 0.;
  double totFOSumCR = 0.;
  
  int nCRB =0;
  int nCRE= 0;
  int nCRF= 0;

  bool select((int)nVtx == vtxSel_);
  
  for (L1CaloRegionCollection::const_iterator r = towers->begin(); r != towers->end() ; ++r) {

    int ieta = r->gctEta();
    double rET = 0.5*r->et();

    bool eneSelect( rET >= etTh_ );

    if ( ! eneSelect) { continue; }

    // HF

    if (r->isHf()) {
      if ( select ) {
        nCRF++;
        CRFtotETHisto_->Fill(rET, theWeight);
        totSumFCR += rET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumFCR[nVtx] += rET; 
        totFOSumCR += rET;
      }
    } 
    
    // Endcap

    else if (ieta < 7 || ieta > 14) {
      if ( select ) {
        nCRE++;
        CREtotETHisto_->Fill(rET, theWeight);
        totSumECR += rET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumECR[nVtx] += rET; 
        totSumCR += rET;
        totENSumCR += rET;
      }
    } 

    // Barrel 

    else {
      if ( select ) {
        nCRB++;
        CRBtotETHisto_->Fill(rET, theWeight);
        totSumBCR += rET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumBCR[nVtx] += rET; 
        totSumCR += rET;
        totBASumCR += rET;
      }
    }    


  }

  // CaloTower analysis for comparison with Offline quantities

  edm::Handle<CaloTowerCollection> CTtowers;
  iEvent.getByLabel(caloTowerCollection_,CTtowers);
  CaloTowerCollection::const_iterator cal;

  double totSumCT = 0.;
  double totBASumCT = 0.;
  double totENSumCT = 0.;
  double totFOSumCT = 0.;

  for (CaloTowerCollection::const_iterator cal = CTtowers->begin(); cal != CTtowers->end() ; ++cal) {

    double totE = cal->energy();
    double totET = cal->et();

    //    std::cout << "CT Energy " << emE << " " << hadE << " " << totE << std::endl; 
    //    std::cout << "CT ET     " << emET << " " << hadET << " " << totET << std::endl; 

    bool eneCTSelect(totET >= etCTTh_ );
    if ( ! cutCTEt_ ) { eneCTSelect = totE >= etCTTh_ ; }

    //    std::cout << "cutEt = " << cutEt_ << " eneSelect = " << eneSelect << std::endl;

    if ( ! eneCTSelect) { continue; }

    if ( std::fabs(cal->eta()) <= 3. && nVtx > 0 ) {
      if ( (int)nVtx <= numvtx ) {
        totSumCT += totET; 
      }
    }
    
    if ( std::fabs(cal->eta()) <= 1.48 ) {
      if ( (int)nVtx <= numvtx ) {
        totBASumCT += totET; 
      }
    }
    else if ( std::fabs(cal->eta()) > 1.48 && std::fabs(cal->eta()) <= 3.) {
      if ( (int)nVtx <= numvtx ) {
        totENSumCT += totET; 
      }
    }
    else {
      if ( (int)nVtx <= numvtx ) {
        totFOSumCT += totET; 
      }
    }

  }


  if ( nVtx > 0 ) {

    //    std::cout << "L1: totSumCR = " << totSumCR << " totSumCT = " << totSumCT << std::endl;

    L1CRsumVSvtx_->Fill((float)nVtx,totSumCR,theWeight);
    ETTCTdiffVSvtx_->Fill((float)nVtx,totSumCR-totSumCT,theWeight);
    ETTvsCT_->Fill(totSumCT,totSumCR,theWeight);
    ETTBarrelCTdiffVSvtx_->Fill((float)nVtx,totBASumCR-totBASumCT,theWeight);
    ETTEndcapCTdiffVSvtx_->Fill((float)nVtx,totENSumCR-totENSumCT,theWeight);
    ETTForwardCTdiffVSvtx_->Fill((float)nVtx,totFOSumCR-totFOSumCT,theWeight);
    for ( int irank = 0; irank < nrankTh_; irank++ ) {
      float threshold = minRankTh_+(float)irank;
      if ( totSumCR > threshold ) { CRsumRank_->Fill(threshold+0.5,theWeight); }
      normaRank_->Fill(threshold+0.5,theWeight); 
    }

    edm::Handle<L1GctEtTotalCollection> et_tots;
    iEvent.getByLabel(ettCollection_, et_tots);
    for (L1GctEtTotalCollection::const_iterator et = et_tots->begin();
         et != et_tots->end(); ++et) {
      if ( et->bx() == 0 ) {
        if ( select ) L1ETTHisto_->Fill(et->et()*0.5,theWeight);
        L1ETTVSvtx_->Fill((float)nVtx,et->et()*0.5,theWeight);
        //        std::cout << "L1: ETT = " << et->et()*0.5 << std::endl; 

        for ( int irank = 0; irank < nrankTh_; irank++ ) {
          float threshold = minRankTh_+(float)irank;
          if ( et->et()*0.5 > threshold ) { ETTRank_->Fill(threshold+0.5,theWeight); }
        }
        
      }
    }
    
    edm::Handle<L1GctEtMissCollection> et_misss;
    iEvent.getByLabel(etmCollection_, et_misss);
    for (L1GctEtMissCollection::const_iterator et = et_misss->begin();
         et != et_misss->end(); ++et) {
      if ( et->bx() == 0 ) {
        if ( select ) L1ETMHisto_->Fill(et->et()*0.5,theWeight);
        L1ETMVSvtx_->Fill((float)nVtx,et->et()*0.5,theWeight);
      }
    }
    
    edm::Handle<L1GctEtHadCollection> et_hads;
    iEvent.getByLabel(httCollection_, et_hads);
    for (L1GctEtHadCollection::const_iterator et = et_hads->begin();
         et != et_hads->end(); ++et) {
      if ( et->bx() == 0 ) {
        if ( select ) L1HTTHisto_->Fill(et->et()*0.5,theWeight);
        L1HTTVSvtx_->Fill((float)nVtx,et->et()*0.5,theWeight);
        
        for ( int irank = 0; irank < nrankTh_; irank++ ) {
          float threshold = minRankTh_+(float)irank;
          if ( et->et()*0.5 > threshold ) { HTTRank_->Fill(threshold+0.5,theWeight); }
        }

      }
    }
    
    
    if (first_) {
      first_ = false;
      edm::ESHandle<L1GtTriggerMenu> l1menu;
      iSetup.get<L1GtTriggerMenuRcd>().get(l1menu);
      for (const auto& p: l1menu->gtAlgorithmMap()) {
        L1TrigHisto_->GetXaxis()->SetBinLabel(p.second.algoBitNumber() + 1, p.first.c_str());
        L1TrigNorma_->GetXaxis()->SetBinLabel(p.second.algoBitNumber() + 1, p.first.c_str());
        L1TrigRateHisto_->GetXaxis()->SetBinLabel(p.second.algoBitNumber() + 1, p.first.c_str());
      }
    }
    
    edm::Handle<L1GlobalTriggerReadoutRecord> bits;
    iEvent.getByLabel(l1TrigCollection_, bits);
    for (int i = 0; i < 128; ++i) {
      L1TrigHisto_->Fill((float)i, bits->decisionWord()[i] * theWeight);
      L1TrigNorma_->Fill((float)i, theWeight);
    }
    
  }
  
  if ( select ) {
    if ( nCRB > 0 ) { CRBmultiHisto_->Fill(nCRB,theWeight); }
    if ( nCRE > 0 ) { CREmultiHisto_->Fill(nCRE,theWeight); }
    if ( nCRF > 0 ) { CRFmultiHisto_->Fill(nCRF,theWeight); }
    if ( totSumBCR > 0. ) { CRBtotSumETHisto_->Fill(totSumBCR,theWeight); }
    if ( totSumECR > 0. ) { CREtotSumETHisto_->Fill(totSumECR,theWeight); }
    if ( totSumFCR > 0. ) { CRFtotSumETHisto_->Fill(totSumFCR,theWeight); }
  }

  if ( (int)nVtx <= numvtx ) {
    if ( sumBCR[nVtx] > 0. ) { CRBtotSumETVSvtx_->Fill((float)nVtx,sumBCR[nVtx],theWeight); }
    if ( sumECR[nVtx] > 0. ) { CREtotSumETVSvtx_->Fill((float)nVtx,sumECR[nVtx],theWeight); }
    if ( sumFCR[nVtx] > 0. ) { CRFtotSumETVSvtx_->Fill((float)nVtx,sumFCR[nVtx],theWeight); }
  }


}//analyze

void L1CaloAnalysis::endJob(){

  CRsumEff_->Divide(CRsumRank_,normaRank_,1.,1.,"B");
  ETTEff_->Divide(ETTRank_,normaRank_,1.,1.,"B");
  HTTEff_->Divide(HTTRank_,normaRank_,1.,1.,"B");
  L1TrigRateHisto_->Divide(L1TrigHisto_,L1TrigNorma_,1.,1.,"B");

  return;

}


//define this as a plug-in
DEFINE_FWK_MODULE(L1CaloAnalysis);
