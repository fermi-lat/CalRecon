# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/SConscript,v 1.48 2012/01/23 19:52:44 jrb Exp $
# Authors: Alexandre Chekhtman <chehtman@gamma.nrl.navy.mil>, David Chamont <chamont@poly.in2p3.fr
# Version: CalRecon-06-08-02-gr04

Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='CalRecon', toBuild='component')

CalRecon=libEnv.ComponentLibrary('CalRecon',
                              listFiles(['src/*.cxx',
                                         'src/Display/*.cxx',
                                         'src/Clustering/*.cxx',
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
             xml = listFiles(['xml/*.data', 'xml/*.xml', 'xml/*.txt']),
             jo = listFiles(['src/test/*.txt', 'src/test/*.root']))
