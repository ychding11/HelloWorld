#!/usr/bin/python

import sys
from ctypes import *
import polyhedra, blob

# Import OpenCTM python bindings
from openctm import *
from platform_open import *

###############################

filename = "sphere.ctm"
num_subdivisions = int(sys.argv[1])
print "- Subdiv level: ", num_subdivisions

verts, faces = polyhedra.icosahedron()
for x in xrange(num_subdivisions):
    verts, faces = polyhedra.subdivide(verts, faces)

pVerts = blob.make_blob(verts, c_float)
pFaces = blob.make_blob(faces, c_uint)
pNormals = POINTER(c_float)()

ctm = ctmNewContext(CTM_EXPORT)
ctmDefineMesh(ctm, pVerts, len(verts), pFaces, len(faces), pNormals)
ctmSave(ctm, filename)
ctmFreeContext(ctm)

# Open ctm file
platform_open(filename)
