#!/bin/bash

set -x

# git clone https://github.com/cms-sw/RecoLuminosity-LumiDB.git $CMSSW_BASE/src/RecoLuminosity/LumiDB
# cd $CMSSW_BASE/src/RecoLuminosity/LumiDB
# git checkout V04-02-10
# scram b
# cmsenv

pixelLumiCalc.py lumibyls -i 202299.json --hltpath "HLT_ZeroBias_v7" -o lumi_202299.csv

pileupReCalc_HLTpaths.py -i lumi_202299.csv --inputLumiJSON pileup_latest.txt -o PU_202299.json

#pileupCalc.py -i 202299.json --inputLumiJSON pileup_latest.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 200 --numPileupBins 200  202299_truePileUpHisto.root
pileupCalc.py -i 202299.json --inputLumiJSON PU_202299.json --calcMode true --minBiasXsec 69400 --maxPileupBin 200 --numPileupBins 200  202299_truePileUpHisto.root

pixelLumiCalc.py lumibyls -i 200473.json --hltpath "HLT_ZeroBias_v7" -o lumi_200473.csv

pileupReCalc_HLTpaths.py -i lumi_200473.csv --inputLumiJSON pileup_latest.txt -o PU_200473.json

#pileupCalc.py -i 200473.json --inputLumiJSON pileup_latest.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 200 --numPileupBins 200  200473_truePileUpHisto.root
pileupCalc.py -i 200473.json --inputLumiJSON PU_200473.json --calcMode true --minBiasXsec 69400 --maxPileupBin 200 --numPileupBins 200  200473_truePileUpHisto.root

