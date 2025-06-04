import lzma
from pathlib import Path

import serial

from .project import Project, load_project_file
from .simplify import simplify


def upload_data(proj: Project, port: serial.Serial) -> None:
    all_points = list(simplify(proj.directives))
    point_count = len(all_points)

    port.write(point_count.to_bytes(2, "little", signed=False))

    done = False

    while not done:
        while port.in_waiting < 2:
            pass

        idx = int.from_bytes(port.read(2), "little", signed=False)
        port.write(all_points[idx][0].to_bytes, "little", signed=False)
        port.write(all_points[idx][1].to_bytes, "little", signed=False)

        print(idx)
        if idx >= point_count - 1:
            done = True


def main() -> None:
    file = Path("/home/bellflwr/Documents/Blotter/Plots/beziertest.bplot")

    proj = load_project_file(file)

    with open("test.bbplot", "wb") as f:
        for bytes_ in proj.bin_serialize():
            f.write(bytes_)

    with lzma.open("test.bbplot.lzma", "wb") as f:
        for bytes_ in proj.bin_serialize():
            f.write(bytes_)

    # arduino = serial.Serial(port="/dev/ttyACM0", baudrate=115200, timeout=0.1)
    # upload_data(proj, arduino)
