//
// Created by mikhail on 8/1/21.
//
void v1_run8(){
  auto file = TFile::Open( "/home/mikhail/Correlations/correlation_run8_vf_dca_2023_09_16.root" );
  std::vector<std::string> ep_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED" };
  std::vector<std::string> res_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED", "Tneg_RESCALED", "Tpos_RESCALED"};
  std::vector<std::string> sts_vectors{ "Tneg_RESCALED", "Tpos_RESCALED" };
  std::array<std::string, 2> components{"x1x1centrality", "y1y1centrality"};

  auto file_out = TFile::Open( "~/Flow/BM@N/run8_vf_dca_2023_09_16.root", "recreate" );
  file_out->cd();
  file_out->mkdir("resolutions");
  file_out->mkdir("proton");
  file_out->mkdir("proton400");
  file_out->mkdir("proton700");

  for( auto qa : ep_vectors ){
    Correlation<2> proton_qa( file, "/", std::array{"proton_RESCALED"s, qa}, components);
    // Correlation<2> proton400_qa( file, "/", std::array{"proton400_RESCALED"s, qa}, components);
    // Correlation<2> proton700_qa( file, "/", std::array{"proton700_RESCALED"s, qa}, components);
    auto res_v = Functions::VectorResolutions3S( file, "/", qa, res_vectors, components );
    for( auto R1 : res_v ) {
      auto v1_proton = proton_qa * 2 / R1;
      // auto v1_proton400 = proton400_qa * 2 / R1;
      // auto v1_proton700 = proton700_qa * 2 / R1;

      file_out->cd("resolutions");
      R1.Save("R1."+R1.Title());

      file_out->cd("proton");
      v1_proton.Save("v1."+R1.Title());
      
      // file_out->cd("proton400");
      // v1_proton400.Save("v1."+R1.Title());
      
      // file_out->cd("proton700");
      // v1_proton700.Save("v1."+R1.Title());

    }
  }
//  *******************************************
//  Calculation of flow for F2 with 4-sub method
//  *******************************************
//
  {
    std::string qa = "F2_RESCALED";
    auto res_v = Functions::VectorResolutions4S(file, "/", qa, sts_vectors, ep_vectors, components);
    Correlation<2> proton_qa( file, "/", std::array{"proton_RESCALED"s, qa}, components);
    // Correlation<2> proton400_qa( file, "/", std::array{"proton400_RESCALED"s, qa}, components);
    // Correlation<2> proton700_qa( file, "/", std::array{"proton700_RESCALED"s, qa}, components);
    for (auto R1 : res_v) {
      auto v1_proton = proton_qa*2 / R1;
      // auto v1_proton400 = proton400_qa * 2 / R1;
      // auto v1_proton700 = proton700_qa * 2 / R1;

      file_out->cd("resolutions");
      R1.Save("R1." + R1.Title());

      file_out->cd("proton");
      v1_proton.Save("v1."+R1.Title());

      // file_out->cd("proton400");
      // v1_proton400.Save("v1."+R1.Title());
      
      // file_out->cd("proton700");
      // v1_proton700.Save("v1."+R1.Title());
    }
  }

  file_out->Close();
  file->Close();
}