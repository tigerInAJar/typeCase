#include <TTree.h>
#include <TFile.h>
#include "event.h"
#include "setup.h"
#include "algorithm.h"

class AWriteDataToRootFile:public AAlgorithm
{
Q_OBJECT
 private:
    TFile **rootFile;//!
    TTree *tree;//!
    TDirectory *currentDir;//!
  TSetup &setup;
  TEvent *event;
  TObjArray **hits;//!
  TCalibHit ***saveHits;//!
  TObjArray **pixels;//!
  TPixel ***savePixel;//!
  TObjArray **clusters;//!
  TCluster ***saveCluster;//!
  TObjArray *tracks;//!
  TTrack **saveTracks;//!
  TObjArray *reactions;//!
  int *numTr;
  int **numHits;
  int **numPix;
  int **numClus;
  int *eventNumber;
  int *runNumber;
  const int maxDet;
  const int maxTrack;
  const int maxCluster;
  const int maxPixel;
  const int maxHit;
  bool server;
 public:
  AWriteDataToRootFile(const string &filename, const string &directory, const string &treeName, TEvent &eventIn,TSetup &setupIn,int maxTracks,bool serv);
  AWriteDataToRootFile(TFile **rootFileIn, const string &directory, const string &treeName, TEvent &eventIn,TSetup &setup,int maxTracks,bool serv);
  virtual ~AWriteDataToRootFile();
  virtual void *process(void*ptr);
  TFile *getFile();
 signals:
  void destroyer(TTree *);
 public slots:
  virtual void getDestroyTree(TTree* tr);
};
