rem ---- DONT RUN THIS as a GIT USER, No need --------------------------------
rem ---- Set up all paths in the project as relative paths - Saab Dev Only ---

rem ------------- GizmoSDK dependencies --------------------------------------

if not defined GIZMOSDK (
  set GIZMOSDK=..\GizmoSDK
)

rem ----- Link in Gizmo Examples -------------------------------------------------

rmdir /S /Q GizmoBase
rmdir /S /Q GizmoDistribution
rmdir /S /Q Gizmo3D
rmdir /S /Q Coordinate

mklink /J GizmoBase %GIZMOSDK%\GizmoBase\examples
mklink /J GizmoDistribution %GIZMOSDK%\GizmoDistribution\examples
mklink /J Gizmo3D %GIZMOSDK%\Gizmo3D\examples
mklink /J Coordinate %GIZMOSDK%\plugins\gzCoordinate\examples

rem ------------- End of GizmoSDK dependencies -------------------

exit

