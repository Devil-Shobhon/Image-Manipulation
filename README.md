# 🎨 Image Manipulation Software in C

![C](https://img.shields.io/badge/Language-C-blue.svg)
![OS](https://img.shields.io/badge/OS-Ubuntu%2FDebian-orange.svg)
![GUI](https://img.shields.io/badge/GUI-IUP-lightgrey.svg)

A graphical Image Manipulation Software developed using the C programming language and the IUP GUI toolkit.

The application allows users to open images and perform various image manipulation operations through a graphical user interface.

## 👨‍💻 Author

- **Name:** MD. Nahin Hasan Shobhon
- **Course:** CSE 1101L - Structured Programming Lab
- **Program:** Bachelor of Science in Software Engineering (BSSE)
- **Institution:** Institute of Information Technology, University of Dhaka
- **GitHub:** [@Devil-Shobhon](https://github.com/Devil-Shobhon)

---

## ✨ Features

- Open Image
- Save Image
- Grayscale Conversion
- Image Inversion
- Brightness Adjustment
- Image Flipping
- Image Rotation
- Image Cropping
- Blur Filter
- Sharpen Filter
- Graphical User Interface

---

## 🛠️ Technologies Used

- C
- GCC
- IUP
- IM
- GTK
- Linux / Ubuntu

---

## 📁 Project Structure

```text
project/
├── src/
│   └── Source files
│
├── include/
│   └── Header files
│
├── iup/
│   └── IUP library files
│
├── im/
│   └── IM library files
│
├── images/
│   └── Sample images
│
├── Makefile
├── README.md
└── .gitignore
```

---

## ⚙️ Requirements

The following tools and libraries are required:

- Ubuntu / Debian based Linux
- GCC
- Make
- GTK 3
- IUP
- IM

Install the required system dependencies:

```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev libx11-dev pkg-config
```

---

## 🚀 Build and Run

Clone the repository:

```bash
git clone https://github.com/Devil-Shobhon/image_manipulation.git
cd image_manipulation
```

Build the project:

```bash
make
```

Run the application:

```bash
make run
```

---

## 🧹 Clean

To remove the compiled executable:

```bash
make clean
```

---

## 📌 Notes

The image manipulation operations are implemented in C.

The IUP toolkit is used to create the graphical user interface, while the IM library is used for image handling.

This project was developed as an academic project for the Structured Programming Lab course.
