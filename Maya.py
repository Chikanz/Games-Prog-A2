import maya.cmds as cmds
import sys

filename = 'C:/Users/Dook/Desktop/LevelBounds.Zac'
logfile = open(filename, 'w+')
levelList = cmds.ls('pCube*') #filter, change later
for x in range (0,len(levelList)-1):
    ln=""
    cmds.select(levelList[x])
    cmds.polyTriangulate(); #Triangulate mesh for engine
    try:
        ln = str(cmds.xform(q=1,bb=1,ws=1)) #query bounds in world space
        #Format output
        ln = ln[1:-1]
        ln += ", \n"
        logfile.write(ln)
        print x #for debug
    except:
        print sys.exc_info()[0] #sometimes things don't work
    
    cmds.select(clear=True) #clear selection
 
logfile.close() 
print "ayy"

cmds.select(all=True)
cmds.file("C:\Users\dook\Documents\.Uni\Games Prog A2\FIT2096 - Week 7\Assets\Meshes\level.obj",pr=1,typ="OBJexport",es=1,op="groups=0; ptgroups=0; materials=0; smoothing=0; normals=0")
