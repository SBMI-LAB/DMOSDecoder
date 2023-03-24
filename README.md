# DMOSDecoder

C++ library to retrieve the binary contents from a DMOS block, by reading the FastQ files after nanopore sequencing.

To compile this library, it requires QtCreator from  https://www.qt.io/download

During compilation, QtCreator generates a build folder. Copy the contents of the /bin folder of this repository inside the build folder, where the compiler saves the executable file.

### Prerequisites

Qt Creator 

Python 3

PyQt5


### Using the GUI

The GUI script is located in the folder python_GUI. 
To run the script, open a terminal in the same folder and execute the following command:

python3 GUI_decoder.py

It will show a window that helps to configure the parameters for the DMOS decoder:

#### Parameters

Click on "configure", and it will show a window with options:

App Location: Folder with the executable file for the DMOS decoder.

Working Directory: Folder where the decoder saves the partial results after 100 reads.

FastQ Directory: Folder that contains the FastQ (uncompressed) files.

###### Addressing: 
Select one of the addressing schemes

Barcode : First set of barcodes used to identify the registers [Experiment x]. 

Barcode2: Second set of barcodes used to identify the registers [Experiment x]. 

shuffle : Combination of high entropy shuffling domains and low entropy shuffling domains for addressing.

shuffle2: Low entropy permutation domains for addressing.


###### Output calculations: 

Select one of the output calculations, according to the needs:

C>T decode     : Partial C>T ratio generated for the first Bayesian decoder.

Bayesian decode: Partial output from the second Bayesian decoder.

Mixed decode   : Classification scores from the combination of both Bayesian decoders.

C pos analysis : Generates the analysis per individual Cytosine position.

Domains CT     : Geneartes analysis for individual domains regardless of the address. This requires a reference file.

Use all Cs     : Returns all the cytosines of the domain, instead of just the target area.



Reference file: Allows to set a reference file. It is a list of 1s and 0s, comma separated, with the expected file. 
