# Adaptive Steganography System

This project is an advanced adaptive steganography system using chaotic maps, cellular automata, and wavelet transform for secure data embedding and verification in audio files. Now featuring a graphical user interface (GUI), this system is accessible for users who prefer a visual interface.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

**Adaptive Steganography System** provides a secure, user-friendly steganography solution to embed and verify data in audio files. With the new GUI, users can easily load files, configure parameters, and view results of data verification. The system integrates **chaotic maps** for random data distribution, **cellular automata** for encryption, **wavelet transform** for frequency-based data hiding, and advanced **verification metrics** for quality assurance.

## Features

- **User-Friendly GUI**: Easily accessible GUI for loading, processing, and verifying data in audio files.
- **Adaptive Steganography**: Selects optimal regions in the audio for embedding data, minimizing perceptual impact.
- **Chaotic Sequence Generation**: Uses chaotic maps for secure and pseudo-random data distribution.
- **Wavelet Transform**: Discrete wavelet transform (DWT) splits audio into frequency bands.
- **Verification Metrics**: Ensures data integrity with Mean Squared Error (MSE), Peak Signal-to-Noise Ratio (PSNR), and Structural Similarity Index (SSIM).

## Architecture

The project follows the **Model-View-Controller (MVC)** architecture for seamless integration between the GUI and processing layers:

1. **View (GUI)**: Provides a graphical interface for loading files, starting processes, and displaying results.
2. **Controller (`SteganographyController`)**: Connects the GUI with the processing layers, coordinating all steps from data embedding to verification.
3. **Model (Processing Layers)**: Contains all logic for audio processing, encryption, steganography, and verification.

## Installation

### Prerequisites

- **C++17** or later
- **FFmpeg**: For audio extraction from video
- **Eigen**: For mathematical operations
- **OpenCV**: For handling images and embedding chaotic sequences
- **Qt**: For building the graphical user interface
- **CMake**: For building the project

### Steps

1. **Clone the Repository**

   ```bash
   git clone https://github.com/novato-intlignt/AdaptiveSteganographySystem.git
   cd AdaptiveSteganographySystem
   ```

2. **Install Dependencies**
   Ensure that FFmpeg, Eigen, OpenCV, and Qt are installed on your system.

3. **Build the Project**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage

### Launching the GUI

To start the GUI, run the following command from the build directory:

```bash
./AdaptiveSteganographySystem
```
