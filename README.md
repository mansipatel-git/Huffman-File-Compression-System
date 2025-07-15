# Huffman-File-Compression-System

#  Huffman File Compression Tool

A C++ utility to compress and decompress **any file type** (text, images, audio, video, etc.) using the **Huffman Coding Algorithm**. It implements a **lossless** compression technique that reduces file size based on character frequency, using a greedy approach and priority queue (min-heap).

---

## 🚀 Features

- Compress **any file** — `.txt`, `.jpg`, `.pdf`, `.mp3`, `.mp4`, `.exe`, etc.
-  Decompress back to **original binary** with 100% accuracy
-  Huffman encoding using min-heap & binary tree
-  Shows original size, compressed size & compression ratio
-  Cross-platform: works on Windows, Linux, Mac

---


## 📁 Supported File Types

| File Type   | Supported | Note |
|-------------|-----------|------|
| `.txt`      | ✅        | High compression |
| `.jpg`, `.png` | ✅     | Minimal gain (already compressed) |
| `.pdf`, `.mp4`, `.mp3` | ✅ | Works well |
| `.exe`, `.zip`, `.dll` | ✅ | Binary-safe |

---

## 🛠️ Build Instructions

### 🧾 Requirements:
- C++ compiler (`g++`, `clang++`, MSVC)
- C++11 or newer

### Compression

🗜️  Huffman Compression Tool
1. Compress any file
2. Decompress a file
Enter your choice: 1
Enter input file to compress: test.pdf
Enter output compressed file: test.huff
✅ Compression complete: test.huff
Original size: 12048 bytes
Compressed size: 7812 bytes
Compression ratio: 64.84%

### Decompression
🗜️  Huffman Compression Tool
1. Compress any file
2. Decompress a file
Enter your choice: 2
Enter compressed input file: test.huff
Enter output file to save decompressed data: recovered.pdf
✅ Decompression complete: recovered.pdf

### Concepts Used
Greedy Algorithm (Huffman Coding)

Min-Heap / Priority Queue

Binary Tree Traversal

Unordered Map (hash table)

Bit Manipulation

Binary File I/O


