"""Run native C tests using the keycode definitions from a QMK checkout."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile

tests = Path(__file__).resolve().parent
qmk = Path(sys.argv[1] if len(sys.argv) > 1 else os.environ["QMK_HOME"]).resolve()
with tempfile.TemporaryDirectory(prefix="pc-azerty-") as directory:
    binary = str(Path(directory) / "pc_azerty_test")
    subprocess.run([
        os.environ.get("CC", "cc"), "-std=c11", "-Wall", "-Wextra", "-Werror",
        '-DQMK_KEYBOARD_H="keyboard.h"',
        "-I", str(tests), "-I", str(qmk / "quantum"),
        "-I", str(qmk / "quantum/keymap_extras"),
        "-I", str(qmk / "quantum/sequencer"),
        str(tests / "pc_azerty.c"), str(tests.parent / "features/pc_azerty.c"),
        "-o", binary,
    ], check=True)
    subprocess.run([binary], check=True)
