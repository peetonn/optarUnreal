# Optar Unreal Plugin Unreal Project 

This is a sample project for handheld AR (built originally for Android) which demonstrates use of Optar UnrealEngine plugin.
Optar plugin uses [Optar C++ library](https://github.com/peetonn/optarLibrary), which must be built firsthand.

## Project set up

* clone the repo
```
git clone https://github.com/peetonn/optarUnreal --recursive
cd optarUnreal
```

* create `ThirdParty` folder in `Plugins/optar/Source`
```
mkdir -p Plugins/optar/Source/ThirdParty
```

* clone (or symlink if already cloned) `optarLibrary` into created folder:
```
git clone https://github.com/peetonn/optarLibrary Plugins/optar/Source/ThirdParty/optarLibrary
```

* [build](https://github.com/peetonn/optarLibrary/blob/master/README.md) optarLibrary

* open UnrealEngine project `arcpp1.uproject` and compile
