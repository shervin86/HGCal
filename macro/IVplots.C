{
  TMultiGraph multi_N, multi_P;

  TLegend *leg_P = new TLegend(0.1,0.7,0.5,1);
  TLegend *leg_N = new TLegend(0.1,0.7,0.5,1);

  multi_P->Add(refP120,"p");

  irrN1500->SetMarkerStyle(22);
  multi_N->Add(irrN1500,"p");
  leg_N->AddEntry(irrN1500,irrN1500->GetTitle(),"p");

  irrN1501->SetMarkerStyle(26);
  multi_N->Add(irrN1501,"p");
  leg_N->AddEntry(irrN1501,irrN1501->GetTitle(),"p");

  irrN1510->SetMarkerStyle(29);
  multi_N->Add(irrN1510,"p");
  leg_N->AddEntry(irrN1510,irrN1510->GetTitle(),"p");

  irrN1511->SetMarkerStyle(30);
  multi_N->Add(irrN1511,"p");
  leg_N->AddEntry(irrN1511,irrN1511->GetTitle(),"p");

  multi_N->Add(irrN1611,"p");
  irrN1611->SetMarkerStyle(20);
  leg_N->AddEntry(irrN1611,irrN1611->GetTitle(),"p");

  multi_N->Add(irrN1612,"p");
  irrN1612->SetMarkerStyle(24);
  leg_N->AddEntry(irrN1612,irrN1612->GetTitle(),"p");

 
  multi_N->Add(irrN1621,"p");
  irrN1621->SetMarkerStyle(21);
  leg_N->AddEntry(irrN1621,irrN1621->GetTitle(),"p");

  Double_t *y1 = irrN1621->GetY();
  Double_t *y2 = irrN1622->GetY();
  Double_t y3[200];
  for(Int_t i=0; i< irrN1621->GetN(); i++){
    y3[i] = y1[i]/y2[i];
  }
  TGraph g(irrN1621->GetN(), irrN1621->GetX(), y3);

  multi_N->Add(irrN1622,"p");
  irrN1622->SetMarkerStyle(25);
  leg_N->AddEntry(irrN1622,irrN1622->GetTitle(),"p");
  



  //multi_P->Draw("A");
  irrN1611->Draw("A");
  multi_N->Draw("A");
  multi_N->GetXaxis()->SetTitle(irrN1611->GetXaxis()->GetTitle());
  multi_N->GetYaxis()->SetTitle(irrN1611->GetYaxis()->GetTitle());
  
  leg_N->Draw();

  //g->Draw("AP");
  g->SetMarkerStyle(20);
}
