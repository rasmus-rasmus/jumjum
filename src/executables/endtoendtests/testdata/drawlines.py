import matplotlib.pyplot as plt

## Just a quick 'n' dirty script to draw line segments from a file

def extract_line_segments(file_path):
    line_segments = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        for line in lines:
            parts = line.strip().split()
            if len(parts) == 4:
                try:
                    # Convert parts to integers
                    x1, y1, x2, y2 = map(int, parts)
                    # Append the tuple to the list
                    line_segments.append(((x1, y1), (x2, y2)))
                except ValueError:
                    print(f"Skipping line due to value error: {line}")
            else:
                print(f"Skipping line due to incorrect format: {line}")

    return line_segments

def draw_line_segments(line_segments, output_file):
    plt.figure()
    ax = plt.gca()
    
    # Draw each line segment
    for segment in line_segments:
        (x1, y1), (x2, y2) = segment
        ax.plot([x1, x2], [y1, y2], marker='x', linewidth=.5)
        
    ax.plot([0, 900], [694.74692179420242, 694.74692179420242], marker='x', linewidth=.5)
    
    # Set aspect of the plot to be equal
    ax.set_aspect('equal')
    plt.grid(True)
    
    # Save the plot to a file
    plt.savefig(output_file)
    plt.close()

file_path = 'lines.txt'
output_file = 'line_segments.png'

segments = extract_line_segments(file_path)
draw_line_segments(segments, output_file)

print(f"Line segments have been drawn and saved to {output_file}")