# Info
- this is a fork of https://github.com/free-audio/clap-plugins
- with simplified build steps for linux based systems
  - no git submodules
  - no downloads in cmake files
  - less bloat
  
# Overview
- [Example Clap Plugins](#example-clap-plugins)
  - [Notes on GUI, static build vs dynamic build and symbols](#notes-on-gui-static-build-vs-dynamic-build-and-symbols)
- [Building on Linux](#building-on-linux)

# Example Clap Plugins

This repo serves as an example to demonstrate how to create a CLAP plugin.

<br/><a href="https://repology.org/project/clap-plugins/versions" target="_blank" rel="noopener" title="Packaging status"><img src="https://repology.org/badge/vertical-allrepos/clap-plugins.svg"></a>

## Notes on GUI, static build vs dynamic build and symbols

The plugins use Qt for the GUI.

It is fine to dynamically link to Qt for a host, but it is very dangerous for a plugin.

Also one very important aspect of the plugin is the distribution.
Ideally a clap plugin should be self contained: it should not rely upon symbols from the host,
and it should export only one symbol: `clap_entry`.

You should be aware that even if you hide all your symbols some may still remain visible
at unexpected places. Objective-C seems to register every classes including those coming from
plugins in a **flat namespace**. Which means that if two plugins define two different
Objective-C classes but with the same, they will clash which will result in undeflined behavior.

Qt uses a few Objective-C classes on macOS. So it is crucial to use `QT_NAMESPACE`.

We abstracted the relation between the plugin and the GUI:
[`AbstractGui`](plugins/gui/abstract-gui.hh) and [`AbstractGuiListener`](plugins/gui/abstract-gui-listener.hh)
which lets us transparently insert proxies.

The GUI itself work with proxy objects to the parameters, transport info, ...
They are then bound into QML objects.
See [`Knob.qml`](plugins/gui/qml/clap/Knob.qml) and [`parameter-proxy.hh`](plugins/gui/parameter-proxy.hh).

# Building on Linux
## Build with GUI
```bash
git clone https://github.com/majorx234/clap-plugins-linux
cd clap-plugins-linux
mkdir build
cd build
cmake ..
make
```

# TODO
- simplified headless version
