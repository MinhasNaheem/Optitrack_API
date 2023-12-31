#!/usr/bin/env python
import vtk

# Test conversion of cells to polyhedra.
# Create an unstructured grid containing a vtkPolyhedron, a 3D hex cell, and
# a 2D quad cell.
grid = vtk.vtkUnstructuredGrid()

# Insert points
pts = vtk.vtkPoints()
pts.SetNumberOfPoints(12)

# Hex points
pts.SetPoint(0, 0,0,0)
pts.SetPoint(1, 1,0,0)
pts.SetPoint(2, 1,1,0)
pts.SetPoint(3, 0,1,0)
pts.SetPoint(4, 0,0,1)
pts.SetPoint(5, 1,0,1)
pts.SetPoint(6, 1,1,1)
pts.SetPoint(7, 0,1,1)

# Quad points
pts.SetPoint(8, 4,0,0.5)
pts.SetPoint(9, 5,0,0.5)
pts.SetPoint(10, 5,1,0.5)
pts.SetPoint(11, 4,1,0.5)

grid.SetPoints(pts)

# Insert cells: hex, quad
cellPts = [0,1,2,3,4,5,6,7]
grid.InsertNextCell(12,8,cellPts) # Insert hex

cellPts = [8,9,10,11]
grid.InsertNextCell(9,4,cellPts) # Insert quad

# Create some cell scalars
cellScalars = vtk.vtkFloatArray()
cellScalars.SetNumberOfTuples(2)
cellScalars.SetTuple1(0,0)
cellScalars.SetTuple1(1,2)

grid.GetCellData().SetScalars(cellScalars)

# Convert mesh to polyhedra
convert = vtk.vtkConvertToPolyhedra()
convert.SetInputData(grid)
convert.OutputAllCellsOn()
convert.Update()

# Make sure conversion also handles polyhedra
convert2 = vtk.vtkConvertToPolyhedra()
convert2.SetInputConnection(convert.GetOutputPort())
convert2.OutputAllCellsOn()
convert2.Update()

# Make sure conversion only passes convertable types
convert3 = vtk.vtkConvertToPolyhedra()
convert3.SetInputConnection(convert.GetOutputPort())
convert3.OutputAllCellsOff()
convert3.Update()

# Graphics stuff
# Create the RenderWindow, Renderers and both Actors
ren0 = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.SetMultiSamples(0)
renWin.AddRenderer(ren0)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

mapper1 = vtk.vtkDataSetMapper()
mapper1.SetInputConnection(convert2.GetOutputPort())
mapper1.SetScalarRange(0,2)

actor1 = vtk.vtkActor()
actor1.SetMapper(mapper1)
actor1.GetProperty().SetInterpolationToFlat()

mapper2 = vtk.vtkDataSetMapper()
mapper2.SetInputConnection(convert3.GetOutputPort())
mapper2.SetScalarRange(0,2)

actor2 = vtk.vtkActor()
actor2.SetMapper(mapper2)
actor2.GetProperty().SetInterpolationToFlat()
actor2.AddPosition(2,0,0)

# Add the actors to the renderer, set the background and size
ren0.AddActor(actor1)
ren0.AddActor(actor2)

ren0.SetBackground(0,0,0)
renWin.SetSize(300,100)
camera = ren0.GetActiveCamera()
camera.SetPosition(2.5,0.5,6)
camera.SetFocalPoint(2.5,0.5,0)
iren.Initialize()

renWin.Render()
iren.Start()
# --- end of script --
