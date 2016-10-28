import os
from RobartICPCore.RobartsICPWrapper import runRICP
import numpy as np

homeFolder = os.path.expanduser('~')

# ----------------------------------------------------------------------------------------------------------------------

# dirPath = os.path.join(homeFolder, 'DataAndResults', 'morphology', 'OriginalData', 'GICP_tests', 'car')
#
# expNames = [
#             'car0',
#             'car1',
#             'car2',
#             'car3',
#             'car4',
#             'car5',
#             ]
#
# resDir = os.path.join(homeFolder, 'DataAndResults', 'morphology', 'GICP', 'GICP_tests', 'car')
# refInd = 0
# ----------------------------------------------------------------------------------------------------------------------

# dirPath = os.path.join(homeFolder, 'DataAndResults', 'morphology', 'OriginalData', 'GICP_tests', 'outdoor')
#
# expNames = ['outdoor0[300x541]',
#             # 'outdoor10[300x541]',
#             # 'outdoor11[300x541]',
#             # 'outdoor12[300x541]',
#             # 'outdoor13[300x541]',
#             'outdoor14[300x541]',
#             'outdoor15[300x541]',
#             # 'outdoor1[300x541]',
#             # 'outdoor2[300x541]',
#             # 'outdoor3[300x541]',
#             # 'outdoor4[300x541]',
#             # 'outdoor5[300x541]',
#             # 'outdoor6[300x541]',
#             # 'outdoor7[300x541]',
#             # 'outdoor8[300x541]',
#             # 'outdoor9[300x541]'
#             ]
#
#
# resDir = os.path.join(homeFolder, 'DataAndResults', 'morphology', 'GICP', 'GICP_tests', 'outdoor')
# refInd = 0
# ----------------------------------------------------------------------------------------------------------------------
#
dirPath = homeFolder + '/DataAndResults/morphology/OriginalData/Tests'
#
# expNames = [
#             # 'HB130313-4',
#             # 'HB130313-4RandTrans',
#             # 'HB130313-4RandTrans1',
#             # 'HB130313-4RandTrans2',
#             # 'HB130313-4RandTrans3',
#             # 'HB130313-4RandTrans4',
#             # 'HB130313-4RandTrans5',
#             # 'HB130313-4RandTrans6',
#             # 'HB130313-4RandTrans7',
#             # 'HB130313-4RandTrans8',
#             # 'HB130313-4RandTrans9',
#             # 'HB130313-4NoiseStd1RandTrans',
#             # 'HB130313-4NoiseStd2RandTrans',
#             # 'HB130313-4NoiseStd3RandTrans',
#             # 'HB130313-4NoiseStd4RandTrans',
#             # 'HB130313-4NoiseStd5RandTrans',
#             # 'HB130313-4NoiseStd6RandTrans',
#             # 'HB130313-4NoiseStd7RandTrans',
#             # 'HB130313-4NoiseStd8RandTrans',
#             # 'HB130313-4NoiseStd9RandTrans',
#
#
#             'HSN-fluoro01.CNG',
#             'HSN-fluoro02.CNG',
#             # 'HSN-fluoro01.CNGRandTrans',
#             # 'HSN-fluoro01.CNGRandTrans1',
#             # 'HSN-fluoro01.CNGRandTrans2',
#             # 'HSN-fluoro01.CNGRandTrans3',
#             # 'HSN-fluoro01.CNGRandTrans4',
#             # 'HSN-fluoro01.CNGRandTrans5',
#             # 'HSN-fluoro01.CNGRandTrans6',
#             # 'HSN-fluoro01.CNGRandTrans7',
#             # 'HSN-fluoro01.CNGRandTrans8',
#             # 'HSN-fluoro01.CNGRandTrans9',
#
#             # 'HSN-fluoro01.CNGNoiseStd1RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd2RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd3RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd4RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd5RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd6RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd7RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd8RandTrans',
#             # 'HSN-fluoro01.CNGNoiseStd9RandTrans',
#             ]
# #
refName = 'HSN-fluoro01.CNG'

baseNames = [
            # 'HB130313-4',
            # 'HB130313-5',
            # 'HB130313-6',
            # 'HB130313-7',
            # 'HB130313-8',
            # 'HSN-fluoro01.CNG',
            'HSN-fluoro01.CNGNoiseStd1',
            'HSN-fluoro01.CNGNoiseStd2',
            'HSN-fluoro01.CNGNoiseStd3',
            'HSN-fluoro01.CNGNoiseStd4',
            'HSN-fluoro01.CNGNoiseStd5',
            'HSN-fluoro01.CNGNoiseStd6',
            'HSN-fluoro01.CNGNoiseStd7',
            'HSN-fluoro01.CNGNoiseStd8',
            'HSN-fluoro01.CNGNoiseStd9',
            'HSN-fluoro01.CNGNoiseStd10',
            'HSN-fluoro01.CNGNoiseStd11',
            'HSN-fluoro01.CNGNoiseStd12',
            'HSN-fluoro01.CNGNoiseStd13',
            'HSN-fluoro01.CNGNoiseStd14',
            'HSN-fluoro01.CNGNoiseStd15',
            'HSN-fluoro01.CNGNoiseStd16',
            'HSN-fluoro01.CNGNoiseStd17',
            'HSN-fluoro01.CNGNoiseStd18',
            'HSN-fluoro01.CNGNoiseStd19',
            ]

Ns = [5]

if len(Ns) == 1:
    Ns = [Ns[0], Ns[0] + 1]

expNames = [refName]

for n in range(Ns[0], Ns[1]):
    for bn in baseNames:
        expNames.append(bn + 'RandTrans' + str(n))

#
refInd = 0
resDir = homeFolder + '/DataAndResults/morphology/RobartsICP/Tests/'
if not os.path.isdir(resDir):
    os.mkdir(resDir)


# ----------------------------------------------------------------------------------------------------------------------

refSWC = os.path.join(dirPath, expNames[refInd] + '.swc')

for expInd, expName in enumerate(expNames):
    if refInd != expInd:
        print('Doing ' + expName)

        runRICP(refSWC, os.path.join(dirPath, expName + '.swc'), os.path.join(resDir, expName + '.swc'))