import matplotlib.pyplot as plt

filename = './debug/triangulation3'

# Read the file
with open(filename+'.txt', 'r') as file:
    lines = file.readlines()

# Parse the number of points and triangles
num_points, num_triangles = map(int, lines[0].split())

# Parse the points
points = {}
for i in range(1, num_points + 1):
    index, x, y = map(int, lines[i].split())
    points[index] = (x, y)

# Parse the triangles
triangles = []
for i in range(num_points + 1, num_points + num_triangles + 1):
    a, b, c = map(int, lines[i].split())
    triangles.append((a, b, c))

# Plot the points
x_coords = [points[i][0] for i in points]
y_coords = [points[i][1] for i in points]
plt.scatter(x_coords, y_coords, color='blue')

# Annotate the points with their indices
for index, (x, y) in points.items():
    plt.text(x, y, str(index), fontsize=12, ha='right')

# Draw the triangles
for a, b, c in triangles:
    triangle = [points[a], points[b], points[c], points[a]]  # Close the triangle
    x, y = zip(*triangle)
    plt.plot(x, y, color='black')

# Set plot title and labels
plt.title('Triangulation')
plt.xlabel('X')
plt.ylabel('Y')
plt.gca().set_aspect('equal', adjustable='box')

# Show the plot
plt.savefig(filename+'.png')