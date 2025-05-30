import json


def map_(num: float, in_min: float, in_max: float, out_min: float, out_max: float):
    return (num - in_min) / (in_max - in_min) * (out_max - out_min) + out_min


with open("/home/bellflwr/Downloads/canadap.json") as f:
    data = json.load(f)

polygons = data["features"][0]["geometry"]["coordinates"]

min_x = None
max_x = None
min_y = None
max_y = None

for polygon in polygons:
    for point in polygon:
        x, y = point
        if min_x is None or x < min_x:
            min_x = x
        if min_y is None or y < min_y:
            min_y = y

        if max_x is None or x > max_x:
            max_x = x
        if max_y is None or y > max_y:
            max_y = y

print(min_x, max_x, min_y, max_y)

assert min_x is not None
assert min_y is not None
assert max_x is not None
assert max_y is not None

x_range = max_x - min_x
y_range = max_y - min_y

print(x_range, y_range)

longest_side = max([x_range, y_range])

output_width = 1024
output_height = 1024

scale_factor = output_width / longest_side

directives = []


def fanagle(x: float, y: float) -> tuple[float, float]:
    x -= min_x
    y -= min_y
    x *= scale_factor
    y *= scale_factor

    y *= -1
    y += y_range * scale_factor

    return round(x), round(y)


for polygon in polygons:
    directives.append({"type": "draw", "draw": False})

    x, y = polygon[-1]
    x, y = fanagle(x, y)
    directives.append({"type": "point", "dest": {"x": x, "y": y}})

    directives.append({"type": "draw", "draw": True})

    for point in polygon:
        x, y = point
        x, y = fanagle(x, y)

        dire = {"type": "point", "dest": {"x": x, "y": y}}

        if directives[-1] == dire:
            continue

        directives.append(dire)


with open("/home/bellflwr/Documents/Blotter/Plots/canada.bplot", "w") as f:
    json.dump(
        {"canvas_width": 1024, "canvas_height": 1024, "directives": directives},
        f,
        indent=4,
    )
