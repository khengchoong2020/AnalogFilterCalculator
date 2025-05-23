/*
 * Chebyshev Filter Design Calculator with Table Output
 * Supports Low-pass, High-pass, Band-pass, and Band-reject filters (PI and Tee network, up to 7th order)
 * Only applies for odd order filter design for equal source and load termination
 * Author: kheng choong (extended by GitHub Copilot)
 * Date: 2025-05-20
 * Version: 2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.141592653589793

int main() {
    int n, Top, filterType, rippleOption;
    double Freq = 0, Fc = 0, BW = 0;
    double R = 50.0; // System impedance

    // Chebyshev filter coefficients for various ripple values (odd order only)
    double G_0pt01[7][7] = {
        {1.0,    0,      0,      0,      0,      0,      0}, // 1st order
        {0.0,    0,      0,      0,      0,      0,      0}, // 2nd order
        {1.1810, 1.8210, 1.1810, 0,      0,      0,      0}, // 3rd order
        {0.0,    0,      0,      0,      0,      0,      0}, // 4th order
        {0.9770, 1.6850, 2.0370, 1.6850, 0.9770, 0,      0}, // 5th order
        {0.0,    0,      0,      0,      0,      0,      0}, // 6th order
        {0.913,  1.5950, 2.0020, 1.8700, 2.0020, 1.5950, 0.9130} // 7th order
    };
    double G_0pt1[7][7] = {
        {1.0,    0,      0,      0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.4330, 1.5940, 1.4330, 0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.3010, 1.5560, 2.2410, 1.5560, 1.3010, 0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.2620, 1.5200, 2.2390, 1.6800, 2.2390, 1.5200, 1.2620}
    };
    double G_0pt5[7][7] = {
        {1.0,    0,      0,      0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.8640, 1.2800, 1.8340, 0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.8070, 1.3030, 2.6910, 1.3030, 1.8070, 0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {1.7900, 1.2960, 2.7180, 1.3850, 2.7180, 1.2960, 1.7900}
    };
    double G_1pt0[7][7] = {
        {1.0,    0,      0,      0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {2.2160, 1.0880, 2.2160, 0,      0,      0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {2.2070, 1.1280, 3.1030, 1.1280, 2.2070, 0,      0},
        {0.0,    0,      0,      0,      0,      0,      0},
        {2.2040, 1.1310, 3.1470, 1.1940, 3.1470, 1.1310, 2.2040}
    };

    printf("Chebyshev filter design calculator for equal source and load termination\n");
    printf("Supports up to 7th order, LP/HP/BP/BR, PI and Tee network.\n");
    printf("Select filter type:\n1. Low-pass\n2. High-pass\n3. Band-pass\n4. Band-reject\n");
    scanf("%d", &filterType);

    printf("Enter filter order (1-7, odd only): ");
    scanf("%d", &n);

    if (n < 1 || n > 7 || n % 2 == 0) {
        printf("Order out of range or not odd (1,3,5,7 only).\n");
        return 1;
    }

    printf("Enter ripple dB:\n1. 0.01dB\n2. 0.1dB\n3. 0.5dB\n4. 1.0dB\n");
    scanf("%d", &rippleOption);

    if (rippleOption < 1 || rippleOption > 4) {
        printf("Invalid ripple option.\n");
        return 1;
    }

    // Select the correct coefficient table
    double chebyshev_g[7][7] = {0};
    for (int i = 0; i < n; i++) {
        if (rippleOption == 1)
            chebyshev_g[n-1][i] = G_0pt01[n-1][i];
        else if (rippleOption == 2)
            chebyshev_g[n-1][i] = G_0pt1[n-1][i];
        else if (rippleOption == 3)
            chebyshev_g[n-1][i] = G_0pt5[n-1][i];
        else if (rippleOption == 4)
            chebyshev_g[n-1][i] = G_1pt0[n-1][i];
    }

    if (filterType == 3 || filterType == 4) {
        printf("Enter center frequency in Hz: ");
        scanf("%lf", &Fc);
        printf("Enter bandwidth in Hz: ");
        scanf("%lf", &BW);
    } else {
        printf("Enter cutoff frequency in Hz (limit to 4.2GHz): ");
        scanf("%lf", &Freq);
    }

    printf("Enter 1 for PI network or 2 for Tee network: ");
    scanf("%d", &Top);

    // Table header
    if (filterType == 1 || filterType == 2) {
        printf("\n| Element | Coefficient | Capacitance (nF) | Inductance (uH) |\n");
        printf("|---------|-------------|------------------|-----------------|\n");
    } else if (filterType == 3) {
        printf("\nBand-pass filter selected\n");
        printf("| Element | Coefficient | Series C (nF)    | Parallel L (uH) |\n");
        printf("|---------|-------------|------------------|-----------------|\n");
    } else if (filterType == 4) {
        printf("\nBand-reject filter selected\n");
        printf("| Element | Coefficient | Parallel C (nF)  | Series L (uH)   |\n");
        printf("|---------|-------------|------------------|-----------------|\n");
    }

    // Calculation and table output
    if (filterType == 1) { // Low-pass
        if (Top == 1) {
            printf("\nPI network selected (Low-pass) and ripple =%d\n",rippleOption);
            for (int i = 0; i < n; i++) {
                double g = chebyshev_g[n-1][i];
                double cap = g / (2 * PI * Freq * R) * 1e9; // nF
                double ind = (g * R) / (2 * PI * Freq) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
            }
        } else if (Top == 2) {
            printf("\nTee network selected (Low-pass) and ripple =%d\n",rippleOption);
            for (int i = 0; i < n; i++) {
                double g = chebyshev_g[n-1][i];
                double cap = g / (2 * PI * Freq * R) * 1e9; // nF
                double ind = (g * R) / (2 * PI * Freq) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
            }
        } else {
            printf("\nInvalid topology option.");
        }
    } else if (filterType == 2) { // High-pass
        if (Top == 1) {
            printf("\nPI network selected (High-pass) and ripple =%d\n",rippleOption);
            for (int i = 0; i < n; i++) {
                double g = chebyshev_g[n-1][i];
                double cap = (1.0 / (R * 2 * PI * Freq * g)) * 1e9; // nF
                double ind = (R / (2 * PI * Freq * g)) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
            }
        } else if (Top == 2) {
            printf("\nTee network selected (High-pass) and ripple =%d\n",rippleOption);
            for (int i = 0; i < n; i++) {
                double g = chebyshev_g[n-1][i];
                double cap = (1.0 / (R * 2 * PI * Freq * g)) * 1e9; // nF
                double ind = (R / (2 * PI * Freq * g)) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
            }
        } else {
            printf("\nInvalid topology option.");
        }
    } else if (filterType == 3) { // Band-pass
        for (int i = 0; i < n; i++) {
            double g = chebyshev_g[n-1][i];
            double C = (g * BW) / (2 * PI * Fc * Fc * R) * 1e9; // nF
            double L = (R * g) / (2 * PI * BW) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i+1, g, C, L);
        }
    } else if (filterType == 4) { // Band-reject
        for (int i = 0; i < n; i++) {
            double g = chebyshev_g[n-1][i];
            double C = (1.0 / (R * 2 * PI * BW * g)) * 1e9; // nF
            double L = (R / (2 * PI * BW * g)) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i+1, g, C, L);
        }
    } else {
        printf("\nInvalid filter type.");
    }

    return 0;
}