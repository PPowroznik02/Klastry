# PGM Image Cluster Analyzer
## Overview

This project is a C-based tool designed to load a Portable GrayMap (PGM) image file and analyze it to determine the sizes of clusters of contiguous pixels of the same color. The tool can be used for statistical  image processing tasks.

## Features

- PGM Image Loading: Load and parse PGM files (both ASCII and binary formats).
- Cluster Detection: Identify clusters of contiguous pixels that share the same intensity value.
- Cluster Size Calculation: Determine the size of each detected cluster.
- Output: Display  the results, including the color code and size of each cluster.

## Usage

To run the program, use the following command:


```sh
Klastry.exe <pgm_image_file_path>
```
Example:

```sh
Klastry.exe Images/sample1.pgm
```

Command-Line Arguments
```
<pgm_image_file_path>: Path to the PGM file to be analyzed.
```

## Output

The program will output the sizes of each detected cluster in the image. For example:
```
Kolor : Rozmiary
 10 : 1
 71: 60
 76: 27 59
 80: 42 25
 85: 55 2 42
 94: 27 43 29 45
101: 69 61
106: 42 36
110: 27 43
116: 56 36
123: 43 43 43 45
130: 7 24 45
255: 8844 9 10 10 10 2 2 2 8 10 2 10 2 2
```

PGM File Format

- PGM (Portable GrayMap): A grayscale image format that can be stored in either ASCII or binary format.
- The file starts with a "magic number" (P2 for ASCII or P5 for binary).
- The next line contains the image width and height.
- The following line indicates the maximum grayscale value (e.g., 255).
- The remaining lines contain pixel intensity values.
- Lines starting from # are used as comments.