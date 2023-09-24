//
// Created by mikhail on 8/1/21.
//
void v1_mpd_fxt(){
  auto file = TFile::Open( "~/Correlations/correlation_mpd_fxt_1.46agev_2023_09_01.root" );
  std::vector<std::string> ep_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED" };
  std::vector<std::string> res_vectors{ "F1_RESCALED", "F2_RESCALED", "F3_RESCALED", "Tp_RESCALED", "Tpi_RESCALED", };
  std::vector<std::string> sts_vectors{ "Tp_RESCALED", "Tpi_RESCALED", };
  std::array<std::string, 2> q1q1_components{"x1x1b_norm", "y1y1b_norm"};
  std::array<std::string, 2> q2q2_components{"x2x2b_norm", "y2y2b_norm"};

  auto file_out = TFile::Open( "~/Flow/BM@N/mpd_fxt_1.46agev_2023_09_01.root", "recreate" );
  file_out->cd();
  file_out->mkdir("resolutions");
  file_out->mkdir("proton");
  file_out->mkdir("pi_pos");
  file_out->mkdir("pi_neg");
//  *******************************************
//  Calculating v1 for sim-tru particles
//  *******************************************
//
  {
    Correlation<2> proton1_psi1( file, "/", std::array{"tru_proton_PLAIN"s, "psi_rp_PLAIN"s}, q1q1_components);
    Correlation<2> pi_pos1_psi1( file, "/", std::array{"tru_pi_pos_PLAIN"s, "psi_rp_PLAIN"s}, q1q1_components);
    Correlation<2> pi_neg1_psi1( file, "/", std::array{"tru_pi_neg_PLAIN"s, "psi_rp_PLAIN"s}, q1q1_components);

    Correlation<2> proton2_psi2( file, "/", std::array{"tru_proton_PLAIN"s, "psi_rp_PLAIN"s}, q2q2_components);
    Correlation<2> pi_pos2_psi2( file, "/", std::array{"tru_pi_pos_PLAIN"s, "psi_rp_PLAIN"s}, q2q2_components);
    Correlation<2> pi_neg2_psi2( file, "/", std::array{"tru_pi_neg_PLAIN"s, "psi_rp_PLAIN"s}, q2q2_components);

    proton1_psi1 = proton1_psi1*2.0;
    pi_pos1_psi1 = pi_pos1_psi1*2.0;
    pi_neg1_psi1 = pi_neg1_psi1*2.0;

    proton2_psi2 = proton2_psi2*2.0;
    pi_pos2_psi2 = pi_pos2_psi2*2.0;
    pi_neg2_psi2 = pi_neg2_psi2*2.0;

    file_out->cd("proton");
    proton1_psi1.Save("v1.tru_proton_PLAIN.psi_rp_PLAIN");
    proton2_psi2.Save("v2.tru_proton_PLAIN.psi_rp_PLAIN");
    file_out->cd("pi_pos");
    pi_pos1_psi1.Save("v1.tru_pi_pos_PLAIN.psi_rp_PLAIN");
    pi_pos2_psi2.Save("v2.tru_pi_pos_PLAIN.psi_rp_PLAIN");
    file_out->cd("pi_neg");
    pi_neg1_psi1.Save("v1.tru_pi_neg_PLAIN.psi_rp_PLAIN");
    pi_neg2_psi2.Save("v2.tru_pi_neg_PLAIN.psi_rp_PLAIN");
  }
//  *******************************************
//  Calculating v1 for rec particles wrt RP
//  *******************************************
//
  {
    Correlation proton1_psi1( file, "/", std::array{"proton_RESCALED"s, "psi_rp_PLAIN"s}, q1q1_components);
    Correlation pi_pos1_psi1( file, "/", std::array{"pi_pos_RESCALED"s, "psi_rp_PLAIN"s}, q1q1_components);
    Correlation pi_neg1_psi1( file, "/", std::array{"pi_neg_RESCALED"s, "psi_rp_PLAIN"s}, q1q1_components);

    Correlation proton2_psi2( file, "/", std::array{"proton_RESCALED"s, "psi_rp_PLAIN"s}, q2q2_components);
    Correlation pi_pos2_psi2( file, "/", std::array{"pi_pos_RESCALED"s, "psi_rp_PLAIN"s}, q2q2_components);
    Correlation pi_neg2_psi2( file, "/", std::array{"pi_neg_RESCALED"s, "psi_rp_PLAIN"s}, q2q2_components);

    proton1_psi1 = proton1_psi1*2.0;
    pi_pos1_psi1 = pi_pos1_psi1*2.0;
    pi_neg1_psi1 = pi_neg1_psi1*2.0;

    proton2_psi2 = proton2_psi2*2.0;
    pi_pos2_psi2 = pi_pos2_psi2*2.0;
    pi_neg2_psi2 = pi_neg2_psi2*2.0;

    file_out->cd("proton");
    proton1_psi1.Save("v1.proton_RESCALED.psi_rp_PLAIN");
    proton2_psi2.Save("v2.proton_RESCALED.psi_rp_PLAIN");
    file_out->cd("pi_pos");
    pi_pos1_psi1.Save("v1.pi_pos_RESCALED.psi_rp_PLAIN");
    pi_pos2_psi2.Save("v2.pi_pos_RESCALED.psi_rp_PLAIN");
    file_out->cd("pi_neg");
    pi_neg1_psi1.Save("v1.pi_neg_RESCALED.psi_rp_PLAIN");
    pi_neg2_psi2.Save("v2.pi_neg_RESCALED.psi_rp_PLAIN");
  }


//  *******************************************
//  Calculation of v1 with regular 3-Sub
//  *******************************************
//
  for( auto qa : ep_vectors ){
    Correlation proton_qa( file, "/", std::array{"proton_RESCALED"s, qa}, q1q1_components);
    Correlation pi_pos_qa( file, "/", std::array{"pi_pos_RESCALED"s, qa}, q1q1_components);
    Correlation pi_neg_qa( file, "/", std::array{"pi_neg_RESCALED"s, qa}, q1q1_components);

    auto res_v = Functions::VectorResolutions3S( file, "/", qa, res_vectors, q1q1_components );

    for( auto R1 : res_v ) {
      auto v1_proton = proton_qa * 2 / R1;
      auto v1_pi_pos = pi_pos_qa * 2 / R1;
      auto v1_pi_neg = pi_neg_qa * 2 / R1;

      file_out->cd("resolutions");
      R1.Save("R1."+R1.Title());

      file_out->cd("proton");
      v1_proton.Save("v1."+R1.Title());
      file_out->cd("pi_pos");
      v1_pi_pos.Save("v1."+R1.Title());
      file_out->cd("pi_neg");
      v1_pi_neg.Save("v1."+R1.Title());
    }
  }

//  *******************************************
//  Calculation of flow for F2 with 4-sub method
//  *******************************************
//
  {
    auto res_v =
        Functions::VectorResolutions4S(file, "/", "F2_RESCALED",
                                       sts_vectors, ep_vectors, q1q1_components);

    std::string qa = "F2_RESCALED";
    auto proton_qa = Correlation(file, "/", std::array{"proton_RESCALED"s, qa}, q1q1_components);
    auto pi_pos_qa = Correlation( file, "/", std::array{"pi_pos_RESCALED"s, qa}, q1q1_components);
    auto pi_neg_qa = Correlation( file, "/", std::array{"pi_neg_RESCALED"s, qa}, q1q1_components);

    for (auto R1 : res_v) {
      auto v1_proton = proton_qa * 2 / R1;
      auto v1_pi_pos = pi_pos_qa * 2 / R1;
      auto v1_pi_neg = pi_neg_qa * 2 / R1;

      file_out->cd("resolutions");
      R1.Save("R1." + R1.Title());

      file_out->cd("proton");
      v1_proton.Save("v1." + R1.Title());
      file_out->cd("pi_pos");
      v1_pi_pos.Save("v1."+R1.Title());
      file_out->cd("pi_neg");
      v1_pi_neg.Save("v1."+R1.Title());
    }
  }

//  *******************************************
//  Calculating v2 with MH method
//  *******************************************
  {
    Correlation<2> proton_qa_qb( file, "/", {"proton_RESCALED"s, "F1_RESCALED"s, "F3_RESCALED"s}, {
      "x2x1x1b_norm",
      "x2y1y1b_norm",
    });
    Correlation<2> pi_pos_qa_qb( file, "/", {"pi_pos_RESCALED"s, "F1_RESCALED"s, "F3_RESCALED"s}, {
      "x2x1x1b_norm",
      "x2y1y1b_norm",
    });
    Correlation<2> pi_neg_qa_qb( file, "/", {"pi_neg_RESCALED"s, "F1_RESCALED"s, "F3_RESCALED"s}, {
      "x2x1x1b_norm",
      "x2y1y1b_norm",
    });

    proton_qa_qb = proton_qa_qb*4;
    pi_pos_qa_qb = pi_pos_qa_qb*4;
    pi_neg_qa_qb = pi_neg_qa_qb*4;
    Correlation<2> qa_qb( file, "/", std::array{"F1_RESCALED"s, "F3_RESCALED"s}, {
      "x1x1b_norm",
      "y1y1b_norm",
    });
    auto proton_v2 = proton_qa_qb / (qa_qb * 2.0);
    auto pi_pos_v2 = pi_pos_qa_qb / (qa_qb * 2.0);
    auto pi_neg_v2 = pi_neg_qa_qb / (qa_qb * 2.0);
    file_out->cd("proton");
    proton_v2.Save("v2.F1_RESCALED.F3_RESCALED");
    file_out->cd("pi_pos");
    pi_pos_v2.Save("v2.F1_RESCALED.F3_RESCALED");
    file_out->cd("pi_neg");
    pi_neg_v2.Save("v2.F1_RESCALED.F3_RESCALED");
  }

  file_out->Close();
  file->Close();
}