# 🎴 ID Card Generator (C++ & OpenCV)

A **C++ project using OpenCV** to create and manage **different types of ID cards** such as Student ID Card, Business ID Card, and Library ID Card.

The program allows you to:

✅ Enter personal details with validation

✅ Capture a photo using your **webcam**

✅ Generate an **ID Card with all details & photo**

✅ Show the ID Card in a window

✅ Delete stored ID card folders if needed

---

## 🚀 Features

* 📸  **Camera Integration** : Capture photo directly from webcam.
* 🖼️  **Card Design** : Automatically generates Student, Business, or Library ID cards.
* ✅  **Input Validation** : Checks name, phone, email, dates, etc. using regex.
* 🗂️  **Folder Management** : Creates and deletes person-specific folders.
* 🎨  **Custom Styling** : Adds photo, details, and signature area on the ID card.

---

## 🛠️ Requirements

* **C++17 or later**
* **OpenCV** (4.x recommended)
* **CMake** or your preferred build tool
* A working **webcam** for photo capture

---

## 📂 Project Structure

```
ID-Card-Generator/
│── idcard.cpp       # The main project source code
│── StudentIDCard/   # Saved student ID cards
│── BusinessIDCard/  # Saved business ID cards
│── LibraryIDCard/   # Saved library ID cards
│── README.md        # Project Documentation
```

---

## ⚙️ Installation & Setup

### 1. Clone this Repository

```bash
git clone https://github.com/your-username/ID-Card-Generator.git
cd ID-Card-Generator
```

### 2. Install Dependencies

On Ubuntu/Debian:

```bash
sudo apt update
sudo apt install libopencv-dev g++
```

On Windows, install **OpenCV** and configure it with your IDE (Visual Studio / CodeBlocks).

### 3. Compile the Program

```bash
g++ idcard.cpp -o idcard `pkg-config --cflags --libs opencv4`
```

### 4. Run the Program

```bash
./idcard
```

---

## 🖥️ Usage

When you run the program, you will see:

```
Select Card Type:
1. Student ID Card
2. Business ID Card
3. Library ID Card
4. Exit
Choice:
```

👉 Choose a card type, enter details, capture a photo, and generate the ID card.

Then you get another menu:

```
1. Capture Photo
2. Generate ID Card
3. Show ID Card
4. Delete Person Folder
5. Back to Card Type Menu
```

---

## 🎨 Example ID Card Layout

* 📸 Photo (top center)
* 📝 Details (Name, Roll, Email, etc.)
* ✍️ Signature (bottom right)
* 📌 Card Type Label (bottom left)

---


## 📷 Screenshot (Add Later)

Save a sample generated ID card and add it like this:

```
![Student ID Card](screenshots/student-id.jpg)
```

---

## 🤝 Contribution

Feel free to fork this project and improve designs, add QR codes, or support more ID card types.

---

## 📜 License

This project is licensed under the **MIT License** – free to use and modify.
