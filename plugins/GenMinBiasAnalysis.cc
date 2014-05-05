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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

#include "CLHEP/Units/PhysicalConstants.h"

class GenMinBiasAnalysis : public edm::EDAnalyzer
{

public:
  explicit GenMinBiasAnalysis(const edm::ParameterSet&);
  virtual ~GenMinBiasAnalysis();
  virtual void beginJob();
  virtual void endJob();  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  
private:
  
  edm::InputTag hepmcCollection_;
  bool verbose_;
  
  const float lowEta;
  const float highEta;
  const int nEta;
  float sumW;

  float rmin,rmax;
  
  /// PDT table
  edm::ESHandle<HepPDT::ParticleDataTable> fPDGTable ;

  // Histograms

  TH1F * weightHisto_;

  TH1F * chargedEta_;
  TH1F * photonEta_;
  TH1F * neutralEta_;

  TProfile * chargedEflowVSeta_;
  TProfile * photonEflowVSeta_;
  TProfile * neutralEflowVSeta_;
  TProfile * totalEflowVSeta_;

  TH1F * chargedEflow_;
  TH1F * photonEflow_;
  TH1F * neutralEflow_;
  TH1F * totalEflow_;
  
};

using namespace edm;

GenMinBiasAnalysis::GenMinBiasAnalysis(const edm::ParameterSet& iPSet):  
  hepmcCollection_(iPSet.getParameter<edm::InputTag>("hepmcCollection")),
  verbose_(iPSet.getUntrackedParameter<bool>("verbose","false")),
  lowEta(1.4),highEta(5.),nEta(180),sumW(0.),rmin(0.),rmax(0.)
{    

  // distances in mm, use a surface at 3000. mm from the interaction point
  // to compute the p_T corresponding to the minimum radius needed to enter into
  // the eta acceptance at that distance

  float tanth05_min = std::exp(-4.);
  float tanth05_max = std::exp(-1.6);
  float tanth_min = 2.*tanth05_min/(1.-tanth05_min*tanth05_min);
  float tanth_max = 2.*tanth05_max/(1.-tanth05_max*tanth05_max);
  rmin = 3000.*tanth_min;
  rmax = 3000.*tanth_max;

  std::cout << "minimum radius (mm) = " << rmin << std::endl;
  std::cout << "maximum radius (mm) = " << rmax << std::endl;

  edm::Service<TFileService> fs;

  weightHisto_ = fs->make<TH1F>( "weightHisto", "event weight distribution", 100, 0., 2. ) ;

  chargedEta_ = fs->make<TH1F>( "chargedEta", "charged particle eflow vs eta", nEta, lowEta, highEta ) ;
  photonEta_ = fs->make<TH1F>( "photonEta", "photon eflow vs eta", nEta, lowEta, highEta ) ;
  neutralEta_ = fs->make<TH1F>( "neutralEta", "neutral particle eflow vs eta", nEta, lowEta, highEta ) ;

  chargedEflowVSeta_ = fs->make<TProfile>( "chargedEflowVSeta", " charged particle eflow VS eta", nEta, lowEta, highEta, 0., 1500. ) ;
  photonEflowVSeta_ = fs->make<TProfile>( "photonEflowVSeta", " photon eflow VS eta", nEta, lowEta, highEta, 0., 1500. ) ;
  neutralEflowVSeta_ = fs->make<TProfile>( "neutralEflowVSeta", " neutral particle eflow VS eta", nEta, lowEta, highEta, 0., 1500. ) ;
  totalEflowVSeta_ = fs->make<TProfile>( "totalEflowVSeta", " total particle eflow VS eta", nEta, lowEta, highEta, 0., 1500. ) ;

  chargedEflow_ = fs->make<TH1F>( "chargedEflow", "charged particle eflow in eta [1.6,4]", 150, 0., 1500. ) ;
  photonEflow_ = fs->make<TH1F>( "photonEflow", "photon eflow in eta [1.6,4]", 150, 0., 1500. ) ;
  neutralEflow_ = fs->make<TH1F>( "neutralEflow", "neutral particle eflow in eta [1.6,4]", 150, 0., 1500. ) ;
  totalEflow_ = fs->make<TH1F>( "totalEflow", "total particle eflow in eta [1.6,4]", 150, 0., 1500. ) ;

}

GenMinBiasAnalysis::~GenMinBiasAnalysis() {}

void GenMinBiasAnalysis::beginJob() {}

void GenMinBiasAnalysis::endJob(){

  // float norma = sumW > 0. ? 1./sumW : 1. ;

  // chargedEflow_->Scale(norma);
  // photonEflow_->Scale(norma);
  // neutralEflow_->Scale(norma);
  // totalEflow_->Scale(norma);

  // chargedEflow_->Sumw2();
  // photonEflow_->Sumw2();
  // neutralEflow_->Sumw2();
  // totalEflow_->Sumw2();

  return;

}

void GenMinBiasAnalysis::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup)
{
  ///Get PDT Table
  iSetup.getData( fPDGTable );
  return;
}

void GenMinBiasAnalysis::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}

void GenMinBiasAnalysis::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 

  // if simulation, perform pileup reweighting

  float theWeight = 1.;

  ///Gathering the HepMCProduct information
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(hepmcCollection_, evt);

  //Get HepMC EVENT
  HepMC::GenEvent *myGenEvent = new HepMC::GenEvent(*(evt->GetEvent()));

  if ( verbose_ ) { std::cout << "\n" << std::endl; myGenEvent->print(); std::cout << "\n" << std::endl; }

  theWeight = myGenEvent->weights()[0];
  sumW += theWeight;

  weightHisto_->Fill(theWeight,1.);

  float chaE(0.);
  float phoE(0.);
  float neuE(0.);
  float totE(0.);

  chargedEta_->Reset("M");
  photonEta_->Reset("M");
  neutralEta_->Reset("M");

  ///Looping through the PARTICLES in the event
  HepMC::GenEvent::particle_const_iterator ptclBegin = myGenEvent->particles_begin();
  HepMC::GenEvent::particle_const_iterator ptclEnd = myGenEvent->particles_end();
  for(HepMC::GenEvent::particle_const_iterator ptclIt = ptclBegin; ptclIt!=ptclEnd; ++ptclIt)
    {
    
      ///Particles
      HepMC::GenParticle *ptcl = *ptclIt;
      int status = ptcl->status();

      ///Stable particles
      if( status == 1 ){

        float p = ptcl->momentum().rho() ;
        float theta = ptcl->momentum().theta() ;
        float eta = -1.*std::log( std::tan( theta*0.5 ));
        //        float eta = ptcl->momentum().eta() ;

        float charge = 999.;	// for the charge it's needed a HepPDT method
        int Id = ptcl->pdg_id(); // std::cout << Id << '\n'; 
        const HepPDT::ParticleData* PData = fPDGTable->particle(HepPDT::ParticleID(Id));
        if(PData!=0) charge = PData->charge();

        bool etaWindow = ( std::fabs(eta) >= 1.6 && std::fabs(eta) <= 4. ) ? true : false ;

        // charged particles

        if ( std::fabs(charge) != 0. && charge != 999. ) {

          // Oversimplified propagation in magnetic field ( 4 tesla constant along z )

          float Dz = 3000.-ptcl->production_vertex()->point3d().z(); // in mm

          float r = 1000.*ptcl->momentum().perp()/0.3/4.; // p_T = 0.3*B(Tesla)*r(m)
          float R_c = r; // assume particle on the z axis
          float csign = charge > 0. ? 1. : -1. ;
          
          float tmax = Dz*ptcl->momentum().e()/ptcl->momentum().pz();
          float omega = csign*ptcl->momentum().perp()/ptcl->momentum().e()/r;

          float R_t = std::sqrt( R_c*R_c + r*r + 2.*R_c*r*std::cos( CLHEP::pi - omega*tmax ) );

          float sinthe = R_t/std::sqrt(3000.*3000.+R_t*R_t);
          float costhe = 3000./std::sqrt(3000.*3000.+R_t*R_t);
          float etaeff = -1.*eta/std::fabs(eta)*std::log(sinthe/(1.+costhe));

          // std::cout << ptcl->barcode() << " " << ptcl->momentum().perp() << " " << ptcl->momentum().eta() << std::endl;
          // std::cout << " tmax = " << tmax/3.e11 << " beta_t " << ptcl->momentum().perp()/ptcl->momentum().e() << std::endl;
          // std::cout << "R_t = " << R_t << " r = " << r << " " << rmin << " " << rmax << " etaeff = " << etaeff << std::endl;

          if ( verbose_ ) { 
            std::cout << "Cha " << std::fixed << std::setw(7) << Id 
                      << " p = " << std::fixed << std::setw(10) << std::setprecision(3) << p 
                      << " eta = " << std::fixed << std::setw(6) << std::setprecision(2) << eta 
                      << " etaeff = " << std::fixed << std::setw(6) << std::setprecision(2) << etaeff << std::endl; 
          }
          chargedEta_->Fill(std::fabs(etaeff),p);
          if ( std::fabs(etaeff) > 1.6 && std::fabs(etaeff) < 4. ) { chaE += p; }

        } 

        // neutral particles

        else {

          // skip neutrinos

          if ( std::fabs( Id ) == 12 || std::fabs( Id ) == 14 || std::fabs( Id ) == 16 ) continue;

          // photons

          if ( Id == 22 ) {

            if ( verbose_ ) { 
              std::cout << "Pho " << std::fixed << std::setw(7) << Id 
                        << " p = " << std::fixed << std::setw(10) << std::setprecision(3) << p 
                        << " eta = " << std::fixed << std::setw(6) << std::setprecision(2) << eta << std::endl; 
            }
            photonEta_->Fill(std::fabs(eta),p);
            if ( etaWindow ) { phoE += p; }

          } 

          // other neutrals

          else {

            if ( verbose_ ) { 
              std::cout << "Neu " << std::fixed << std::setw(7) << Id 
                        << " p = " << std::fixed << std::setw(10) << std::setprecision(3) << p 
                        << " eta = " << std::fixed << std::setw(6) << std::setprecision(2) << eta << std::endl; 
            }
            neutralEta_->Fill(std::fabs(eta),p);
            if ( etaWindow ) { neuE += p; }

          }

        }

      }

    }

  Double_t tmpC(0.);
  float wBin = (highEta-lowEta)/(float)nEta;
  //  std::cout << "wBin = " << wBin << std::endl;
  for ( int iEta=0; iEta < nEta; iEta++ ) {
    float etot(0.);
    tmpC = chargedEta_->GetBinContent(iEta);
    chargedEflowVSeta_->Fill(lowEta+((float)iEta+0.5)*wBin,tmpC,theWeight);
    etot += tmpC;
    tmpC = photonEta_->GetBinContent(iEta);
    photonEflowVSeta_->Fill(lowEta+((float)iEta+0.5)*wBin,tmpC,theWeight);
    etot += tmpC;
    tmpC = neutralEta_->GetBinContent(iEta);
    neutralEflowVSeta_->Fill(lowEta+((float)iEta+0.5)*wBin,tmpC,theWeight);
    etot += tmpC;
    totalEflowVSeta_->Fill(lowEta+((float)iEta+0.5)*wBin,etot,theWeight);
  }

  totE = chaE+phoE+neuE;
  if ( verbose_ ) { std::cout << "\n T Cha = " << std::fixed << std::setw(10) << std::setprecision(3) << chaE 
                              << " T Pho = " << std::fixed << std::setw(10) << std::setprecision(3) << phoE 
                              << " T Neu = " << std::fixed << std::setw(10) << std::setprecision(3) << neuE 
                              << " Total = " << std::fixed << std::setw(10) << std::setprecision(3) << totE << "\n" << std::endl; }

  chargedEflow_->Fill(chaE,theWeight);
  photonEflow_->Fill(phoE,theWeight);
  neutralEflow_->Fill(neuE,theWeight);
  totalEflow_->Fill(totE,theWeight);

  delete myGenEvent;

}//analyze


//define this as a plug-in
DEFINE_FWK_MODULE(GenMinBiasAnalysis);
