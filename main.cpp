#include <QCoreApplication>
#include <stdio.h>
#include "dmosdec.h"
#include <string.h>
#include <iostream>
#include <sys/stat.h>


using std::string;
using std::cout;



string format_directory(const std::string &s)
{
    char L = s[s.size()-1];
    string ns = "";

    if (L != '/')
        ns = s + "/";
    else
        ns = s;

    return ns;
}

bool is_directory(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}


int main(int argc, char *argv[])
{

    //// Parameters
    /// -f Folder:  Source folder of fastq files        [1]
    /// -w Folder:  Working directory. Default /tmp/    [2]
    /// -a [option] Addressing mode                     [3]
    ///    barcode  Using barcodes
    ///    shuffle  Standard shuffle scheme from file
    /// -af File    Address list                        [4]
    /// -g [B] Generate the following analysis,         [5]
    ///    B    Bayesian file       (   DMOSBay.txt   )
    ///    CT   CT Calculation      (   DMOSCT.txt    )
    ///    M    Mixed calculation   (   DMOS_out.txt  )
    ///    P    C pos results       (   CPOS/#Address.txt   ) [ addresses]
    ///    D    Domain CT           (   CTD/#D.txt    )
    /// -c [option]: Number of cores. Default 1         [6]
    ///
    /// -r File: Reference file for tests               [7]
    ///
    /// -allC: Use all C's                              [8]
    ///


    string F_folder = "/tmp/Down/pass";
    string W_folder = "/tmp/DMOS";
    string addressing = "shuffle";
    string A_List = "Addresses.txt";

    string gB = "0";  // Bayesian
    string gCT = "0"; // CT
    string gM = "0";  // Mixed
    string gP = "0";  // CPos
    string gD = "0";  // Domain CT address

    string cores = "1";

    string rfile = ""; //"/tmp/DMOS/ReferenceJSNN.txt";

    bool allC = false;

    /// Parse parameters
    ///


    int preCommand = 0;


    for (int i = 0; i < argc; i++)
    {
        string args = (string) argv[i];
        if (preCommand == 0)
        {

            if (args == "-f")
                preCommand = 1;

            if (args == "-w")
                preCommand = 2;
            if (args == "-af")
                preCommand = 3;
            if (args == "-a")
                preCommand = 4;
            if (args == "-g")
                preCommand = 5;
            if (args == "-c")
                preCommand = 6;
            if (args == "-r")
                preCommand = 7;
            if (args == "-allC")
                allC = true;

        }
        else
        {
            switch (preCommand) {

            case 1: /// should check existence of folder
                F_folder = argv[i];
                break;
            case 2: /// should check existence of folder
                W_folder = argv[i];
                break;
            case 3: /// should check existence of the file
                A_List = argv[i];
                break;
            case 4: /// should verify if domain, shuffle or other
                    addressing = argv[i];
                break;
            case 5:
                /// Check if there are elements in the list
                if (((string) argv[i]).find("B")!=-1)
                    gB = "1";
                if (((string) argv[i]).find("C")!=-1)
                    gCT = "1";
                if (((string) argv[i]).find("M")!=-1)
                    gM = "1";
                if (((string) argv[i]).find("P")!=-1)
                    gP = "1";
                if (((string) argv[i]).find("D")!=-1)
                    gD = "1";
                break;
            case 6:
                cores = argv[i];
                break;
            case 7:
                rfile = argv[i];
                break;

            }
            preCommand = 0;
        }
    }


    DMOSDec Dec = DMOSDec();

    F_folder = format_directory(F_folder);
    W_folder = format_directory(W_folder);

    /// Assign the parameters to the decoder
    ///
    cout << "Fastq Folder    -f : " << F_folder << "\n";
    cout << "Working Folder  -w : " << W_folder << "\n";
    cout << "Addressing file -af: " << A_List << "\n";
    cout << "Addressing mode -a : " << addressing << "\n";
    cout << "[barcode, shuffle]\n";
    cout << "Number of cores -c : " << cores << "\n";
    cout << "Analysis outputs -g [BCMPD]: \n";
    cout << "Bayesian        [B]: " << gB << "\n";
    cout << "CT Ratio        [C]: " << gCT << "\n";
    cout << "Mixed analysis  [M]: " << gM << "\n";
    cout << "C Pos analysis  [P]: " << gP << "\n";
    cout << "Domains CT      [D]: " << gD << "\n";

    if (allC)
        cout << "Use all Cs    -allC: Yes \n";
    else
        cout << "Use all Cs    -allC: No \n";

    if (rfile != "")
        cout << "Reference file     -r: " << rfile << "\n";
    else
        cout << "Reference file     -r: None\n";



    bool Passed = true;


    cout << "\n\nValidating parameters:\n";
    /// Check parameters
    if (!is_directory(F_folder))
    {
        Passed = false;
        cout << "Invalid FastQ folder \n";
    }

    if (!is_directory(W_folder))
    {
        Passed = false;
        cout << "Invalid working folder \n";
    }

    if (!is_directory(A_List))
    {
        Passed = false;
        cout << "Invalid addressing file \n";
    }



    if (addressing == "barcode2")
    {
        addressing = "barcode";
        Dec.wa1 = true;  /// workaround 1
    }



    if (addressing != "barcode" && addressing != "shuffle")
    {
        Passed = false;
        cout << "Invalid addressing scheme \n";
    }

    if (rfile != "")
    {
        if (!is_directory(rfile))
        {
            Passed = false;
            cout << "Invalid reference file \n";
        }
        else
        {
            Dec.rF = true;
            Dec.Ref_File = rfile;
        }
    }

    if (Passed)
    {
        cout << "All parameters valid, proceeding to run analysis\n";
    }

    Dec.F_folder = F_folder;
    Dec.W_folder = W_folder;



    Dec.addressing = addressing;



    Dec.Address_File = A_List;

    Dec.gB  =  (gB =="1"?true:false);
    Dec.gCT =  (gCT=="1"?true:false);
    Dec.gM  =  (gM =="1"?true:false);
    Dec.gP  =  (gP =="1"?true:false);
    Dec.gD  =  (gD =="1"?true:false);

    Dec.allC = allC;

    /// Run decoder

    if (Passed)
        Dec.RunAnalysis();


    printf("Finish\n");


}
