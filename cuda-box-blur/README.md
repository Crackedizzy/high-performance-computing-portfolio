# 🖼️ CUDA Box Blur

## 🧠 Description
Applies a 3×3 box blur filter to an image using CUDA.  
Reads an image, processes pixels in parallel on the GPU, and writes a blurred output.

## 🛠️ How to Compile
```bash
make
```

## ▶️ How to Run
```bash
./box_blur input.png output.png
```

## 🧹 Clean Up
```bash
make clean
```

## 📝 Note
Requires PNG image read/write support (you may need to link against `lodepng` or similar).
