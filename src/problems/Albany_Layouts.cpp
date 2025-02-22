// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.
#include "Albany_Layouts.hpp"

#include "PHAL_Dimension.hpp"
#include "Phalanx_DataLayout_MDALayout.hpp"

/*********************** Helper Functions*********************************/

Albany::Layouts::Layouts(
    int worksetSize,
    int numVertices,
    int numNodes,
    int numQPts,
    int numCellDim,
    int vecDim,
    int numFace)
// numCellDim is the number of spatial dimensions
// vecDim is the length of a vector quantity
// -- For many problems, numCellDim is used for both since there are
// typically numCellDim displacements and velocities.
{
  using PHX::MDALayout;
  using Teuchos::rcp;

  if (vecDim == -1) vecDim = numCellDim;
  if (vecDim == numCellDim) {
    vectorAndGradientLayoutsAreEquivalent = true;
  } else {
    vectorAndGradientLayoutsAreEquivalent = false;
  }

  isSideLayouts = false;

  // Solution Fields
  node_scalar  = rcp(new MDALayout<Cell, Node>(worksetSize, numNodes));
  qp_scalar    = rcp(new MDALayout<Cell, QuadPoint>(worksetSize, numQPts));
  cell_scalar  = rcp(new MDALayout<Cell, QuadPoint>(worksetSize, 1));
  cell_scalar2 = rcp(new MDALayout<Cell>(worksetSize));
  face_scalar  = rcp(new MDALayout<Cell, Side>(worksetSize, numFace));

  node_vector = rcp(new MDALayout<Cell, Node, Dim>(worksetSize, numNodes, vecDim));
  qp_vector   = rcp(new MDALayout<Cell, QuadPoint, Dim>(worksetSize, numQPts, vecDim));
  cell_vector = rcp(new MDALayout<Cell, Dim>(worksetSize, vecDim));
  face_vector = rcp(new MDALayout<Cell, Side, Dim>(worksetSize, numFace, vecDim));

  node_gradient      = rcp(new MDALayout<Cell, Node, Dim>(worksetSize, numNodes, numCellDim));
  qp_gradient        = rcp(new MDALayout<Cell, QuadPoint, Dim>(worksetSize, numQPts, numCellDim));
  cell_gradient      = rcp(new MDALayout<Cell, Dim>(worksetSize, numCellDim));
  face_gradient      = rcp(new MDALayout<Cell, Side, Dim>(worksetSize, numFace, numCellDim));
  qp_vector_spacedim = qp_gradient;

  node_tensor = rcp(new MDALayout<Cell, Node, Dim, Dim>(worksetSize, numNodes, numCellDim, numCellDim));
  qp_tensor   = rcp(new MDALayout<Cell, QuadPoint, Dim, Dim>(worksetSize, numQPts, numCellDim, numCellDim));
  cell_tensor = rcp(new MDALayout<Cell, Dim, Dim>(worksetSize, numCellDim, numCellDim));
  face_tensor = rcp(new MDALayout<Cell, Side, Dim, Dim>(worksetSize, numFace, numCellDim, numCellDim));

  node_vecgradient = rcp(new MDALayout<Cell, Node, Dim, Dim>(worksetSize, numNodes, vecDim, numCellDim));
  qp_vecgradient   = rcp(new MDALayout<Cell, QuadPoint, Dim, Dim>(worksetSize, numQPts, vecDim, numCellDim));
  cell_vecgradient = rcp(new MDALayout<Cell, Dim, Dim>(worksetSize, vecDim, numCellDim));
  face_vecgradient = rcp(new MDALayout<Cell, Side, Dim, Dim>(worksetSize, numFace, vecDim, numCellDim));

  qp_tensorgradient =
      rcp(new MDALayout<Cell, QuadPoint, Dim, Dim, Dim>(worksetSize, numQPts, vecDim, vecDim, numCellDim));

  node_tensor3 =
      rcp(new MDALayout<Cell, Node, Dim, Dim, Dim>(worksetSize, numNodes, numCellDim, numCellDim, numCellDim));
  qp_tensor3 =
      rcp(new MDALayout<Cell, QuadPoint, Dim, Dim, Dim>(worksetSize, numQPts, numCellDim, numCellDim, numCellDim));
  cell_tensor3 = rcp(new MDALayout<Cell, Dim, Dim, Dim>(worksetSize, numCellDim, numCellDim, numCellDim));
  face_tensor3 =
      rcp(new MDALayout<Cell, Side, Dim, Dim, Dim>(worksetSize, numFace, numCellDim, numCellDim, numCellDim));

  node_tensor4 = rcp(new MDALayout<Cell, Node, Dim, Dim, Dim, Dim>(
      worksetSize, numNodes, numCellDim, numCellDim, numCellDim, numCellDim));
  qp_tensor4   = rcp(new MDALayout<Cell, QuadPoint, Dim, Dim, Dim, Dim>(
      worksetSize, numQPts, numCellDim, numCellDim, numCellDim, numCellDim));
  cell_tensor4 =
      rcp(new MDALayout<Cell, Dim, Dim, Dim, Dim>(worksetSize, numCellDim, numCellDim, numCellDim, numCellDim));
  face_tensor4 = rcp(new MDALayout<Cell, Side, Dim, Dim, Dim, Dim>(
      worksetSize, numFace, numCellDim, numCellDim, numCellDim, numCellDim));

  node_node_scalar = rcp(new MDALayout<Node, Dim>(worksetSize, 1));
  node_node_vector = rcp(new MDALayout<Node, Dim>(worksetSize, vecDim));
  node_node_tensor = rcp(new MDALayout<Node, Dim, Dim>(worksetSize, numCellDim, numCellDim));

  // Coordinates:  3vector is for shells 2D topology 3 coordinates
  vertices_vector = rcp(new MDALayout<Cell, Vertex, Dim>(worksetSize, numVertices, numCellDim));
  node_3vector    = rcp(new MDALayout<Cell, Node, Dim>(worksetSize, numNodes, 3));
  qp_coords       = rcp(new MDALayout<Cell, QuadPoint, Dim>(worksetSize, numQPts, numCellDim));

  // Basis Functions
  node_qp_scalar   = rcp(new MDALayout<Cell, Node, QuadPoint>(worksetSize, numNodes, numQPts));
  node_qp_gradient = rcp(new MDALayout<Cell, Node, QuadPoint, Dim>(worksetSize, numNodes, numQPts, numCellDim));
  node_qp_vector   = node_qp_gradient;

  workset_scalar      = rcp(new MDALayout<Dummy>(1));
  workset_vector      = rcp(new MDALayout<Dim>(vecDim));
  workset_gradient    = rcp(new MDALayout<Dim>(numCellDim));
  workset_tensor      = rcp(new MDALayout<Dim, Dim>(numCellDim, numCellDim));
  workset_vecgradient = rcp(new MDALayout<Dim, Dim>(vecDim, numCellDim));

  shared_param     = rcp(new MDALayout<Dim>(1));
  shared_param_vec = rcp(new MDALayout<Dim>(vecDim));
  dummy            = rcp(new MDALayout<Dummy>(0));

  // NOTE: vector data layouts here are used both for vectors fields
  // as well as gradients of scalar fields. This only works when
  // the vector fields are length numCellDim (which tends to be true
  // for displacements and velocities). Could be separated to
  // have separate node_vector and node_gradient layouts.
}

Albany::Layouts::Layouts(
    int worksetSize,
    int numVertices,
    int numNodes,
    int numQPts,
    int numSideDim,
    int numSpaceDim,
    int numSides,
    int vecDim)
// numSideDim is the number of spatial dimensions
// vecDim is the length of a vector quantity
// -- For many problems, numSideDim is used for both since there are
// typically numSideDim displacements and velocities.
{
  using PHX::MDALayout;
  using Teuchos::rcp;

  if (vecDim == -1) vecDim = numSideDim;
  if (vecDim == numSideDim) {
    vectorAndGradientLayoutsAreEquivalent = true;
  } else {
    vectorAndGradientLayoutsAreEquivalent = false;
  }

  isSideLayouts = true;

  // Solution Fields
  node_scalar  = rcp(new MDALayout<Cell, Side, Node>(worksetSize, numSides, numNodes));
  qp_scalar    = rcp(new MDALayout<Cell, Side, QuadPoint>(worksetSize, numSides, numQPts));
  cell_scalar  = rcp(new MDALayout<Cell, Side, QuadPoint>(worksetSize, numSides, 1));
  cell_scalar2 = rcp(new MDALayout<Cell, Side>(worksetSize, numSides));

  node_vector = rcp(new MDALayout<Cell, Side, Node, Dim>(worksetSize, numSides, numNodes, vecDim));
  qp_vector   = rcp(new MDALayout<Cell, Side, QuadPoint, Dim>(worksetSize, numSides, numQPts, vecDim));
  cell_vector = rcp(new MDALayout<Cell, Side, Dim>(worksetSize, numSides, vecDim));

  node_gradient = rcp(new MDALayout<Cell, Side, Node, Dim>(worksetSize, numSides, numNodes, numSideDim));
  qp_gradient   = rcp(new MDALayout<Cell, Side, QuadPoint, Dim>(worksetSize, numSides, numQPts, numSideDim));
  cell_gradient = rcp(new MDALayout<Cell, Side, Dim>(worksetSize, numSides, numSideDim));

  qp_vector_spacedim = rcp(new MDALayout<Cell, Side, QuadPoint, Dim>(worksetSize, numSides, numQPts, numSpaceDim));

  node_tensor = rcp(new MDALayout<Cell, Side, Node, Dim, Dim>(worksetSize, numSides, numNodes, numSideDim, numSideDim));
  qp_tensor =
      rcp(new MDALayout<Cell, Side, QuadPoint, Dim, Dim>(worksetSize, numSides, numQPts, numSideDim, numSideDim));
  cell_tensor = rcp(new MDALayout<Cell, Side, Dim, Dim>(worksetSize, numSides, numSideDim, numSideDim));
  qp_tensor_cd_sd =
      rcp(new MDALayout<Cell, Side, QuadPoint, Dim, Dim>(worksetSize, numSides, numQPts, numSideDim + 1, numSideDim));

  node_vecgradient =
      rcp(new MDALayout<Cell, Side, Node, Dim, Dim>(worksetSize, numSides, numNodes, vecDim, numSideDim));
  qp_vecgradient =
      rcp(new MDALayout<Cell, Side, QuadPoint, Dim, Dim>(worksetSize, numSides, numQPts, vecDim, numSideDim));
  cell_vecgradient = rcp(new MDALayout<Cell, Side, Dim, Dim>(worksetSize, numSides, vecDim, numSideDim));

  qp_tensorgradient = rcp(
      new MDALayout<Cell, Side, QuadPoint, Dim, Dim, Dim>(worksetSize, numSides, numQPts, vecDim, vecDim, numSideDim));

  node_tensor3 = rcp(new MDALayout<Cell, Side, Node, Dim, Dim, Dim>(
      worksetSize, numSides, numNodes, numSideDim, numSideDim, numSideDim));
  qp_tensor3   = rcp(new MDALayout<Cell, Side, QuadPoint, Dim, Dim, Dim>(
      worksetSize, numSides, numQPts, numSideDim, numSideDim, numSideDim));
  cell_tensor3 =
      rcp(new MDALayout<Cell, Side, Dim, Dim, Dim>(worksetSize, numSides, numSideDim, numSideDim, numSideDim));

  node_tensor4 = rcp(new MDALayout<Cell, Side, Node, Dim, Dim, Dim, Dim>(
      worksetSize, numSides, numNodes, numSideDim, numSideDim, numSideDim, numSideDim));
  qp_tensor4   = rcp(new MDALayout<Cell, Side, QuadPoint, Dim, Dim, Dim, Dim>(
      worksetSize, numSides, numQPts, numSideDim, numSideDim, numSideDim, numSideDim));
  cell_tensor4 = rcp(new MDALayout<Cell, Side, Dim, Dim, Dim, Dim>(
      worksetSize, numSides, numSideDim, numSideDim, numSideDim, numSideDim));

  node_node_scalar = rcp(new MDALayout<Node, Side, Dim>(worksetSize, numSides, 1));
  node_node_vector = rcp(new MDALayout<Node, Side, Dim>(worksetSize, numSides, vecDim));
  node_node_tensor = rcp(new MDALayout<Node, Side, Dim, Dim>(worksetSize, numSides, numSideDim, numSideDim));

  // Coordinates: 3vector is for shells 2D topology 3 coordinates
  // Note: vertices coordinates always have the dimension of the ambient space.
  // In fact, you need full n-dim coordinates
  //       to build the manifold metric and other structures.
  // WARNING: if you change the above fact, make sure to make the appropriate
  // corrections to the parts of
  //          the library that rely on it!
  vertices_vector = rcp(new MDALayout<Cell, Side, Vertex, Dim>(worksetSize, numSides, numVertices, numSpaceDim));
  node_3vector    = rcp(new MDALayout<Cell, Side, Node, Dim>(worksetSize, numSides, numNodes, 3));
  qp_coords       = rcp(new MDALayout<Cell, Side, QuadPoint, Dim>(worksetSize, numSides, numQPts, numSpaceDim));

  // Basis Functions
  node_qp_scalar = rcp(new MDALayout<Cell, Side, Node, QuadPoint>(worksetSize, numSides, numNodes, numQPts));
  node_qp_gradient =
      rcp(new MDALayout<Cell, Side, Node, QuadPoint, Dim>(worksetSize, numSides, numNodes, numQPts, numSideDim));
  node_qp_vector =
      rcp(new MDALayout<Cell, Side, Node, QuadPoint, Dim>(worksetSize, numSides, numNodes, numQPts, vecDim));

  workset_scalar      = rcp(new MDALayout<Dummy>(1));
  workset_vector      = rcp(new MDALayout<Dim>(vecDim));
  workset_gradient    = rcp(new MDALayout<Dim>(numSideDim));
  workset_tensor      = rcp(new MDALayout<Dim, Dim>(numSideDim, numSideDim));
  workset_vecgradient = rcp(new MDALayout<Dim, Dim>(vecDim, numSideDim));

  shared_param     = rcp(new MDALayout<Dim>(1));
  shared_param_vec = rcp(new MDALayout<Dim>(vecDim));
  dummy            = rcp(new MDALayout<Dummy>(0));

  // NOTE: vector data layouts here are used both for vectors fields
  // as well as gradients of scalar fields. This only works when
  // the vector fields are length numSideDim (which tends to be true
  // for displacements and velocities). Could be separated to
  // have separate node_vector and node_gradient layouts.
}
