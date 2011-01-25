import ROOT


#Set preferences for ROOT output
ROOT.gStyle.SetCanvasColor(0)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetPaintTextFormat('.3f')
ROOT.gStyle.SetOptStat(111111)

#Set energy range and number of bins 
LOG_E_MIN = 1
LOG_E_MAX = 6
NUM_E_BINS = 10
#Minimal precut to classify a cluster
PRE_CUT   = 'CalNumXtals > 4'
# Set large number of bins to perform equal pop binning.
INI_NUM_BINS = 4000

#Define the energy bins
ENERGY_BINS = [(1.0,1.5),(1.5,2.0),(2.0,2.5),(2.5,3.0),(3.0,3.5),
              (3.5,4.0),(4.0,4.5),(4.5,5.0),(5.0,5.5),(5.5,6.0)]



class ClusterVariable:
    #Basic class to handle variable info
    def __init__(self, expression, minValue, maxValue, numBins = 50,
                 label = None):
        self.Expression = expression
        self.MinValue = minValue
        self.MaxValue = maxValue
        self.NumBins = numBins
        self.Label = label or expression

    def __str__(self):
        return 'Variable "%s" (%.3f--%.3f in %d bins)' %\
               (self.Expression, self.MinValue, self.MaxValue, self.NumBins)



#Variable list with range and expressions.
VARIABLE_LIST  = [ClusterVariable('Cal1TransRms', 0, 100),
                  ClusterVariable('Cal1LongRmsAsym', 0, 0.25),
                  ClusterVariable('log10(Cal1LongRms/Cal1TransRms)',
                                  0, 2.5, label = 'MomentRatio'),
                  ClusterVariable('Cal1CoreEneFrac', 0, 1),
                  ClusterVariable('Cal1XtalEneRms/CalEnergyRaw',0, 0.4,
                                  label = 'XtalEneRms'),
                  ClusterVariable('Cal1XtalEneSkewness',-2, 10),
                  ClusterVariable('Cal1dEdxAve/Cal1FullLength', 0, 100,
                                  label = 'dEdxperLength')
                #  ClusterVariable('Cal1NumXtals/log10(CalEnergyRaw)', 0, 150,
                #                  label = 'NumXtals')
                  ]

#File path where the datasets for training are located. NB! The version of
#GR which is writen in the output xml file is taken from this path in method
# getGRversion() in ClusterClassifier class.
MAIN_FILE_PATH = '/data/work/recon/v18r8p3/work/firstlook'


FILE_PATH_DICT = {#'gam': '%s/all_gamma_180GeV-*.root'%MAIN_FILE_PATH,
                  #'had': '%s/CrProtonMix-*.root'%MAIN_FILE_PATH,
                  'mip': '%s/high_e_surface_muons-*.root'%MAIN_FILE_PATH}



TOPOLOGY_DICT = {'gam': 0,
                 'had': 1,
                 'mip': 2
             #    'ghost':3
                 }

PRE_CUT_DICT = {'gam': None,
                'had': 'abs(CalMIPRatio - 1) > 0.75',
                'mip': 'abs(CalMIPRatio - 1) < 0.75'
              #  'ghost': None
                }
COLORS_DICT = {'gam': ROOT.kRed,
               'had': ROOT.kBlue,
               'mip': ROOT.kBlack
              # 'ghost': ROOT.kGray + 2
               }


def hname(label, topology):
    return 'fPdf_%s_%s' % (label, topology)


def hVarname(label,topology,Ebin):
    return 'fPdf_%s_%s_varBin_%s' % (label, topology,Ebin)

def getColor(topology):
    try:
        return COLORS_DICT[topology]
    except KeyError:
        return ROOT.kBlack

def getCut(topology):
    try:
        cut = PRE_CUT_DICT[topology]
    except KeyError:
        return PRE_CUT
    if cut is None or cut.strip() == '':
        return PRE_CUT
    return '(%s) && (%s)' % (PRE_CUT, cut)

def getTopologyIndex(topology):
    try:
        return TOPOLOGY_DICT[topology]
    except KeyError:
        return -1

def normalizeHist(hist1d, miny = 0, maxy = 1):
    numBinsX = hist1d.GetNbinsX()
    sum = 0.0
    for i in xrange(1, numBinsX + 1):
        sum += hist1d.GetBinContent(i)
    for i in xrange(1, numBinsX + 1):
        try:
            value = hist1d.GetBinContent(i)/sum
            hist1d.SetBinContent(i, value)
        except ZeroDivisionError:
            pass
    hist1d.SetMinimum(miny)
    hist1d.SetMaximum(maxy)


def normalizeSlices(hist2d, minz = 1e-3, maxz = 1):
    numBinsX = hist2d.GetNbinsX()
    numBinsY = hist2d.GetNbinsY()
    for i in xrange(1, numBinsX + 1):
        sum = 0.0
        for j in xrange(0, numBinsY + 2):
            sum += hist2d.GetBinContent(i, j)
        for j in xrange(0, numBinsY + 2):
            try:
                value = hist2d.GetBinContent(i, j)/sum
                hist2d.SetBinContent(i, j, value)
            except ZeroDivisionError:
                pass
    hist2d.SetMinimum(minz)
    hist2d.SetMaximum(maxz)



OBJECT_POOL = {}

def toPool(rootObject):
    OBJECT_POOL[rootObject.GetName()] = rootObject



def saveToFile(objectList, filePath):
    outputFile = ROOT.TFile(filePath, 'RECREATE')
    for object in objectList:
        object.Write()
    outputFile.Close()
