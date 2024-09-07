//
// Created by mikhail on 8/1/21.
//
// #include <DataContainer.hpp>
void v1_run8_chi2(){
  auto file = TFile::Open( "/home/mikhail/bmn_run8/correlation.vf.recent.chi2.2024.06.16.root" );
  std::vector<std::string> ep_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED" };
  std::vector<std::string> res_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED", "Tneg_RESCALED", "Tpos_RESCALED"};
  std::vector<std::string> sts_vectors{ "Tneg_RESCALED", "Tpos_RESCALED" };
  std::array<std::string, 2> components{"x1x1centrality", "y1y1centrality"};

  auto file_out = TFile::Open( "~/Flow/BM@N/vf.recent.chi2.2024.06.16.root", "recreate" );
  file_out->cd();
  file_out->mkdir("resolutions");
  file_out->mkdir("proton_chi2_3");
  file_out->mkdir("proton_chi2_4");
  file_out->mkdir("proton_chi2_5");

  for( auto qa : ep_vectors ){
    Correlation<2> proton_chi2_3_qa( file, "/", std::array{"proton_chi2_3_RESCALED"s, qa}, components);
    Correlation<2> proton_chi2_4_qa( file, "/", std::array{"proton_chi2_4_RESCALED"s, qa}, components);
    Correlation<2> proton_chi2_5_qa( file, "/", std::array{"proton_chi2_5_RESCALED"s, qa}, components);

    auto res_v = Functions::VectorResolutions3S( file, "/", qa, res_vectors, components );
    for( auto R1 : res_v ) {
      auto v1_proton_chi2_3 = proton_chi2_3_qa * 2 / R1;
      auto v1_proton_chi2_4 = proton_chi2_4_qa * 2 / R1;
      auto v1_proton_chi2_5 = proton_chi2_5_qa * 2 / R1;
      
      file_out->cd("resolutions");
      R1.Save("R1."+R1.Title());

      file_out->cd("proton_chi2_3");
      v1_proton_chi2_3.Save("v1."+R1.Title());

      file_out->cd("proton_chi2_4");
      v1_proton_chi2_4.Save("v1."+R1.Title());

      file_out->cd("proton_chi2_5");
      v1_proton_chi2_5.Save("v1."+R1.Title());
    }
  }

//  *******************************************
//  Calculation of flow for F2 with 4-sub method
//  *******************************************
//
  // {
  //   std::string qa = "F2_RESCALED";
  //   auto res_v = Functions::VectorResolutions4S(file, "/", qa, sts_vectors, ep_vectors, components);
  //   Correlation<2> proton_qa( file, "/", std::array{"proton_RESCALED"s, qa}, components);
  //   Correlation<2> proton_l_qa( file, "/", std::array{"proton_400_RESCALED"s, qa}, components);
  //   Correlation<2> proton_r_qa( file, "/", std::array{"proton_700_RESCALED"s, qa}, components);
    
  //   for (auto R1 : res_v) {
  //     auto v1_proton = proton_qa * 2 / R1;
  //     auto v1_proton_l = proton_l_qa * 2 / R1;
  //     auto v1_proton_r = proton_r_qa * 2 / R1;

  //     file_out->cd("resolutions");
  //     R1.Save("R1." + R1.Title());

  //     file_out->cd("proton");
  //     v1_proton.Save("v1."+R1.Title());

  //     file_out->cd("proton_400");
  //     v1_proton_l.Save("v1."+R1.Title());

  //     file_out->cd("proton_700");
  //     v1_proton_r.Save("v1."+R1.Title());
  //   }
  // }

  file_out->Close();
  file->Close();
}