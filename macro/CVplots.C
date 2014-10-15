{


 irrN1621->Draw("A");

 irrN1621->GetYaxis()->SetRangeUser(1e-11, 1e-6);
 c1->SetLogy();
 c1->BuildLegend(0.7,0.2,0.85,0.6);
 TPaveText *pv = new TPaveText(0.2,0.4,0.7,0.6,"NDC");
 pv->AddText("FZ200N_07_DiodeL_11, phi=2.5e15, T=-20");
 pv->Draw("same");
}
