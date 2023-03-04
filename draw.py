import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from ast import literal_eval

def generate_polygon(coords):
    fig, ax = plt.subplots()
    ax.set_xlim([min(coords, key=lambda x: x[0])[0]-1, max(coords, key=lambda x: x[0])[0]+1])
    ax.set_ylim([min(coords, key=lambda x: x[1])[1]-1, max(coords, key=lambda x: x[1])[1]+1])
    polygon = Polygon(coords, closed=True, linewidth=1, edgecolor='black', facecolor='none')
    ax.add_patch(polygon)
    plt.show()

# Example usage
result = []
with open('inp.txt', 'r') as f:
    for line in f:
        result.extend(literal_eval(line.strip()))


generate_polygon(result)
