Analog filter calculator
# Analog Filter Calculator

A C program (and probably python as well)for designing analog Butterworth filters (Low-pass, High-pass, Band-pass, Band-reject) with equal source and load termination.  
**Author:** kheng choong

## Features

- Butterworth filter calculation (LP, HP, BP, BR)
- PI and Tee network topologies
- User-friendly CLI

## Planned Extensions

- Chebyshev filter support
- Bessel filter support
- Elliptical filter support

## Usage

1. Compile:
    ```sh
    gcc Butteerworth_Extended.c -o AnalogFilterCalculator -lm
    ```
2. Run:
    ```sh
    ./AnalogFilterCalculator
    ```

## Contributing

Pull requests are welcome! Please open an issue first to discuss major changes.

---

**Future Structure Suggestion:**
- `butterworth.c` / `butterworth.h`
- `chebyshev.c` / `chebyshev.h`
- `bessel.c` / `bessel.h`
- `elliptical.c` / `elliptical.h`
- `main.c` (for CLI and menu)
- `README.md`

This modular approach will make it easy to add new filter types.

---

**Ready to upload!**  
If you want, I can help you scaffold the Chebyshev, Bessel, and Elliptical filter files or menu structure.