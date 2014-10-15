{
  TString outDir="img/";

  gROOT->ProcessLine(".L macro/plotFunctions.C");
  TCanvas *c1 = new TCanvas("c1","", 1920, 1080);
  float startSignal=0.85e-7, endSignal=1.00e-7;
  checkBaselines_m30->Draw("A");
  c1->BuildLegend(0.7,0.7,1,1);
  c1->SaveAs(outDir+"checkBaselines_m30.png");
  checkBaselines_m30->GetXaxis()->SetRangeUser(startSignal, endSignal);
  c1->SaveAs(outDir+"checkBaselines_m30-zoom.png");
  
  checkBaselines_m20->Draw("A");
  c1->BuildLegend(0.7,0.7,1,1);
  c1->SaveAs(outDir+"checkBaselines_m20.png");
  checkBaselines_m20->GetXaxis()->SetRangeUser(startSignal, endSignal);
  c1->SaveAs(outDir+"checkBaselines_m20-zoom.png");
  
  c1=   Q_CCE_vs_V(refN107_QvsV, irrN107_QvsV, irrN107_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN109.png");

  c1=   Q_CCE_vs_V(refN105_QvsV, irrN109_QvsV, irrN109_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN109.png");

  c1=   Q_CCE_vs_V(refN110_QvsV, irrN110_QvsV, irrN110_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN110.png");

  c1=   Q_CCE_vs_V(refN111_QvsV, irrN111_QvsV, irrN111_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN111.png");

  c1=   Q_CCE_vs_V(refN120_QvsV, irrN120_QvsV, irrN120_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN120.png");

  c1=   Q_CCE_vs_V(refN121_QvsV, irrN121_QvsV, irrN121_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN121.png");

  c1=   Q_CCE_vs_V(refN120_QvsV, irrN122_QvsV, irrN122_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN122.png");

  c1=   Q_CCE_vs_V(refN121_QvsV, irrN123_QvsV, irrN123_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN123.png");

  c1=   Q_CCE_vs_V(refN130_QvsV, irrN130_QvsV, irrN130_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN130.png");


  c1=   Q_CCE_vs_V(refN131_QvsV, irrN131_QvsV, irrN131_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN131.png");
  
  c1=   Q_CCE_vs_V(refN130_QvsV, irrN132_QvsV, irrN132_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN132.png");

  c1=   Q_CCE_vs_V(refN131_QvsV, irrN133_QvsV, irrN133_CCEvsV, outDir);
  c1->SaveAs(outDir+"irrN133.png");


}


}
  TCanvas *c1 = new TCanvas("c1","");
  
  TPad *pad1 = new TPad("pad1","Q vs V",0,0,1,1);
  pad1->Draw();
  TPad *pad2 = new TPad("pad2","CCE vs V",0,0,1,1);
  pad2->SetFillStyle(4000); //will be transparent
  pad2->SetFrameFillStyle(0);

  refN100_QvsV->Draw("A");
  TMultiGraph *g_m = (TMultiGraph *)_file0->Get("irrN100_QvsV");
  TGraph *g = (TGraph *)g_m->GetListOfGraphs()->At(0);
  g->SetTitle("irradiated");
  g->SetMarkerStyle(22);
  g->SetMarkerColor(kBlue);
  g->SetFillColor(0);
  refN100_QvsV->Add(g,"p");


  refN100_QvsV->Draw("AY");
  irrN100_CCEvsV->Draw("A");
  refN100_QvsV->Draw("AY");

  g = (TGraph *)refN100_QvsV->GetListOfGraphs()->At(0);
  g->SetTitle("reference");
  refN100_QvsV->GetYaxis()->SetRangeUser(0,4e-11);
  g->SetFillColor(0);

  
//  c1->BuildLegend();
  c1->SaveAs(outDir+"refN100_irrN100_QvsV.pdf");
  c1->cd();
						       
  
  pad2->Draw();
  pad2->cd();

  Float_t rightmax = 1.1*g_m->GetYaxis()->GetXmax();
  rightmax=1;

   ci = TColor::GetColor("#ff0000");

  irrN100_CCEvsV->GetHistogram()->Draw("Y+");
  irrN100_CCEvsV->GetHistogram()->GetYaxis()->SetTitleColor(ci);
  //irrN100_CCEvsV->Draw("Y+");
  irrN100_CCEvsV->GetYaxis()->SetRangeUser(0,1);
  TGaxis *gaxis = new TGaxis(irrN100_CCEvsV->GetXaxis()->GetXmax(), //*(1-pad2->GetRightMargin()),
			     0, //pad2->GetBottomMargin(),
			     //pad2->GetUymin()+(pad2->GetBottomMargin()*pad2->GetUymax())),
			     irrN100_CCEvsV->GetXaxis()->GetXmax(),
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

((TGraph *)irrN100_CCEvsV->GetListOfGraphs().At(0)).SetMarkerColor(ci);
  irrN100_CCEvsV->Draw("P");
}

  checkBaselines->Draw("A");
  //   TCanvas *c1 = new TCanvas("checkBaselines","");
  //c1->cd();
 
  
  refN100_spectra->Draw("A");
  c1->SaveAs(outDir+"refN100_spectra.pdf");
  
  refN100_spectra->Draw("A");
  c1->SaveAs(outDir+"refN100_spectra.pdf");


  // need to verify the correctness of the baseline subtraction!
  refN100_spectra->GetXaxis()->SetRangeUser(0.8e-7, 0.88e-7);
  refN100_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line(0.8e-7,0,0.88e-7,0);
  line.Draw();
  line.SetLineWidth(2);
  c1->SaveAs(outDir+"refN100_spectra-baseline_pre_zoom.pdf");


  // long tail at low bias voltage (slow)
  // fast signal is below baseline!
  refN100_spectra->GetXaxis()->SetRangeUser(0.9e-7, 1.1e-7);
  refN100_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line2(0.9e-7, 0, 1.1e-7,0);
  line2.SetLineWidth(2);
  refN100_spectra->Draw("A");
  line2.Draw();
  c1->SaveAs(outDir+"refN100_spectra-baseline_post_zoom.pdf");


  refN110_spectra->Draw("A");
  c1->SaveAs(outDir+"refN110_spectra.pdf");


  // need to verify the correctness of the baseline subtraction!
  refN110_spectra->GetXaxis()->SetRangeUser(0.8e-7, 0.88e-7);
  refN110_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line(0.8e-7,0,0.88e-7,0);
  line.Draw();
  line.SetLineWidth(2);
  c1->SaveAs(outDir+"refN110_spectra-baseline_pre_zoom.pdf");


  // long tail at low bias voltage (slow)
  // fast signal is below baseline!
  refN110_spectra->GetXaxis()->SetRangeUser(0.9e-7, 1.1e-7);
  refN110_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line2(0.9e-7, 0, 1.1e-7,0);
  line2.SetLineWidth(2);
  refN110_spectra->Draw("A");
  line2.Draw();
  c1->SaveAs(outDir+"refN110_spectra-baseline_post_zoom.pdf");


  //------------------------------
  irrN100_spectra->Draw("A");
  c1->SaveAs(outDir+"irrN100_spectra.pdf");


  // need to verify the correctness of the baseline subtraction!
  irrN100_spectra->GetXaxis()->SetRangeUser(0.8e-7, 0.88e-7);
  irrN100_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line(0.8e-7,0,0.88e-7,0);
  line.Draw();
  line.SetLineWidth(2);
  c1->SaveAs(outDir+"irrN100_spectra-baseline_pre_zoom.pdf");


  // long tail at low bias voltage (slow)
  // fast signal is below baseline!
  irrN100_spectra->GetXaxis()->SetRangeUser(0.9e-7, 1.1e-7);
  irrN100_spectra->GetYaxis()->SetRangeUser(-0.1e-3, 0.3e-3);
  TLine line2(0.9e-7, 0, 1.1e-7,0);
  line2.SetLineWidth(2);
  irrN100_spectra->Draw("A");
  line2.Draw();
  c1->SaveAs(outDir+"irrN100_spectra-baseline_post_zoom.pdf");


  //------------------------------
  /// plot for each irradiated diode one Voltage spectrum for the diode and the reference
  
      
  TGraph *g_irr= (TGraph *)irrN100_spectra->GetListOfGraphs().At(50);
  TGraph *g_ref= (TGraph *)refN100_spectra->GetListOfGraphs().At(50);
    
  g_ref->Draw("Al");
  g_ref->GetXaxis()->SetRangeUser(0.085e-6,0.116e-6);
  g_ref->SetLineWidth(2);
  g_irr->Draw("l same");
  g_irr->SetLineColor(kBlack);

  irrN100_QvsV->Draw("A");


  // check if Vdep correspond in CV, IV and TCT measurements
  irrP122_QvsV->Draw("A");
  refP122_QvsV->Draw("A");

  TFitResultPtr fitresPtr = refP122_spectra->Fit("pol1","S","",0.095e-6,0.115e-6);
  TF1* f = refP122_spectra->GetFunction("pol1");
  f->SetLineWidth(5);
  refP122_spectra->Draw("A");
  f->Draw("same");
  fitresPtr->Draw();
  c1->SaveAs("refP122.pdf");

  irrP122_spectra->Draw("A");
  // f->SetName("pol1A");
  // irrP122_spectra->Fit(f,"S","",0.1e-6,0.115e-6);
  // f = irrP122_spectra->GetFunction("pol1A");
  // f->SetLineWidth(5);
  // f->SetLineColor(kBlue);
  // f->Draw("same");
  // TF1 *f2= f->Clone("pol1B");
  // fitresPtr = irrP122_spectra->Fit(f2,"S","",0.093e-6,0.096e-6);
  irrP122_spectra->GetXaxis()->SetRangeUser(0.085e-6,0.116e-6);
  // //f = irrP122_spectra->GetFunction("pol1");
  // f2->SetLineWidth(5);
  // //irrP122_spectra->Draw("A");
  // f2->Draw("same");
  // fitresPtr->Draw();
  c1->SaveAs("irrP122.pdf");
}
