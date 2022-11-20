#!/usr/bin/python3

"""
This module is used to verify that your lab solution will build with the lab
submission system.
"""

import atexit
import pathlib
import argparse
import shutil
import subprocess
import sys
import zipfile
import getpass
import re

STUDENT_REPO_PATH = pathlib.Path(__file__).absolute().parent.resolve()
TEST_REPO_PATH = (STUDENT_REPO_PATH / "test_repo").resolve()
BUILD_PATH = TEST_REPO_PATH / "build"
CHECKER_PATH = STUDENT_REPO_PATH / "tools" / "checker"


class TermColors:
    """Terminal codes for printing in color"""

    # pylint: disable=too-few-public-methods

    PURPLE = "\033[95m"
    BLUE = "\033[94m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RED = "\033[91m"
    END = "\033[0m"
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"


def print_color(color, *msg):
    """Print a message in color"""
    print(color + " ".join(str(item) for item in msg), TermColors.END)


def error(*msg, returncode=-1):
    """Print an error message and exit program"""

    print_color(TermColors.RED, "ERROR:", " ".join(str(item) for item in msg))
    sys.exit(returncode)


def format_code():
    """Run ./format.py to format student code"""
    print_color(TermColors.BLUE, "Running ./format.py to format your code")

    subprocess.run(
        [
            "./format.py",
        ],
        cwd=STUDENT_REPO_PATH,
        check=True,
    )


def clone_student_repo():
    """Clone a clean 330 student repo into 'test_repo_path', deleting existing one if it exists"""

    # Delete existing repo
    shutil.rmtree(TEST_REPO_PATH, ignore_errors=True)

    if TEST_REPO_PATH.is_dir():
        error("Could not delete", TEST_REPO_PATH)

    print_color(TermColors.BLUE, "Cloning ecen330 base repo into", TEST_REPO_PATH)
    proc = subprocess.run(
        [
            "git",
            "clone",
            "https://github.com/byu-cpe/ecen330_student",
            str(TEST_REPO_PATH),
        ],
        cwd=STUDENT_REPO_PATH,
        check=False,
    )
    if proc.returncode:
        return False
    return True


def get_lab_folder_name(lab):
    if lab == "lab1":
        return "lab1_helloworld"
    if lab == "lab2":
        return "lab2_gpio"
    if lab == "lab3":
        return "lab3_timer"
    if lab == "lab4":
        return "lab4_interrupts"
    if lab == "lab5":
        return "lab5_touchscreen"
    if lab == "lab6":
        return "lab6_clock"
    if lab in ("lab7m1", "lab7m2"):
        return "lab7_tictactoe"
    if lab in ("lab8m1", "lab8m2", "lab8m3"):
        return "lab8_missilecommand"
    if lab == "lab9":
        return "lab9_project"
    return lab


def get_files_to_copy_and_zip(lab):
    """Build a list of (src,dest) files to copy into the temp repo given the lab"""

    print_color(TermColors.BLUE, "Enumerating files to copy/zip")

    chk_lab_path = CHECKER_PATH / lab
    src_lab_path = STUDENT_REPO_PATH / get_lab_folder_name(lab)
    src_libs_path = STUDENT_REPO_PATH / "drivers"
    dest_libs_path = TEST_REPO_PATH / "drivers"
    dest_lab_path = TEST_REPO_PATH / get_lab_folder_name(lab)
    lasertag_path = STUDENT_REPO_PATH / "lasertag"

    # Build a list of files
    # Each entry in this list is a tuple in format (src - pathlib.Path, dest - pathlib.Path, include_in_zip? - boolean)
    files = []
    files.append((CHECKER_PATH / "CMakeLists.txt", TEST_REPO_PATH, False))
    files.append((chk_lab_path / "checker.cmake", TEST_REPO_PATH, False))
    if lab == "lab1":
        files.append((src_lab_path / "main.c", dest_lab_path, True))
    elif lab == "lab2":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, True))
        files.append((src_libs_path / "switches.c", dest_libs_path, True))
        files.append((src_lab_path / "gpioTest.c", dest_lab_path, True))
    elif lab == "lab3":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, False))
        files.append((src_libs_path / "switches.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, True))
    elif lab == "lab4":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, True))
        files.append((src_lab_path / "interrupt_test.c", dest_lab_path, True))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
    elif lab == "lab5":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, True))
    elif lab == "lab6":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, False))
        files.append((src_lab_path / "clockControl.c", dest_lab_path, True))
    elif lab == "lab7m1":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_lab_path / "minimax.c", dest_lab_path, True))
        files.append((src_lab_path / "testBoards.c", dest_lab_path, True))
    elif lab == "lab7m2":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, False))
        files.append((src_libs_path / "switches.c", dest_libs_path, False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "ticTacToeControl.c", dest_lab_path, True))
        files.append((src_lab_path / "minimax.c", dest_lab_path, True))
    elif lab == "lab8m1":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "missile.c", dest_lab_path, True))
    elif lab == "lab8m2":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "missile.c", dest_lab_path, True))
        files.append((src_lab_path / "gameControl.c", dest_lab_path, True))
    elif lab == "lab8m3":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "missile.c", dest_lab_path, True))
        files.append((src_lab_path / "plane.c", dest_lab_path, True))
        files.append((src_lab_path / "gameControl.c", dest_lab_path, True))
    elif lab == "lab9":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, False))
        files.append((src_libs_path / "switches.c", dest_libs_path, False))
        files.append((src_libs_path / "interrupts.c", dest_libs_path, False))
        files.append((src_libs_path / "touchscreen.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        for f in src_lab_path.iterdir():
            files.append((f, dest_lab_path, True))

    elif lab == "simon":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, False))
        files.append((src_libs_path / "switches.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "buttonHandler.c", dest_lab_path, True))
        files.append((src_lab_path / "flashSequence.c", dest_lab_path, True))
        files.append((src_lab_path / "verifySequence.c", dest_lab_path, True))
        files.append((src_lab_path / "simonDisplay.c", dest_lab_path, True))
        files.append((src_lab_path / "simonControl.c", dest_lab_path, True))
        files.append((src_lab_path / "globals.c", dest_lab_path, True))
    elif lab == "wam":
        files.append((chk_lab_path / "drivers.cmake", dest_libs_path / "CMakeLists.txt", False))
        files.append((chk_lab_path / "cmake", dest_lab_path / "CMakeLists.txt", False))
        files.append((src_libs_path / "buttons.c", dest_libs_path, False))
        files.append((src_libs_path / "switches.c", dest_libs_path, False))
        files.append((src_libs_path / "intervalTimer.c", dest_libs_path, False))
        files.append((src_lab_path / "wamControl.c", dest_lab_path, True))
        files.append((src_lab_path / "wamDisplay.c", dest_lab_path, True))
    elif lab == "390m3-1":
        files.append((lasertag_path / "filter.c", None, True))
    elif lab == "390m3-2":
        files.append((lasertag_path / "filter.c", None, True))
        files.append((lasertag_path / "hitLedTimer.c", None, True))
        files.append((lasertag_path / "lockoutTimer.c", None, True))
        files.append((lasertag_path / "transmitter.c", None, True))
        files.append((lasertag_path / "trigger.c", None, True))
    elif lab == "390m3-3":
        files.append((lasertag_path / "detector.c", None, True))
        #        files.append((lasertag_path / "autoReloadTimer.c", None, True))
        #        files.append((lasertag_path / "invincibilityTimer.c", None, True))
        #        files.append((lasertag_path / "ledTimer.c", None, True))
        files.append((lasertag_path / "lockoutTimer.c", None, True))
        files.append((lasertag_path / "isr.c", None, True))
    elif lab == "390m5":
        files.append((lasertag_path / "runningModes2.c", None, True))

    if lab.startswith("390"):
        print(
            len([f for f in files if f[2]]), "files to be included in the submission zip archive."
        )
    else:
        print(
            len(files),
            "files to copy to the fresh code base,",
            len([f for f in files if f[2]]),
            "of these will be included in the submission zip archive.",
        )
    return files


def copy_solution_files(files_to_copy):
    """Copy student files to the temp repo"""

    print_color(TermColors.BLUE, "Copying your solution files to the test_repo")

    # files_to_copy provides a list of files in (src_path, dest_path, include_in_zip?) format
    for (src, dest, _) in files_to_copy:
        print(
            "Copying", src.relative_to(STUDENT_REPO_PATH), "to", dest.relative_to(STUDENT_REPO_PATH)
        )
        if src.is_file():
            shutil.copy(src, dest)
        elif src.is_dir():
            shutil.copytree(src, dest / src.name)
        else:
            error("Required file", src, "does not exist.")


def build():
    """Run cmake/make"""

    print_color(TermColors.BLUE, "Trying to build")

    print_color(TermColors.BLUE, "Removing build directory (" + str(BUILD_PATH) + ")")
    shutil.rmtree(BUILD_PATH)
    print_color(TermColors.BLUE, "Creating build directory (" + str(BUILD_PATH) + ")")
    BUILD_PATH.mkdir()

    # Run cmake
    cmake_cmd = ["cmake", "..", "-DEMU=1"]
    proc = subprocess.run(cmake_cmd, cwd=BUILD_PATH, check=False)
    if proc.returncode:
        return False

    # Run make
    proc = subprocess.run(
        ["make", "-j4"],
        cwd=BUILD_PATH,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    build_output = proc.stdout.decode()
    print(build_output)
    if proc.returncode:
        return False

    # If user code has warnings, ask if they still want to compile.
    matches = re.findall(" warning: ", build_output)
    if matches:
        input_txt = ""
        while input_txt not in ["y", "n"]:
            input_txt = input(
                TermColors.YELLOW
                + "Your code has "
                + str(len(matches))
                + " warning(s).  You will lose a coding standard point for each warning.  Are you sure you want to continue? (y/n) "
                + TermColors.END
            ).lower()
        if input_txt == "n":
            error("User cancelled zip process.")

    return True


def run(lab, elf_name=None):
    """Run the lab program in the emulator"""
    if elf_name is None:
        elf_name = lab + ".elf"
    try:
        subprocess.run([str(BUILD_PATH / get_lab_folder_name(lab) / elf_name)], check=True)
    except KeyboardInterrupt:
        print()


def zip(lab, files):
    """Zip the lab files"""

    zip_path = STUDENT_REPO_PATH / (getpass.getuser() + "_" + lab + ".zip")
    print_color(TermColors.BLUE, "Creating zip file", zip_path.relative_to(STUDENT_REPO_PATH))
    if zip_path.is_file():
        print("Deleting existing file.")
        zip_path.unlink()
    with zipfile.ZipFile(zip_path, "w") as zf:
        print("Created new zip file")
        # Loop through files that are marked for zip (f[2] == True)
        for f in (f for f in files if f[2]):
            if f[0].is_file():
                # Write file to zip file
                print("Adding", f[0].relative_to(STUDENT_REPO_PATH))
                zf.write(f[0], arcname=f[0].name)
            elif f[0].is_dir():
                # Directory -- do a glob search and write all files to zip file
                for sub_f in f[0].rglob("*"):
                    print("Adding", sub_f.relative_to(STUDENT_REPO_PATH))
                    zf.write(sub_f, arcname=sub_f.relative_to(f[0].parent))
            else:
                error(f[0].relative_to(STUDENT_REPO_PATH), "does not exist")

    return zip_path.relative_to(STUDENT_REPO_PATH)


def get_milestones(lab):
    """Return the different milestones for the lab."""

    # Return list of configurations in (name, CMAKE_DEFINE) format
    if lab == "lab3":
        return [
            ("MILESTONE_1", "lab3_m1.elf"),
            ("MILESTONE_2", "lab3_m2.elf"),
        ]
    elif lab == "lab7":
        return [
            ("MILESTONE_1", "lab7_m1.elf"),
            ("MILESTONE_2", "lab7_m2.elf"),
        ]
    elif lab == "lab8":
        return [
            ("MILESTONE_1", "lab8_m1.elf"),
            ("MILESTONE_2", "lab8_m2.elf"),
            ("MILESTONE_3", "lab8_m3.elf"),
        ]
    else:
        return [("main", None)]


def main():
    """Copy files into temp repo, build and run lab"""

    atexit.register(exit_handler)

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "lab",
        choices=[
            "lab1",
            "lab2",
            "lab3",
            "lab4",
            "lab5",
            "lab6",
            "lab7m1",
            "lab7m2",
            "lab8m1",
            "lab8m2",
            "lab8m3",
            "lab9",
            "390m3-1",
            "390m3-2",
            "390m3-3",
            "390m5",
        ],
    )
    parser.add_argument(
        "--no_run", action="store_true", help="Test the lab build, but don't run the emulator"
    )
    args = parser.parse_args()

    # First format student's code
    format_code()

    # Get a list of files need to build and zip
    files = get_files_to_copy_and_zip(args.lab)

    # For 390, we don't build/run anything, so just skip to the zip process
    if not args.lab.startswith("390"):
        # Clone/clean 330 repo
        if not clone_student_repo():
            input_txt = ""
            while input_txt not in ["y", "n"]:
                input_txt = input(
                    TermColors.YELLOW
                    + "Could not clone Github repo.  Perhaps you are not connected to the internet. "
                    "It is recommended that you cancel the process, connect to the internet, and retry. "
                    "If you proceed, the generated zip file will be untested, and may not build properly on the TA's evaluation system. "
                    "Are you sure you want to proceed? (y/n) " + TermColors.END
                ).lower()
            if input_txt == "n":
                error("User cancelled zip process.")

        else:
            # Copy over necessary files to test repo
            copy_solution_files(files)

            # See if the code builds
            build_success = build()

            if not build_success:
                s = ""
                while s not in ("y", "n"):
                    s = input(
                        TermColors.RED + "Build failed. Continue? (y/n)" + TermColors.END
                    ).lower()
                if s == "n":
                    sys.exit(0)

            # Loop through executables
            for (milestone_name, elf_name) in get_milestones(args.lab):
                if args.no_run:
                    print_color(TermColors.BLUE, "Now Testing", milestone_name)
                else:
                    input(
                        TermColors.BLUE
                        + "Now Testing "
                        + milestone_name
                        + ". Hit <Enter> to continue."
                        + TermColors.END
                    )

                # Run it
                if not args.no_run:
                    print_color(TermColors.BLUE, "Running", args.lab, milestone_name)
                    print_color(
                        TermColors.BLUE,
                        "If the emulator won't close, press Ctrl+C in this terminal.",
                    )
                    run(args.lab, elf_name)

    # Zip it
    zip_relpath = zip(args.lab, files)

    print_color(TermColors.BLUE, "Created", zip_relpath, "\nDone.")


def exit_handler():
    # Delete test repo
    print_color(TermColors.BLUE, "Removing", TEST_REPO_PATH.name)
    shutil.rmtree(TEST_REPO_PATH, ignore_errors=True)


if __name__ == "__main__":
    main()
