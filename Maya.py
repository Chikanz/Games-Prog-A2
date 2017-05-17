import maya.cmds as cmds
import sys

def WriteFile(filename, search, enemy):
    logfile = open(filename, 'w+')
    #'pCube*'
    levelList = cmds.ls(search)
    print levelList
    for x in range (0,len(levelList)-1):
        ln=""
        cmds.select(levelList[x])
        try:
            if enemy == False:
                cmds.polyTriangulate(); #Triangulate mesh for engine
        except:
            print sys.exc_info()[0] #sometimes things don't work
            
        try:
            if enemy == False:
                ln = str(cmds.xform(q=1,bb=1,ws=1)) #query bounds in world space
                ln = ln[1:-1] #format
            else:
                ln = str(cmds.xform(q=1,rp=1, ws=1)) #query pivot in world space
                ln = ln[1:-1]
                rot = cmds.xform(q=1,ro=1) #query pivot in world space
                ln += ", " + str(rot[1])
                ln += ", " + levelList[x][5]
                
            ln += ", \n"
            logfile.write(ln)
            print ln #for debug
        except:
            print sys.exc_info()[0] #sometimes things don't work
        cmds.select(clear=True) #clear selection
    logfile.close() 
    print search + " file written!"

f = 'C:/Users/dook/Documents/.Uni/Games Prog A2/FIT2096 - Week 7/Assets/LevelBounds.Zac'
WriteFile(f,'pCube*',False)

f = 'C:/Users/dook/Documents/.Uni/Games Prog A2/FIT2096 - Week 7/Assets/Enemies.Zac'
WriteFile(f,'enemy*',True)

cmds.select(all=True)
deselect = cmds.ls('enemy*')#deselct colliders
for x in range (0,len(deselect)-1):
    cmds.select(deselect[x], d=True)

#cmds.file("C:\Users\dook\Documents\.Uni\Games Prog A2\FIT2096 - Week 7\Assets\Meshes\level.obj",pr=1,typ="OBJexport",es=1,op="groups=1; ptgroups=1; materials=1; smoothing=1; normals=1")
