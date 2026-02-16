import sys
import math

if __name__ == "__main__":
    our_cord = (0, 0, 0)
    i = 0
    print("=== Player Score Analytics ===")
    try:
        if len(sys.argv[1::]) == 0:
            print("No coordinates ...")
        else:
            cord = sys.argv[1::][0].split(',')
            for pos in sys.argv[1::][0].split(','):
                cord[i] = int(pos)
                i += 1
            cord = tuple(cord)
            print("Position created:", cord)
            x1 = our_cord[0]
            x2 = cord[0]
            y1 = our_cord[1]
            y2 = cord[1]
            z1 = our_cord[2]
            z2 = cord[2]
            distance = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
            print(f"Distance between {our_cord} {cord}: {distance}")
            print("Unpacking demonstration:")
            print(f"Player at x={x2}, y={y2}, z={z2}")
            print(f"Coordinates: X={x2}, Y={y2}, Z={z2}")
    except Exception as e:
        print(f'Parsing invaliding cordinates: "{sys.argv[1]}"')
        print(f"Error parsing coordinates: {e}")
        print(f"Error details - Type: ValueError, Args: ({e})")
