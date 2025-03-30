#!/usr/bin/env python3
import sys
import os

def bin_to_header(bin_file, header_file):
    # Read the entire binary file
    try:
        with open(bin_file, "rb") as bf:
            data = bf.read()
    except IOError as e:
        print(f"Error reading binary file: {e}")
        sys.exit(1)

    # Convert binary data to comma-separated list of hex bytes formatted as 0xXX.
    hex_bytes = [f"0x{b:02X}" for b in data]

    # Format hex bytes by grouping them to a line width of 16 bytes per line.
    lines = []
    line_length = 16
    for i in range(0, len(hex_bytes), line_length):
        line = ", ".join(hex_bytes[i:i+line_length]) + ","
        lines.append("    " + line)

    # Create content for header file
    header_content = []
    header_content.append("#include <pico/platform.h>\n")
    header_content.append("static const uint8_t __in_flash() payload [] = {")
    header_content.extend(lines)
    header_content.append("};\n")

    # Write header file
    try:
        with open(header_file, "w") as hf:
            hf.write("\n".join(header_content))
    except IOError as e:
        print(f"Error writing header file: {e}")
        sys.exit(1)

    print(f"Header file '{header_file}' generated from '{bin_file}'.")

def main():
    if len(sys.argv) != 3:
        print("Usage: python bin_to_header.py input_binary_file output_header_file")
        sys.exit(1)

    bin_file = sys.argv[1]
    header_file = sys.argv[2]

    # Check if input binary file exists.
    if not os.path.isfile(bin_file):
        print(f"Input file '{bin_file}' does not exist.")
        sys.exit(1)

    bin_to_header(bin_file, header_file)

if __name__ == "__main__":
    main()
