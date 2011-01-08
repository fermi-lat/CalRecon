# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/SConscript,v 1.23 2011/01/06 20:21:27 lsrea Exp $
# Authors: Alexandre Chekhtman <chehtman@gamma.nrl.navy.mil>, David Chamont <chamont@poly.in2p3.fr
# Version: CalRecon-06-12-06
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='CalRecon', toBuild='component')

CalRecon=libEnv.SharedLibrary('CalRecon',
                              listFiles(['src/*.cxx','src/Dll/*.cpp',
                                         'src/Display/*.cxx',
                                         'src/Clustering/*.cxx',
                                         'src/Classify/*.cxx',
                                         'src/MipFinding/*.cxx',
                                         'src/EnergyCorrections/*.cxx',
                                         'src/Utilities/*.cxx']))

progEnv.Tool('CalReconLib')

test_CalRecon = progEnv.GaudiProgram('test_CalRecon',
                                     listFiles(['src/test/*.cxx']),
                                     test = 1, package='CalRecon')

progEnv.Tool('registerTargets', package = 'CalRecon',
             libraryCxts = [[CalRecon, libEnv]],
             testAppCxts = [[test_CalRecon, progEnv]],
             includes = listFiles(['CalRecon/*.h']),
             xml = listFiles(['xml/*.data', 'xml/*.xml']),
             jo = listFiles(['src/test/*.txt']))
