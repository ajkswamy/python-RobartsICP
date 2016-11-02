import subprocess32
from swcFuncs import transSWC
from asciiFuncs import readASCII_numpy
from asciiSWCConverters import ascii2swc, swc2ascii
import tempfile
import os
import shutil
import numpy as np
import pandas as pd

def getTransformFromRICPOut(outStr, marker):

    outSplit = outStr.splitlines()
    markerInd = outSplit.index(marker)
    minR = np.loadtxt(outSplit[markerInd + 5: markerInd + 8])
    minA = np.loadtxt(outSplit[markerInd + 9: markerInd + 12])
    minT = np.loadtxt(outSplit[markerInd + 13: markerInd + 16]).reshape((3, ))
    minFRE = float(outSplit[markerInd + 17])
    return minR, minA, minT, minFRE


def runRICPCPP(testASCII, refASCII, tempDir, initTrans=None, logFile=None, timeout=240):

    filePath = os.path.split(__file__)[0]
    old_cwd = os.getcwd()
    os.chdir(tempDir)

    try:

        if initTrans is None:
            initTransStr = 'No init specified'
            out = subprocess32.check_output([os.path.join(filePath, 'bin', 'ASICP_MD_ANN'),
                                             testASCII,
                                             refASCII,
                                             ],
                                            stderr=subprocess32.STDOUT, timeout=timeout)
        else:
            with open(initTrans) as fle:
                initTransStr = fle.read()
            out = subprocess32.check_output([os.path.join(filePath, 'bin', 'ASICP_MD_ANN'),
                                           testASCII,
                                           refASCII,
                                           initTrans
                                           ],
                                          stderr=subprocess32.STDOUT, timeout=timeout)

        minR, minA, minT, minFRE = getTransformFromRICPOut(out, 'Final answer: 3 3')
        print('Finished with a minFRE of {}'.format(minFRE))

    except subprocess32.TimeoutExpired:

        minR = minA = minT = None
        minFRE = np.inf

        out = 'Did not finish even after {} seconds'.format(timeout)
        print(out)

    out = 'Using following Inittrans:\n' + initTransStr + '\n' + out
    os.chdir(old_cwd)
    if logFile:
        with open(logFile, 'w') as fle:
            fle.write(out)

    return minR, minA, minT, minFRE


class InitTransGen:

    def __init__(self, refFile, testFile):

        self.refFile = refFile
        self.testFile = testFile
        self.refData = readASCII_numpy(refFile)
        self.refStd = self.refData.std(axis=0)
        self.testData = readASCII_numpy(testFile)
        meanDiff = self.refData.mean(axis=0) - self.testData.mean(axis=0)
        self.firstTrans = np.eye(4)
        self.firstTrans[:3, 3] = meanDiff
        self.count = -1

    def __iter__(self):

        self.count = -1

    def next(self):

        self.count += 1

        if self.count == 0:

            return None
        elif self.count == 1:
            return self.firstTrans

        else:
            tmp = self.firstTrans.copy()
            tmp[:3, 3] += 3 * self.refStd * np.random.rand(3)
            return tmp




def runRICP(refInFile, testInFile, outFile, deleteTempFiles=True, maxTries=10):
    tempDir = tempfile.mkdtemp(dir=os.getcwd())
    inAsciiFiles = []
    inSWCFiles = []
    for inFle in [refInFile, testInFile]:
        if inFle.endswith('.swc'):
            thrash, tempFile = tempfile.mkstemp(dir=tempDir, suffix='.ascii')
            swc2ascii(inFle, tempFile)
            inSWCFiles.append(inFle)
            inAsciiFiles.append(tempFile)
        elif inFle.endswith('.ascii'):
            thrash, tempFile = tempfile.mkstemp(dir=tempDir, suffix='.swc')
            inAsciiFiles.append(inFle)
            ascii2swc(inFle, tempFile)
            inSWCFiles.append(tempFile)
        else:
            raise(IOError('Unsupported file format for %s. Supported formats are ascii and swc,' % inFle))

    [refAsciiFile, testAsciiFile] = inAsciiFiles
    [refSWCFile, testSWCFile] = inSWCFiles

    initTransGen = InitTransGen(refFile=refAsciiFile, testFile=testAsciiFile)

    allRes = []
    allResSuccess = []

    for iterNo in range(maxTries):

        print('Trial number {}'.format(iterNo))
        initTrans = initTransGen.next()

        if initTrans is None:
            initTransFile = None
        else:
            thrash, initTransFile = tempfile.mkstemp(dir=tempDir, suffix='.ascii')

            np.savetxt(initTransFile, initTrans)

        minR, minA, minT, minFRE = runRICPCPP(testAsciiFile, refAsciiFile, tempDir, initTransFile,
                                      '{}_try{}.log'.format(outFile, iterNo + 1))

        if all([x is not None for x in [minR, minA, minT, minFRE]]):
            allResSuccess.append(True)
        else:
            allResSuccess.append(False)

        allRes.append((minR, minA, minT, minFRE))

    print('The minFRES:\n{}'.format([x[3] for x in allRes]))

    if any(allResSuccess):
        bestInd = np.argmin([x[3] for x in allRes])
        print('Choosing index {}'.format(bestInd))
        (bestMinR, bestMinA, bestMinT, bestMinFRE) = allRes[bestInd]
        transSWC(testSWCFile, np.dot(bestMinR, bestMinA), bestMinT, outFile)
    else:
        print('No solutions found for {} number of trials'.format(maxTries))


    if deleteTempFiles:
        shutil.rmtree(tempDir)
    else:
        print('TempFiles in %s not deleted' % tempDir)





