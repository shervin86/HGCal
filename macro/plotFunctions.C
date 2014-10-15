#include<vector>
#include<set>
#include<map>
#include<utility>
void PlotAll(TFile *f){

  Int_t n = f->GetListOfKeys()->GetSize();
  TCanvas *c1 = new TCanvas("c1","");
  c1->cd();
  for(Int_t i=0; i<n; i++){
    TKey *key= (TKey *)f->GetListOfKeys()->At(i);
    if(TString(key->GetClassName())!="TMultiGraph") continue;
    TMultiGraph *g = (TMultiGraph *) f->Get(key->GetName());
    TString name = g->GetName();
    std::cout << name << std::endl;
    g->Draw("A");
    //g->Draw("Al");
       c1->BuildLegend();
    g->SaveAs("tmp/img/"+name+".pdf");
    g->SaveAs("tmp/img/"+name+".png");
  }
}

Int_t pPaletteColor[15] = {kBlack,   
			   kRed,   
			   kBlue,   
			   kGreen+2,   
			   kGray+1,  
			   kMagenta, kRed+2, kBlue+2, kAzure, kYellow+4, kGreen+4, kMagenta+3, kBlue-4};

TMultiGraph * PlotAllIV(TCanvas *c1, TFile *f, TString match, TString plotOption="p"){
  TMultiGraph *m = new TMultiGraph();
  unsigned int j=0;
  Int_t n = f->GetListOfKeys()->GetSize();
  c1->cd();
  for(Int_t i=0; i<n; i++){
    TKey *key= (TKey *)f->GetListOfKeys()->At(i);
    if(TString(key->GetClassName())!="TGraph") continue;
    TGraph *g = (TGraph *) f->Get(key->GetName());
    TString name = g->GetName();
    std::cout << name << std::endl;
    if(!name.Contains(match)) continue;
    g->SetFillColor(0);
    g->SetLineWidth(3);

    if(plotOption.Contains("p")) g->SetMarkerColor(pPaletteColor[j]);
    else g->SetMarkerSize(0);

    if(plotOption.Contains("l")) g->SetLineColor(pPaletteColor[j]);
    else g->SetLineColor(0);

    m->Add(g,plotOption);
    j++;
  }
 
  m->Draw("A");
  m->GetXaxis()->SetTitle("U [V]");
  m->GetYaxis()->SetTitle("I [A]");
 TLegend *leg=c1->BuildLegend(0.82,0.2,1,1);
 leg->SetBorderSize(0);
 return m;
}

std::vector<TMultiGraph*> GetCCEs(TFile *f, TString match){
  std::vector<TMultiGraph*> vec;

  Int_t n = f->GetListOfKeys()->GetSize();
  for(Int_t i=0; i<n; i++){
    TKey *key= (TKey *)f->GetListOfKeys()->At(i);
    if(TString(key->GetClassName())!="TMultiGraph") continue;
    TMultiGraph *g = (TMultiGraph *) f->Get(key->GetName());
    TString name = g->GetName();
    if(!name.Contains(match))continue;
    std::cout << name << std::endl;
    vec.push_back(g);
  }
  return vec;
}

std::vector<TMultiGraph*> SplitCCEs(std::vector<TMultiGraph*> irrVec){
  std::vector<TMultiGraph*> newVec;
  
  std::set<TString> refLabels, irrLabels;
  std::map<TString, Int_t> newVecIndex;

  Int_t colorBase[10]={kRed,kBlue,kGreen,kOrange};
  Int_t colorMod[10]={0,2,+3,-kGreen+kBlack};
  Int_t i=0,j=0;


  for(auto itr=irrVec.begin();      itr!=irrVec.end();      itr++){
    TGraph *g = (TGraph*)(*itr)->GetListOfGraphs()->At(1);
    if(refLabels.count(g->GetTitle())!=0){
      continue;
    }
    TMultiGraph *m = new TMultiGraph();
    m->SetTitle(g->GetTitle());
    //    m->SetName(g->GetName());
    refLabels.insert(g->GetTitle());
    g->SetMarkerColor(colorBase[j]);
    g->SetLineColor(colorBase[j]);
    g->SetLineWidth(0);
    g->SetMarkerStyle(23);
    g->SetFillStyle(0);
    g->SetFillColor(0);
    m->Add(g, "p");
    m->Draw("A");
    m->GetXaxis()->SetTitle("U [V]");
    m->GetYaxis()->SetTitle("CCE");

    newVec.push_back(m);
    std::pair<TString, Int_t> p; p.first=g->GetTitle();p.second=j;
    newVecIndex.insert(p);
    j++;
  }

  refLabels.clear();
  j=-1;
  i=-1;
  for(auto itr=irrVec.begin();      itr!=irrVec.end();      itr++){
    g = (TGraph*)(*itr)->GetListOfGraphs()->At(1);
    if((*itr)==NULL) continue;
    TGraph *g = (TGraph*)(*itr)->GetListOfGraphs()->At(0);
    TString irrTitle=g->GetTitle();
    TString refTitle = ((TGraph*)(*itr)->GetListOfGraphs()->At(1))->GetTitle();
    if(refLabels.count(refTitle)==0){
     refLabels.insert(refTitle);
      j++;
      i=-1;
      std::cout << refTitle << std::endl;
    }

    if(irrLabels.count(irrTitle)==0){
      irrLabels.insert(irrTitle);
      i++;
    }

    std::cout << j << i << std::endl;
    g->SetMarkerColor(colorBase[j]+colorMod[i]);
    g->SetFillStyle(0);
    g->SetFillColor(0);
    newVec[newVecIndex[refTitle]]->Add(g, "p");
  
 }
}  


  
void CCE_vs_V(TCanvas *c1, std::vector<TMultiGraph*> irrVec){
  TMultiGraph *CCEvsV = new TMultiGraph();
  std::set<TString> refLabels, irrLabels;
  Int_t colorBase[10]={kRed,kBlue,kGreen,kOrange};
  Int_t colorMod[10]={0,2,+3,-kGreen+kBlack};
  Int_t i=0,j=0;

  TLine *l = new TLine(0,1,1000,1);
  for(auto itr=irrVec.begin();      itr!=irrVec.end();      itr++){
    TGraph *g = (TGraph*)(*itr)->GetListOfGraphs()->At(1);
    if(refLabels.count(g->GetTitle())!=0){
      continue;
    }
    refLabels.insert(g->GetTitle());
    g->SetMarkerColor(colorBase[j]);
    g->SetLineColor(colorBase[j]);
    g->SetLineWidth(0);
    g->SetMarkerStyle(23);
    g->SetFillStyle(0);
    g->SetFillColor(0);
    CCEvsV->Add(g, "p");
    j++;
  }

  refLabels.clear();
  j=-1;
  i=-1;
  for(auto itr=irrVec.begin();      itr!=irrVec.end();      itr++){
    g = (TGraph*)(*itr)->GetListOfGraphs()->At(1);
    if((*itr)==NULL) continue;
    TGraph *g = (TGraph*)(*itr)->GetListOfGraphs()->At(0);
    TString irrTitle=g->GetTitle();
    TString refTitle = ((TGraph*)(*itr)->GetListOfGraphs()->At(1))->GetTitle();
    if(refLabels.count(refTitle)==0){
     refLabels.insert(refTitle);
      j++;
      i=-1;
      std::cout << refTitle << std::endl;
    }

    if(irrLabels.count(irrTitle)==0){
      irrLabels.insert(irrTitle);
      i++;
    }

    std::cout << j << i << std::endl;
    g->SetMarkerColor(colorBase[j]+colorMod[i]);
    g->SetFillStyle(0);
    g->SetFillColor(0);
    CCEvsV->Add(g, "p");
  
 }
  CCEvsV->Draw("A");
  CCEvsV->GetXaxis()->SetTitle("U [V]");
  CCEvsV->GetYaxis()->SetTitle("CCE");
  TLegend *leg = c1->BuildLegend(0.82,0.2,1,1);
  leg->SetBorderSize(0);
  l->Draw();
}

TCanvas * Q_CCE_vs_V(TMultiGraph *refQvsV, TMultiGraph *irrQvsV, TMultiGraph *CCEvsV, TString outDir="img/"){
  TMultiGraph *QvsV = new TMultiGraph();

  TCanvas *c1 = new TCanvas("c1","",1920,1080);
  c1->SetGridy();
  TPad *pad1 = new TPad("pad1","Q vs V",0,0,1,1);
  pad1->Draw();
  pad1->SetGridy();
  //  TPad *pad2 = new TPad("pad2","CCE vs V",0,0,1,1);
  TPad *pad2 = new TPad("pad2","CCE vs V",0,0,1,1);
  pad2->SetFillStyle(4000); //will be transparent
  pad2->SetFrameFillStyle(0);
  pad2->Draw();

  pad1->cd();

  TMultiGraph *g_m = irrQvsV;
  TGraph *g = (TGraph *)g_m->GetListOfGraphs()->At(0);
  g->SetTitle("irradiated");
  g->SetMarkerStyle(22);
  g->SetMarkerColor(kBlue);
  g->SetFillColor(0);


  //QvsV->Draw("A");
  QvsV->Add(refQvsV);
  QvsV->Add(g,"p");


  QvsV->Draw("A");
  //  irrN100_CCEvsV->Draw("A");
  //  QvsV->Draw("AY");

  //g = (TGraph *)QvsV->GetListOfGraphs()->At(0);
  //g->SetTitle("reference");
  QvsV->GetYaxis()->SetTitle(((TGraph *)QvsV->GetListOfGraphs()->At(0))->GetYaxis()->GetTitle());
  QvsV->GetYaxis()->SetRangeUser(0,4e-11);
  QvsV->GetXaxis()->SetRangeUser(0,1000);
  //  g->SetFillColor(0);

  
  //c1->BuildLegend();
  //c1->SaveAs(outDir+"irrN100_QvsV.pdf");
  
  pad2->cd();


  Int_t ci = TColor::GetColor("#ff0000");
  CCEvsV->GetHistogram()->GetXaxis()->SetRangeUser(0,1000);
  CCEvsV->GetHistogram()->Draw("Y+");
  //CCEvsV->GetHistogram()->Draw("");
  CCEvsV->GetHistogram()->GetYaxis()->SetTitleColor(ci);
  
  CCEvsV->GetYaxis()->SetRangeUser(0,1);
  CCEvsV->GetXaxis()->SetRangeUser(0,1000);
 TGaxis *gaxis = new TGaxis(CCEvsV->GetXaxis()->GetXmax(), //*(1-pad2->GetRightMargin()),
			     0, //pad2->GetBottomMargin(),
			     //pad2->GetUymin()+(pad2->GetBottomMargin()*pad2->GetUymax())),
			     CCEvsV->GetXaxis()->GetXmax(),
			     //pad2->GetUxmax()*(1-pad2->GetRightMargin()),
			     1, //1-pad2->GetTopMargin(),
			     //pad2->GetUymax(),
			     0,1,510,"+L");
   gaxis->SetLabelOffset(0.005);
   gaxis->SetLabelSize(0.04);
   gaxis->SetTickSize(0.03);
   gaxis->SetGridLength(0);
   gaxis->SetTitleOffset(1);
   gaxis->SetTitleSize(0.04);
   gaxis->SetTitleColor(1);
   gaxis->SetTitleFont(62);
   gaxis->SetLabelColor(ci);

   gaxis->SetLineColor(ci);
   gaxis->Draw();

  ((TGraph *)CCEvsV->GetListOfGraphs()->At(0))->SetMarkerColor(ci);
  CCEvsV->Draw("");
  CCEvsV->GetXaxis()->SetRangeUser(0,1000);
  TLegend *leg = new TLegend(0.15,0.6,0.5,0.9);
  TLegendEntry *entry = leg->AddEntry(refQvsV->GetListOfGraphs()->At(0), "reference", "p");
  
  leg->AddEntry(irrQvsV->GetListOfGraphs()->At(0), "irradiated", "p");
  leg->AddEntry(CCEvsV->GetListOfGraphs()->At(0), "CCE", "p");
  
  leg->Draw();
 

  pad1->cd();
  QvsV->Draw("A");
 
  return c1;
}
