/*class CaloTowerAnalysis
 *  
 *  Class to study ECAL response in Minimum Bias events
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

class CaloTowerAnalysis : public edm::EDAnalyzer
{

public:
  explicit CaloTowerAnalysis(const edm::ParameterSet&);
  virtual ~CaloTowerAnalysis();
  virtual void beginJob();
  virtual void endJob();  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  
private:
  
  edm::InputTag recoVertexCollection_;
  edm::InputTag caloTowerCollection_; 

  int vtxSel_;
  bool   cutEt_;
  double etTh_,etEmTh_,etHadTh_;
  
  TH1F * hvtxHisto_;

  // Barrel

  TH1F * CTBmultiHisto_;
  TH1F * CTBemEHisto_;
  TH1F * CTBtotEHisto_;
  TH1F * CTBhadEHisto_;
  TH1F * CTBemETHisto_;
  TH1F * CTBtotETHisto_;
  TH1F * CTBhadETHisto_;
  TH1F * CTBemSumETHisto_;
  TH1F * CTBtotSumETHisto_;
  TH1F * CTBhadSumETHisto_;
  TProfile* CTBemSumETVSvtx_;
  TProfile* CTBhadSumETVSvtx_;
  TProfile* CTBtotSumETVSvtx_;

  // Endcap

  TH1F * CTEmultiHisto_;
  TH1F * CTEemEHisto_;
  TH1F * CTEtotEHisto_;
  TH1F * CTEhadEHisto_;
  TH1F * CTEemETHisto_;
  TH1F * CTEtotETHisto_;
  TH1F * CTEhadETHisto_;
  TH1F * CTEemSumETHisto_;
  TH1F * CTEtotSumETHisto_;
  TH1F * CTEhadSumETHisto_;
  TProfile* CTEemSumETVSvtx_;
  TProfile* CTEhadSumETVSvtx_;
  TProfile* CTEtotSumETVSvtx_;

  // Forward

  TH1F * CTFmultiHisto_;
  TH1F * CTFemEHisto_;
  TH1F * CTFtotEHisto_;
  TH1F * CTFhadEHisto_;
  TH1F * CTFemETHisto_;
  TH1F * CTFtotETHisto_;
  TH1F * CTFhadETHisto_;
  TH1F * CTFemSumETHisto_;
  TH1F * CTFtotSumETHisto_;
  TH1F * CTFhadSumETHisto_;
  TProfile* CTFemSumETVSvtx_;
  TProfile* CTFhadSumETVSvtx_;
  TProfile* CTFtotSumETVSvtx_;

  // Total 

  TH1F * CTemSumETHisto_;
  TH1F * CTtotSumETHisto_;
  TH1F * CThadSumETHisto_;
  TProfile* CTemSumETVSvtx_;
  TProfile* CThadSumETVSvtx_;
  TProfile* CTtotSumETVSvtx_;

  TH1F * CTemSumRank_;
  TH1F * CThadSumRank_;
  TH1F * CTtotSumRank_;

  TH1F * CTemEff_;
  TH1F * CThadEff_;
  TH1F * CTtotEff_;

  TH1F * CTnormaRank_;

  // High ET tail

  TH1F * vtxSumETtailHisto_;

  TH1F * CTBtmultiHisto_;
  TH1F * CTBtemETHisto_;
  TH1F * CTBttotETHisto_;
  TH1F * CTBthadETHisto_;
  TH1F * CTBtemSumETHisto_;
  TH1F * CTBttotSumETHisto_;
  TH1F * CTBthadSumETHisto_;

  TH1F * CTEtmultiHisto_;
  TH1F * CTEtemETHisto_;
  TH1F * CTEttotETHisto_;
  TH1F * CTEthadETHisto_;
  TH1F * CTEtemSumETHisto_;
  TH1F * CTEttotSumETHisto_;
  TH1F * CTEthadSumETHisto_;

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

};

using namespace edm;

CaloTowerAnalysis::CaloTowerAnalysis(const edm::ParameterSet& iPSet):  
  recoVertexCollection_(iPSet.getParameter<edm::InputTag>("recoVertexCollection")),
  caloTowerCollection_(iPSet.getParameter<edm::InputTag>("caloTowerCollection")),
  vtxSel_(iPSet.getParameter<int>("vtxSel")),
  cutEt_(iPSet.getParameter<bool>("cutEt")),
  etTh_(iPSet.getParameter<double>("etTh")),
  etEmTh_(iPSet.getParameter<double>("etEmTh")),
  etHadTh_(iPSet.getParameter<double>("etHadTh")),
  numvtx(60),nrankTh_(300),minRankTh_(10.)
{    

  edm::Service<TFileService> fs;

  hvtxHisto_ = fs->make<TH1F>( "vtx", "vertex ndof.gt.4 multiplicity", 100, 0., 100. ) ;

  CTBmultiHisto_ = fs->make<TH1F>( "CTBmulti", "CaloTower barrel multiplicity", 50, 0., 200. ); 

  CTBemEHisto_ = fs->make<TH1F>( "CTBemE", "CaloTower barrel em energy", 100, 0., 100. ); 
  CTBhadEHisto_ = fs->make<TH1F>( "CTBhadE", "CaloTower barrel had energy", 100, 0., 100. ); 
  CTBtotEHisto_ = fs->make<TH1F>( "CTBtotE", "CaloTower barrel tot energy", 100, 0., 100. ); 
  CTBemETHisto_ = fs->make<TH1F>( "CTBemET", "CaloTower barrel em E_T", 500, 0., 50. ); 
  CTBhadETHisto_ = fs->make<TH1F>( "CTBhadET", "CaloTower barrel had E_T", 500, 0., 50. ); 
  CTBtotETHisto_ = fs->make<TH1F>( "CTBtotET", "CaloTower barrel tot E_T", 500, 0., 50. ); 
  CTBemSumETHisto_ = fs->make<TH1F>( "CTBemSumET", "CaloTower barrel em sum E_T", 500, 0., 500. ); 
  CTBhadSumETHisto_ = fs->make<TH1F>( "CTBhadSumET", "CaloTower barrel had sum E_T", 500, 0., 500. ); 
  CTBtotSumETHisto_ = fs->make<TH1F>( "CTBtotSumET", "CaloTower barrel tot sum E_T", 500, 0., 500. ); 

  CTBemSumETVSvtx_ = fs->make<TProfile>( "CTBemSumETVSvtx", "CaloTower barrel em sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTBhadSumETVSvtx_ = fs->make<TProfile>( "CTBhadSumETVSvtx", "CaloTower barrel had sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTBtotSumETVSvtx_ = fs->make<TProfile>( "CTBtotSumETVSvtx", "CaloTower barrel tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  CTEmultiHisto_ = fs->make<TH1F>( "CTEmulti", "CaloTower endcap multiplicity", 50, 0., 200. ); 

  CTEemEHisto_ = fs->make<TH1F>( "CTEemE", "CaloTower endcap em energy", 100, 0., 100. ); 
  CTEhadEHisto_ = fs->make<TH1F>( "CTEhadE", "CaloTower endcap had energy", 100, 0., 100. ); 
  CTEtotEHisto_ = fs->make<TH1F>( "CTEtotE", "CaloTower endcap tot energy", 100, 0., 100. ); 
  CTEemETHisto_ = fs->make<TH1F>( "CTEemET", "CaloTower endcap em E_T", 500, 0., 50. ); 
  CTEhadETHisto_ = fs->make<TH1F>( "CTEhadET", "CaloTower endcap had E_T", 500, 0., 50. ); 
  CTEtotETHisto_ = fs->make<TH1F>( "CTEtotET", "CaloTower endcap tot E_T", 500, 0., 50. ); 
  CTEemSumETHisto_ = fs->make<TH1F>( "CTEemSumET", "CaloTower endcap em sum E_T", 500, 0., 500. ); 
  CTEhadSumETHisto_ = fs->make<TH1F>( "CTEhadSumET", "CaloTower endcap had sum E_T", 500, 0., 500. ); 
  CTEtotSumETHisto_ = fs->make<TH1F>( "CTEtotSumET", "CaloTower endcap tot sum E_T", 500, 0., 500. ); 

  CTEemSumETVSvtx_ = fs->make<TProfile>( "CTEemSumETVSvtx", "CaloTower endcap em sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTEhadSumETVSvtx_ = fs->make<TProfile>( "CTEhadSumETVSvtx", "CaloTower endcap had sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTEtotSumETVSvtx_ = fs->make<TProfile>( "CTEtotSumETVSvtx", "CaloTower endcap tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  CTFmultiHisto_ = fs->make<TH1F>( "CTFmulti", "CaloTower forward multiplicity", 50, 0., 200. ); 

  CTFemEHisto_ = fs->make<TH1F>( "CTFemE", "CaloTower forward em energy", 100, 0., 100. ); 
  CTFhadEHisto_ = fs->make<TH1F>( "CTFhadE", "CaloTower forward had energy", 100, 0., 100. ); 
  CTFtotEHisto_ = fs->make<TH1F>( "CTFtotE", "CaloTower forward tot energy", 100, 0., 100. ); 
  CTFemETHisto_ = fs->make<TH1F>( "CTFemET", "CaloTower forward em E_T", 500, 0., 50. ); 
  CTFhadETHisto_ = fs->make<TH1F>( "CTFhadET", "CaloTower forward had E_T", 500, 0., 50. ); 
  CTFtotETHisto_ = fs->make<TH1F>( "CTFtotET", "CaloTower forward tot E_T", 500, 0., 50. ); 
  CTFemSumETHisto_ = fs->make<TH1F>( "CTFemSumET", "CaloTower forward em sum E_T", 500, 0., 500. ); 
  CTFhadSumETHisto_ = fs->make<TH1F>( "CTFhadSumET", "CaloTower forward had sum E_T", 500, 0., 500. ); 
  CTFtotSumETHisto_ = fs->make<TH1F>( "CTFtotSumET", "CaloTower forward tot sum E_T", 500, 0., 500. ); 

  CTFemSumETVSvtx_ = fs->make<TProfile>( "CTFemSumETVSvtx", "CaloTower forward em sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTFhadSumETVSvtx_ = fs->make<TProfile>( "CTFhadSumETVSvtx", "CaloTower forward had sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 
  CTFtotSumETVSvtx_ = fs->make<TProfile>( "CTFtotSumETVSvtx", "CaloTower forward tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 500.); 

  CTemSumETHisto_ = fs->make<TH1F>( "CTemSumET", "CaloTower all em sum E_T", 500, 0., 1000. ); 
  CThadSumETHisto_ = fs->make<TH1F>( "CThadSumET", "CaloTower all had sum E_T", 500, 0., 1000. ); 
  CTtotSumETHisto_ = fs->make<TH1F>( "CTtotSumET", "CaloTower all tot sum E_T", 500, 0., 1000. ); 

  CTemSumETVSvtx_ = fs->make<TProfile>( "CTemSumETVSvtx", "CaloTower all em sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 
  CThadSumETVSvtx_ = fs->make<TProfile>( "CThadSumETVSvtx", "CaloTower all had sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 
  CTtotSumETVSvtx_ = fs->make<TProfile>( "CTtotSumETVSvtx", "CaloTower all tot sum ET VS Vtx", numvtx, 0., (float)numvtx, 0., 1000.); 

  float maxRankTh_ = minRankTh_+(float)nrankTh_ ;

  CTemSumRank_ = fs->make<TH1F>( "CTemSumRank", "number of events passing sumET em threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  CTemSumRank_->Sumw2();
  CThadSumRank_ = fs->make<TH1F>( "CThadSumRank", "number of events passing sumET had threshold", nrankTh_, minRankTh_, maxRankTh_ ); 
  CThadSumRank_->Sumw2();
  CTtotSumRank_ = fs->make<TH1F>( "CTtotSumRank", "number of events passing sumET tot threshold", nrankTh_, minRankTh_, maxRankTh_ ); 
  CTtotSumRank_->Sumw2();

  CTemEff_ = fs->make<TH1F>( "CTemEff", "efficiency passing sumET em threshold", nrankTh_, minRankTh_, maxRankTh_  ); 
  CTemEff_->Sumw2();
  CThadEff_ = fs->make<TH1F>( "CThadEff", "efficiency passing sumET had threshold", nrankTh_, minRankTh_, maxRankTh_ ); 
  CThadEff_->Sumw2();
  CTtotEff_ = fs->make<TH1F>( "CTtotEff", "efficiency passing sumET tot threshold", nrankTh_, minRankTh_, maxRankTh_ ); 
  CTtotEff_->Sumw2();

  CTnormaRank_ = fs->make<TH1F>( "CTnormaRank", "number of events with at least 1 vtx", nrankTh_, minRankTh_, maxRankTh_ ); 
  CTnormaRank_->Sumw2();

  vtxSumETtailHisto_ = fs->make<TH1F>( "vtxSumETtail", "vertex ndof.gt.4 multiplicity high ET tail", 100, 0., 100. ) ;

  CTBtmultiHisto_ = fs->make<TH1F>( "CTBtmulti", "CaloTower barrel high ET tail multiplicity", 50, 0., 200. ); 

  CTBtemETHisto_ = fs->make<TH1F>( "CTBtemET", "CaloTower barrel high ET tail em E_T", 500, 0., 50. ); 
  CTBthadETHisto_ = fs->make<TH1F>( "CTBthadET", "CaloTower barrel high ET tail had E_T", 500, 0., 50. ); 
  CTBttotETHisto_ = fs->make<TH1F>( "CTBttotET", "CaloTower barrel high ET tail tot E_T", 500, 0., 50. ); 
  CTBtemSumETHisto_ = fs->make<TH1F>( "CTBtemSumET", "CaloTower barrel high ET tail em sum E_T", 500, 0., 500. ); 
  CTBthadSumETHisto_ = fs->make<TH1F>( "CTBthadSumET", "CaloTower barrel high ET tail had sum E_T", 500, 0., 500. ); 
  CTBttotSumETHisto_ = fs->make<TH1F>( "CTBttotSumET", "CaloTower barrel high ET tail tot sum E_T", 500, 0., 500. ); 

  CTEtmultiHisto_ = fs->make<TH1F>( "CTEtmulti", "CaloTower endcap high ET tail multiplicity", 50, 0., 200. ); 

  CTEtemETHisto_ = fs->make<TH1F>( "CTEtemET", "CaloTower endcap high ET tail em E_T", 500, 0., 50. ); 
  CTEthadETHisto_ = fs->make<TH1F>( "CTEthadET", "CaloTower endcap high ET tail had E_T", 500, 0., 50. ); 
  CTEttotETHisto_ = fs->make<TH1F>( "CTEttotET", "CaloTower endcap high ET tail tot E_T", 500, 0., 50. ); 
  CTEtemSumETHisto_ = fs->make<TH1F>( "CTEtemSumET", "CaloTower endcap high ET tail em sum E_T", 500, 0., 500. ); 
  CTEthadSumETHisto_ = fs->make<TH1F>( "CTEthadSumET", "CaloTower endcap high ET tail had sum E_T", 500, 0., 500. ); 
  CTEttotSumETHisto_ = fs->make<TH1F>( "CTEttotSumET", "CaloTower endcap high ET tail tot sum E_T", 500, 0., 500. ); 

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

CaloTowerAnalysis::~CaloTowerAnalysis() {}

void CaloTowerAnalysis::beginJob()
{
}

void CaloTowerAnalysis::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
}
void CaloTowerAnalysis::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void CaloTowerAnalysis::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
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

  // Start CaloTower analysis

  edm::Handle<reco::VertexCollection> vtxCollection;
  iEvent.getByLabel(recoVertexCollection_, vtxCollection );
  
  unsigned int nVtx = 0;
  for ( unsigned int iloop = 0; iloop < (*vtxCollection).size(); iloop++ ) {
    if ( (*vtxCollection)[iloop].ndof() > 4 ) { nVtx++; }
  }

  hvtxHisto_->Fill((float)nVtx,theWeight);

  edm::Handle<CaloTowerCollection> towers;
  iEvent.getByLabel(caloTowerCollection_,towers);
  CaloTowerCollection::const_iterator cal;

  std::vector<float> sumBEmET(numvtx+1,0.);
  std::vector<float> sumBHadET(numvtx+1,0.);
  std::vector<float> sumBTotET(numvtx+1,0.);

  std::vector<float> sumEEmET(numvtx+1,0.);
  std::vector<float> sumEHadET(numvtx+1,0.);
  std::vector<float> sumETotET(numvtx+1,0.);

  std::vector<float> sumFEmET(numvtx+1,0.);
  std::vector<float> sumFHadET(numvtx+1,0.);
  std::vector<float> sumFTotET(numvtx+1,0.);

  std::vector<float> sumEmET(numvtx+1,0.);
  std::vector<float> sumHadET(numvtx+1,0.);
  std::vector<float> sumTotET(numvtx+1,0.);

  double emSumBET = 0.;
  double hadSumBET = 0.;
  double totSumBET = 0.;
  
  double emSumEET = 0.;
  double hadSumEET = 0.;
  double totSumEET = 0.;
  
  double emSumFET = 0.;
  double hadSumFET = 0.;
  double totSumFET = 0.;
  
  double emSumET = 0.;
  double hadSumET = 0.;
  double totSumET = 0.;

  int nCTB =0;
  int nCTE= 0;
  int nCTF= 0;

  bool select((int)nVtx == vtxSel_);
  
  for (CaloTowerCollection::const_iterator cal = towers->begin(); cal != towers->end() ; ++cal) {

    double emE = cal->emEnergy();
    double hadE = cal->hadEnergy();
    double totE = cal->energy();
    double emET = cal->emEt();
    double hadET = cal->hadEt();
    double totET = cal->et();

    //    std::cout << "CT Energy " << emE << " " << hadE << " " << totE << std::endl; 
    //    std::cout << "CT ET     " << emET << " " << hadET << " " << totET << std::endl; 

    bool eneSelect(totET >= etTh_ && emET >= etEmTh_ && hadET >= etHadTh_);
    if ( ! cutEt_ ) { eneSelect = totE >= etTh_ && emE >= etEmTh_ && hadE >= etHadTh_ ; }

    //    std::cout << "cutEt = " << cutEt_ << " eneSelect = " << eneSelect << std::endl;

    if ( ! eneSelect) { continue; }

    if ( std::fabs(cal->eta()) <= 3. && nVtx > 0 ) {
      totSumET += totET;
      emSumET += emET;
      hadSumET += hadET;
      if ( (int)nVtx <= numvtx ) {
        sumEmET[nVtx] += emET; 
        sumHadET[nVtx] += hadET; 
        sumTotET[nVtx] += totET; 
      }
    }
    
    if ( std::fabs(cal->eta()) <= 1.48 ) {
      if ( select ) {
        nCTB++;
        CTBemEHisto_->Fill(emE,theWeight);
        CTBhadEHisto_->Fill(hadE,theWeight);
        CTBtotEHisto_->Fill(totE,theWeight);
        CTBemETHisto_->Fill(emET,theWeight);
        CTBhadETHisto_->Fill(hadET,theWeight);
        CTBtotETHisto_->Fill(totET,theWeight);
        emSumBET += emET;
        hadSumBET += hadET;
        totSumBET += totET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumBEmET[nVtx] += emET; 
        sumBHadET[nVtx] += hadET; 
        sumBTotET[nVtx] += totET; 
      }
    }
    else if ( std::fabs(cal->eta()) > 1.48 && std::fabs(cal->eta()) <= 3.) {
      if ( select ) {
        nCTE++;
        CTEemEHisto_->Fill(emE,theWeight);
        CTEhadEHisto_->Fill(hadE,theWeight);
        CTEtotEHisto_->Fill(totE,theWeight);
        CTEemETHisto_->Fill(emET,theWeight);
        CTEhadETHisto_->Fill(hadET,theWeight);
        CTEtotETHisto_->Fill(totET,theWeight);
        emSumEET += emET;
        hadSumEET += hadET;
        totSumEET += totET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumEEmET[nVtx] += emET; 
        sumEHadET[nVtx] += hadET; 
        sumETotET[nVtx] += totET; 
      }
    }
    else {
      if ( select ) {
        nCTF++;
        CTFemEHisto_->Fill(emE,theWeight);
        CTFhadEHisto_->Fill(hadE,theWeight);
        CTFtotEHisto_->Fill(totE,theWeight);
        CTFemETHisto_->Fill(emET,theWeight);
        CTFhadETHisto_->Fill(hadET,theWeight);
        CTFtotETHisto_->Fill(totET,theWeight);
        emSumFET += emET;
        hadSumFET += hadET;
        totSumFET += totET;
      }
      if ( (int)nVtx <= numvtx ) {
        sumFEmET[nVtx] += emET; 
        sumFHadET[nVtx] += hadET; 
        sumFTotET[nVtx] += totET; 
      }
    }

  }

  if ( select ) {
    if ( nCTB > 0 ) { CTBmultiHisto_->Fill(nCTB,theWeight); }
    if ( nCTE > 0 ) { CTEmultiHisto_->Fill(nCTE,theWeight); }
    if ( nCTF > 0 ) { CTFmultiHisto_->Fill(nCTF,theWeight); }
    if ( emSumBET > 0. ) { CTBemSumETHisto_->Fill(emSumBET,theWeight); }
    if ( hadSumBET > 0. ) { CTBhadSumETHisto_->Fill(hadSumBET,theWeight); }
    if ( totSumBET > 0. ) { CTBtotSumETHisto_->Fill(totSumBET,theWeight); }
    if ( emSumEET > 0. ) { CTEemSumETHisto_->Fill(emSumEET,theWeight); }
    if ( hadSumEET > 0. ) { CTEhadSumETHisto_->Fill(hadSumEET,theWeight); }
    if ( totSumEET > 0. ) { CTEtotSumETHisto_->Fill(totSumEET,theWeight); }
    if ( emSumFET > 0. ) { CTFemSumETHisto_->Fill(emSumFET,theWeight); }
    if ( hadSumFET > 0. ) { CTFhadSumETHisto_->Fill(hadSumFET,theWeight); }
    if ( totSumFET > 0. ) { CTFtotSumETHisto_->Fill(totSumFET,theWeight); }
    if ( emSumET > 0. ) { CTemSumETHisto_->Fill(emSumET,theWeight); }
    if ( hadSumET > 0. ) { CThadSumETHisto_->Fill(hadSumET,theWeight); }
    if ( totSumET > 0. ) { CTtotSumETHisto_->Fill(totSumET,theWeight); }
  }

  if ( (int)nVtx <= numvtx ) {
    if ( sumBEmET[nVtx] > 0. ) { CTBemSumETVSvtx_->Fill((float)nVtx,sumBEmET[nVtx],theWeight); }
    if ( sumBHadET[nVtx] > 0. ) { CTBhadSumETVSvtx_->Fill((float)nVtx,sumBHadET[nVtx],theWeight); }
    if ( sumBTotET[nVtx] > 0. ) { CTBtotSumETVSvtx_->Fill((float)nVtx,sumBTotET[nVtx],theWeight); }
    if ( sumEEmET[nVtx] > 0. ) { CTEemSumETVSvtx_->Fill((float)nVtx,sumEEmET[nVtx],theWeight); }
    if ( sumEHadET[nVtx] > 0. ) { CTEhadSumETVSvtx_->Fill((float)nVtx,sumEHadET[nVtx],theWeight); }
    if ( sumETotET[nVtx] > 0. ) { CTEtotSumETVSvtx_->Fill((float)nVtx,sumETotET[nVtx],theWeight); }
    if ( sumFEmET[nVtx] > 0. ) { CTFemSumETVSvtx_->Fill((float)nVtx,sumFEmET[nVtx],theWeight); }
    if ( sumFHadET[nVtx] > 0. ) { CTFhadSumETVSvtx_->Fill((float)nVtx,sumFHadET[nVtx],theWeight); }
    if ( sumFTotET[nVtx] > 0. ) { CTFtotSumETVSvtx_->Fill((float)nVtx,sumFTotET[nVtx],theWeight); }
    if ( sumEmET[nVtx] > 0. ) { CTemSumETVSvtx_->Fill((float)nVtx,sumEmET[nVtx],theWeight); }
    if ( sumHadET[nVtx] > 0. ) { CThadSumETVSvtx_->Fill((float)nVtx,sumHadET[nVtx],theWeight); }
    if ( sumTotET[nVtx] > 0. ) { CTtotSumETVSvtx_->Fill((float)nVtx,sumTotET[nVtx],theWeight); }
    //    std::cout << "CT: sumTotET[nVtx] = " << sumTotET[nVtx] << std::endl; 
  }
    
  if (nVtx > 0 ) {
    for ( int irank = 0; irank < nrankTh_; irank++ ) {
      float threshold = minRankTh_+(float)irank;
      if ( emSumET > threshold ) { CTemSumRank_->Fill(threshold+0.5,theWeight); }
      if ( hadSumET > threshold ) { CThadSumRank_->Fill(threshold+0.5,theWeight); }
      if ( totSumET > threshold ) { CTtotSumRank_->Fill(threshold+0.5,theWeight); }
      CTnormaRank_->Fill(threshold+0.5,theWeight); 
    }
  }

  // High sumET tail

  if ( totSumET > 200. ) {

    double emSumBET = 0.;
    double hadSumBET = 0.;
    double totSumBET = 0.;
    
    double emSumEET = 0.;
    double hadSumEET = 0.;
    double totSumEET = 0.;
    
    int nCTB =0;
    int nCTE= 0;

    for (CaloTowerCollection::const_iterator cal = towers->begin(); cal != towers->end() ; ++cal) {
      
      double emET = cal->emEt();
      double hadET = cal->hadEt();
      double totET = cal->et();

      bool eneSelect(totET >= etTh_ && emET >= etEmTh_ && hadET >= etHadTh_);
      
      if ( ! eneSelect) { continue; }

      if ( std::fabs(cal->eta()) <= 1.48 ) {
        nCTB++;
        emSumBET += emET;
        hadSumBET += hadET;
        totSumBET += totET;
        CTBtemETHisto_->Fill(emET,theWeight);
        CTBthadETHisto_->Fill(hadET,theWeight);
        CTBttotETHisto_->Fill(totET,theWeight);
      }
      else if ( std::fabs(cal->eta()) > 1.48 && std::fabs(cal->eta()) <= 3.) {
        nCTE++;
        emSumEET += emET;
        hadSumEET += hadET;
        totSumEET += totET;
        CTEtemETHisto_->Fill(emET,theWeight);
        CTEthadETHisto_->Fill(hadET,theWeight);
        CTEttotETHisto_->Fill(totET,theWeight);
      }
    }

    vtxSumETtailHisto_->Fill((float)nVtx,theWeight);
    CTBtmultiHisto_->Fill(nCTB,theWeight); 
    CTEtmultiHisto_->Fill(nCTE,theWeight); 
    CTBtemSumETHisto_->Fill(emSumBET,theWeight); 
    CTBthadSumETHisto_->Fill(hadSumBET,theWeight); 
    CTBttotSumETHisto_->Fill(totSumBET,theWeight); 
    CTEtemSumETHisto_->Fill(emSumEET,theWeight); 
    CTEthadSumETHisto_->Fill(hadSumEET,theWeight); 
    CTEttotSumETHisto_->Fill(totSumEET,theWeight); 

  }

}//analyze

void CaloTowerAnalysis::endJob(){

  CTemEff_->Divide(CTemSumRank_,CTnormaRank_,1.,1.,"B");
  CThadEff_->Divide(CThadSumRank_,CTnormaRank_,1.,1.,"B");
  CTtotEff_->Divide(CTtotSumRank_,CTnormaRank_,1.,1.,"B");

  return;

}


//define this as a plug-in
DEFINE_FWK_MODULE(CaloTowerAnalysis);
