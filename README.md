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
* Don't treat it as a cheat to compile.
* You should always modify the SDK, never use cheats built-in, as they are examples, from which you can take a inspiration.

### Changelog

#### v0.5.0

* **Features / Visuals**: Added Chams.
* **Features / ESP**: Optimized text rendering.
* **Features / ESP**: Added 3D-box ESP for objects.
* **SDK**: added collision-based bounding-box calculation.
* **SDK**: added hitbox-based bounding-box calculation.

#### v0.4.0 ("Everyday" Release)

* **SDK**: Corrected indexes of ***CEngineClient*** after the game update.
* **SDK**: Corrected ***C_TraceHitboxData*** struct.
* **SDK**: introduced ***C_PointCamera*** schema.
* **SDK / Bone System**: Introduced ***GetBoneName*** method. 
* **SDK / CCSPlayer_CameraServices**: Corrected to the new implementation (no ``m_iFOV`` property anymore, ***CPlayer_CameraServices*** class instance pointer reachable now).
* **SDK / CBasePlayerController**: extended implementation.
* **SDK / m_szLastPlaceName**: this property got moved from ***C_CSPlayerPawnBase*** to ***C_CSPlayerPawn*** with the game update.
* **SDK / C_BaseEntity**: added a boolean ``IsPointCamera()`` check to ***C_BaseEntity*** class.
* **SDK**: Cleaned the code.
* **Features**: Organized features with macros and dependency injection.
* **Features / Drawing**: Introduced a (Function class instance) wrapper for the methods related to drawing on the screen.
* **Features / Drawing**: Introduced a mutex lock for thread-safe rendering.
* **Features / Drawing**: Introduced ***RenderArrowToAngle*** function.
* **Math**: Introduced functions: ***ToAngle***, ***CalculateRelativeAngle***, ***CalculateFOV***, ***CalculateAngleRadians***, ***deg2rad***, ***FromAngle***.
* **Features: TriggerBot**: For now removed the smoke check due to update. Introduced a function pattern signature for further investigation.
* **Features: AimBot**: Introduced a collection of methods related with AimBot. For now renders ***Enemies List***.
* **Features: FOV Changer**: FOV Changer with an option to choose different FOV for in-game view and scenes view (eg. match start animation).

##### v0.3.0 (First Stable Release)
* SDK: Introduced TraceSmoke function (returns smoke density between two lines).
* SDK: fixed unload crash.
* SDK: fixed unload pop-up "No" button exception.
* Added new font: Red Hat Display Regular.
* Fixed fonts allocation.
* Trigger Bot: introduced "Max Flash Intensity" filter
* Trigger Bot: introduced "Max Smoke Density" filter
* Trigger Bot: introduced "Scoped Only" filter for sniper weapons.
* Trigger Bot: corrected Trace Line end vector by the aim punch (recoil).

# Screenshots

### v0.5.0
![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.5.0-1.png?raw=true)
![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.5.0-2.png?raw=true)

### v0.4.0
![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.4.0.png?raw=true)

### v0.3.0
![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.3.0.png?raw=true)

### v0.2.0
![Cheat SDK GUI](https://github.com/alza54/opensource2/blob/main/media/gui-v0.2.0.png?raw=true)