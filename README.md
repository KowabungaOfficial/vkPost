# <img style="width: 50px; height: 50px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostLogo.svg">vkPost
vkPost is a Vulkan post processing layer to enhance the visuals of games.

Currently, the built in effects are:
- Contrast Adaptive Sharpening
- Denoised Luma Sharpening
- Fast Approximate Anti-Aliasing
- Enhanced Subpixel Morphological Anti-Aliasing
- 3D color LookUp Table

---------------------------------------------------------

## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> Disclaimer
This project was originally created by [DadSchoorse](https://github.com/DadSchoorse/).

vkPost is a continuation of where vkBasalt left off.

---------------------------------------------------------

## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> How To Install

### <img style="width: 30px; height: 30px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/ArchLinuxLogo.svg"> Arch/Arch Based Distros (Temporary Until I Can Figure Out AUR):
*Recommended To Install Both 64bit and 32bit Versions*
> [!IMPORTANT]
> Download [vkPost.conf](https://github.com/KowabungaOfficial/vkPost/releases/download/v0.4.6/vkPost.conf) and place it in any of these [locations](https://github.com/KowabungaOfficial/vkPost/tree/main#-configure) 

64bit Version:
```
wget https://github.com/KowabungaOfficial/vkPost/releases/download/v0.4.6/vkpost-0.4.6-2-x86_64.pkg.tar.zst
sudo pacman -U vkpost-0.4.6-2-x86_64.pkg.tar.zst
```

32bit Version:
```
wget https://github.com/KowabungaOfficial/vkPost/releases/download/v0.4.6/lib32-vkpost-0.4.6-2-x86_64.pkg.tar.zst
sudo pacman -U lib32-vkpost-0.4.6-2-x86_64.pkg.tar.zst
```

### Other Linux Distros Coming Soon (I need others who can publish to different distros):

---------------------------------------------------------

## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> How To Enable

### <img style="width: 30px; height: 30px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/TerminalLogo.svg"> Standard
When using the terminal or an application (.desktop) file, execute:
```ini
ENABLE_VKPOST=1 yourgame
```

If you've enabled vkPost globally, you can disable it for a specific application using:

```ini
DISABLE_VKPOST=1 yourgame
```

### <img style="width: 30px; height: 30px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/SteamLogo.svg">  Steam
With Steam, edit your launch options and add:
```ini
ENABLE_VKPOST=1 %command% 
```

### <img style="width: 30px; height: 30px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/LutrisLogo.svg"> Lutris
With Lutris, follow these steps below:
1. Right click on a game, and press `configure`.
2. Go to the `System options` tab and scroll down to `Environment variables`.
3. Press on `Add`, and add `ENABLE_VKPOST` under `Key`, and add `1` under `Value`.

### <img style="width: 35px; height: 35px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/HeroicGamesLauncherLogo.svg">Heroic Games Launcher
With Heroic Games Launcher, follow these steps below:

For All Games:
1. Go to `Settings` tab, then click `Game Defaults`
2. Go to `ADVANCED` tab and scroll down to `Environment Variables`
3. Add `ENABLE_VKPOST` to `Variable Name` and `1` to `Value`
   
Per Game Basis
1. Right click on a game, then click on `Settings`
2. Go to `ADVANCED` tab and scroll down to `Environment Variables`
3. Add `ENABLE_VKPOST` to `Variable Name` and `1` to `Value`

### Configuration Environment Variable
To override some default config options use `VKPOST_CONFIG`,

Here is an example, along with how to seperate effects by adding `;` 
 
`VKPOST_CONFIG='effects=smaa:cas;casSharpness=1.0'`

---------------------------------------------------------

## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> Configure

Settings like the CAS sharpening strength can be changed in the config file.
The config file will be searched for in the following locations:
* a file set with the environment variable`VKPOST_CONFIG_FILE=/path/to/vkPost.conf`
* `vkPost.conf` in the working directory of the game
* `$XDG_CONFIG_HOME/vkPost/vkPost.conf` or `~/.config/vkPost/vkPost.conf` if `XDG_CONFIG_HOME` is not set
* `$XDG_DATA_HOME/vkPost/vkPost.conf` or `~/.local/share/vkPost/vkPost.conf` if `XDG_DATA_HOME` is not set
* `/etc/vkPost.conf`
* `/etc/vkPost/vkPost.conf`
* `/usr/share/vkPost/vkPost.conf`

If you want to make changes for one game only, you can create a file named `vkPost.conf` in the working directory of the game and change the values there.

#### Reshade Fx shaders

To run reshade fx shaders e.g. shaders from the [reshade repo](https://github.com/crosire/reshade-shaders), you have to set `reshadeTexturePath` and `reshadeIncludePath` to the matching dirctories from the repo. To then use a specific shader you need to set a custom effect name to the shader path and then add that effect name to `effects` like every other effect.

```ini
effects = colorfulness:denoise

colorfulness = /home/user/reshade-shaders/Shaders/Colourfulness.fx
denoise = /home/user/reshade-shaders/Shaders/Denoise.fx
reshadeTexturePath = /home/user/reshade-shaders/Textures
reshadeIncludePath = /home/user/reshade-shaders/Shaders
```

#### Ingame Input

The [HOME](https://en.wikipedia.org/wiki/Home_key) key can be used to disable and re-enable the applied effects, the key can also be changed in the config file. This is based on X11 so it won't work on pure wayland. It **should** however at least not crash without X11.

---------------------------------------------------------
## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> Building From Source And Debugging

### Dependencies
Before building, you will need:
- GCC >= 9
- X11 development files
- glslang
- SPIR-V Headers
- Vulkan Headers

### Building

**These instructions use `--prefix=/usr`, which is generally not recommened since vkPost will be installed in directories that are meant for the package manager. The alternative is not setting the prefix, it will then be installed in `/usr/local`. But you need to make sure that `ld` finds the library since /usr/local is very likely not in the default path.** 

In general, prefer using distro provided packages.

```
git clone https://github.com/KowabungaOfficial/vkPost.git
cd vkPost
```

#### Docker

If you have Docker installed on your system, you can kick off the Docker build using: `./docker-build`. This will run both the 32 and 64-bit builds, and copy the results to the `out` directory.

If you don't have Docker installed or prefer not to use it, see the following sections.

#### 64bit

```
meson setup --buildtype=release --prefix=/usr builddir
ninja -C builddir install
```
#### 32bit

Make sure that `PKG_CONFIG_PATH=/usr/lib32/pkgconfig` and `--libdir=lib32` are correct for your distro and change them if needed. On Debian based distros you need to replace `lib32` with `lib/i386-linux-gnu`, for example.
```
ASFLAGS=--32 CFLAGS=-m32 CXXFLAGS=-m32 PKG_CONFIG_PATH=/usr/lib32/pkgconfig meson setup --prefix=/usr --buildtype=release --libdir=lib32 -Dwith_json=false builddir.32
ninja -C builddir.32 install
```
#### Debug Output

The amount of debug output can be set with the `VKPOST_LOG_LEVEL` environment variable, e.g. `VKPOST_LOG_LEVEL=debug`. 

Possible values are: `trace, debug, info, warn, error, none`.

By default the logger outputs to stderr, a file as output location can be set with the `VKPOST_LOG_FILE` env var, e.g. `VKPOST_LOG_FILE="vkPost.log"`.

---------------------------------------------------------

## <img style="width: 18px; height: 18px;" src="https://github.com/KowabungaOfficial/vkPost/blob/main/misc/vkPostRightArrow.svg"> FAQ

#### Where can I see what's being worked on?
Here you go my friend: [To Do List](https://github.com/KowabungaOfficial/vkPost/discussions/5)
#### Why is it called vkPost?
Based on vkBasalt, but takes the word Basalt out for Post, which hints at Post-Processing.
#### Does vkPost work with dxvk and vkd3d?
Yes.
#### Will there be a openGl version?
I will look into it. In the meantime if anyone wants to add it I will happily include it.
#### Will you support the Steam Deck
Yes, I am working on a way to get it to work on steam deck. Check for updates in releases.
#### Will you add  Wayland support?
If anyone would like to add it, they are free to contribute.
#### Will there be a GUI in the future?
Not for this project, but if anyone wants to make one they are free to.
#### Can you make reshade shaders work?
If someone wants to add it they are free to.

---------------------------------------------------------
