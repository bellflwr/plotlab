from collections.abc import Generator, Iterable

from .project import Directive, PointDirective


def simplify(directives: Iterable[Directive]) -> Generator[tuple[int, int]]:
    for dir in directives:
        if isinstance(dir, PointDirective):
            yield dir.dest.x, dir.dest.y
