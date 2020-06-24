# (M)odular (P)ixelated (N)eutron (D)etector Geant4 Simulation #
# Introduction #
Adapted from micahfolsom.

# Dependencies #
Geant4, ROOT

# Available Geometries #
1. **singlepixel**: a single scintillator pixel
  * Default: 2x2x2 mm
2. **pixelarray**: an array of pixels
  * Default: 8x8 array of 2x2x2mm pixels<br>

# UI Commands #
## Geometry Commands ##
Geometry-related commands defined in the
[GeometryMessenger](@ref MPND::GeometryMessenger)<br>
Command directory: /mpnd/geometry/<br>

**General Commands**<br>
`/mpnd/geometry/reload`<br>
*Arguments*: none<br>
*Description*: clears out all geometric objects and rebuilds them based
on the current parameters. *Must* run after changing a physical detector
parameter.<br>

`/mpnd/geometry/config`<br>
*Arguments*: singlepixel pixelarray<br>
*Description*: switch between detector geometries.<br>
*Default*: singlepixel<br>

`/mpnd/geometry/singlepixel/dimensions`<br>
*Arguments*: [x] [y] [z] [units]<br>
*Description*: set the pixel size.<br>
*Default*: (2, 2, 2) mm<br>

`/mpnd/geometry/singlepixel/material`<br>
*Arguments*: [material name]<br>
*Description*: set the scintillator material.<br>
*Default*: ej200<br>

`/mpnd/geometry/pixelarray/dimensions`<br>
*Arguments*: [x] [y] [z] [units]<br>
*Description*: set the pixel size.<br>
*Default*: (2, 2, 2) mm<br>

`/mpnd/geometry/pixelarray/nx`<br>
*Arguments*: [nx]<br>
*Description*: set the number of X pixels in the array.<br>
*Default*: 8<br>

`/mpnd/geometry/pixelarray/ny`<br>
*Arguments*: [ny]<br>
*Description*: set the number of Y pixels in the array.<br>
*Default*: 8<br>

## Gun Commands ##
This code uses the built-in Geant4 General Particle Source
(G4GeneralParticleSource). I am not aware of a good tutorial for GPS, but
if you look around online you can find various examples. It also helps
to look at the source code: https://geant4.kek.jp/Reference/10.06.p01/classG4GeneralParticleSource.html<br>

`/gps/particle` <br>
*Arguments*: gamma neutron opticalphoton e- proton alpha <br>
*Description*: defines the particle type using Geant4 naming convention <br>
*Default*: geantino <br>

`/gps/pos/type/`<br>
*Arguments*: Point Plane Beam Surface Volume <br>
*Description*: set the source positional distribution type <br>

`/gps/pos/shape` <br>
*Arguments*: Circle Annulus Ellipse Square Rectangle for a Plane] <br>
Sphere Ellipsoid Cylinder, Para (parallelpiped) [For Surface or Volume] <br>
*Descriptions*: sets the source shape type after `gps/pos/type`. <br>

`/gps/direction` <br>
*Arguments*:  [Px] [Py] [Pz] <br>
*Description*: set the momentum direction of generated particles using direction cosines. <br>
*Default*: (1, 0, 0) <br>

`/gps/position` <br>
*Arguments*: [x] [y] [z] [units] <br>
*Description*: sets the center coordinates (x,y,z) of the source.<br>
*Default*: (0, 0, 0) cm <br>

`/gps/pos/halfx` <br>
*Arguments*:  <br>
*Description*: sets the half-length in x of the source for Square/Rectangle shape. <br>
*Default*: 0 cm

`/gps/pos/halfy` <br>
*Arguments*:  <br>
*Description*: sets the half-length in y of the source Square/Rectangle shape. <br>
*Default*: 0 cm

`/gps/ang/type` <br>
*Arguments*:  iso cos planar beam1d beam2d focused user<br>
*Description*: sets the angular distribution type. <br>
*Default*: iso

`/gps/ene/type` <br>
*Arguments*:  Mono Lin Pow Exp Gauss Brem Bbody <br>
*Description*: sets the energy distribution type. <br>
*Default*: Mono (mono-energetic)

`/gps/ene/min` <br>
*Arguments*:  [Emin] [unit] <br>
*Description*: sets the minimum for the energy distribution<br>
*Default*: 0 keV <br>

`/gps/ene/max` <br>
*Arguments*:  [Emax] [unit] <br>
*Description*: sets the maximum for the energy distribution<br>
*Default*: 0 keV <br>

`/gps/ene/alpha` <br>
*Arguments*:  [alpha] <br>
*Description*: sets the exponent alpha for a power-law distribution. <br>
*Default*: 0 <br>

## Tracking/Filter Commands ##
Commands to modify the different filters applied to the sensitive
detectors. This controls the interaction information that is written to
disk. Defined in [SDMessenger](@ref MPND::SDMessenger)<br>
Command directory: /mpnd/sd/<br>

`/mpnd/sd/select_sd`<br>
*Arguments*: [sd\_name]<br>
*Description*: change current selection to specified SD.<br>
*Default*: scintillator\_sd<br>

`/mpnd/sd/particle/add`<br>
*Arguments*: [particle name]<br>
*Description*: add a particle to the filter for the current SD.<br>

`/mpnd/sd/particle/remove`<br>
*Arguments*: [particle name]<br>
*Description*: remove a particle from the filter for the current SD.<br>

`/mpnd/sd/particle/clear`<br>
*Description*: clear the particle filter for the current SD.<br>

`/mpnd/sd/particle/print`<br>
*Description*: print the particle filter for the current SD.<br>

`/mpnd/sd/energy/add`<br>
*Arguments*: [min, max)<br>
*Description*: add an energy window to the filter for the current SD.<br>

`/mpnd/sd/energy/remove`<br>
*Arguments*: [index]<br>
*Description*: remove an energy window from the filter for the current SD.
<br>

`/mpnd/sd/energy/clear`<br>
*Description*: clear the energy filter for the current SD.<br>

`/mpnd/sd/energy/print`<br>
*Description*: print the energy filter for the current SD.<br>

## Run Commands ##
General run (file path, etc) commands defined in the
[RunMessenger](@ref MPND::RunMessenger)<br>
Command directory: /mpnd/run/<br>

`/mpnd/run/path`<br>
*Arguments*: [path/to/file.root]<br>
*Description*: specify the save file path for the simulation data.<br>
*Default*: sim.root<br>

`/mpnd/run/write_data`<br>
*Arguments*: true false<br>
*Description*: toggle writing simulation data to disk.<br>
*Default*: true<br>

`/mpnd/run/buffer_limit`<br>
*Arguments*: [number of events] \(> 0\)<br>
*Description*: number of events to hold before emptying the event buffer.
The FileManager will block as it empties the queue so if you are doing a
short run, this should be set larger than the number of events being run,
and if a long run, something reasonable (ie. 1000), depending on how long
you expect each write to take.<br>
*Default*: 1000<br>

`/mpnd/run/max_file_size`<br>
*Arguments*: [GB] \(> 0\)<br>
*Description*: set the maximum file size allowed before automatic file
splitting by ROOT. Calls TTree::SetMaxTreeSize().<br>
*Default*: 100 GB<br>

# Other Notes #
1. Do not \#include G4SystemOfUnits.hh in header (.hpp) files! You must
only \#include it from .cpp files. Doing it in the headers can cause name
conflicts with ROOT which will break things. If you see complaints about
a shadow declaration of variable `s` in ROOT headers, this is why.<br>
