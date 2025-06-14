# Huffman-File-Compression-Tool-
This project implements a Huffman Compression Tool using C++. The tool can compress and decompress text files using Huffman coding — a lossless data compression algorithm.

🚀 Features

Compresses a text file (input.txt) into a binary file (compressed.bin)

Generates a Huffman code map (code_map.txt)

Decompresses the binary file back into a text file (decompressed.txt)

Displays file size before and after compression

Command-line menu-driven interface

🛠️ How It Works

Compression:

Counts frequency of each character in the input file.

Builds a Huffman tree based on character frequencies.

Generates binary codes for each character.

Encodes the input file using the Huffman codes.

Writes the encoded binary data and code map to disk.

Decompression:

Loads the code map from file.

Reads binary data from the compressed file.

Decodes the binary string back to the original text.

Writes the decompressed text to the output file.

📂 File Structure

main.cpp – Main source code

input.txt – Input file to be compressed

compressed.bin – Output compressed binary file

code_map.txt – Saved Huffman code map

decompressed.txt – Final output after decompression

🔧 How to Compile and Run

# Compile
g++ main.cpp -o huffman_tool

# Run
./huffman_tool

Make sure input.txt exists in the same directory.

🧪 Sample Menu

=========== Huffman Compression Tool ===========
1. Compress
2. Decompress
0. Exit
===============================================
Enter your choice:

📊 Example Output

✅ Compression complete.
🔹 Original Size: 234 bytes
🔹 Compressed Size: 112 bytes

✅ Decompression complete → decompressed.txt

📌 Note

Supports newline (\n) and space (\s) character mappings

Automatically pads bits to complete bytes during binary write

📃 License

This project is open-source and available for free use, modification, and distribution.

🙌 Credits

Built with ❤️ in C++ for educational and practical use in file compression algorithms.

📈 Future Improvements

GUI using Qt for user interaction

Support for other file formats

Compression ratio optimization analysis
