//
// Created by mikhail on 8/1/21.
//
// #include <DataContainer.hpp>

void v1_run8_clean(){
  auto file = TFile::Open( "/home/mikhail/bmn_run8/correlation.vf.recent.noeff.2024.06.22.root" );
  std::vector<std::string> ep_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED" };
  std::vector<std::string> res_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED", "Tneg_RESCALED", "Tpos_RESCALED"};
  std::vector<std::string> sts_vectors{ "Tneg_RESCALED", "Tpos_RESCALED" };
  std::array<std::string, 2> components{"x1x1centrality", "y1y1centrality"};

  auto file_out = TFile::Open( "~/Flow/BM@N/vf.recent.noeff.2024.06.22.root", "recreate" );
  file_out->cd();
  file_out->mkdir("resolutions");
  file_out->mkdir("proton");

  for( auto qa : ep_vectors ){
    Correlation<2> proton_qa( file, "/", std::array{"proton_RESCALED"s, qa}, components);

    auto res_v = Functions::VectorResolutions3S( file, "/", qa, res_vectors, components );
    for( auto R1 : res_v ) {
      auto v1_proton = proton_qa * 2 / R1;
      
      file_out->cd("resolutions");
      R1.Save("R1."+R1.Title());

      file_out->cd("proton");
      v1_proton.Save("v1."+R1.Title());
    }
  }

  file_out->Close();
  file->Close();
}