This is a sample MFC integration of the GizmoSDK libraries.

The main issue is the memory management. MFC can use its own memory manager, debugging etc that is not compatible with gizmo
as gizmo tries to provide the same features.

The GizmoView.h file shows how to create a isolation between the two memory systems.

The file Gizmoview.cpp is a pure gizmo application environment wich integrates with the MFC enviroment.

