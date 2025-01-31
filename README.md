# Number Reader

## Project Overview
This is repository containing the development of simple GUI App that reads the number that was drawn on the black canvas

## How to Build the Project
## 1. Building the App
**Prerequisites**

>[Compiler]

-> Compiler: Cl.exe

-> Version: MSVC 14.x (from Visual Studio 2022)

-> Platform: Windows 64-bit

>[Dependecies]

-> wxWidget 3.2.5

-> onnxruntime 1.16.1

>[Build System]

-> CMake 3.29.5-msvc4

**Steps**
- Download and build the the prerequisites library which are the wxWidget and the Onnxruntime then put both of them into the external folder
- Run the command "Cmake build and (gdb) launch Debug" which already set in the launch.json
- Alternatively run the command as follow

```sh
-G "Visual Studio 17 2022" -S . -B build
cmake --build --config Debug
```

- Then go to build/Debug directory and run the .exe file

## 2. Training the model
The checkpoint for the model is already saved within the model/ directory

However if the user wish to retrain the model here are the following steps, that the user requires to take,

**Prerequisites**

>[Dependencies]

torch==2.4.0+cu118

torchaudio==2.4.0+cu118

torchvision==0.19.0+cu118

// Alternatively download pytorch with cuda 11.8 from the website

>[Dataset]

MNIST dataset downloaded via torchvision.datasets (already in the model.py script)

**Steps**

Downloading the dataset already been taken care by the script, the only thing needs to do when training the model is to run the model.py script as follow

```sh
python model.py --batch-size <batch_size> --lr <learning_rate> --epochs <epochs> --verbose(optional) --skip-test(optional)
```

By default, batch size is 32, learning rate is 0.001, and epochs is 10.
Optional argument like verbose will enable the program to provide more detailed log or information and skip-test will skip the validation process in each epochs.