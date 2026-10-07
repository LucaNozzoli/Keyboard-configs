# Sofle keymap: setup, compile and flash

This guide covers the custom 2-layer keymap (`keymap.c` + `rules.mk`): what it does, how to build it, and how to flash it with QMK Toolbox.

---

## 1. What this keymap does

### Layers

| # | Name    | How to get there                 | What it's for                                      |
|---|---------|----------------------------------|----------------------------------------------------|
| 0 | `_BASE` | Default                          | Normal QWERTY typing                               |
| 1 | `_NAV`  | Hold the **NAV** left thumb key  | Arrows, Page Up/Down, Home/End, editing shortcuts  |

### Encoders

| Action             | BASE (layer 0)    | NAV (layer 1, holding NAV) |
|--------------------|-------------------|----------------------------|
| Left knob turn     | Mouse scroll      | Volume down / up           |
| Right knob turn    | Mouse scroll      | Volume down / up           |
| Left knob click    | Mute              | Mute                       |
| Right knob click   | Next track        | Next track                 |

### Files

| File       | Purpose                                                                 |
|------------|-------------------------------------------------------------------------|
| `keymap.c` | The keys on each layer and the encoder behavior. Commented throughout. |
| `rules.mk` | Turns on the two QMK features this keymap needs (encoder map, mouse keys). |
| `sofle-keymap-guide.md` | This guide. |

---

## 2. One-time setup: install QMK

QMK Configurator (the website) **can't** set encoder rotation, so this keymap has to be built with the QMK command-line tool. You only need to do this once.

**Windows**
1. Download and install **QMK MSYS** from <https://msys.qmk.fm>.
2. Open the **QMK MSYS** app (a terminal window).

**macOS / Linux**
1. If you're inside a Python virtualenv, leave it first with `deactivate`.
2. In a terminal, run QMK's official installer:
   ```sh
   curl -fsSL https://install.qmk.fm | sh
   ```
   This installs the QMK CLI **and** the compilers and flashing tools it needs (`avr-gcc`, `arm-none-eabi-gcc`, `avrdude`, `dfu-util`, etc.).
3. Close the terminal and open a new one, so the new tools are on your path.

> Alternative with Homebrew: `brew install qmk/qmk/qmk`.
>
> Don't use `pip install qmk` on its own: it installs only the CLI, without the compilers, and `qmk doctor` will report them missing.

**Then, on any system:**
```sh
qmk setup
```
Answer **y** to the prompts. This downloads the QMK source code into a folder called `qmk_firmware` in your home folder. If that folder already exists, it reuses it. It takes a few minutes.

To check it worked:
```sh
qmk doctor
```
It should finish with "QMK is ready to go".

If it reports **"Can't find avr-gcc"** (or `arm-none-eabi-gcc`, `avrdude`, …), the compilers aren't installed. Re-run the installer from step 2 in a new terminal, outside any virtualenv.

---

## 3. Link the repo into QMK (one time)

The keymap lives in this repo (`~/Keyboard-configs`):
```
Keyboard-configs/
├── keymap.c
├── rules.mk
└── sofle-keymap-guide.md
```

QMK only builds keymaps that sit inside `qmk_firmware/keyboards/<keyboard>/keymaps/`. Instead of copying the files there every time, create a **symlink** once, so QMK sees the repo as a keymap called `mine`. From the repo folder, run:

```sh
cd ~/Keyboard-configs
ln -s "$(pwd)" ~/qmk_firmware/keyboards/sofle/keymaps/mine
```

Check it worked:
```sh
ls ~/qmk_firmware/keyboards/sofle/keymaps/mine
```
It should list `keymap.c`, `rules.mk` and this guide. From now on, any edit you make in the repo is what QMK compiles. There's nothing to copy.

> QMK ignores the extra files (this guide, `.git`) in the folder. Only `keymap.c` and `rules.mk` matter.

---

## 4. Compile

From the repo folder, run:
```sh
qmk compile -kb sofle/rev1 -km mine
```

- `-kb sofle/rev1` is the keyboard. The Sofle v2 uses the `rev1` firmware.
- `-km mine` is the name of the symlink from step 3.

When it succeeds, the last lines say `[OK]`. QMK always puts the firmware in `~/qmk_firmware`, so copy it into the repo to have it at hand for flashing:
```sh
cp ~/qmk_firmware/sofle_rev1_mine.hex .
```

Or do both in one go:
```sh
qmk compile -kb sofle/rev1 -km mine && cp ~/qmk_firmware/sofle_rev1_mine.hex .
```

> The `.hex` is a build output, so you probably don't want it committed. Add it to `.gitignore` once:
> ```sh
> echo "*.hex" >> .gitignore
> echo "*.uf2" >> .gitignore
> ```

> If your controller is an RP2040 board instead of a Pro Micro, the file will be `sofle_rev1_mine.uf2`. Use that name in the `cp` command and see the note in step 5.

### If it fails
- **"keymap not found"**: the symlink is missing or broken. Re-run the `ls` check from step 3. If the repo has moved, remove the old link with `rm ~/qmk_firmware/keyboards/sofle/keymaps/mine` and create it again.
- **An error pointing at a line in `keymap.c`**: usually a missing comma or bracket from an edit. The error tells you the line number.
- **Unknown keycode `MS_WHLU` / `MS_WHLD`**: your QMK copy is old. Run `qmk setup` again, or `git pull` inside `~/qmk_firmware` followed by `qmk git-submodule`.

---

## 5. Flash with QMK Toolbox

Both halves need the same firmware, so you'll do this **twice**: once with the left half plugged in, once with the right.

1. Open **QMK Toolbox**.
2. Click **Open** and select `sofle_rev1_mine.hex` from the `Keyboard-configs` folder.
3. Tick **Auto-Flash**.
4. Plug **only the left half** into your computer by USB. Unplug the TRRS cable between the halves.
5. Press the **reset button** on that half. On some boards you need to press it twice quickly.
6. Toolbox detects the bootloader and flashes automatically. Wait for the "Flash complete" message.
7. Unplug it, plug in the **right half**, and repeat steps 5 and 6.
8. Reconnect the TRRS cable, then plug the USB back into the left half.

> **RP2040 controllers:** after pressing reset, the board shows up as a USB drive. Drag the `.uf2` file onto it instead of using Toolbox.

> **No boot key:** this keymap doesn't include `QK_BOOT`, so use the physical reset button whenever you want to flash.

---

## 6. Making changes later

1. Edit `keymap.c` in the repo.
2. From the repo folder, compile and copy the firmware:
   ```sh
   qmk compile -kb sofle/rev1 -km mine && cp ~/qmk_firmware/sofle_rev1_mine.hex .
   ```
3. Flash both halves again (step 5).
4. Commit the change:
   ```sh
   git add keymap.c rules.mk
   git commit -m "Describe what you changed"
   ```

### Quick editing tips

- **Change a key:** find it in the `LAYOUT(...)` block. The ASCII diagram above each layer shows where every key sits.
- **Make a key do nothing:** use `XXXXXXX`.
- **Let a NAV key fall through to BASE:** use `_______`.
- **Ctrl + a key:** wrap it as `C(KC_X)`. Shift is `S(...)`, Alt is `A(...)`, GUI/Win is `G(...)`.
- **Flip a knob's direction:** in the `encoder_map` section, swap the two keycodes inside `ENCODER_CCW_CW(...)`.
- **Encoder clicks** are the keys marked "encoder clicks" in the 4th row of each layer (`KC_MUTE` and `KC_MNXT` on BASE).

### Useful keycodes

| Keycode   | Does                  | Keycode   | Does                 |
|-----------|-----------------------|-----------|----------------------|
| `KC_MPLY` | Play / pause          | `KC_VOLU` | Volume up            |
| `KC_MNXT` | Next track            | `KC_VOLD` | Volume down          |
| `KC_MPRV` | Previous track        | `KC_MUTE` | Mute                 |
| `MS_WHLU` | Scroll up             | `MS_WHLD` | Scroll down          |
| `MS_WHLL` | Scroll left           | `MS_WHLR` | Scroll right         |
| `QK_BOOT` | Enter flashing mode   | `KC_CAPS` | Caps Lock            |

Full keycode list: <https://docs.qmk.fm/keycodes>
