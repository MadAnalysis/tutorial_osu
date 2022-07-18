#ifndef ANALYSIS_ANALYSIS_H
#define ANALYSIS_ANALYSIS_H

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
    class analysis : public AnalyzerBase
    {
    INIT_ANALYSIS(analysis, "analysis")

    private:
        Substructure::HTT tagger; /// HTT interface
        Substructure::SoftDrop softDrop; /// SoftDrop interface

    public:

        // -----------------------------------------------------------------------------
        // Initialize
        // function called one time at the beginning of the analysis
        // -----------------------------------------------------------------------------
        bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
        {
            INFO << "    <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endmsg;
            INFO << "    <>               Tutorial for Oklahoma State University           <>" << endmsg;
            INFO << "    <>    TTbar analysis with resolved and boosted signal regions     <>" << endmsg;
            INFO << "    <>    Author : J.Y. Araz                                          <>" << endmsg;
            INFO << "    <>    Contact: jack.araz@durham.ac.uk                             <>" << endmsg;
            INFO << "    <>    Based on MadAnalysis 5 v2.0.4 and above                     <>" << endmsg;
            INFO << "    <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endmsg;

            // ========================= //
            // ===== Signal region ===== //
            // ========================= //

            Manager()->AddRegionSelection("Resolved");
            Manager()->AddRegionSelection("Boosted");

            std::string allSRs[2] = {"Resolved", "Boosted"};

            // ====================== //
            // ===== Selections ===== //
            // ====================== //

            /// Common selections
            Manager()->AddCut("$N_l=1$", allSRs);
            Manager()->AddCut("$p_T(l)>50$ [GeV]", allSRs);
            Manager()->AddCut("$N_b\\geq 2$", allSRs);
            Manager()->AddCut("$N_j\\geq 2$", allSRs);
            Manager()->AddCut("$\\slashed{E}_T>30$ [GeV]", allSRs);

            /// Resolved selection
            Manager()->AddCut("$m_{t^{rec}_L}<200$ [GeV]", "Resolved");

            /// Boosted selection
            Manager()->AddCut("At least one fatjet", "Boosted");
            Manager()->AddCut("is leading fatjet tagged", "Boosted");

            // ====================== //
            // ===== Histograms ===== //
            // ====================== //

            Manager()->AddHisto("$N_{l}$",  5,0.,5., allSRs);
            Manager()->AddHisto("$N_{b}$",  5,0.,5., allSRs);
            Manager()->AddHisto("$N_{\\rm light\\ jet}$",  10, 0., 10., allSRs);
            Manager()->AddHisto("$p_T(l_1)\\ [{\\rm GeV}]$",  50, 0., 500., allSRs);
            Manager()->AddHisto("$E^{\\rm miss}_T\\ [{\\rm GeV}]$",  25, 0., 500., allSRs);

            Manager()->AddHisto("$m_{t^{rec}_L}\\ [{\\rm GeV}]$",  20, 0.0, 400.0, "Resolved");

            Manager()->AddHisto("$N_{\\rm fat\\ jet}$",  5,0.,5., "Boosted");
            Manager()->AddHisto("$m_{t^{rec}_H}\\ [{\\rm GeV}]$",  20, 0.0, 400.0, "Boosted");
            Manager()->AddHisto("$m_{t^{SD}_H}\\ [{\\rm GeV}]$",  20, 0.0, 400.0, "Boosted");
            Manager()->AddHisto("$m_{t\\bar{t}}\\ [{\\rm GeV}]$",  25, 500., 3000., "Boosted");

            /// Initialize substructure tools
            MAfloat32 z_cut = 0.10;
            MAfloat32 beta  = 2.0;
            softDrop.Initialize(beta, z_cut);

            Substructure::HTT::InputParameters htt_param;
            htt_param.mode = Substructure::HTT::EARLY_MASSRATIO_SORT_MASS;
            htt_param.do_optimalR = true;
            tagger.Initialize(htt_param);

            return true;
        }


        // -----------------------------------------------------------------------------
        // Finalize
        // function called one time at the end of the analysis
        // -----------------------------------------------------------------------------
        void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files) {}

        // -----------------------------------------------------------------------------
        // Execute
        // function called each time one event is read
        // -----------------------------------------------------------------------------
        bool Execute(SampleFormat& sample, const EventFormat& event)
        {
            // Initialize event weight
            MAdouble64 EvWeight;
            if(Configuration().IsNoEventWeight()) EvWeight=1.;
            else if(event.mc()->weight()!=0.) EvWeight=event.mc()->weight();
            else { return false;}
            Manager()->InitializeForNewEvent(EvWeight);
            // Empty event
            if (event.rec()==0) {return true;}

            // ======================== //
            // ===== PreSelection ===== //
            // ======================== //

            /// Collect electrons with pT > 10 GeV and |eta| < 2.5
            RecLeptons Electrons = filter(event.rec()->electrons(), 10., 2.5);
            RecLeptons Muons = filter(event.rec()->muons(), 10., 2.5);
            RecLeptons SignalLeptons = Electrons + Muons;

            /// Collect antikt jets with pT > 30 GeV
            RecJets AK4 = filter(event.rec()->jets(), 30.);

            /// Remove jets which are overlaping with leptons if dR < 0.1
            AK4 = OverlapRemoval(AK4, SignalLeptons, 0.1);

            /// Remove leptons which are overlaping with jets if dR < 0.4
            SignalLeptons = OverlapRemoval(SignalLeptons, AK4, 0.4);

            /// Collect B-jets
            RecJets Bjets = filter_select(AK4,
                                          [](const RecJet jet){return jet->btag() && jet->pt() > 30.;});

            /// collect light jets
            RecJets LightJets = filter_select(AK4, [](const RecJet jet){return !jet->btag();});

            /// Sort collections
            SORTER->sort(LightJets, PTordering);
            SORTER->sort(Bjets, PTordering);
            SORTER->sort(SignalLeptons, PTordering);

            /// Missing transverse momenta
            MALorentzVector pTmiss = event.rec()->MET().momentum();

            // =================== //
            // ===== Cutflow ===== //
            // =================== //

            Manager()->FillHisto("$N_{l}$", SignalLeptons.size());
            /// Accept only one lepton
            if (!Manager()->ApplyCut(SignalLeptons.size() == 1, "$N_l=1$")) return true;

            Manager()->FillHisto("$p_T(l_1)\\ [{\\rm GeV}]$", SignalLeptons[0]->pt());
            /// Require minimum lepton pT to be 50 GeV
            if (!Manager()->ApplyCut(SignalLeptons[0]->pt() > 50., "$p_T(l)>50$ [GeV]")) return true;

            Manager()->FillHisto("$N_{b}$", Bjets.size());
            /// Accept at least 2 bjets
            if (!Manager()->ApplyCut(Bjets.size() >= 2, "$N_b\\geq 2$")) return true;

            Manager()->FillHisto("$N_{\\rm light\\ jet}$", LightJets.size());
            /// Accept at least 2 light jets
            if (!Manager()->ApplyCut(LightJets.size() >= 2, "$N_j\\geq 2$")) return true;

            Manager()->FillHisto("$E^{\\rm miss}_T\\ [{\\rm GeV}]$", pTmiss.Pt());
            /// Set MET requirement to > 30 GeV
            if (!Manager()->ApplyCut(pTmiss.Pt() > 30., "$\\slashed{E}_T>30$ [GeV]")) return true;

            /// Reconstruct leptonic top
            MAdouble64 dR_bl = 1e99;
            MALorentzVector LeptonicTop;
            for (auto &bjet: Bjets)
            {
                if (SignalLeptons[0]->dr(bjet) < dR_bl)
                {
                    dR_bl = SignalLeptons[0]->dr(bjet);
                    LeptonicTop = bjet->momentum();
                }
            }
            LeptonicTop += SignalLeptons[0]->momentum() + pTmiss;

            Manager()->FillHisto("$m_{t^{rec}_L}\\ [{\\rm GeV}]$", LeptonicTop.M());
            /// Require leptonic top mass to be less than 200 GeV
            if (!Manager()->ApplyCut(LeptonicTop.M() < 200., "$m_{t^{rec}_L}<200$ [GeV]")) return true;

            /// Collect CA15 jets with pT > 200 GeV and |eta| < 2.5
            RecJets CA15 = filter(event.rec()->jets("CA15"), 200., 2.5);
            SORTER->sort(CA15, PTordering);

            Manager()->FillHisto("$N_{\\rm fat\\ jet}$", CA15.size());
            /// Require at least one fatjet
            if (!Manager()->ApplyCut(CA15.size() >= 1, "At least one fatjet")) return true;

            /// Execute HTT with leading jet
            MAbool isTagged = false;
            const RecJet SDjet;
            if (CA15.size() > 0)
            {
                tagger.Execute(CA15[0]);
                SDjet = softDrop.Execute(CA15[0]);
                isTagged = tagger.is_tagged();
            }
            if (!Manager()->ApplyCut(isTagged, "is leading fatjet tagged")) return true;

            const RecJet top = isTagged ? tagger.top() : new RecJetFormat();
            Manager()->FillHisto("$m_{t^{rec}_H}\\ [{\\rm GeV}]$", top->m());
            if (CA15.size() > 0) Manager()->FillHisto("$m_{t^{SD}_H}\\ [{\\rm GeV}]$", SDjet->m());
            Manager()->FillHisto("$m_{t\\bar{t}}\\ [{\\rm GeV}]$", (top->momentum() + LeptonicTop).M());

            return true;
        }
    };
}

#endif // ANALYSIS_ANALYSIS_H