# ⚙️ High Performance Computing Portfolio

This repository contains four low-level programming tasks developed for the **High Performance Computing** module of my BSc Computer Science degree. These projects demonstrate my understanding of multithreaded and GPU-accelerated computing using **C**, **POSIX threads**, and **CUDA**.

Each folder contains source code, a Makefile for easy compilation, and a README with instructions.

---

## 📂 Project Overview

### 🔐 1. Multithreaded Password Cracker
Brute-force password cracking using multiple threads in C.  
Uses POSIX threads to parallelize the cracking of passwords in the format `AA99`.

📁 [`multithreaded-password-cracker/`](./multithreaded-password-cracker)

---

### ✖️ 2. Multithreaded Matrix Multiplication
Performs matrix multiplication on matrices loaded from file using multithreading.  
The program is optimized for parallel row-wise processing.

📁 [`multithreaded-matrix-multiplication/`](./multithreaded-matrix-multiplication)

---

### ⚡ 3. CUDA Password Cracker
Implements a GPU-based password cracker using a custom hash function.  
Utilizes thousands of parallel CUDA threads to search the keyspace efficiently.

📁 [`cuda-password-cracker/`](./cuda-password-cracker)

---

### 🖼️ 4. CUDA Box Blur
Applies a 3×3 box blur filter to a PNG image using CUDA.  
Processes image pixels in parallel and writes a blurred output image.

📁 [`cuda-box-blur/`](./cuda-box-blur)

---

## 🛠️ Technologies Used

- C (GCC)
- POSIX Threads (`pthread`)
- CUDA (NVIDIA GPU)
- Linux Terminal
- Makefiles

---

## 👨‍💻 Author

**Israel Morakinyo**  
BSc (Hons) Computer Science – First Class  
[GitHub](https://github.com/Crackedizzy) | [LinkedIn](https://www.linkedin.com/in/israel-morakinyo-98b00a204/)

---

⭐ *If you found this project helpful or interesting, feel free to star the repo!*
