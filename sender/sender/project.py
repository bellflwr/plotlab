import json
from ast import TypeAlias
from pathlib import Path
from typing import Any

import cattrs
from attrs import define, field

converter = cattrs.Converter()


@define
class Point:
    x: int = field()
    y: int = field()


@define
class DrawDirective:
    draw: bool = field()


@define
class PointDirective:
    dest: Point = field()


@define
class BezierDirective:
    h1: Point = field()
    h2: Point = field()
    dest: Point = field()


Directive = DrawDirective | PointDirective | BezierDirective


@define
class Project:
    canvas_width: int = field()
    canvas_height: int = field()
    directives: list[Directive] = field()


def directive_deserialize(val: dict[str, Any], _) -> Directive:
    types = {"draw": DrawDirective, "point": PointDirective, "bezier": BezierDirective}
    return converter.structure(val, types[val["type"]])


converter.register_structure_hook(Directive, directive_deserialize)


def load_project_file(file: Path) -> Project:
    with open(file) as f:
        data = json.load(f)
        return converter.structure(data, Project)
