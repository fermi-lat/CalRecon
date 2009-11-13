#$Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalRecon/CalReconLib.py,v 1.2 2008/09/25 21:41:14 ecephas Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['CalRecon'])
    env.Tool('addLibrary', library = env['minuitLibs'])
    env.Tool('CalXtalResponseLib')
    env.Tool('TkrUtilLib')
    env.Tool('CalUtilLib')
    env.Tool('GuiSvcLib')
    env.Tool('GlastSvcLib')
    env.Tool('RootIoLib')
    #env.Tool('mcRootDataLib')
def exists(env):
    return 1;
