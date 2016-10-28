import numpy as np
import StringIO

def writeASCII_numpy(asciiData, asciiFile, headr=''):

    fakeFile = StringIO.StringIO()
    np.savetxt(fakeFile, asciiData, fmt='%0.7e')
    with open(asciiFile, 'w') as fle:
        fle.write("%d\n" % asciiData.shape[0])
        fle.write(fakeFile.getvalue())

def readASCII_numpy(asciiFile):

    asciiData = np.loadtxt(asciiFile, skiprows=1)

    return asciiData