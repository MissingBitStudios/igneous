# igneous

[![GitHub tag](https://img.shields.io/github/tag/MissingBitStudios/igneous.svg)](https://github.com/MissingBitStudios/igneous/releases)
[![GitHub stars](https://img.shields.io/github/stars/MissingBitStudios/igneous.svg)](https://github.com/MissingBitStudios/igneous/stargazers)
![GitHub repo size in bytes](https://img.shields.io/github/repo-size/MissingBitStudios/igneous.svg)
[![GitHub license](https://img.shields.io/badge/license-CC%20BY--NC--SA-green.svg)](https://github.com/MissingBitStudios/igneous/blob/master/LICENSE.md)
[![Join the chat at https://discord.gg/CCUwTar](https://img.shields.io/badge/chat-on%20discord-mediumpurple.svg)](https://discord.gg/CCUwTar)
[![Join the chat at https://gitter.im/MissingBitStudios/igneous](https://badges.gitter.im/MissingBitStudios/igneous.svg)](https://gitter.im/MissingBitStudios/igneous?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Igneous is an open source game engine written in C++.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

* CMake
* Mono
* OpenAL SDK

### Environment Variables

The install directories for the OpenAL SDK and for Mono should be added as environment variables in your system. This is not necessary, but is a safeguard in the case that your system cannot find the install directories in one of the default search paths.

The OpenAL variable should be named 'OPENALDIR' exactly as written here without the quotes. The Mono variable should be named 'MONO_ROOT' exactly as written here without the quotes.

### Checkout

```
git clone https://github.com/MissingBitStudios/igneous.git
cd igneous
git submodule update --init --recursive
mkdir build
cd build
cmake ..
```

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Changelog

All notable changes to this project will be documented in [CHANGELOG.md](CHANGELOG.md). The changelog format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) and adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## Versioning

We use [SemVer](http://semver.org/spec/v2.0.0.html) for versioning. For the versions available, see the [releases page](https://github.com/prowolf/Igneous/releases). 

## Authors

* **ProWolf** - *Initial work, Rendering* - [ProWolf](https://github.com/prowolf)
* **pcguy123** - *ECS, Documentation* - [pcguy123](https://github.com/pcguy123)

See also the list of [contributors](https://github.com/prowolf/Igneous/graphs/contributors) who participated in this project.

## License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License (CC BY-NC-SA) - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgements

* Thank you to the authors of the dependencies used.
* Thank you to the amazing teachers on YouTube: [ThinMatrix](https://www.youtube.com/user/ThinMatrix), [TheChernoProject](https://www.youtube.com/user/TheChernoProject), [thebennybox](https://www.youtube.com/user/thebennybox), [Code Rad](https://www.youtube.com/user/YouKondziu), and [Derek Banas](https://www.youtube.com/user/derekbanas).
