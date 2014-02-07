Project Effects Everywhere
==========================

Creating a showcase game to showcase our particle system

Setting up Irrlicht
-------------------

- Add the Irrlicht sdk in <repo>/Irrlicht. So there is at least the bin,
  include, source and lib.

- Open EffectsEverywhere visual studio project and check if Irrlicht is in the solution. If it isn't
  you have to add it doing the following: Solution (right mouse) -> add -> existing solution.
  Browse to <repo>/Irrlicht/source/Irrlicht and select Irrlicht 10.0

- Check if EffectsEverywhere project has a reference to the Irrlicht project. By clicking right mouse
  on the EffectsEverywhere project in the solution -> Properties -> Common Properties -> Framework and
  References. Check if Irrlicht is a reference, otherwise add it with "Add New Reference..."

- Check if the include is added in the EffectsEverywhere project properties -> C/C++ -> General ->
  Additional Include Directories. Add ../Irrlicht/include; if it isn't there already.

- Go to <repo>/Irrlicht/bin/Win32-VisualStudio/ and copy the Irrlicht.dll to
  <repo>/Debug if the Irrlicht engine is build in debug mode or in
  <repo>/Release if it is build in release mode.

- EffectsEverywhere project is now able to run!
