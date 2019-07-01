# igneous

[![GitHub tag](https://img.shields.io/github/tag/MissingBitStudios/igneous.svg)](https://github.com/MissingBitStudios/igneous/releases)
[![Build Status](https://travis-ci.org/MissingBitStudios/igneous.svg?branch=master)](https://travis-ci.org/MissingBitStudios/igneous)
[![GitHub license](https://img.shields.io/badge/license-CC%20BY%204.0-green.svg)](https://github.com/MissingBitStudios/igneous/blob/master/LICENSE.md)
[![Join the chat at https://discord.gg/CCUwTar](https://img.shields.io/badge/chat-on%20discord-mediumpurple.svg)](https://discord.gg/CCUwTar)
[![Join the chat at https://gitter.im/MissingBitStudios/igneous](https://badges.gitter.im/MissingBitStudios/igneous.svg)](https://gitter.im/MissingBitStudios/igneous?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Igneous is an open source game engine written in C++.

![Sponza]()

![Barn]()

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

* CMake
* OpenAL Redistributable
* OpenAL SDK

### Environment Variables

The install directory for the OpenAL SDK should be added as an environment variable in your system. This is not necessary, but is a safeguard in case CMake cannot find the install directory in one of the default search paths.

The OpenAL variable should be named `OPENALDIR`.

### Checkout

```sh
git clone https://github.com/MissingBitStudios/igneous.git
cd igneous
git submodule update --init --recursive
```

### Build

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

### Update

```sh
git pull
git submodule sync
git submodule update --init --recursive
```

## Built With

* [Assimp](https://github.com/assimp/assimp) - [Website](http://assimp.sourceforge.net/)
* [bgfx](https://github.com/bkaradzic/bgfx)
* [bigg](https://github.com/JoshuaBrookover/bigg)
* [Bullet Physics](https://github.com/bulletphysics/bullet3) - [Website](http://bulletphysics.org/)
* [EnTT](https://github.com/skypjack/entt)
* [GLFW](https://github.com/glfw/glfw) - [Website](http://www.glfw.org/)
* [GLM](https://github.com/g-truc/glm) - [Website](https://glm.g-truc.net/)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [OpenAL](https://www.openal.org/)
* [RakNet](https://github.com/facebookarchive/RakNet) - [Website](http://www.raknet.net/)
* [spdlog](https://github.com/gabime/spdlog)
* [stb](https://github.com/nothings/stb)

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Changelog

All notable changes to this project will be documented in [CHANGELOG.md](CHANGELOG.md). The changelog format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) and adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## Versioning

We use [SemVer](http://semver.org/spec/v2.0.0.html) for versioning. For the versions available, see the [releases page](https://github.com/MissingBitStudios/igneous/releases). 

## Authors

* **ProWolf** - *Initial work, Rendering, Scripting, Console, Audio, Events* - [ProWolf](https://github.com/prowolf)
* **pcguy123** - *ECS, Documentation* - [pcguy123](https://github.com/pcguy123)

See also the list of [contributors](https://github.com/MissingBitStudios/igneous/graphs/contributors) who participated in this project.

## License

This project is licensed under the Creative Commons Attribution 4.0 International Public License (CC BY 4.0) - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgements

* Thank you to the authors of the dependencies used.
* Thank you to the amazing teachers on YouTube: [ThinMatrix](https://www.youtube.com/user/ThinMatrix), [TheChernoProject](https://www.youtube.com/user/TheChernoProject), [thebennybox](https://www.youtube.com/user/thebennybox), [Code Rad](https://www.youtube.com/user/YouKondziu), and [Derek Banas](https://www.youtube.com/user/derekbanas).
