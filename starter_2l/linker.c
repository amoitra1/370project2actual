/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 500
#define MAXLINELENGTH 1000
#define MAXFILES 6

static inline void printHexToFile(FILE *, int);

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
	char label[7];
	char location;
	unsigned int offset;
};

struct RelocationTableEntry {
    unsigned int file;
	unsigned int offset;
	char inst[6];
	char label[7];
};

struct FileData {
	unsigned int textSize;
	unsigned int dataSize;
	unsigned int symbolTableSize;
	unsigned int relocationTableSize;
	unsigned int textStartingLine; // in final executable
	unsigned int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	unsigned int textSize;
	unsigned int dataSize;
	unsigned int symbolTableSize;
	unsigned int relocationTableSize;
	int text[MAXSIZE * MAXFILES];
	int data[MAXSIZE * MAXFILES];
	SymbolTableEntry symbolTable[MAXSIZE * MAXFILES];
	RelocationTableEntry relocTable[MAXSIZE * MAXFILES];
};

int main(int argc, char *argv[]) {
	char *inFileStr, *outFileStr;
	FILE *inFilePtr, *outFilePtr; 
	unsigned int i, j;

    if (argc <= 2 || argc > 8 ) {
        printf("error: usage: %s <MAIN-object-file> ... <object-file> ... <output-exe-file>, with at most 5 object files\n",
				argv[0]);
		exit(1);
	}

	outFileStr = argv[argc - 1];

	outFilePtr = fopen(outFileStr, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileStr);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; ++i) {
		inFileStr = argv[i+1];

		inFilePtr = fopen(inFileStr, "r");
		printf("opening %s\n", inFileStr);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileStr);
			exit(1);
		}

		char line[MAXLINELENGTH];
		unsigned int textSize, dataSize, symbolTableSize, relocationTableSize;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&textSize, &dataSize, &symbolTableSize, &relocationTableSize);

		files[i].textSize = textSize;
		files[i].dataSize = dataSize;
		files[i].symbolTableSize = symbolTableSize;
		files[i].relocationTableSize = relocationTableSize;

		// read in text section
		int instr;
		for (j = 0; j < textSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = strtol(line, NULL, 0);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < dataSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = strtol(line, NULL, 0);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		unsigned int addr;
		for (j = 0; j < symbolTableSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < relocationTableSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	// *** INSERT YOUR CODE BELOW ***
	//    Begin the linking process
	//    Happy coding!!!
	unsigned int numfiles = (unsigned int)(argc - 2);
	unsigned int totalnumtexts = 0;
	unsigned int totalnumdata = 0;

	// need to get start locations first
	for (i = 0; i < numfiles; ++i) {
        files[i].textStartingLine = totalnumtexts;
        totalnumtexts += files[i].textSize;
    }
    for (i = 0; i < numfiles; ++i) {
        files[i].dataStartingLine = totalnumtexts + totalnumdata;
        totalnumdata += files[i].dataSize;
    }

	// need to then copy text and copy data
	CombinedFiles combfiles;
    combfiles.textSize = totalnumtexts;
    combfiles.dataSize = totalnumdata;
    combfiles.symbolTableSize = 0;
    combfiles.relocationTableSize = 0;

	for (i = 0; i < numfiles; ++i) {
		for (j = 0; j < files[i].textSize; ++j) {
			combfiles.text[files[i].textStartingLine + j] = files[i].text[j];
		}
	} // fpr text
	for (i = 0; i < numfiles; ++i) {
		unsigned int alldata = files[i].dataStartingLine - totalnumtexts;  /* <-- key fix */
		for (j = 0; j < files[i].dataSize; ++j) {
			combfiles.data[alldata + j] = files[i].data[j];
		}
	} // for data, if not replace with same logic as text

	// build a map of globals basically like symbol table
	unsigned int counter = 0;
	unsigned int address[MAXSIZE * MAXFILES];
	char labels[MAXSIZE * MAXFILES][7];
	unsigned int addressofstack = totalnumtexts + totalnumdata;
	unsigned int iterator;
	/*struct SymbolTableEntry { for reference
		char label[7];
		char location;
		unsigned int offset;
	};*/
	for (i = 0; i < numfiles; ++i) {
        for (j = 0; j < files[i].symbolTableSize; ++j) {
			SymbolTableEntry *symtab = &files[i].symbolTable[j]; // so to access stuff inside symtab i would do symtab->...
			if (strcmp(symtab->label, "Stack") == 0 && (symtab->location == 'T' || symtab->location == 'D')) {
				exit(1);
			}
			if (symtab->location == 'T' || symtab->location == 'D') {
                unsigned int addressoflabel;
                if (symtab->location == 'T') {
                    addressoflabel = files[i].textStartingLine + symtab->offset;
                } 
				else {
                    addressoflabel = files[i].dataStartingLine + symtab->offset;
                }
                for (iterator = 0; iterator < counter; ++iterator) {
                    if (strcmp(labels[iterator], symtab->label) == 0) {
                        exit(1);
                    }
                }
                strcpy(labels[counter], symtab->label); // flipped look at 2a
                address[counter] = addressoflabel;
                counter++;
            }
		}
	}
	// relocation table
	for (i = 0; i < numfiles; ++i) {
        for (j = 0; j < files[i].relocationTableSize; ++j) {
            RelocationTableEntry *reltab = &files[i].relocTable[j];
			int fill = 0;
            if (strcmp(reltab->inst, ".fill") == 0) {
                fill = 1;
            }

            unsigned int tgtind; // for etxt and data
            if (fill == 1) {
                //tgtind = files[i].dataStartingLine + reltab->offset;
				unsigned int datab = files[i].dataStartingLine - totalnumtexts;
				tgtind = datab + reltab->offset;
            } 
			else {
                tgtind = files[i].textStartingLine + reltab->offset;
            }

            int original;
			if (fill == 1) {
				original = combfiles.data[tgtind];
			} 
			else {
				int word = combfiles.text[tgtind];
				unsigned int lowsixt = (unsigned int)word & 0xFFFF;
				original = (int)lowsixt;
			}

            unsigned int resolved = 0;
            int global = (reltab->label[0] >= 'A' && reltab->label[0] <= 'Z');
            if (strcmp(reltab->label, "Stack") == 0) {
                resolved = addressofstack;
            } 
			else if (global) {
                int found = 0;
                unsigned int outaddress = 0;
                for (iterator = 0; iterator < counter; ++iterator) {
                    if (strcmp(labels[iterator], reltab->label) == 0) {
                        outaddress = address[iterator];
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    exit(1);
                }
                resolved = outaddress;
            } 
			else {
				unsigned int orig16 = (unsigned int)original & 0xFFFF;
				/*if ((unsigned int)original < files[i].textSize) {
                    resolved = files[i].textStartingLine + (unsigned int)original;
                }
				else {
                    unsigned int datain = (unsigned int)original - files[i].textSize;
					//resolved = files[i].textStartingLine + (unsigned int)original;
                    resolved = files[i].dataStartingLine + datain;
                }*/
				if (orig16 < files[i].textSize) {
					resolved = files[i].textStartingLine + orig16;
				}
				else {
					unsigned int datain = orig16 - files[i].textSize;
					resolved = files[i].dataStartingLine + datain;
				}
            }
            if (fill == 1) {
                combfiles.data[tgtind] = (int)resolved;
            } 
			else {
                int word = combfiles.text[tgtind];
                word = (word & 0xFFFF0000) | ((int)resolved & 0xFFFF);
                combfiles.text[tgtind] = word;
            }
		}
	}
	// printing
    /* here is an example of using printHexToFile. This will print a
       machine code word / number in the proper hex format to the output file */
    //printHexToFile(outFilePtr, 123);
	for (i = 0; i < combfiles.textSize; ++i) {
        printHexToFile(outFilePtr, combfiles.text[i]);
    }
    for (i = 0; i < combfiles.dataSize; ++i) {
        printHexToFile(outFilePtr, combfiles.data[i]);
    }

} // main

// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}

