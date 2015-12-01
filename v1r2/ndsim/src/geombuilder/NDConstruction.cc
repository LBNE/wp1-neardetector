//
// Title: TPC Vessel
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: Detector geometry for root in the ND geometry

#include "NDConstruction.hh"

NDConstruction::NDConstruction()
{
}

void NDConstruction::initializeTPC(double length, double radius, double density, std::string material){
  tpc_length_    = length*CLHEP::m;
  tpc_radius_    = radius*CLHEP::m;
  tpc_density_   = density*(CLHEP::g / CLHEP::cm3);
  tpc_matName_   = material;
  tpc_mass_      = 3.14159*(tpc_length_/CLHEP::m)*(tpc_radius_/CLHEP::m)*(tpc_radius_/CLHEP::m)*(tpc_density_*(CLHEP::m3 / CLHEP::kg));
}

void NDConstruction::initializeVessel(double thickness, double density, std::string material){
  vessel_thickness_ = thickness*CLHEP::m;
  vessel_density_   = density*(CLHEP::g / CLHEP::cm3);
  vessel_matName_   = material;
}

void NDConstruction::initializeEcal(double density, double thickness, int layers, double absdensity, double absthickness, std::string material, std::string absmaterial, bool buildlayers){
  ecal_density_      = density*(CLHEP::g / CLHEP::cm3);
  ecal_thickness_    = thickness*(CLHEP::cm);
  ecal_layers        = layers;
  ecal_absdensity_   = absdensity*(CLHEP::g / CLHEP::cm3);
  ecal_absthickness_ = absthickness*(CLHEP::cm);
  ecal_gap           = 0.05*(CLHEP::m); 
  ecal_matName_      = material;
  ecal_absmatName_   = absmaterial;
  ecal_buildlayers_  = buildlayers;
}

void NDConstruction::initializeMagnet(double density, double thickness, std::string material){
  magnet_density_   = density*(CLHEP::g / CLHEP::cm3);
  magnet_thickness_ = thickness*(CLHEP::cm);
  magnet_matName_   = material;
}

void NDConstruction::construction(Materials * mats, bool ecal, bool magnet){
  
  TGeoMaterial *wmaterial = (TGeoMixture*)mats->FindMaterial("Air");
  TGeoMedium *wmedium = new TGeoMedium("Air",1,wmaterial);
  TGeoBBox *worldbox = new TGeoBBox("WorldBox",1500,1500,1500);

  volume_ = new TGeoVolume("DetectorWorld",worldbox,wmedium);
  
  // TPC material
  TGeoMixture *tpcmaterial = (TGeoMixture*)mats->FindMaterial(tpc_matName_.c_str());  
  if(tpcmaterial)
    tpcmaterial->SetDensity(tpc_density_/(CLHEP::g/CLHEP::cm3));
  else{
    tpcmaterial = (TGeoMixture*)mats->FindMaterial("T2KArgonGas");
    tpcmaterial->SetDensity(tpc_density_/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::NDConstruction::Construct MATERIAL NOT SET for gas volume- using T2KArgonGas as default"
	      << "\n------------------------------------------------------------" << std::endl;
  }
  TGeoMedium *innerMedium = new TGeoMedium(tpc_matName_.c_str(),1,tpcmaterial);
  
  // Vessel material
  TGeoMaterial *vesselmaterial = (TGeoMixture*)mats->FindMaterial(vessel_matName_.c_str());  
  if(vesselmaterial)
    vesselmaterial->SetDensity(vessel_density_/(CLHEP::g/CLHEP::cm3));
  else{
    vesselmaterial = (TGeoMixture*)mats->FindMaterial("Aluminium");
    vesselmaterial->SetDensity(tpc_density_/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::NDConstruction::Construct MATERIAL NOT SET for vessel - using Aluminium as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  TGeoMedium *medium  = new TGeoMedium(vessel_matName_.c_str(),1,vesselmaterial);
  //--------------- Anode and Cathode plates ------------------//
  std::string anodePlateMaterialName = "G10Roha";
  TGeoMixture *anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
  if(!anodePlateMaterial){
    anodePlateMaterialName = "Aluminium";
    anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
  }
  double plateDensity = 0.2877*(CLHEP::g / CLHEP::cm3);
  anodePlateMaterial->SetDensity(plateDensity/(CLHEP::g/CLHEP::cm3));
  TGeoMedium *anodeMedium   = new TGeoMedium(anodePlateMaterialName.c_str(),3,anodePlateMaterial);

  std::string cathodePlateMaterialName = anodePlateMaterialName;
  TGeoMixture *cathodePlateMaterial = anodePlateMaterial;
  TGeoMedium *cathodeMedium = new TGeoMedium(cathodePlateMaterialName.c_str(),2,cathodePlateMaterial);
  // Ecal scintillation bars
  TGeoMixture *ecalmaterial = (TGeoMixture*)mats->FindMaterial(ecal_matName_.c_str());
  if(ecalmaterial)
    ecalmaterial->SetDensity(ecal_density_/(CLHEP::g/CLHEP::cm3));
  else{
    ecalmaterial = (TGeoMixture*)mats->FindMaterial("Scintillator");
    ecalmaterial->SetDensity(ecal_density_/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::NDConstruction::Ecal layer MATERIAL NOT SET - using Scintillator as default"
	      << "\n------------------------------------------------------------" << std::endl;
  }
  TGeoMedium *ecalmedium  = new TGeoMedium(ecal_matName_.c_str(),1,ecalmaterial);
  // Ecal absorber
  TGeoMixture *ecalabsmaterial = (TGeoMixture*)mats->FindMaterial(ecal_absmatName_.c_str());
  if(ecalabsmaterial)
    ecalabsmaterial->SetDensity(ecal_absdensity_/(CLHEP::g/CLHEP::cm3));
  else{
    ecalabsmaterial = (TGeoMixture*)mats->FindMaterial("Lead");
    ecalabsmaterial->SetDensity(ecal_absdensity_/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::NDConstruction::Ecal absorber MATERIAL NOT SET - using Lead as default"
	      << "\n------------------------------------------------------------" << std::endl;
  }
  TGeoMedium *ecalabsmedium  = new TGeoMedium(ecal_absmatName_.c_str(),1,ecalmaterial);
  // Magnet
  TGeoMixture *magnetmaterial = (TGeoMixture*)mats->FindMaterial(magnet_matName_.c_str());
  if(magnetmaterial)
    magnetmaterial->SetDensity(magnet_density_/(CLHEP::g/CLHEP::cm3));
  else{
    magnetmaterial = (TGeoMixture*)mats->FindMaterial("Iron");
    magnetmaterial->SetDensity(magnet_density_/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::NDConstruction::Magnet MATERIAL NOT SET - using Iron as default"
	      << "\n------------------------------------------------------------" << std::endl;
  }
  TGeoMedium *magnetmedium  = new TGeoMedium(magnet_matName_.c_str(),1,magnetmaterial);
  // -------------------------------------------------------------------------------------
  // Build volumes
  double z_center = tpc_length_/2 + vessel_thickness_/2  + ecal_gap + (double)ecal_layers*ecal_thickness_ + (double)ecal_layers*ecal_absthickness_ + 2*ecal_gap + magnet_thickness_;
  /*
  if(ecal){
    z_center = z_center + ecal_gap + (double)ecal_layers*ecal_thickness_ + ecal_layers*ecal_absthickness_;
    if(magnet)
      z_center = z_center + 2*ecal_gap + magnet_thickness_;
  }
  */
  z_center = -z_center/(CLHEP::cm);
  start_vol_.SetXYZ(0,0,z_center);
  // Vessel
  double outradio_  = tpc_radius_ + vessel_thickness_; 
  double outlength_ = tpc_length_ + vessel_thickness_; 
  TGeoTube *tube_ = new TGeoTube("vesselCenterTube",0,outradio_/(CLHEP::cm),outlength_/(2.0*(CLHEP::cm)));
  TGeoVolume *Volume   = new TGeoVolume("VesselVolume",tube_,medium);
  volume_->AddNodeOverlap(Volume,0,new TGeoTranslation(0,0,0 - z_center));
  // TPC
  TGeoTube *innerTube_    = new TGeoTube("vesselCenterTube",0,tpc_radius_/(CLHEP::cm),tpc_length_/(2.0*(CLHEP::cm)));
  TGeoVolume *innerVolume_ = new TGeoVolume("innerVesselVolume",innerTube_,innerMedium);
  volume_->AddNodeOverlap(innerVolume_,1,new TGeoTranslation(0,0,0 - z_center));
  //--------------- Anode and Cathode plates ------------------//
  double cathodeThickness = 1.32*CLHEP::cm;
  double anodeThickness = cathodeThickness;
  TGeoTube * anodeTube = new TGeoTube("anodeTube",0,tpc_radius_/(CLHEP::cm),anodeThickness/(2.0*(CLHEP::cm)));
  TGeoVolume *anodeVolume_ = new TGeoVolume("anodeVolume",anodeTube,anodeMedium);
  volume_->AddNodeOverlap(anodeVolume_,2,new TGeoTranslation(0,0,tpc_length_/(2.0*(CLHEP::cm)) - z_center ));
  
  TGeoTube * cathodeTube = new TGeoTube("cathodeTube",0,tpc_radius_/(CLHEP::cm),anodeThickness/(2.0*(CLHEP::cm)));
  TGeoVolume *cathodeVolume_ = new TGeoVolume("cathodeVolume",cathodeTube,cathodeMedium);
  volume_->AddNodeOverlap(cathodeVolume_,3,new TGeoTranslation(0,0,-tpc_length_/(2.0*(CLHEP::cm)) - z_center ));

  // Ecal
  if(ecal){
    double ecalradius = outradio_ + ecal_gap;
    double ecallength =  outlength_/2 + ecal_gap;
    double totalthickness = (double)ecal_layers*ecal_thickness_;
    double x1 = ecalradius/(CLHEP::cm);
    double y1 = ecalradius/(CLHEP::cm);
    double z1 = totalthickness/(CLHEP::cm)/2;
    double pos = ecallength/(CLHEP::cm) + totalthickness/(CLHEP::cm)/2;

    if(!ecal_buildlayers_){ // ecal without layers
      TGeoBBox *motherBoxZ = new TGeoBBox("EcalMotherBoxZ",x1,y1,z1);
      TGeoVolume *ez1volume = new TGeoVolume("EcalScintVolumeZ1",motherBoxZ,ecalmedium);
      volume_->AddNodeOverlap(ez1volume,4,new TGeoTranslation(0,0,pos - z_center));
      // Better to define separate volumes for all the ecals - 6 in total
      TGeoVolume *ez2volume = new TGeoVolume("EcalEcintVolumeZ2",motherBoxZ,ecalmedium);
      volume_->AddNodeOverlap(ez2volume,5,new TGeoTranslation(0,0,-pos - z_center));
      
      // Y
      y1 = z1;
      z1 = ecallength/(CLHEP::cm) + totalthickness/(CLHEP::cm);
      TGeoBBox *motherBoxY = new TGeoBBox("EcalMotherBoxY",x1,y1,z1);
      TGeoVolume *ey1volume = new TGeoVolume("EcalScintVolumeY1",motherBoxY,ecalmedium);
      pos = ecalradius/(CLHEP::cm) + totalthickness/(CLHEP::cm)/2;
      volume_->AddNodeOverlap(ey1volume,6,new TGeoTranslation(0,pos,0 - z_center));
      
      TGeoVolume *ey2volume = new TGeoVolume("EcalScintVolumeY2",motherBoxY,ecalmedium);
      volume_->AddNodeOverlap(ey2volume,7,new TGeoTranslation(0,-pos,0 - z_center));
      
      // X
      x1 = y1;
      y1 = ecalradius/(CLHEP::cm);//
      z1 = ecallength/(CLHEP::cm);
      TGeoBBox *motherBoxX = new TGeoBBox("EcalMotherBoxX",x1,y1,z1);
      TGeoVolume *ex1volume = new TGeoVolume("EcalScintVolumeX1",motherBoxX,ecalmedium);
      pos = -pos;
      volume_->AddNodeOverlap(ex1volume,8,new TGeoTranslation(pos,0,0 - z_center));
      
      TGeoVolume *ex2volume = new TGeoVolume("EcalScintVolumeX2",motherBoxX,ecalmedium);
      volume_->AddNodeOverlap(ex2volume,9,new TGeoTranslation(-pos,0,0 - z_center));
    }
    else{ // Add layers
      totalthickness += (double)ecal_layers*ecal_absthickness_;
      double scintthickness = ecal_thickness_;
      double absthickness = ecal_absthickness_;
      int norder = 4;
      for(int i=0;i<ecal_layers;i++){
	// Z
	x1 = ecalradius/(CLHEP::cm) + totalthickness/(CLHEP::cm);//
	y1 = ecalradius/(CLHEP::cm) + totalthickness/(CLHEP::cm);//
	z1 = scintthickness/(CLHEP::cm)/2;
	pos = ecallength/(CLHEP::cm) + i*scintthickness/(CLHEP::cm) + i*absthickness/(CLHEP::cm);
	TGeoBBox *BoxZ1 = new TGeoBBox("EcalBoxZ1",x1,y1,z1);
	TGeoVolume *ez1volume = new TGeoVolume(Form("EcalScintZ1_%iVolume",i),BoxZ1,ecalmedium);
	volume_->AddNodeOverlap(ez1volume,norder,new TGeoTranslation(0,0,pos - z_center));
	norder++;
	TGeoBBox *BoxZ2 = new TGeoBBox("EcalBoxZ2",x1,y1,z1);
	TGeoVolume *ez2volume = new TGeoVolume(Form("EcalScintZ2_%iVolume",i),BoxZ2,ecalmedium);
	volume_->AddNodeOverlap(ez2volume,norder,new TGeoTranslation(0,0,-pos - z_center));
	norder++;
	// Y
	y1 = z1;
	x1 = ecalradius/(CLHEP::cm) + totalthickness/(CLHEP::cm);//
	z1 = ecallength/(CLHEP::cm); //+ totalthickness/(CLHEP::cm);
	pos = ecalradius/(CLHEP::cm) + i*scintthickness/(CLHEP::cm) + i*absthickness/(CLHEP::cm);
	TGeoBBox *BoxY1 = new TGeoBBox("EcalBoxY1",x1,y1,z1);
	TGeoVolume *ey1volume = new TGeoVolume(Form("EcalScintY1_%iVolume",i),BoxY1,ecalmedium);
	volume_->AddNodeOverlap(ey1volume,norder,new TGeoTranslation(0,pos,0 - z_center));
	norder++;
	
	TGeoBBox *BoxY2 = new TGeoBBox("EcalBoxY2",x1,y1,z1);
	TGeoVolume *ey2volume = new TGeoVolume(Form("EcalScintY2_%iVolume",i),BoxY2,ecalmedium);
	volume_->AddNodeOverlap(ey2volume,norder,new TGeoTranslation(0,-pos,0 - z_center));
	norder++;

	// X
	x1 = y1;
	y1 = ecalradius/(CLHEP::cm);
	z1 = ecallength/(CLHEP::cm);
	TGeoBBox *BoxX1 = new TGeoBBox("EcalBoxX1",x1,y1,z1);
	TGeoVolume *ex1volume = new TGeoVolume(Form("EcalScintX1_%iVolume",i),BoxX1,ecalmedium);
	volume_->AddNodeOverlap(ex1volume,norder,new TGeoTranslation(pos,0,0 - z_center));
	norder++;

	TGeoBBox *BoxX2 = new TGeoBBox("EcalBoxX2",x1,y1,z1);
	TGeoVolume *ex2volume = new TGeoVolume(Form("EcalScintX2_%iVolume",i),BoxX2,ecalmedium);
	volume_->AddNodeOverlap(ex2volume,norder,new TGeoTranslation(-pos,0,0 - z_center));
	norder++;
	// Now the lead layer
	if(absthickness > 0.0){ // only add it if the thickness is greater than zero
	  x1 = ecalradius/(CLHEP::cm);
	  y1 = ecalradius/(CLHEP::cm);
	  z1 = absthickness/(CLHEP::cm)/2;
	  pos = ecallength/(CLHEP::cm) + i*scintthickness/(CLHEP::cm) + i*absthickness/(CLHEP::cm);
	  
	  pos = pos + scintthickness/(CLHEP::cm)/2 + absthickness/(CLHEP::cm)/2;
	  TGeoBBox *LBoxZ1 = new TGeoBBox("LeadBoxZ1",x1,y1,z1);
	  TGeoVolume *lz1volume = new TGeoVolume(Form("EcalLeadZ1_%iVolume",i),LBoxZ1,ecalabsmedium);
	  volume_->AddNodeOverlap(lz1volume,norder,new TGeoTranslation(0,0,pos - z_center));
	  norder++;

	  TGeoBBox *LBoxZ2 = new TGeoBBox("LeadBoxZ2",x1,y1,z1);
	  TGeoVolume *lz2volume = new TGeoVolume(Form("EcalLeadZ2_%iVolume",i),LBoxZ2,ecalabsmedium);
	  volume_->AddNodeOverlap(lz2volume,norder,new TGeoTranslation(0,0,-pos - z_center));
	  norder++;

	  // Y
	  y1 = z1;
	  z1 = ecallength/(CLHEP::cm) + totalthickness/(CLHEP::cm);
	  pos = ecalradius/(CLHEP::cm) + i*scintthickness/(CLHEP::cm) + i*absthickness/(CLHEP::cm);
	  pos = pos + scintthickness/(CLHEP::cm)/2 + absthickness/(CLHEP::cm)/2;
	  TGeoBBox *LBoxY1 = new TGeoBBox("LeadBoxY1",x1,y1,z1);
	  TGeoVolume *ly1volume = new TGeoVolume(Form("EcalLeadY1_%iVolume",i),LBoxY1,ecalabsmedium);
	  volume_->AddNodeOverlap(ly1volume,norder,new TGeoTranslation(0,pos,0 - z_center));
	  norder++;

	  TGeoBBox *LBoxY2 = new TGeoBBox("LeadBoxY2",x1,y1,z1);
	  TGeoVolume *ly2volume = new TGeoVolume(Form("EcalLeadY2_%iVolume",i),LBoxY2,ecalabsmedium);
	  volume_->AddNodeOverlap(ly2volume,norder,new TGeoTranslation(0,-pos,0 - z_center));
	  norder++;

	  // X
	  x1 = y1;
	  y1 = ecalradius/(CLHEP::cm);
	  z1 = ecallength/(CLHEP::cm);
	  TGeoBBox *LBoxX1 = new TGeoBBox("LeadBoxX1",x1,y1,z1);
	  TGeoVolume *lx1volume = new TGeoVolume(Form("EcalLeadX1_%iVolume",i),LBoxX1,ecalabsmedium);
	  volume_->AddNodeOverlap(lx1volume,norder,new TGeoTranslation(pos,0,0 - z_center));
	  norder++;

	  TGeoBBox *LBoxX2 = new TGeoBBox("LeadBoxX2",x1,y1,z1);
	  TGeoVolume *lx2volume = new TGeoVolume(Form("EcalLeadX2_%iVolume",i),LBoxX2,ecalabsmedium);
	  volume_->AddNodeOverlap(lx2volume,norder,new TGeoTranslation(-pos,0,0 - z_center));
	  norder++;
	}
	
      }
      
    }

    if(magnet){
      double magnet_gap = 2*ecal_gap;
      double magnetradius = ecalradius + magnet_gap + totalthickness;
      double magnetlength = ecallength + magnet_gap + totalthickness;
      x1 = magnetradius/(CLHEP::cm);
      y1 = magnetradius/(CLHEP::cm);
      z1 = magnet_thickness_/(CLHEP::cm)/2;
      TGeoBBox *MmotherBoxZ = new TGeoBBox("MagnetMotherBoxZ",x1,y1,z1);
      TGeoVolume *mz1volume = new TGeoVolume("MagnetVolumeZ1",MmotherBoxZ,magnetmedium);
      pos = magnetlength/(CLHEP::cm) + magnet_thickness_/(CLHEP::cm)/2;
      volume_->AddNodeOverlap(mz1volume,10,new TGeoTranslation(0,0,pos - z_center));
      // Better to define separate volumes for the magnet - 6 in total
      TGeoVolume *mz2volume = new TGeoVolume("MagnetVolumeZ2",MmotherBoxZ,magnetmedium);
      volume_->AddNodeOverlap(mz2volume,11,new TGeoTranslation(0,0,-pos - z_center));
 
      // Y
      y1 = z1;
      z1 = magnetlength/(CLHEP::cm) + magnet_thickness_/(CLHEP::cm);
      TGeoBBox *MmotherBoxY = new TGeoBBox("MagnetMotherBoxY",x1,y1,z1);
      TGeoVolume *my1volume = new TGeoVolume("MagnetVolumeY1",MmotherBoxY,magnetmedium);
      pos = magnetradius/(CLHEP::cm) + magnet_thickness_/(CLHEP::cm)/2;
      volume_->AddNodeOverlap(my1volume,12,new TGeoTranslation(0,pos,0 - z_center));
      
      TGeoVolume *my2volume = new TGeoVolume("MagnetVolumeY2",MmotherBoxY,magnetmedium);
      volume_->AddNodeOverlap(my2volume,13,new TGeoTranslation(0,-pos,0 - z_center));
 
      // X
      x1 = y1;
      y1 = magnetradius/(CLHEP::cm);
      z1 = magnetlength/(CLHEP::cm);
      TGeoBBox *MmotherBoxX = new TGeoBBox("MagnetMotherBoxX",x1,y1,z1);
      TGeoVolume *mx1volume = new TGeoVolume("MagnetVolumeX1",MmotherBoxX,magnetmedium);
      pos = -pos;
      volume_->AddNodeOverlap(mx1volume,14,new TGeoTranslation(pos,0,0 - z_center));
      
      TGeoVolume *mx2volume = new TGeoVolume("MagnetVolumeX2",MmotherBoxX,magnetmedium);
      volume_->AddNodeOverlap(mx2volume,15,new TGeoTranslation(-pos,0,0 - z_center));
 
    }
    
  }// if ecal

}



NDConstruction::~NDConstruction()
{
  if(volume_)
    delete volume_;
}

