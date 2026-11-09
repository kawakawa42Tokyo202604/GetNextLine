"""Read-line contracts across buffer sizes, descriptors, and bonus mode."""

from __future__ import annotations

import os
from pathlib import Path
import shlex
import subprocess
import tempfile
from typing import ClassVar
import unittest

ROOT = Path(__file__).resolve().parents[1]


class GetNextLineTests(unittest.TestCase):
    workspace: ClassVar[tempfile.TemporaryDirectory[str]]
    programs: ClassVar[dict[tuple[bool, int], Path]]

    @classmethod
    def setUpClass(cls) -> None:
        cls.workspace = tempfile.TemporaryDirectory(prefix="gnl-test-")
        cls.addClassCleanup(cls.workspace.cleanup)
        cls.programs = {}
        for bonus in (False, True):
            suffix = "_bonus" if bonus else ""
            for size in (1, 42, 1024):
                binary = Path(cls.workspace.name) / f"gnl{suffix}_{size}"
                command = shlex.split(os.environ.get("CC", "cc"))
                command += shlex.split(os.environ.get(
                    "TEST_CFLAGS", "-Wall -Wextra -Werror"))
                command += [f"-DBUFFER_SIZE={size}", "-I", str(ROOT)]
                if bonus:
                    command += ["-DTEST_BONUS"]
                command += [str(ROOT / "tests/driver.c")]
                command += [str(ROOT / f"get_next_line{suffix}.c")]
                command += [str(ROOT / f"get_next_line_utils{suffix}.c")]
                command += ["-o", str(binary)]
                subprocess.run(command, check=True)
                cls.programs[bonus, size] = binary

    def test_line_boundaries_and_eof(self) -> None:
        for key, binary in self.programs.items():
            for case in range(6):
                with self.subTest(mode=key, case=case):
                    result = subprocess.run(
                        [str(binary), str(case)], input=b"",
                        capture_output=True, timeout=5,
                        cwd=self.workspace.name)
                    self.assertEqual(result.returncode, 0, result.stderr)

    def test_invalid_and_closed_descriptors(self) -> None:
        for key, binary in self.programs.items():
            for case, name in ((6, "negative fd"), (7, "INT_MAX fd"),
                               (8, "closed fd")):
                with self.subTest(mode=key, case=name):
                    result = subprocess.run(
                        [str(binary), str(case)], input=b"",
                        capture_output=True, timeout=5,
                        cwd=self.workspace.name)
                    self.assertEqual(result.returncode, 0, result.stderr)

    def test_bonus_interleaves_two_descriptors(self) -> None:
        for (bonus, size), binary in self.programs.items():
            if bonus:
                with self.subTest(size=size):
                    result = subprocess.run(
                        [str(binary), "9"], capture_output=True, timeout=5,
                        cwd=self.workspace.name)
                    self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
