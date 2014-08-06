#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <assert.h>
#include <vector>
#include <map>

#include "TAxis.h"
#include "TProfile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"
#include "TTree.h"
#include "TVector2.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TAttLine.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TString.h"
#include "TAttMarker.h"
#include "TProfile2D.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TProfile.h"


int main (int argc, char* argv[]){

  std::string runFile="root file";
  if(argc>1){
    runFile=argv[1];
  }
  std::cout<<"Going to analyse file: "<<runFile<<std::endl;

  TFile* file = TFile::Open(runFile.c_str());


  if(file==0){
    std::cout<<"Warning, the file selected doesn't exists!!!"<<std::endl;
    exit(-1);
  }

  //Profile period part

  TTree* tree = (TTree*)file->Get("eventData");

  float crossings_thr_5[10][10];

  tree->SetBranchAddress("crossings_thr_5",crossings_thr_5);

  TH2F *test=new TH2F("test","prova profile", 200, 7,21,200,28,32);

  TF1 *profile_fit= new TF1("profile_fit", "pol0", 164, 174);
  //  profile_fit->SetParameters(23.);
  int entries;
  entries=tree->GetEntries();
  TGraph *gr= new TGraph(0);
  TProfile *hprof  = new TProfile("hprof","Period Profile Third Peak",200,163,175,22,25);
  std::cout<<"ENTRIES: "<<entries<<std::endl;

  for(int y=0;y<entries;++y){

    tree->GetEntry(y);

    std::cout<<"Threshold5 Crossing0: "<<crossings_thr_5[3][0]<<" crossing01: "<<crossings_thr_5[3][1]<<std::endl;
    std::cout<<"differenza: "<<crossings_thr_5[1][0]-crossings_thr_5[1][1]<<std::endl;
    //    test->Fill(crossings_thr_0[1][0],crossings_thr_0[1][1]-crossings_thr_0[1][0]);
    gr->SetPoint(y,crossings_thr_5[1][6],crossings_thr_5[1][7]-crossings_thr_5[1][6]);
    hprof->Fill(crossings_thr_5[1][6],crossings_thr_5[1][7]-crossings_thr_5[1][6]);
  }

  //  test->Rebin2D();
  // TProfile *prof = test->ProfileX();

  TCanvas *p3= new TCanvas("p3", "Period", 200, 10, 600, 400);
  //  prof->Draw();
  gr->SetTitle("Period Study Third Peak");
  //  gr->GetHistogram()->SetMaximum(31);
  // gr->GetHistogram()->SetMinimum(29);
  gr->GetXaxis()->SetTitle("t0 [ns]");
  gr->GetYaxis()->SetTitle("(t1-t0) [ns]");
  //gr->Draw("AP");
  //  test->Draw();
  hprof->GetXaxis()->SetTitle("t0 [ns]");
  hprof->GetYaxis()->SetTitle("(t1-t0) [ns]");
  hprof->SetMaximum(24.1);
  hprof->SetMinimum(23.7);
  gStyle->SetOptFit(11111);
  hprof->Draw();


  //  hprof->Fit("profile_fit","R");
  //  profile_fit->Draw("SAME");

  p3->SaveAs("profile.pdf");

  TCanvas *p4= new TCanvas("p4", "Period", 200, 10, 600, 400);
  gr->Draw("AP");
  p4->SaveAs("profile_graph.pdf");

  //Analysing the first part:

  TString name;
  std::map<TString,TObject*> periodObject;

  int channel, threshold, i, thr_value, cont;

 
  float mean_tmp,sigma_tmp;

  TGraphErrors *period[2][4];
  //  TH1F* wave_timeCross[1][1][1];


  
  //Inizializing graph
  int color,marker;

  TString name_map;
  TLegend *leg=new TLegend(0.6,0.9,0.9,0.7);
  leg->SetFillColor(0);

  TLegend *leg1=new TLegend(0.6,0.9,0.9,0.7);
  leg1->SetFillColor(0);


  for(int ch=0;ch<2;++ch){ 
    color=1;
    marker=24;
    for(int y=1; y<5; ++y){
      name_map=Form("graph_map_ch_%d_peak_%d", ch,y);
      period[ch][y-1]=new TGraphErrors(0);

      period[ch][y-1]->GetXaxis()->SetTitle("Threshold");
      period[ch][y-1]->GetYaxis()->SetTitle("Period [ns]");
      period[ch][y-1]->SetMarkerColor(color);
      period[ch][y-1]->SetMarkerStyle(marker);
      period[ch][y-1]->SetMarkerSize(0.6);
      if(ch<1){
	period[ch][y-1]->SetTitle("Period CH4");                     //!!!
	leg->AddEntry(period[ch][y-1],Form("Peak %d", y) ,"p");
      }else{
	period[ch][y-1]->SetTitle("Period CH5");                     //!!!  
	leg1->AddEntry(period[ch][y-1],Form("Peak %d", y) ,"p");
      }
      periodObject[name_map]=(TObject*) (period[ch][y-1]);

      ++color;
      ++marker;
    }
  }

  //Reading data
  channel=0;
  while(channel<2){
    threshold=0;
    thr_value=2000;

    std::cout<<"==========================================="<<std::endl;
    std::cout<<"READING channel: "<<channel<<std::endl;
    std::cout<<"==========================================="<<std::endl;

    while(threshold<10){
      i=0;
      cont=0;
      while(i<7){

	if(channel<1){
	  name=Form("wave_%d_thr_%d_timeCross_%d_%d", channel, threshold,i,i+1);                  //!!!
	}else{
	  name=Form("wave_%d_thr_%d_timeCross_%d_%d", channel, threshold,i,i+1);                 //!!!
	}
	
	//wave_timeCross[channel][threshold][i]=new TH1F(name,name,10000,0,1024);
	std::cout<<"Istogramma: "<<name<<std::endl;
  
	mean_tmp=((TH1F *)(file->Get(name)))->GetMean();      
	sigma_tmp=((TH1F *)(file->Get(name)))->GetRMS();
	std::cout<<"MEAN: "<<mean_tmp<<" SIGMA: "<<sigma_tmp<<" THR: "<<thr_value<<" CONT: "<<cont<<std::endl;


	
	//  wave1_ch0_diff->SetPoint(threshold,thr_value,mean_tmp);
	//wave1_ch0_diff->SetPointError(threshold,0,sigma_tmp);

	period[channel][cont]->SetPoint(threshold,thr_value,mean_tmp);
	period[channel][cont]->SetPointError(threshold,0,sigma_tmp);

	i+=2;
	++cont;
      }//end while i
      thr_value+=100;
      ++threshold;
    }//end while threshold<10
    ++channel;
  }// end while channel<2


  TCanvas *c1= new TCanvas("c1", "Period", 200, 10, 600, 400);

  TH2F *a0=new TH2F ("a0","a",10,1950.,2950.,10,18.,32.);
  a0->SetTitle("Period CH4");                                   // !!!
  a0->GetXaxis()->SetTitle("Threshold");
  a0->GetYaxis()->SetTitle("Period [ns]");
  a0->SetStats(0);  

  std::map<TString,TObject*>::iterator it= periodObject.begin();
  a0->Draw();
  it->second->Draw("PSAME");
  ++it;
  leg->Draw();
  for(int p=0; p<3;++p){

    std::cout<<"ITERATOR: "<<&it<<std::endl;

    it->second->Draw("PSAME");

    ++it;
  }

  c1->SaveAs("graph/periodo0.pdf");

  TH2F *a1=new TH2F ("a1","a",10,1950.,2950.,10,18.,32.);        //se il canale e' calibrato l'intervallo in y e' 18 - 32, altrimenti e' 4 - 37
  a1->SetTitle("Period CH5");                                   // !!!
  a1->GetXaxis()->SetTitle("Threshold");
  a1->GetYaxis()->SetTitle("Period [ns]");
  a1->SetStats(0);

  TCanvas *c2= new TCanvas("c2", "Period", 200, 10, 600, 400);
  a1->Draw();
  it->second->Draw("PSAME");
  ++it;
  leg1->Draw();
  for(int p=0; p<3;++p){

    std::cout<<"ITERATOR: "<<&it<<std::endl;

    it->second->Draw("PSAME");

    ++it;
    
  }
  c2->SaveAs("graph/periodo1.pdf");
  
  //Ending threshold part

  std::cout<<"==========================================="<<std::endl;
  std::cout<<"STARTING THE CHANNEL DIFFERENCE PART"<<std::endl;

  //Inizialising graph
  std::map<TString,TObject*> differenceObject;
  // int channel, threshold, i, thr_value, cont;
  TGraphErrors *difference[8];
  TString name_diff_map;

  TLegend *leg2=new TLegend(0.5,0.9,0.8,0.6);
  leg2->SetFillColor(0);

  color=2;
  marker=24;
  for(int y=1; y<9; ++y){
    name_diff_map=Form("graph_map_peak_%d",y);
    difference[y-1]=new TGraphErrors(0);

    difference[y-1]->GetXaxis()->SetTitle("Threshold");
    difference[y-1]->GetYaxis()->SetTitle("Period [ns]");
    difference[y-1]->SetMarkerColor(color);
    difference[y-1]->SetMarkerStyle(marker);
    difference[y-1]->SetMarkerSize(0.8);
    difference[y-1]->SetTitle("Time Difference CH1-CH3");                 //!!!
    leg2->AddEntry(difference[y-1],Form("Point %d", y) ,"p");
    differenceObject[name_diff_map]=(TObject*) (difference[y-1]);

    ++color;
    ++marker;
    if(marker==29) marker=32;
  }//ending run up,down wave part



  TString name_channel_difference;

  //Reading data


  std::cout<<"==========================================="<<std::endl;
  std::cout<<"START READING"<<std::endl;
  std::cout<<"==========================================="<<std::endl;

  float reso_tmp, reso_err;

  TF1 *fit= new TF1("fit","[0]*exp(-0.5*((x-[1])/[2])**2)",-0.2,0.2);
  //fit->SetParameters(10000,0.01,0.001);

  threshold=0;
  thr_value=2000;
    
  while(threshold<10){
    i=0;
    cont=0;
    while(i<8){
      
      name_channel_difference=Form("waveAlignDifference_0_1_thr_%d_timeCrossDifference_%d", threshold,i);                    //!!!
      
      std::cout<<"Istogramma: "<<name_channel_difference<<std::endl;
      
      fit->SetParameters(20000,0.001,0.0001);
      reso_tmp=((TH1F *)(file->Get(name_channel_difference)))->GetRMS();

      (((TH1F *)(file->Get(name_channel_difference)))->Rebin())->Fit("fit","RQ");

      reso_err=fit->GetParError(2);
      
      std::cout<<"SIGMA: "<<fit->GetParameter(2)<<" REAL: "<<reso_tmp<<" error: "<<reso_err<<" POINT: "<<i<<std::endl;

      difference[i]->SetPoint(threshold,thr_value,reso_tmp*1000 );
      difference[i]->SetPointError(threshold,0,0 );

      ++i;

    }//end while over points

    if(thr_value==2000 || thr_value==2500){

      std::cout<<"+++++++++++++++++++++++++++++"<<std::endl;
      std::cout<<"THR: "<<thr_value<<" RESO: "<<reso_tmp<<std::endl;
      std::cout<<"+++++++++++++++++++++++++++++"<<std::endl;

    }

    ++threshold;
    thr_value+=100;

  }//end while over thr

  TCanvas *c4= new TCanvas("c4", "Period", 200, 10, 600, 400);

  ((TH1F *)(file->Get(name_channel_difference)))->Draw("AP");
  fit->Draw("SAME");
  c4->SaveAs("contr.pdf");

  TH2F *a2=new TH2F ("a2","a",10,1950.,2950.,10,25,75);       //se il canale e' calibrato, l'intervallo in y e' 4 - 16 altrimenti 25 - 75
  a2->SetTitle("Difference CH1-CH2");                                   // !!!
  a2->GetXaxis()->SetTitle("Threshold");
  a2->GetYaxis()->SetTitle("Resolution [ps]");
  a2->SetStats(0);


  TCanvas *c3= new TCanvas("c3", "Period", 200, 10, 600, 400);

  std::map<TString,TObject*>::iterator it1= differenceObject.begin();
  a2->Draw();
  it1->second->Draw("PSAME");
  ++it1;
  leg2->Draw();
  for(int y=0;y<7;++y){     //ATTENZIONE!
    it1->second->Draw("PSAME");

    ++it1;
  }
  c3->SaveAs("graph/difference_01.pdf");


  std::cout<<"==========================================="<<std::endl;
  std::cout<<"PERIOD PROFILE STUDY THRESHOLD=2500 "<<std::endl;
  std::cout<<"==========================================="<<std::endl;


  TGraphErrors *periodPr = new TGraphErrors(0);

  TString name1;

  float profile_x,profile_y, pr_err_y;

  threshold=5;
  int peak=0;  
  int graphpoint=0;
  while(peak<7){
    // while(threshold<10){
    name=Form("wave_0_thr_%d_timeCross_%d", threshold,peak);
    
    name1=Form("wave_0_thr_%d_timeCross_%d_%d", threshold, peak, peak+1);
    
    std::cout<<"PEAK: "<<peak<<std::endl;

    profile_x=((TH1F *)(file->Get(name)))->GetMean();
    profile_y=((TH1F *)(file->Get(name1)))->GetMean();
    pr_err_y=((TH1F *)(file->Get(name1)))->GetRMS();

    periodPr->SetPoint(graphpoint,profile_x, profile_y);
    periodPr->SetPointError(graphpoint, 0,pr_err_y);

    std::cout<<"X: "<<profile_x<<" MEAN: "<<profile_y<<" SIGMA profile "<<pr_err_y<<std::endl;

    peak+=2;
    ++graphpoint;
    //    threshold++;
  }//end while over peaks

  TCanvas *p= new TCanvas("p", "Period", 200, 10, 600, 400);

  periodPr->SetTitle("Period @ 2500");
  periodPr->GetXaxis()->SetTitle("t0 [ns]");
  periodPr->GetYaxis()->SetTitle("t1-t0 [ns]");
  periodPr->SetMarkerStyle(4);
  periodPr->SetMarkerColor(46);
  periodPr->Draw("AP");

  p->SaveAs("graph/profile.pdf");



  std::cout<<"==========================================="<<std::endl;
  std::cout<<"NOISE FOR SIMULATION PART"<<std::endl;
  std::cout<<"==========================================="<<std::endl;


  float noise, noise_err, noise_der, der_err;
  TH1F *baseline_reso=new TH1F("baseline_reso","Baseline Resolution CH4", 100,-10.,10.);    // !!!
  TH1F *baseline_reso1=new TH1F("baseline_reso1","Baseline Resolution CH5", 100,-10.,10.);  // !!!
  TGraphErrors *der=new TGraphErrors(0);
  TH1F *der_rms=new TH1F("der_rms","Derivate RMS", 30,4.,12.); 
  graphpoint=0;
  TH2F *der_test=new TH2F("der_test","Derivate RMS",1000,-.5,1024.5,20, -5.,5.);
  TH1F *sample=new TH1F("sample","Samples", 1020, 5.5, 1025.5);

  for(int j=6; j<1025; ++j){

    noise_der=((TProfile *)(file->Get("wave_0_deriv_profile")))->GetBinContent(j); // !!!
    der_err=((TProfile *)(file->Get("wave_0_deriv_profile")))->GetBinError(j);   //  !!!
  
    //std::cout<<"DERIVATE: "<<noise_der<<" ERROR: "<<der_err<<std::endl;

    if(((noise_der>-3) &&(noise_der<3))&& (der_err<10) && (der_err>-10)){

      der->SetPoint(graphpoint,j,noise_der);
      der->SetPointError(graphpoint,1,der_err);
      ++graphpoint;
      noise=((TProfile *)(file->Get("wave_0_residual")))->GetBinError(j+1);  // !!!
      der_rms->Fill(der_err);
      der_test->SetBinContent(j,noise_der,der_err);
      baseline_reso->Fill(noise);
      sample->Fill(j);
      //std::cout<<"NOISE: "<<noise<<" SAMPLE: "<<j<<std::endl;

    }//end if over ch0

    noise_der=((TProfile *)(file->Get("wave_1_deriv_profile")))->GetBinContent(j);  // !!!
    der_err=((TProfile *)(file->Get("wave_1_deriv_profile")))->GetBinError(j);      // !!!

    if(((noise_der>-3) &&(noise_der<3))&& (der_err<10)){

      noise=((TProfile *)(file->Get("wave_1_residual")))->GetBinError(j+1);    // !!!

      std::cout<<"Noise second channel: "<<noise<<std::endl;

      baseline_reso1->Fill(noise);

      std::cout<<"NOISE: "<<noise<<" SAMPLE: "<<j<<std::endl;

    }//end if over ch1


  }//end loop over samples

  std::cout<<"==========================================="<<std::endl;
  std::cout<<"NOISE first Channel= "<<baseline_reso->GetMean()<<std::endl;
  std::cout<<"NOISE second Channel= "<<baseline_reso1->GetMean()<<std::endl;
  std::cout<<"==========================================="<<std::endl;

  TCanvas *c5= new TCanvas("c5", "Period", 200, 10, 600, 400);
  baseline_reso->SetTitle("CH4 Noise");                    // !!!
  baseline_reso->GetXaxis()->SetTitle("Noise");
  baseline_reso->GetYaxis()->SetTitle("# events");

  baseline_reso->Draw();
  c5->SaveAs("graph/noise_ch0.pdf");


  TCanvas *p1= new TCanvas("p1", "Period", 200, 10, 600, 400);
  der->SetTitle("Derivate with cut");
  der->GetXaxis()->SetTitle("Sample");
  der->GetYaxis()->SetTitle("Derivate");
  // der->Draw("AP");
  der_rms->SetTitle("Derivate Study");
  der_rms->GetXaxis()->SetTitle("RMS");
  der_rms->GetYaxis()->SetTitle("# Events");
  //  der_rms->Draw();
  sample->SetTitle("Sample Study");
  sample->GetXaxis()->SetTitle("Samples");
  sample->GetYaxis()->SetTitle("# Events");
  sample->Draw();
  //  der_test->Draw("COLZ");
  p1->SaveAs("graph/derivate.pdf");

  TCanvas *c8= new TCanvas("c8", "Period", 200, 10, 600, 400);

  baseline_reso1->SetTitle("CH6 Noise");                    // !!!
  baseline_reso1->GetXaxis()->SetTitle("Noise");
  baseline_reso1->GetYaxis()->SetTitle("# events");


  baseline_reso1->Draw();
  c8->SaveAs("graph/noise_ch1.pdf");


  //Simulation part

  std::string runFileSim="simulation file";

  if(argc>2){
    std::cout<<"==========================================="<<std::endl;
    std::cout<<"SIMULATION ANALYSIS PART"<<std::endl;
    std::cout<<"==========================================="<<std::endl;
  
    runFileSim=argv[2];
    TFile* fileSim = TFile::Open(runFileSim.c_str());
  
    std::cout<<"Going to analyse file: "<<runFileSim<<std::endl;

    if(fileSim==0){
      std::cout<<"Warning, the simulation file selected doesn't exists!!!"<<std::endl;
      //    exit(-1);
    }
    
    //Difference part

    std::map<TString,TObject*> simObject;
    TString name_sim_map;
    TLegend *leg3=new TLegend(0.6,0.9,0.9,0.7);
    leg3->SetFillColor(0);

    TGraphErrors *diff_sim[8];

    color=1;
    marker=24;
    for(int y=1; y<9; ++y){
      name_sim_map=Form("graph_map_peak_%d_sim",y);
      diff_sim[y-1]=new TGraphErrors(0);

      diff_sim[y-1]->GetXaxis()->SetTitle("Threshold");
      diff_sim[y-1]->GetYaxis()->SetTitle("Period [ns]");
      diff_sim[y-1]->SetMarkerColor(color);
      diff_sim[y-1]->SetMarkerStyle(marker);
      diff_sim[y-1]->SetMarkerSize(0.8);
      diff_sim[y-1]->SetTitle("Simulation Time Difference CH0-CH1");
      leg3->AddEntry(diff_sim[y-1],Form("Point %d", y) ,"p");
      diff_sim[y-1]->GetXaxis()->SetRange(0, 100);
      diff_sim[y-1]->GetHistogram()->SetMaximum(0.015); //Range along Y
      diff_sim[y-1]->GetHistogram()->SetMinimum(-0.015);
      simObject[name_sim_map]=(TObject*) (diff_sim[y-1]);

      //    ++color;
      ++marker;
      if(marker==29) marker=32;
    }//ending run up,down wave part  

    std::cout<<"==========================================="<<std::endl;
    std::cout<<"START READING"<<std::endl;
    std::cout<<"==========================================="<<std::endl;

    threshold=0;
    thr_value=2000;

    TString name_sim_difference;
    float sim_reso;

    std::vector<double> test;
    test.clear();


    // TF1 *gaus=new TF1("gaus","gaus(0)", -0.2,0.2);

    //    gaus->SetParameters(10,0.,0.001);


    while(threshold<10){
      i=0;
      cont=0;
      while(i<8){

	name_sim_difference=Form("waveAlignDifference_0_1_thr_%d_timeCrossDifference_%d", threshold,i);

	std::cout<<"Istogramma: "<<name_sim_difference<<std::endl;
	std::cout<<"THRESHOLD: "<<threshold<<std::endl;

	//      fit->SetParameters(20000,0.0001,0.00001);

	sim_reso=((TH1F *)(fileSim->Get(name_sim_difference)))->GetRMS();

	//      ((TH1F *)(file->Get(name_channel_difference)))->Fit("fit","RQ");

	// reso_err=fit->GetParError(2);

	std::cout<<"SIGMA SIM: "<<sim_reso<<std::endl;

	diff_sim[i]->SetPoint(threshold,thr_value,sim_reso*1000 );
	diff_sim[i]->SetPointError(threshold,0,0 );

	++i;
      }//end while over points i

      if(thr_value==2000 || thr_value==2500){


	//	((TH1F *)(fileSim->Get(name_sim_difference)))->Fit(gaus,"RQ");


	test.push_back(sim_reso);

	std::cout<<"+++++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<"THR: "<<thr_value<<" RESO SIM: "<<sim_reso<<std::endl;
	std::cout<<"+++++++++++++++++++++++++++++++++++++++++"<<std::endl;
      }

      ++threshold;
      thr_value+=100;

    }//end while over thr

    TCanvas *c6= new TCanvas("c6", "Period", 200, 10, 600, 400);

    TH2F *a4=new TH2F ("a4","a",10,1950.,2950.,10,3.,16.);

    a4->GetXaxis()->SetTitle("Threshold");
    a4->GetYaxis()->SetTitle("#sigma [ns]");
    a4->SetStats(0);
    a4->SetTitle("Simulation Resolution");



    std::map<TString,TObject*>::iterator it2= simObject.begin();
    a4->Draw();
    it2->second->Draw("PSAME");
    leg3->Draw();
    ++it2;

    for(int y=0; y<7 ; ++y){
      it2->second->Draw("PSAME");

      ++it2;
    }
    c6->SaveAs("graph/sim_diff.pdf");


    std::cout<<"==========================================="<<std::endl;
    std::cout<<"COMPARISON PART"<<std::endl;
    std::cout<<"==========================================="<<std::endl;

    TCanvas *c7= new TCanvas("c7", "Period", 200, 10, 600, 400);

    std::map<TString,TObject*>::iterator it3= differenceObject.begin();
    std::map<TString,TObject*>::iterator it4= simObject.begin();

    TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.52,0.95,0.97);
    TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.02,0.95,0.47);

    pad1->SetFillColor(0);
   
    //   pad1->Draw();
    pad2->SetFillColor(0);
    pad2->RangeAxis(190.0,-0.02,3000.,0.02);
    // pad2->Draw();

    TH2F *a=new TH2F ("a","a",10,1950.,2950.,10,3.,24.);

    a->GetXaxis()->SetTitle("Threshold");
    a->GetYaxis()->SetTitle("#sigma [ns]");
    a->SetStats(0);
    a->SetTitle("Simulation Comparison");

    //pad1->cd();
    c7->Divide(1,2);
    c7->cd(1);
    a->Draw();
    it3->second->Draw("PSAME");
    ++it3;
    

    it4->second->Draw("PSAME");
    ++it4;
    //    pad2->cd();
    c7->cd(2);
    a->GetXaxis()->SetTitle("Threshold");
    a->GetYaxis()->SetTitle("#sigma [ns]");
    a->SetStats(0);

    a->Draw();
    it3->second->Draw("PSAME");
    ++it3;

    it4->second->Draw("PSAME");
    ++it4;

    int j=1;
    for(int y=0; y<6;++y){
     

      if(j>0){
	//pad2->cd();
	c7->cd(1);
      }else{
	//pad1->cd();
	c7->cd(2);
      }

      it3->second->Draw("PSAME");
      it4->second->Draw("PSAME");

      ++it3;
      ++it4;

      j*=-1;
      std::cout<<"J= "<<j<<std::endl;
    }

    c7->SaveAs("graph/prova.pdf");


    std::map<TString,TObject*> periodSimObject;
    TGraphErrors *periodSim[2][4];

    TString name_sim_map1;

    TLegend *leg4=new TLegend(0.6,0.9,0.9,0.7);
    leg4->SetFillColor(0);

    TLegend *leg5=new TLegend(0.6,0.9,0.9,0.7);
    leg5->SetFillColor(0);

    for(int ch=0;ch<2;++ch){
      color=1;
      marker=24;
      for(int y=1; y<5; ++y){
	name_sim_map1=Form("graph_map_ch_%d_peak_%d", ch,y);
	periodSim[ch][y-1]=new TGraphErrors(0);

	periodSim[ch][y-1]->GetXaxis()->SetTitle("Threshold");
	periodSim[ch][y-1]->GetYaxis()->SetTitle("Period [ns]");
	periodSim[ch][y-1]->SetMarkerColor(color);
	periodSim[ch][y-1]->SetMarkerStyle(marker);
	periodSim[ch][y-1]->SetMarkerSize(0.6);
	if(ch<1){
	  periodSim[ch][y-1]->SetTitle("Simulation Period CH0");
	  leg4->AddEntry(period[ch][y-1],Form("Peak %d", y) ,"p");
	}else{
	  periodSim[ch][y-1]->SetTitle("Simulation Period CH1");
	  leg5->AddEntry(periodSim[ch][y-1],Form("Peak %d", y) ,"p");
	}
	periodSimObject[name_sim_map1]=(TObject*) (periodSim[ch][y-1]);

	++color;
	++marker;
      }//end loop over i=peaks
    }//end loo over channels


    //Reading data                                                                                                                                                                                            
    channel=0;
    while(channel<2){
      threshold=0;
      thr_value=2000;

      std::cout<<"==========================================="<<std::endl;
      std::cout<<"READING Simulated channel: "<<channel<<std::endl;
      std::cout<<"==========================================="<<std::endl;

      while(threshold<10){
	i=0;
	cont=0;
	while(i<7){

	  if(channel<1){
	    name=Form("wave_%d_thr_%d_timeCross_%d_%d", channel, threshold,i,i+1);
	  }else{
	    name=Form("wave_%d_thr_%d_timeCross_%d_%d", channel, threshold,i,i+1);
	  }

	  //wave_timeCross[channel][threshold][i]=new TH1F(name,name,10000,0,1024);                                                                                                                           
	  std::cout<<"Istogramma: "<<name<<std::endl;

	  mean_tmp=((TH1F *)(fileSim->Get(name)))->GetMean();
	  sigma_tmp=((TH1F *)(fileSim->Get(name)))->GetRMS();
	  std::cout<<"MEAN: "<<mean_tmp<<" SIGMA: "<<sigma_tmp<<" THR: "<<thr_value<<" CONT: "<<cont<<std::endl;


	  periodSim[channel][cont]->SetPoint(threshold,thr_value,mean_tmp);
	  periodSim[channel][cont]->SetPointError(threshold,0,sigma_tmp);

	  i+=2;
	  ++cont;
	}//end while i                                                                                                                                                                                        
	thr_value+=100;
	++threshold;
      }//end while threshold<10                                                                                                                                                                               
      ++channel;
    }// end while channel<2 


    //end reading the simulation file

    std::map<TString,TObject*>::iterator it5= periodSimObject.begin();

    TCanvas *c10= new TCanvas("c10", "Period", 200, 10, 600, 400);


    TH2F *b=new TH2F ("b","b",10,1950.,2950.,10,10,36);

    b->GetXaxis()->SetTitle("Threshold");
    b->GetYaxis()->SetTitle("Period [ns]");
    b->SetStats(0);
    b->SetTitle("Simulation Period");

    b->Draw();

    it5->second->Draw("PSAME");
    ++it5;
    leg4->Draw();
    for(int p=0; p<3;++p){

      std::cout<<"ITERATOR: "<<&it<<std::endl;

      it5->second->Draw("PSAME");

      ++it5;
    }

    c10->SaveAs("graph/periodo0Sim.pdf");



    TCanvas *c11= new TCanvas("c2", "Period", 200, 10, 600, 400);
    b->Draw();
    it5->second->Draw("PSAME");
    ++it5;
    leg5->Draw();
    for(int p=0; p<3;++p){

      std::cout<<"ITERATOR: "<<&it<<std::endl;

      it5->second->Draw("PSAME");

      ++it5;

    }

    c11->SaveAs("graph/periodo1Sim.pdf");


    std::cout<<"Best Reso: "<<test[1]<<" Worst reso: "<<test[0]<<std::endl;



  }//end if argc>2
  
  return 0;
  

}
