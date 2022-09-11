#ifndef DMOSDEC_H
#define DMOSDEC_H

#include<QVector>
#include <QList>
#include<QString>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <tuple>
#include <string.h>
#include <sys/stat.h>



struct DomainPosInfo{
    int Pos;
    QString CList;
    QString CCList; // Complete CList (including Plus4)
    float score;
};

class DMOSDec
{

public:

    /// Flags
    std::string F_folder;
    std::string W_folder;

    std::string addressing;
    std::string Address_File;

    std::string Ref_File;

    bool gB  = false;   // Bayesian
    bool gCT = false;   // CT
    bool gM  = false;   // Mixed
    bool gP  = false;   // C Pos
    bool gD  = false;   // Domain pos

    bool rF  = false;   // Reference File


    bool wa1 = false;  // workaround

    bool allC = false; // Use all Cs of the domains

    int nCores;


    ///

    QVector<QString> Domains;

    QVector<QString> AddressList;

    QVector <QVector<float> >TempValues;

    QVector <QVector<  QVector  <int> >> MList;

    QVector <QVector<  QVector  <int> >> UList;

    QVector <float> UmbralsM;

    QVector <float> UmbralsU;

    QVector <QVector<  QVector<std::string> > > Grouped;

    QVector <QVector<int> > C_Count;

    QVector <QVector<int> > T_Count;

    QVector <QVector<int> > Reference;

    QVector <QVector<float> > CT_Factor;


    QVector < QVector <QString> > BarcodesA;
    QVector < QVector <QString> > BarcodesB;

    QList < QFile * > FileList ;

    QList < QFile * > FileAddress;

    QString BaseAddress = "0123456789ABCDEF";



    int ActiveThreads = 0;

    int MaxThreads = 6;

    bool Finished = false;

    int TPos = 1;

    QTextStream * qts;

    DMOSDec();

    void RunAnalysis();

    QVector <QString> AddBarcodeString(QString A, QString B);

    void ReadDomains();

    void PrepareTemplates();

    void ReadSeqFiles();

    void ReadFastQFile(QString filename);

    void ReadFastQFileT(QString filename);

    void ReadNextLine();

    void ReadThread();

    QString seqrcomplement(QString seq);

    void analyzeseq(QString seq);

    QString getCigarAlign(QString seq1, QString seq2, std::vector <unsigned int> cigar);

    DomainPosInfo searchSeq(QString dom, QString seq);

    //std::tuple <int, std::string> searchSeqSSW(QString dom, QString seq);

    DomainPosInfo AddressScore(QString seq, QVector<QString> barcode);

    int GetAddress(QString seq);

    DomainPosInfo searchSeqSSW(QString dom, QString seq);

    int getAddress(QVector<DomainPosInfo>Pos, QVector<int>ID);

    void analyzemutations(int Address, QVector<DomainPosInfo> positions, QString seq);


    int checkMutation(QString seq1, QString seq2 );

    float CheckMatrix(QVector<int> Cigar, int domain, bool Mutated );

    float checkMutation(int domain, DomainPosInfo Pos);

    void printTemplate(int Address);

    void writeDomainCT(int domain, int Address, QString seq);

    void CloseFiles();

    void exportFile();

    void exportFileCT();

    void exportFileMixed();

    void ExportCpos(int Address, QString seq, int domain);

    float GetCT(int i, int j);

    void readWordList();

};

#endif // DMOSDEC_H
