%module(directors="1") univermecext
 // STL
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
 // core
%include "mrealadapt.i"
%include "intervaladapt.i"
%include "mtl.i"
 // functions
%include "ifunction.i"
%include "functionparser.i"
%include "enclosures.i"
%include "contractors.i"
 // objects
%include "igeoobj.i"
%include "icsgnode.i"
%include "icsgnodefactory.i"
%include "implsurface.i"
%include "csgfactories.i"
 // decomp
%include "itree.i" 
%include "iivtree.i"
%include "trees.i"
%include "decomputil.i"
 // algorithm
%include "idistobserver.i"
%include "epsdistance.i"
 // IVP
 // %include "ivp.i"
 // UI
%include "scenegraph.i"
