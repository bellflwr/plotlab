import json
from collections.abc import Generator
from pathlib import Path
from typing import Any

import cattrs
from attrs import define, field

converter = cattrs.Converter()


@define
class Point:
    x: int = field()
    y: int = field()

    def bin_serialize(self) -> Generator[bytes, None, None]:
        yield self.x.to_bytes(2, "little", signed=False)
        yield self.y.to_bytes(2, "little", signed=False)


@define
class DrawDirective:
    draw: bool = field()

    def bin_serialize(self) -> Generator[bytes, None, None]:
        type_code = 2 if self.draw else 1
        yield type_code.to_bytes(1, "little", signed=False)


@define
class PointDirective:
    dest: Point = field()

    def bin_serialize(self) -> Generator[bytes, None, None]:
        type_code = 3
        yield type_code.to_bytes(1, "little", signed=False)
        yield from self.dest.bin_serialize()


@define
class BezierDirective:
    h1: Point = field()
    h2: Point = field()
    dest: Point = field()

    def bin_serialize(self) -> Generator[bytes, None, None]:
        type_code = 4
        yield type_code.to_bytes(1, "little", signed=False)
        yield from self.h1.bin_serialize()
        yield from self.h2.bin_serialize()
        yield from self.dest.bin_serialize()


Directive = DrawDirective | PointDirective | BezierDirective


@define
class Project:
    canvas_width: int = field()
    canvas_height: int = field()
    directives: list[Directive] = field()

    def bin_serialize(self) -> Generator[bytes, None, None]:
        yield self.canvas_width.to_bytes(2, "little", signed=False)
        yield self.canvas_height.to_bytes(2, "little", signed=False)
        yield len(self.directives).to_bytes(4, "little", signed=False)
        for d in self.directives:
            yield from d.bin_serialize()


def directive_deserialize(val: dict[str, Any], _) -> Directive:
    types = {"draw": DrawDirective, "point": PointDirective, "bezier": BezierDirective}
    return converter.structure(val, types[val["type"]])


converter.register_structure_hook(Directive, directive_deserialize)


def load_project_file(file: Path) -> Project:
    with open(file) as f:
        data = json.load(f)
        return converter.structure(data, Project)
