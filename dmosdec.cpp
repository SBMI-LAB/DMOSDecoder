#include "dmosdec.h"
#include <QDebug>
#include <QFile>
#include <QDir>

#include <iostream>
#include <thread>
#include <math.h>

#include "ssw_cpp.h"

using std::string;
using std::cout;
using std::endl;
using std::tuple;

DMOSDec::DMOSDec()
{

}

void DMOSDec::RunAnalysis()
{
    ReadDomains();
    PrepareTemplates();
    readWordList();
    //exportFile();
    //exportFileMixed();
    ReadSeqFiles();
    //exportFile();
    //exportFileMixed();
    CloseFiles();
}

QVector <QString> DMOSDec::AddBarcodeString(QString A, QString B)
{
    QVector <QString> Output;
    Output.append(A);
    Output.append(B);
    return Output;
}

void DMOSDec::ReadDomains(){




    QFile inputFile("Templates.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          //qDebug() << line;
          Domains.append(line);
       }
       inputFile.close();
    }


    //QFile inputFile2("Addresses.txt");
    QFile inputFile2( QString::fromUtf8(Address_File.c_str()));
    if (inputFile2.open(QIODevice::ReadOnly))
    {
       QTextStream in2(&inputFile2);
       while (!in2.atEnd())
       {
          QString line = in2.readLine();

          //qDebug() << line;

          AddressList.append(line);
       }
       inputFile2.close();
    }


    QFile inputFile3("Umbrals.csv");
    if (inputFile3.open(QIODevice::ReadOnly))
    {
       QTextStream in3(&inputFile3);
       while (!in3.atEnd())
       {
          QString line = in3.readLine();

          QStringList elems = line.split(QLatin1Char(','));

          float uu = elems[0].toFloat();
          float mu = elems[1].toFloat();

          UmbralsM.append(mu);
          UmbralsU.append(uu);


       }
       inputFile3.close();
    }


    /// Prepare barcodes
    QString dBC1F = "TTTCTGTTGGTGCTGATATTGCGTTGTCGGTGTCTTTGTGATCACGAGGCCCTTTCG";
    QString dBC2F = "TTTCTGTTGGTGCTGATATTGCCCGTTTGTAGTCGTCTGTATCACGAGGCCCTTTCG";
    QString dBC3F = "TTTCTGTTGGTGCTGATATTGCTGTGTCCCAGTTACCAGGATCACGAGGCCCTTTCG";
    QString dBC4F = "TTTCTGTTGGTGCTGATATTGCTTCTATCGTGTTTCCCTAATCACGAGGCCCTTTCG";
    QString dBC5F = "TTTCTGTTGGTGCTGATATTGCCAGGGTTTGTGTAACCTTATCACGAGGCCCTTTCG";
    QString dBC6F = "TTTCTGTTGGTGCTGATATTGCGAACAAACCAAGTTACGTATCACGAGGCCCTTTCG";
    QString dBC1R = seqrcomplement("TTGCCTGTCGCTCTATCTTCCCGTGGGAATGAATCCTTTGATAAACTACCGCATTAAAGC");
    QString dBC2R = seqrcomplement("TTGCCTGTCGCTCTATCTTCCAAAGGCAGAAAGTAGTCTGATAAACTACCGCATTAAAGC");
    QString dBC3R = seqrcomplement("TTGCCTGTCGCTCTATCTTCGCACAGCGAGTCTTGGTTTGATAAACTACCGCATTAAAGC");
    QString dBC4R = seqrcomplement("TTGCCTGTCGCTCTATCTTCTGAAACCTTTGTCCTCTCTGATAAACTACCGCATTAAAGC");
    QString dBC5R = seqrcomplement("TTGCCTGTCGCTCTATCTTCTCTATCGGAGGGAATGGATGATAAACTACCGCATTAAAGC");
    QString dBC6R = seqrcomplement("TTGCCTGTCGCTCTATCTTCGAAAGAAGCAGAATCGGATGATAAACTACCGCATTAAAGC");



    if (wa1 == false){
        BarcodesA.append( AddBarcodeString(dBC1F, dBC1R));
        BarcodesB.append( AddBarcodeString(dBC1F, dBC2R));
        BarcodesA.append( AddBarcodeString(dBC1F, dBC3R));
    }
    BarcodesB.append( AddBarcodeString(dBC1F, dBC4R));
    BarcodesA.append( AddBarcodeString(dBC1F, dBC5R));
    BarcodesB.append( AddBarcodeString(dBC1F, dBC6R));
    BarcodesA.append( AddBarcodeString(dBC2F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC2F, dBC2R));
    BarcodesA.append( AddBarcodeString(dBC2F, dBC3R));
    BarcodesB.append( AddBarcodeString(dBC2F, dBC4R));
    BarcodesA.append( AddBarcodeString(dBC2F, dBC5R));
    BarcodesB.append( AddBarcodeString(dBC2F, dBC6R));
    BarcodesA.append( AddBarcodeString(dBC3F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC3F, dBC2R));
    BarcodesA.append( AddBarcodeString(dBC3F, dBC3R));
    BarcodesB.append( AddBarcodeString(dBC3F, dBC4R));
    BarcodesA.append( AddBarcodeString(dBC3F, dBC5R));
    BarcodesB.append( AddBarcodeString(dBC3F, dBC6R));
    BarcodesA.append( AddBarcodeString(dBC4F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC4F, dBC2R));
    BarcodesA.append( AddBarcodeString(dBC4F, dBC3R));
    BarcodesB.append( AddBarcodeString(dBC4F, dBC4R));
    BarcodesA.append( AddBarcodeString(dBC4F, dBC5R));
    BarcodesB.append( AddBarcodeString(dBC4F, dBC6R));
    BarcodesA.append( AddBarcodeString(dBC5F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC5F, dBC2R));
    BarcodesA.append( AddBarcodeString(dBC5F, dBC3R));
    BarcodesB.append( AddBarcodeString(dBC5F, dBC4R));
    BarcodesA.append( AddBarcodeString(dBC5F, dBC5R));
    BarcodesB.append( AddBarcodeString(dBC5F, dBC6R));
    BarcodesA.append( AddBarcodeString(dBC6F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC6F, dBC2R));

    ///
    /// C1 = dBC6F/dBC3R
    /// C2 = dBC6F/dBC4R

    /// Adding the lost barcodes
    if (wa1 == true)
    {
    BarcodesA.append( AddBarcodeString(dBC1F, dBC1R));
    BarcodesB.append( AddBarcodeString(dBC1F, dBC2R));
    BarcodesA.append( AddBarcodeString(dBC1F, dBC3R));
    }





    /// Prepare the list of files:

    if (gD == true)  /// Domains
    {

        string stringpath = W_folder + "CT/";
        mkdir(stringpath.c_str(),0777);

        for (int k =1; k <= 16; k++)
        {
            QString filename;

            filename.append(stringpath.c_str());
            filename.append(QString::number(k));
            filename.append("_M.txt");

            QFile *file = new QFile(filename);
            file->open(QIODevice::WriteOnly | QIODevice::Text);
            FileList.append(file);

        }

        for (int k =1; k <= 16; k++)
        {
            QString filename;

            filename.append(stringpath.c_str());
            filename.append(QString::number(k));
            filename.append("_U.txt");

            QFile *file = new QFile(filename);
            file->open(QIODevice::WriteOnly | QIODevice::Text);
            FileList.append(file);
        }
    }

    if (gP == true)  // CPos per address
    {
        string stringpath = W_folder + "CTA/";
        mkdir(stringpath.c_str(),0777);

        for (int k =1; k <= 16; k++)
        {
            QString filename;

            filename.append(stringpath.c_str());
            filename.append(QString::number(k));
            filename.append("_A.txt");

            QFile *file = new QFile(filename);
            file->open(QIODevice::WriteOnly | QIODevice::Text);
            FileAddress.append(file);
        }
    }




    /// Read Reference file
    if (rF == true)
    {
        QFile inputFileRef( Ref_File.c_str()   );
        if (inputFileRef.open(QIODevice::ReadOnly))
        {
           QTextStream in3(&inputFileRef);
           while (!in3.atEnd())
           {
              QString line = in3.readLine();

              QStringList elems = line.split(QLatin1Char(','));

              QVector <int> tRow;

              for (int i=0; i < 16; i++)
              {

                int uu = elems[i].toInt();
                tRow.append(uu);

              }

              Reference.append(tRow);


           }
           inputFileRef.close();
        }

    }





}

void DMOSDec::PrepareTemplates(){

    for (int i= 0; i < 48; i++)
    {
        QVector<float> Tm;
        for (int j=0; j<16; j++)
            Tm.append((float)0);

        TempValues.append(Tm);

    }


    /// C-T Counters

    for (int i = 0; i < 48; i++ )
    {
        QVector <int> C_Row;
        QVector <int> T_Row;
        QVector <float> CT_Row;

        for (int j = 0; j < 16; j++)
        {
            C_Row.append(0);
            T_Row.append(0);
            CT_Row.append((float)0);
        }

        C_Count.append(C_Row);
        T_Count.append(T_Row);
        CT_Factor.append(CT_Row);
    }

}

void DMOSDec::readWordList()
{
    QString Location = "MachineLearning/wordlist/";

    for (int k = 1; k <= 16; k++ )
    {

        QString filename;

        filename.append(Location);
        filename.append(QString::number(k));
        filename.append("_M.txt");

        //filename = Location.c_str() + std::to_string(k) + "_M.txt";

        //sprintf(filename, "MachineLearning/wordlist/%d_M.txt", k);

        QFile inputFile( filename );
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);

           int npos = 0;

           QVector< QVector<int> > cM;



           while (!in.atEnd())
           {
              QVector<int> nLine;

              QString line = in.readLine();

              QStringList elems = line.split(QLatin1Char('\t'));

              for (int i = 0; i < elems.size(); i++)
              {
                  int ne = elems[i].toInt();
                  nLine.append(ne);
              }

              cM.append(nLine);


           }
           inputFile.close();

           MList.append(cM);


        }





    }



    for (int k = 1; k <= 16; k++ )
    {

        QString filename;

        filename.append(Location);
        filename.append(QString::number(k));
        filename.append("_U.txt");

        QFile inputFile( filename );
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);

           int npos = 0;

           QVector< QVector<int> > cU;



           while (!in.atEnd())
           {
              QVector<int> nLine;

              QString line = in.readLine();

              QStringList elems = line.split(QLatin1Char('\t'));

              for (int i = 0; i < elems.size(); i++)
              {
                  int ne = elems[i].toInt();
                  nLine.append(ne);
              }

              cU.append(nLine);


           }
           inputFile.close();

           UList.append(cU);


        }





    }



}



void DMOSDec::ReadSeqFiles() {


    QString Location = QString::fromUtf8( F_folder.c_str());

    QDir directory (Location);

    QStringList Sequences = directory.entryList(QStringList() << "*.fastq",QDir::Files);
    foreach(QString filename, Sequences) {

        ReadFastQFile(Location + filename);

        //break;

        //exportFile();
    }

}

void DMOSDec::ReadFastQFile(QString filename){


    qDebug() <<"Reading file "<<filename << "\n";

    QFile inputFile(filename);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);

       int pos = 1;

       while (!in.atEnd())
       {
          in.readLine();  //First id
          QString line = in.readLine();  //sequence


          QString rline = seqrcomplement(line);

          analyzeseq(line);
          analyzeseq(rline);

          in.readLine();  // +
          in.readLine();  // last value


          pos ++;

          if (pos == 100)
          {
              pos = 1;
              printTemplate(16);
              exportFile();
              exportFileCT();
              exportFileMixed();
           //   exportFileCT();
          }




       }
       inputFile.close();
    }

}


void DMOSDec::ReadFastQFileT(QString filename){


    qDebug() <<"Reading file "<<filename << "\n";

    QFile inputFile(filename);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);

       qts = &in;

       int pos = 1;

       while (!in.atEnd())
       {
          in.readLine();  //First id
          QString line = qts->readLine();  //sequence


          QString rline = seqrcomplement(line);

          analyzeseq(line);
          analyzeseq(rline);

          qts->readLine();  // +
          qts->readLine();  // last value


          pos ++;

          if (pos == 100)
          {
              pos = 1;
              printTemplate(16);
              exportFile();
          }




       }
       inputFile.close();
    }

}

void DMOSDec::ReadNextLine()
{
    // Takes the thread to read next
    for (int k = ActiveThreads; k < MaxThreads; k++)
        std::thread nextThread (&DMOSDec::ReadThread, this);
}

void DMOSDec::ReadThread()
{
    // Takes the thread to read next
    ActiveThreads ++;


    ActiveThreads --;

    if (!Finished)
        ReadNextLine();
}


QString DMOSDec::seqrcomplement(QString seq){
    QString rseq = "";

    for (int i = 0; i < seq.size(); i++) {

        if (seq.at(i) == QChar('A'))
            rseq.prepend("T");
        if (seq.at(i) == QChar('T'))
            rseq.prepend("A");
        if (seq.at(i) == QChar('C'))
            rseq.prepend("G");
        if (seq.at(i) == QChar('G'))
            rseq.prepend("C");

    }



    return rseq;

}

DomainPosInfo DMOSDec::AddressScore(QString seq, QVector<QString> barcode){
    float score = 0;
    DomainPosInfo Pos1 = searchSeqSSW(barcode[0], seq.mid(0,100));
    int n = seq.size();
    DomainPosInfo Pos2 = searchSeqSSW(barcode[1], seq.mid(n-101, 100));
    score = Pos1.score * Pos2.score;
    Pos1.score = score;

    return Pos1;
}

int DMOSDec::GetAddress(QString seq)
{
    int Address = -1;
    int state = 0;
    int pos = 0;
    float score = 0;

    float AScore = 0;
    int APos = 0;
    float BScore = 0;
    int BPos = 0;

    int A_Add = -1;
    int B_Add = -1;

    for (int i=0; i < 16; i++)
    {
        DomainPosInfo Pos = AddressScore(seq, BarcodesA[i]);
        if (Pos.score > AScore)
        {
            AScore = Pos.score;
            APos = Pos.Pos;
            A_Add = i;
        }
    }

    for (int i=0; i < 16; i++)
    {
        DomainPosInfo Pos = AddressScore(seq, BarcodesB[i]);
        if (Pos.score > BScore)
        {
            BScore = Pos.score;
            BPos = Pos.Pos;
            B_Add = i;
        }
    }

    if (AScore > BScore)
    {
        Address = A_Add;

        state = 1;
        pos = APos;
        score = AScore;
    }
    else {
        Address = B_Add;

        state = 0;
        pos = BPos;
        score = BScore;

    }


    if (score > 0.8)
        return Address;
    else
        return -1;

}

void DMOSDec::analyzeseq(QString seq){

    int minlength = 1100;

    QVector <DomainPosInfo> Positions;
    QVector <int> Identifiers;

    int found = 0;
    int n = seq.size();

    int prePos = 100;

    //qDebug() << "Analyzing file...\n";

    if (n > minlength)
    {

        if (addressing == "barcode")
        {


            int Address = GetAddress(seq);
            if (Address != -1)
            {

                for (int i = 0; i< 16; i++)
                {

                    DomainPosInfo pos = searchSeqSSW(Domains[i], seq);

                    if (pos.Pos > prePos)
                    {
                        prePos = pos.Pos;

                    }
                    else {

                        //qDebug() << "Failed at "<< i << "\n";
                        return;
                    }



                    if (pos.Pos != -1)
                    {
                        Positions.append(pos);
                        Identifiers.append(i);
                        found ++;
                    }

                }
                //qDebug() << "Address " << Address << "\n";
                analyzemutations(Address, Positions, seq);
            }

        } /// End barcode addressing


        if (addressing == "shuffle")
        {
            for (int i = 0; i< 16; i++)
            {

                DomainPosInfo pos = searchSeq(Domains[i], seq);

                if (pos.Pos != -1)
                {
                    Positions.append(pos);
                    Identifiers.append(i);
                    found ++;
                }

            }

            if (found == 16)
            {

                /// Compare the distances


                //qDebug() << "Found 16 elements... \n";

                /// We need the address here

                int Address = getAddress(Positions, Identifiers);
                if (Address != -1)
                    analyzemutations(Address, Positions, seq);
            }
        }



    }

}



DomainPosInfo DMOSDec::searchSeqSSW(QString dom, QString seq){

    int pos = -1;

    DomainPosInfo Res;

    QString clist;


    string ref   = seq.toUtf8().constData(); //"CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA";
    string query = dom.toUtf8().constData(); //"CTGAGCCGGTAAATC";
    int32_t maskLen = strlen(query.c_str())/2;
    maskLen = maskLen < 15 ? 15 : maskLen;

    // Declares a default Aligner
    StripedSmithWaterman::Aligner aligner;
    // Declares a default filter
    StripedSmithWaterman::Filter filter;
    // Declares an alignment that stores the result
    StripedSmithWaterman::Alignment alignment;
    // Aligns the query to the ref
    aligner.Align(query.c_str(), ref.c_str(), ref.size(), filter, &alignment, maskLen);

    int len = alignment.ref_end - alignment.ref_begin ;
    float score = 0;

    if (len > 27)
    {
        string nref = ref.substr(alignment.ref_begin, len);
        string nquery = query.substr(alignment.query_begin, len);


        score = 1-((float)alignment.mismatches/len);

        //if (score > 0.9)
        //{
            pos = alignment.ref_begin;

            clist = getCigarAlign(QString(nref.c_str()), QString(nquery.c_str()), alignment.cigar);

        //}

        //if (score < 0.85)
        //    pos = -1;

    }

    Res.Pos = pos;
    Res.CList = clist;
    Res.score = score;

    //return {pos, clist };
    return Res;

}

QString DMOSDec::getCigarAlign(QString seq1, QString seq2, std::vector <unsigned int> cigar)
{

 // Format Cigar:

    QString al1 = "";
    QString al2 = "";
    int pos1 = 0;
    int pos2 = 0;

    for (int i=0; i < cigar.size(); i++)
    {
        int OP = cigar[i] & 0x00F;
        int N = cigar[i] & 0xFF0;
        int N2 = N/16;


        if (OP == 7 || OP == 8)
        {
            // equals
            al1.append( seq1.mid(pos1, N2) );
            al2.append( seq2.mid(pos2, N2) );

            pos1 += N2;
            pos2 += N2;
        }

        if (OP == 1)
        {
            // Insertion
            for (int i=0; i< N2; i++)
                al1.append("-");
            //al2.append(seq2.mid(pos2,N2));
            //pos2+=N2;

        }

        if (OP == 4)
        {
            // Start
        }

        if (OP == 2)
        {
            // Deletion
            for (int i=0; i< N2; i++)
                al2.append("-");
            //al1.append(seq1.mid(pos1,N2));
            //pos1++;
        }

    }

    /// Analyze the Cs from the alignment

    QString Clist;

    if (allC == false)
    {
        for (int i=4; i < 29;  i++)
        {
            if (al2[i] == "C")
                Clist.append(al1[i]);

        }
    }
    else {
        for (int i=0; i < al2.size();  i++)
        {
            if (al2[i] == "C")
                if (al1[i] == "A" || al1[i] == "C" || al1[i] == "T" || al1[i] == "G" || al1[i] == "-" )
                    Clist.append(al1[i]);
                else
                    Clist.append("-");

        }
    }



    return Clist;


}


DomainPosInfo DMOSDec::searchSeq(QString dom, QString seq){




    DomainPosInfo pos;

    bool Splitsample = false;


    if (Splitsample)
    {

    int mx = 90;
    int mdx = 30;

    float maxScore = 0;


    for (int x = 0; x < seq.size(); x+= mdx )
    {

        DomainPosInfo posn = searchSeqSSW(dom, seq.mid(x,mx)  );
        if (posn.score > maxScore)
        {
            pos = posn;
            pos.Pos += x;
            maxScore = posn.score;
        }
    }
    }
    else {
        pos = searchSeqSSW(dom, seq);
    }



    return pos;
}

int DMOSDec::getAddress(QVector<DomainPosInfo>Pos, QVector<int>ID)
{
    /// First sort
    int Address = -1;

    for (int i=0; i < 16; i++)
        for (int j=i; j<16; j++)
        {
            if ( Pos[j].Pos < Pos[i].Pos  ){

                DomainPosInfo idT = Pos[j];
                Pos[j] = Pos[i];
                Pos[i] = idT;

                int idTn = ID[j];
                ID[j] = ID[i];
                ID[i] = idTn;

            }
        }

    /// ordered

    QString stAddress = "";

    bool Distance = true;

    for (int i = 0; i<16; i++)
    {
        stAddress.append( BaseAddress.at( ID[i] ) );  // Building the address

        //if (i < 15)
        //{
        //    if ( Pos[i+1].Pos - Pos[i].Pos < 20  )
         //       Distance = false;
        //}
    }

    for (int i = 0; i < 15; i++)
    {
        if ( Pos[i+1].Pos - Pos[i].Pos  < 15)
            Distance = false;

    }


    int p = AddressList.indexOf(stAddress);
    Address = p;

    if (Distance == false)
    {
        Address = -1;

    }



    return Address;
}


void DMOSDec::analyzemutations(int Address, QVector<DomainPosInfo> positions, QString seq)
{

   // qDebug() <<"Analyzing Address " << Address << "\n";

    for (int k=0; k < 16; k++)
    {

        QString Temp = Domains[k].mid( 4, 59);
        QString sTemp = seq.mid( positions[k].Pos+4, 59);


        ExportCpos(Address, positions[k].CList, k);

        float R = checkMutation(k, positions[k]);

        if ( !isnan(R) )
            TempValues[Address][k] += R;


        if (TempValues[Address][k] > 30)
            TempValues[Address][k] = 30;

        if (TempValues[Address][k] < -10)
            TempValues[Address][k] = -10;


        /// Generate the output files
        writeDomainCT(k, Address, positions[k].CList);

        /// Counting C and Ts

        for (int i=0; i < positions[k].CList.size(); i++)
        {
              //if (positions[k].CList[i] == "C")
              //{
                  C_Count[Address][k] ++;
              //}

              if (positions[k].CList[i] == "T")  // T
              {
                  T_Count[Address][k] ++;
              }
        }


//        if (Address == 16 && k == 15)
//        {
//            qDebug() << "R:"  << R << "\n";

//            if (R == NAN)
//                qDebug() << "R is NAN \n";
//        }



        //int R = checkMutation(Temp, sTemp);

        //if (R > 0)
        //    TempValues[Address][k] += R;


        //if (R== 0)
        //    TempValues[Address][k] -= 0.01;

    }



}

float DMOSDec::checkMutation(int domain, DomainPosInfo Pos)
{
    float Result = 0;

    QVector <int> Cigar;

    for (int i=0; i< Pos.CList.size(); i++)
    {

        if (Pos.CList[i] == "A")
                 Cigar.append(1);
        if (Pos.CList[i] == "C")
                 Cigar.append(2);
        if (Pos.CList[i] == "G")
                 Cigar.append(3);
        if (Pos.CList[i] == "T")
                 Cigar.append(4);
        if (Pos.CList[i] == "-")
                 Cigar.append(16);

    }

    // vector created
    float mFrac = CheckMatrix(Cigar, domain, true);
    float uFrac = CheckMatrix(Cigar, domain, false);

//    float mFrac = 0;
//    float uFrac = 0;



    for (int i = 0; i < Cigar.size(); i++)
    {
       //if(Cigar[i] == 4)
       //    mFrac = mFrac*1.4; break;

       if(Cigar[i] == 4)
           mFrac = mFrac*1.1;
    }



    float mLik = mFrac/(mFrac+uFrac);
    float uLik = uFrac/(mFrac+uFrac);

    if (mLik == 0)
        mLik = 1;
    if (uLik == 0)
        uLik = 1;

    Result = -log(uLik) + log(mLik);



    return Result;
}


float DMOSDec::CheckMatrix(QVector<int> Cigar, int domain, bool Mutated )
{

    float mCount = 0;
    int Match = -1;
    float Frac = 0;


    QVector <QVector<  QVector  <int> >> Matrix;

    if (Mutated)
        Matrix = MList;
    else
        Matrix = UList;



    for (int i=0; i < Matrix[domain].size(); i++ )
    {
        int dMatch = 0;
        for (int j=1; j < Matrix[domain][i].size(); j++)
        {

            if (Cigar.size() >= j)
            {

                if (Cigar[j-1] == Matrix[domain][i][j])
                    dMatch ++;


            }

            //mCount += Matrix[domain][i][j];


        }

        if (dMatch == Cigar.size())
        {
            Match = Matrix[domain][i][0];
            break;
        }

        mCount += Matrix[domain][i][0];
    }



    if (Match == -1)
        Frac = 1/mCount;
    else
        Frac = Match/mCount;

    return Frac;


}




int DMOSDec::checkMutation(QString seq1, QString seq2 )
{

    int C1, C2;
    int T1, T2;
    int M = 0;

    C1 = 0;
    C2 = 0;
    T1 = 0;
    T2 = 0;

    float R1, R2 = 0;

    if (seq1.size() == seq2.size())
    {
        for (int k=0; k< seq1.size(); k++)
        {
            if (seq1[k] == 'T')
                T1++;
            if (seq1[k] == 'C')
                C1++;
            if (seq2[k] == 'T')
                T2++;
            if (seq2[k] == 'C')
                C2++;

        }
    }

    R1 = (float) T1/(T1+C1);
    R2 = (float) T2/(T2+C2);

    if (R2 - R1 > 0.1)
        M = 1;


    return M;
}


void DMOSDec::printTemplate(int Address)
{

    printf("\n");
    for (int k=0; k<15; k++)
    {
        printf("%1.2f, ", TempValues[Address][k]);
    }
    printf("%1.2f \n", TempValues[Address][15]);

}

void DMOSDec::exportFile()
{

    if (gB == false)
        return;

    bool Raw = true;
    string filename = W_folder +  "resultsBayesian.txt";
    QFile file( filename.c_str() );
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream out(&file);

         for (int i = 0; i < 48; i++)
         {
             if (Raw)
             {
                 for (int k = 0; k<15; k++)
                 {
                         out <<  TempValues[i][k] << ", ";
                 }
                     out << TempValues[i][15] << "\n ";





             }

             else {



             for (int k = 0; k<15; k++)
             {
                 if (TempValues[i][k] < 0)
                     out << " 0, ";
                 else
                    out << " 1, ";
             }

             if (TempValues[i][15] < 0)
                 out << " 0\n ";
             else
                out << " 1\n ";
             }


         }


    }
    file.close();
}


void DMOSDec::exportFileCT()
{

    if (gCT == false)
        return;

    bool Raw = true;
    string filename = W_folder + "resultsCT.txt";
    QFile file( filename.c_str() );
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream out(&file);

         for (int i = 0; i < 48; i++)
         {
             if (Raw)
             {
                 float CT;

                 for (int k = 0; k<15; k++)
                 {
                      CT =  (float) T_Count[i][k] / (C_Count[i][k]  + T_Count[i][k]  );
                      if (T_Count[i][k] == 0)
                          CT = 0;
                       out <<  CT << ", ";
                 }
                 CT = (float) T_Count[i][15] / (C_Count[i][15]  + T_Count[i][15]  );
                    if (T_Count[i][15] == 0)
                     CT = 0;
                     out << CT << "\n ";





             }

             else {



             for (int k = 0; k<15; k++)
             {
                 if (TempValues[i][k] < 0)
                     out << " 0, ";
                 else
                    out << " 1, ";
             }

             if (TempValues[i][15] < 0)
                 out << " 0\n ";
             else
                out << " 1\n ";
             }


         }


    }
    file.close();
}

float DMOSDec::GetCT(int i, int j)
{
     float CT;
     CT =  (float) T_Count[i][j] / (C_Count[i][j]  + T_Count[i][j]  );

     if (T_Count[i][j] == 0)
         CT = 0;
     /// First check CT ratio against umbrals

     if (CT < UmbralsU[j])
         CT = -10;

     else if (CT > UmbralsM[j])
         CT = 10;

     else  /// Use the standard calculation
     {
         CT = TempValues[i][j];
     }





     return CT;

}

void DMOSDec::exportFileMixed()
{

    if (gM == false)
        return;

    bool Raw = true;

    string filename = W_folder + "resultsMixed.txt";
    QFile file( filename.c_str() );
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream out(&file);

         for (int i = 0; i < 48; i++)
         {
             if (Raw)
             {
                 float CT;

                 for (int k = 0; k<15; k++)
                 {
                      CT =  GetCT(i,k);
                         out <<  CT << ", ";
                 }
                 CT = GetCT(i,15);
                     out << CT << "\n ";





             }

             else {



             for (int k = 0; k<15; k++)
             {
                 if (TempValues[i][k] < 0)
                     out << " 0, ";
                 else
                    out << " 1, ";
             }

             if (TempValues[i][15] < 0)
                 out << " 0\n ";
             else
                out << " 1\n ";
             }


         }


    }
    file.close();
}

void DMOSDec::writeDomainCT(int domain, int Address, QString seq)
{
    if (gD == false)
        return;

    int pos = 0;

    if (rF == false)   // Not using a reference file
    {
        if (domain == Address)
            pos = domain;
        else
            pos = domain+16;
    }
    else /// Using a reference file
    {
        if (Reference[Address][domain] == 1)
            pos = domain;
        else
            pos = domain + 16;
    }


    QFile * file =  FileList[pos];
    QTextStream out(file);

    out << seq;

}

void DMOSDec::ExportCpos(int Address, QString seq, int domain)
{

    if (gP == false)
        return;

    if (Address > 16)
        return;

    QFile * file2 = FileAddress[Address];
    QTextStream out2(file2);
    out2 << seq ;

    if (domain == 15)
        out2 << "\n";
    else
        out2 << ", ";
}

void DMOSDec::CloseFiles()
{
    if (gD == true)
    for (int k=0; k<32; k++)
    {
        QFile *  file = FileList[k];
        file->close();
    }

    if (gP == true)
    for (int k=0; k<16; k++)
    {
        QFile *  file = FileAddress[k];
        file->close();
    }
}
