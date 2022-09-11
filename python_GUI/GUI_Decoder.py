# -*- coding: utf-8 -*-
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import QTimer, Qt

import os, signal
import csv
import time
import pickle

import subprocess

#from FileModificationHandler import *

#sys.path.insert(1,"../MolFS/Interface/DMOS")

#from DMOS_decode import *


class DecoderView:
    
    def __init__(self, win, columns, rows):
        self.rows = rows
        self.columns = columns
        
        self.grid = QGridLayout()
        
        win.addLayout(self.grid)
        
        self.grid.setSpacing(1)
        
        self.elements = []
        
        self.min = 0
        
        self.max = 0.1
        
        
        
        for k in range(rows):
            nrow = []
            for n in range(columns):
                nlabel = QFrame()
                nlabel.setStyleSheet("QFrame { background-color:blue}")
                
                self.grid.addWidget(nlabel, k, n)
                nlabel.innerValue = 0
                nrow.append(nlabel)
                nlabel.setToolTip('0')
                
                
            self.elements.append(nrow)
            

    def clean (self):
        
        for row in self.elements:
            for element in row:
                element.setStyleSheet("QFrame { background-color:rgb(0,0,0)}")
        
        
        
    def update(self, Test):
        
        
        nrows = len(Test)
        
        maxR = self.max - self.min
        
        lmin = 1
        lmax = -1
        
        try:
            for nR in range(nrows):
                for k in range(16):
    
                    nval = Test[nR][k]
                    
                    self.elements[nR][k].innerValue = nval
                    self.elements[nR][k].setToolTip("("+str(nR+1)+","+str(k+1)+")\n"+str(nval))
                    ## Blue: RGB : 00 00 255
                    ## Yellow: RGB: 255 255 0
                    if nval > lmax:
                        lmax = nval
                    if nval < lmin:
                        lmin = nval

                    nval = (nval - self.min)/maxR
                    
                    
                    if nval > 1:
                        nval = 1
                        
                    if nval < 0:
                        nval = 0
                    

                    R = str(int(nval*255))
                    G = str(int(nval*255))
                    B = str(int(255 - nval*255))
                    
                    self.elements[nR][k].setStyleSheet("QFrame { background-color:rgb("+R+","+G+","+B+")}")
            
#            self.min = lmin
#            self.max = lmax
            
        except:
            None
                    
                
                        
        
            
class DMOSConfig():
    """
    This "window" is a QWidget. If it has no parent, it
    will appear as a free-floating window as we want.
    """
    created = False;
    
    ### Fullfill the parameters
#    AppPath = "/home/acroper/Documents/NCAT/Research/DMOS/Decoding/DMOSDecoder/build-DMOSDecoder-Desktop-Debug/DMOSDecoder"
    AppPath = ""
    WorkPath = "/tmp"
    FastQFolder = "/tmp/FastQ"
    
    Addressing = "shuffle"
    
    Ref_File = "/tmp"
    
    addOptn = 0
    ## Calculations
    Bayesian = False
    CT_Calc  = False
    Mixed    = False
    
    CPOS     = False
    DAddress = False
    
    Reference = False
    
    allC = False
    

        
    def create(self, ):
        self.w = QWidget()
        self.w.__init__()
        self.w.setWindowTitle("DMOS configuration")
        self.w.resize(500, 300)
        self.w.move(300, 300)
        
        vbox = QVBoxLayout()
        
        grid = QGridLayout()
        grid.setSpacing(10)
        
        k = 1
        grid.addWidget( QLabel('App Location:'), k, 0  )
        self.label_AppPath = QLineEdit();
        self.label_AppPath.setText (self.AppPath)
        grid.addWidget( self.label_AppPath, k , 1 )
        
        self.btnAppPath = QPushButton("Change")
        grid.addWidget( self.btnAppPath, k , 2 )
        
        k += 1
        
        grid.addWidget( QLabel('Working directory:'), k, 0  )
        self.label_WorkPath = QLineEdit();
        self.label_WorkPath.setText (self.WorkPath)
        grid.addWidget( self.label_WorkPath, k , 1 )
        
        self.btnWorkPath = QPushButton("Change")
        grid.addWidget( self.btnWorkPath, k , 2 )


        k += 1
        
        grid.addWidget( QLabel('FastQ directory:'), k, 0  )
        self.label_FastQPath = QLineEdit();
        self.label_FastQPath.setText (self.FastQFolder)
        grid.addWidget( self.label_FastQPath, k , 1 )
        
        self.btnFastQPath = QPushButton("Change")
        grid.addWidget( self.btnFastQPath, k , 2 )

        k += 1
        
        grid.addWidget( QLabel('Addressing:'), k, 0  )
        
        self.addressOptn = QComboBox()
        self.addressOptn.addItem('barcode')
        self.addressOptn.addItem('barcode2')
        self.addressOptn.addItem('shuffle')
        self.addressOptn.addItem('shuffle2')
       
        grid.addWidget( self.addressOptn, k , 1 )
        
        
        k += 1
        
        grid.addWidget( QLabel('Output calculations:'), k, 0  )
        
        k += 1
        
        self.gB = QCheckBox('Bayesian decode')
        self.gCT = QCheckBox('C>T decode')
        self.gM = QCheckBox('Mixed decode')
        self.gP = QCheckBox('C Pos analysis')
        self.gD = QCheckBox('Domains CT')
        self.gallC = QCheckBox('Use all Cs (Plus4)')
        
        
        grid.addWidget( self.gB , k, 0  )
        grid.addWidget( self.gCT , k, 1  )
        k += 1
        grid.addWidget( self.gM , k, 0  )
        grid.addWidget( self.gP , k, 1  )
        k += 1
        grid.addWidget( self.gD , k, 0  )
        grid.addWidget( self.gallC , k, 1  )
        
        k += 1
        grid.addWidget( QLabel('Reference file:'), k, 0  )
        k += 1
        self.grF = QCheckBox('Use reference file')
        grid.addWidget( self.grF , k, 0  )
        k += 1
        grid.addWidget( QLabel('Location:'), k, 0  )
        self.label_RefPath = QLineEdit();
        self.label_RefPath.setText (self.Ref_File)
        grid.addWidget( self.label_RefPath, k , 1 )
        
        self.btnRefPath = QPushButton("Change")
        grid.addWidget( self.btnRefPath, k , 2 )
        

        vbox.addLayout(grid)
        
        hbox = QHBoxLayout()
        
        self.okButton = QPushButton("Ok")
        self.cancelButton = QPushButton("Cancel")
        
        hbox.addStretch()
        hbox.addWidget(self.okButton)
        hbox.addWidget(self.cancelButton)
        
        
        
        vbox.addStretch()
        
        vbox.addLayout(hbox)
        
        self.w.setLayout(vbox)
        
        
        ### Assign functions
        self.btnAppPath.clicked.connect(self.selectAppFolder)
        self.btnWorkPath.clicked.connect(self.selectWorkFolder)
        self.btnFastQPath.clicked.connect(self.selectFastQFolder)
        self.btnRefPath.clicked.connect(self.selectRefFolder)
        
        self.okButton.clicked.connect(self.save)
        self.cancelButton.clicked.connect(self.cancel)
        
        self.created = True
    

    def openFileNameDialog(self, location):
        title = "Select a file"
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        #fileName, _ = QFileDialog.getOpenFileName(self.w, title, "", "FastQ Files (*.fastq);;Compressed FastQ files (*.fastq.gz)", options=options)
        fileName, _ = QFileDialog.getOpenFileName(self.w, title, "", "", options=options)
        if fileName:
            return fileName
        else:
            return ""
        
    def openFolderNameDialog(self, location):
        title = "Select a folder"
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName = QFileDialog.getExistingDirectory(self.w, title, location)
        if fileName:
            return fileName
        else:
            return ""      
    

    def selectAppFolder(self):
        folder = self.openFolderNameDialog(self.label_AppPath.text())
        self.label_AppPath.setText(folder)
        
    def selectWorkFolder(self):
        folder = self.openFolderNameDialog(self.label_WorkPath.text())
        self.label_WorkPath.setText(folder)        

    def selectFastQFolder(self):
        folder = self.openFolderNameDialog(self.label_FastQPath.text())
        self.label_FastQPath.setText(folder)  
        
    def selectRefFolder(self):
        folder = self.openFileNameDialog(self.label_RefPath.text())
        self.label_RefPath.setText(folder)         
    
    def show(self):
        if self.created == False:
            self.read()
            self.create()
#        else:
        self.label_AppPath.setText(self.AppPath)
        self.label_WorkPath.setText(self.WorkPath)
        self.label_FastQPath.setText(self.FastQFolder)
        self.label_RefPath.setText(self.Ref_File)
        
        
        #self.addressOptn.setCurrentText(self.Addressing)
        self.addressOptn.setCurrentIndex(self.addOptn)
        
        self.gB.setCheckState(self.Bayesian)
        self.gCT.setCheckState(self.CT_Calc)
        self.gM.setCheckState(self.Mixed)
        self.gP.setCheckState(self.CPOS)
        self.gD.setCheckState(self.DAddress)
        self.grF.setCheckState(self.Reference)
        self.gallC.setCheckState(self.allC)
            
            
            
        self.w.show()
    
    def read(self):
        try:
            file1 = open("Parameters.txt", "rb") 
            parameters = pickle.load(file1)
            file1.close()
            self.AppPath = parameters[0]
            self.WorkPath = parameters[1]
            self.FastQFolder = parameters[2]
            self.addOptn = parameters[3]
            self.Addressing = parameters[4]
            self.Bayesian = parameters[5]
            self.CT_Calc = parameters[6]
            self.Mixed = parameters[7]
            self.CPOS = parameters[8]
            self.DAddress = parameters[9]
            self.Reference = parameters[10]
            self.Ref_File = parameters[11]
            self.allC = parameters[12]
            
            print(parameters)
            print(self.WorkPath)
            print(self.Ref_File)
            

            
            
        except:
            None
                
        
    def save(self):
        ## ok button
        self.AppPath = self.label_AppPath.text()
        self.WorkPath= self.label_WorkPath.text()
        self.FastQFolder =  self.label_FastQPath.text()
        self.Ref_File = self.label_RefPath.text()
            
            #self.addressOptn.setCurrentText()
        self.addOptn =  self.addressOptn.currentIndex()
        if self.addOptn == 0:
            self.Addressing = "barcode" 
        elif self.addOptn == 1:
            self.Addressing = "barcode2" 
        elif self.addOptn == 2:
            self.Addressing = "shuffle" 
        else:
            self.Addressing = "shuffle2" 
        
        self.Bayesian = self.gB.isChecked()
        self.CT_Calc = self.gCT.isChecked()
        self.Mixed = self.gM.isChecked()
        self.CPOS = self.gP.isChecked()
        self.DAddress = self.gD.isChecked()
        self.Reference = self.grF.isChecked()
        self.allC= self.gallC.isChecked()

        parameters = []
        parameters.append(self.AppPath)
        parameters.append(self.WorkPath)
        parameters.append(self.FastQFolder)
        parameters.append(self.addOptn)
        parameters.append(self.Addressing)
        parameters.append(self.Bayesian)
        parameters.append(self.CT_Calc)
        parameters.append(self.Mixed)
        parameters.append(self.CPOS)
        parameters.append(self.DAddress)
        parameters.append(self.Reference)
        parameters.append(self.Ref_File)
        parameters.append(self.allC)
        file1 = open("Parameters.txt", "wb") 
        pickle.dump(parameters, file1)
        file1.close()
        
        
                
        
        self.w.close()
        
    def cancel(self):
        self.w.close()
        
    def getCMD(self):
        
        cmd = os.path.join( self.AppPath , "DMOSDecoder" )
        cmd += " -f " + self.FastQFolder 
        cmd += " -w " + self.WorkPath
        
        if self.Addressing == "shuffle2":
            cmd += " -a  shuffle -af " + os.path.join(self.AppPath, "Addresses2.txt")
        else:
            cmd += " -a  " + self.Addressing
        
        if self.Bayesian or self.CT_Calc or  self.Mixed or self.CPOS  or  self.DAddress:
            cmd += " -g "
            if self.Bayesian:
                cmd += "B"
            if self.CT_Calc:
                cmd += "C"
            if self.Mixed:
                cmd += "M"
            if self.CPOS:
                cmd += "P"                
            if self.DAddress:
                cmd += "D"
                
        if self.Reference:
            cmd += " -r " + self.Ref_File
        
        if self.allC:
            cmd += " -allC"
        
        return cmd
                
        
        
    
    

class MainWindow:

    state = "stop"
    
    dconfig = DMOSConfig()
    
    def OpenWindow(self):

        app = QApplication(sys.argv)

        self.w = QWidget()
        self.w.resize(500, 500)
        self.w.move(300, 300)
        self.w.setWindowTitle('Simple DMOS decoder')
        
        
        
        
        vbox = QVBoxLayout()
        
        cfgBox = self.configMenu()
        
        vbox.addLayout(cfgBox)
        
        self.w.setLayout(vbox)
        
        self.grid = DecoderView(vbox, 16,48)
        
        self.timer=QTimer()
        self.timer.timeout.connect(self.Tick)
        
        self.w.show()
        
        

        app.exec_()
        
    
    
    
    def configMenu(self):
        
        self.startButton = QPushButton("Start")
        
        self.confButton = QPushButton("Configure")
        
        self.UDValue = QSlider(Qt.Horizontal)
        self.MDValue = QSlider(Qt.Horizontal)
        
        self.UDValue.setValue(0)
        self.MDValue.setValue(100)
        
        VisorGrid = QGridLayout()
        
        VisorGrid.addWidget( QLabel("Unmutated Val"), 0, 0 )
        VisorGrid.addWidget( QLabel("Mutated Val"), 1, 0)
        VisorGrid.addWidget( self.UDValue, 0, 1 )
        VisorGrid.addWidget(self.MDValue, 1,1)
#        
        
#        self.cancelButton = QPushButton("Cancel")
#        
#        self.getfileBtn = QPushButton("Select file")
        
        hbox = QHBoxLayout()
        hbox.addStretch(1)
        
        hbox.addLayout(VisorGrid)
        
        hbox.addWidget(self.startButton)
        hbox.addWidget(self.confButton)
#        hbox.addWidget(self.cancelButton)
#        hbox.addWidget(self.getfileBtn)
        
        self.startButton.clicked.connect(self.startFn)
        self.confButton.clicked.connect(self.showConfig)
        
        self.UDValue.valueChanged.connect(self.updateRanges)
        self.MDValue.valueChanged.connect(self.updateRanges)
#        self.getfileBtn.clicked.connect(self.getFileFn)
        
        return hbox
    
    def updateRanges(self):
        minVal = -10+10*self.UDValue.value()/100
        maxVal = self.MDValue.value()/10
        self.grid.max = maxVal
        self.grid.min = minVal
        
        
        
        
    def showConfig(self):
        self.dconfig.show()
    
    def getFileFn(self):
        filename = self.openFolderNameDialog()
        print(filename)
    
    
    
    def startFn(self):
        
        if self.state == "stop":
#            self.Configure()
#            self.CheckFile()
            self.timer.start(1000)
            self.startButton.setText ("Stop")
            self.state = "start"
            print("Starting")
            self.StartProgram()
        else:
            self.startButton.setText ("Start")
            self.state = "stop"
            self.timer.stop()
            self.StopProgram()
            print("Stopping")
            
      
    
    def Tick(self):
        print("Tick")
        self.CheckFile()
#        self.timer.stop()
        
    def StopProgram(self):
        try:
            os.kill( self.CDecoder.pid, signal.SIGKILL )
        except:
            None
                
    def StartProgram(self):
        ## Start program
        
        if self.dconfig.Mixed:
            self.sourceFile = self.dconfig.WorkPath + "/resultsMixed.txt"
        elif self.dconfig.Bayesian:
            self.sourceFile = self.dconfig.WorkPath + "/resultsBayesian.txt"
        elif self.dconfig.CT_Calc:
            self.sourceFile = self.dconfig.WorkPath + "/resultsCT.txt"
        
        
        ## remove the output file
        try:
#            os.remove(self.sourceFile)
            None
        except:
            None    
        try:
            os.mkdir(self.dconfig.WorkPath)
        except:
            None
        
        startCMD = self.dconfig.getCMD()
        print("Command line:")
        print(startCMD)
        cwd = os.getcwd()
        os.chdir(self.dconfig.AppPath)
        
#        self.CDecoder = subprocess.Popen( startCMD )
        argsCMD = startCMD.split()
        self.CDecoder = subprocess.Popen( argsCMD )
        
        os.chdir(cwd)
        
        self.grid.clean()
    
    def Configure(self):
        self.sourceFile = "/home/acroper/Documents/NCAT/Research/DMOS/Decoding/QtDecoder/build-QtDecoder-Desktop-Debug/resultsCT.txt"
        self.sourceFile = "/home/acroper/Documents/NCAT/Research/DMOS/Decoding/QtAnalyzer/build-QtAnalyzer-Desktop-Debug/resultsCT.txt"
        self.decodeFile= "/tmp/LDPC/Reconstructed.txt"
        
    
    def CheckFile(self):


        Test  = self.ReadFile(self.sourceFile)
        
        self.grid.update(Test)
        
    #        
    #        encoder = "LDPC_768_PR"
    #        msgSize = 72*8  ##  576
    #        codeSize = 768
    #        decod = DMOSDec()
    #              
    #        decod.LDPC_N = codeSize
    #        
    #        decod.outputFile = decodeFile
    #        decod.decRaw2LDPC(sourceFile, encoder) 
    #    
    #        Valid = "No Valid"
    #        if decod.LDPC_Valid == True:
    #            Valid = "Valid"

    def ReadFile(self, filename):
        
        OutFile = []
        
        with open(filename, newline='') as f:
            reader = csv.reader(f)
            Kn = list(reader)
            
            for rows in Kn:
                nr = []
                for cols in rows:
                    try:
                        nr.append(float(cols))
                    except:
                        None
                
                OutFile.append(nr)    
        
        return OutFile
        
        
        
        
        



MW = MainWindow()
MW.OpenWindow()
