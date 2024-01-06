- Change the skin of your champion, your ward, other champions, towers, minions, and jungle monsters in the game.
- Automatic skin database update.
- Support for spectator mode.
- Change skins anytime and unlimited times in a single game.
- Supports all popular languages ​​in the world.
- In-game configuration with <a href="https://github.com/ocornut/imgui">ImGui</a>.
- <a href="https://github.com/nlohmann/json">JSON</a> based configuration saving & loading

# Usage
   1. Compile source or <a href="https://github.com/sunmanbitch/SunSkin/releases/latest">download</a> a compiled version.
   2. Use <a href="https://github.com/R3nzTheCodeGOD/R3nzSkin/releases/latest">`R3nzSkin_Injector.exe`</a> or inject the built DLL into the game yourself.
      - If you use the `R3nzSkin_Injector.exe` injector, you need to rename `sunskin.dll` to `R3nzSkin.dll` and place `R3nzSkin.dll` in the same directory as `R3nzSkin_Injector.exe`
         - You will find that `R3nzSkin.dll` already exists in the folder, cool! I recommend you to use it
      - *Administrator* privilege may be needed if the injector failed to inject the DLL.
      - League client can crash if `R3nzSkin` is injected before being in the game.
         - A workaround is to not inject until you are in the game (you will need to be fast to not disrupt the game).
   3. Press <kbd>Insert</kbd> to bring up the menu.
   4. Select the skin for you, your teammates, enemies, and wards.

# Further optimizations
   If your CPU supports AVX / AVX2 / AVX-512 instruction set, you can enable it in project settings. This should result in more performant code, optimized for your CPU. Currently AVX2 instructions are selected in project settings.

# Credits
   This program is an learn and study version of the <a href="https://github.com/R3nzTheCodeGOD">R3nzTheCodeGOD</a>/<a href="https://github.com/R3nzTheCodeGOD/R3nzSkin">R3nzSkin</a> project.
