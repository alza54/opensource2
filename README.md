# opensource2
(Open)Source² — a reasonable CS2 Open Source SDK for your projects.

#### Cheat is gonna be updated, milestones planned so far:
* Reaching Osiris/Aimtux functionality in CS2;
* Introducing measurements & collecting data for data science;
* Creation of indistinguishable, highly effective aim-assist;
* Writing an Electron (Node.js) DLL Injector/loader with VAC bypass and automatic updates;

### How to use?
1. Clone repo: ``git clone --recurse-submodules --remote-submodules https://github.com/alza54/opensource2``
2. Make sure to have DirectX 11 SDK installed.
3. Open the solution in Visual Studio 2022 on Windows (ideally).
##### Note: no need to copy & paste any dependencies into the solution, as they are automatically cloned with git.
4. As Counter Strike 2 is a 64-bit application, compile .DLL as x64 Debug or Release.
5. Inject the .DLL into the game with an injector of your choice. Ideally write your own, as it's not a hard thing to do.

### Is it working now?
The software has been tested on [build ID 12321656 released on 9/29/2023](https://steamdb.info/patchnotes/12321656/ "SteamDB.info CS2 Patch Notes")

### Contributions
* The software is in very early development stage, any contributions are welcomed.

### Is it undetected?
* If I had to guess, I'd say yes.
* For now.
* However you don't have any guarantee.
* You should always modify the SDK, never use cheats built-in, as they are examples, from which you can take a inspiration.

# Screenshots

![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.2.0.png?raw=true)