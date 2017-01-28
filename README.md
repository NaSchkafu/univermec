# UniVerMeC #

## Installation ##

The following section contains a possible configuration of the
software. Because UniVerMeC is rather complex framework which utilizes a lot
of thirdparty libraries, it is necessary to install the dependencies
first. The dependencies are divided into three groups:

* Mandatory dependencies
* Dynamic dependencies
* Optional dependencies

If a dependency is dynamic users can choose between various alternatives. For
example, UniVerMeC needs a library for affine arithmetic to function
correctly. But users can choose between libaffa or YalAA or even provide an
adapter for some other library.

In the remainder of this document, it is assumed that a suitable C++
development environment (including a Fortran compiler) is already installed,
and that some Unix-like system (e.g., Linux) is used. UniVerMeC has been
compiled successfully with various gcc verions (5.x / 6.x), and clang. For
creating the build system an installation of the GNU autotools (autoconf,
automake, libtool) is required.

### Dependencies ###

To use UniVerMeC it is necessary to install at least the libraries listed
under *Mandatory*. Additionally, one libary for each group of the dynamic
dependencies required.

In the following tables header only libraries are marked in italics. The
listed versions have been tested. Older versions should also work in most
cases. Recommended choices for libraries from the dynamic section are bold.

We suggest to install all libraries for UniVerMeC in a subdirectory in your
home folder (e.g., $HOME/libs). This allows to choose the correct version
independent of the version bundled with the underlying Linux distribution

#### Mandatory ####

| Library                         | Version                                   | URL                     |
|---------------------------------|-------------------------------------------|-------------------------|
| Boost                           | 1.61 (>= 1.49 should work)                | http://www.boost.org    |
| *FADBAD++*                      | 2.1                                       | http://www.fadbad.com   |
| *Matrix Template Library (MTL)* | 4.0.9555 (other 4.x versions should work) | http://www.simunova.com |

**Note:** It is necessary to patch FADBAD++. Please apply the two patches from $UNIVERMEC_DIR/patches by calling:
`patch badiff.h < $UNIVERMEC_DIR/patches/badiff.h.patch`
`patch fadbad.h < $UNIVERMEC_DIR/patches/fadbad.h.patch`

#### Dynamic ####

**Interval arithmetic**:

| Library | Version    | URL                                                      |
|------------|---------|----------------------------------------------------------|
| **C-XSC**  | 2.5.4   | http://www2.math.uni-wuppertal.de/~xsc/xsc/cxsc_new.html |
| TBD        |         |                                                          |


**Affine arithmetic**:

| Library   | Version | URL                                |
|-----------|---------|------------------------------------|
| libaffa   | 0.9.6   | http://www.nongnu.org/libaffa/     |
| **YalAA** | 0.92    | https://github.com/NaSchkafu/yalaa |

**Taylor models**:

| Library     | Version      | URL                                          |
|-------------|--------------|----------------------------------------------|
| RiOt        | Version 2008 | http://www.math.kit.edu/ianm1/~ingo.eble/de  |
| **RiOt-NG** |              | https://github.com/NaSchkafu/riotng          |

#### Optional dependencies ####

TBD

### Configuration and Building ###

UniVerMeC can be configured by calling the shell script `./configure`. It is
recommended to use an out-of-source build. In this case the `configure` script
has to be called from the build directory.

`cd $UNIVERMEC_DIR`
`mkdir build`
`cd build`
`../configure`

Usually it is necessary to pass some parameters to the configure script to
locate all used libraries correctly. The file `config_template.global` can be
used as a starting point for figuring out the necessary parameters for the
used system.

After successful configuration the UniVerMeC library can be build by calling
`make` in the build directoy.
