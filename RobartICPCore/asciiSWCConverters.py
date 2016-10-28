import numpy as np
from swcFuncs import readSWC_numpy, writeSWC_numpy
from asciiFuncs import readASCII_numpy, writeASCII_numpy
import StringIO



def ascii2swc(asciiFile, outFile, defaultRadius=0.5):

    asciiData = readASCII_numpy(asciiFile)
    swcData = np.empty((asciiData.shape[0], 7))
    swcData[:, 0] = np.arange(asciiData.shape[0])
    swcData[:, 1] = 3
    swcData[:, 2:5] = asciiData
    swcData[:, 5] = defaultRadius
    swcData[:, 6] = np.arange(-1, -asciiData.shape[0] - 1, -1)

    writeSWC_numpy(outFile, swcData)

def swc2ascii(swcFile, asciiFile):

    headr, swcData = readSWC_numpy(swcFile)
    writeASCII_numpy(swcData[:, 2:5].copy(), asciiFile)
