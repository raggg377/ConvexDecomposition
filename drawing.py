import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from ast import literal_eval

# Read polygon coordinates from a text file
# with open('polygons.txt', 'r') as f:
#     polygon_lines = f.readlines()

# Convert each line of the text file to a list of (x, y) tuples
# polygons = []
# for line in polygon_lines:
#     polygon = []
#     coords = line.strip().split(',')
#     for i in range(0, len(coords), 2):
#         x = float(coords[i])
#         y = float(coords[i+1])
#         polygon.append((x, y))
#     polygons.append(polygon)
polygons = []
with open('polygons.txt', 'r') as f:
    for line in f:
        polygons.extend(literal_eval(line.strip()))
# Create a figure and axis object
fig, ax = plt.subplots()

# Iterate through each polygon and plot it on the axis
for polygon in polygons:
    poly = Polygon(polygon, facecolor='blue', edgecolor='black', linewidth=2)
    ax.add_patch(poly)

# Set the x and y limits for the plot
xmin = min([p[0] for polygon in polygons for p in polygon])
xmax = max([p[0] for polygon in polygons for p in polygon])
ymin = min([p[1] for polygon in polygons for p in polygon])
ymax = max([p[1] for polygon in polygons for p in polygon])
ax.set_xlim([xmin-1, xmax+1])
ax.set_ylim([ymin-1, ymax+1])

# Add grid lines and labels to the plot
ax.grid(True)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_facecolor('black')
ax.set_title('Multiple Convex Polygons')

# Show the plot
plt.show()
