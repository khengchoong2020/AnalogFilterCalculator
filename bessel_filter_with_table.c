/*
 * Bessel Filter Design Calculator with Table Output
 * Supports Low-pass, High-pass, Band-pass, and Band-reject filters (PI and Tee network, up to 7th order)
 * Author: kheng choong (extended by GitHub Copilot)
 * Date: 2023-10-12
 * Version: 2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.141592653589793

int main() {
    int n, Top, filterType;
    uint32_t Freq = 0, Fc = 0, BW = 0;
    double R = 50.0; // System impedance

    // Bessel normalized g-values for up to 7th order
    double bessel_g[7][7] = {
        {1.0000, 0,      0,      0,      0     , 0      , 0     }, // 1st order
        {0.5760, 2.1480, 0,      0,      0     , 0      , 0     }, // 2nd order
        {0.3370, 0.9710, 2.2030, 0,      0     , 0      , 0     }, // 3rd order
        {0.2330, 0.6730, 1.0820, 2.2400, 0     , 0      , 0     }, // 4th order
        {0.1740, 0.5070, 0.8040, 1.1110, 2.2580, 0      , 0     }, // 5th order
        {0.1370, 0.4000, 0.6390, 0.8540, 1.1130, 2.2650 , 0     }, // 6th order
        {0.1110, 0.3260, 0.5250, 0.7020, 0.8690, 1.1050 , 2.2660}  // 7th order
    };

    printf("Bessel filter design calculator for equal source and load termination\n");
    printf("Supports up to 7th order, LP/HP/BP/BR, PI and Tee network.\n");
    printf("Select filter type:\n1. Low-pass\n2. High-pass\n3. Band-pass\n4. Band-reject\n");
    scanf("%d", &filterType);

    printf("Enter filter order (1-7): ");
    scanf("%d", &n);

    if (n < 1 || n > 7) {
        printf("Order out of range (1-7 only).\n");
        return 1;
    }

    if (filterType == 3 || filterType == 4) {
        printf("Enter center frequency in Hz: ");
        scanf("%d", &Fc);
        printf("Enter bandwidth in Hz: ");
        scanf("%d", &BW);
    } else {
        printf("Enter cutoff frequency in Hz (limit to 4.2GHz): ");
        scanf("%d", &Freq);
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

    if (filterType == 1) { // Low-pass
        if (Top == 1) {
            printf("\nPI network selected (Low-pass)\n");
            for (int i = 0; i < n; i++) {
                double g = bessel_g[n-1][i];
                double cap = g / (2 * PI * Freq * R) * 1e9; // nF
                double ind = (g * R) / (2 * PI * Freq) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
            }
        } else if (Top == 2) {
            printf("\nTee network selected (Low-pass)\n");
            for (int i = 0; i < n; i++) {
                double g = bessel_g[n-1][i];
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
            printf("\nPI network selected (High-pass)\n");
            for (int i = 0; i < n; i++) {
                double g = bessel_g[n-1][i];
                double cap = (1.0 / (R * 2 * PI * Freq * g)) * 1e9; // nF
                double ind = (R / (2 * PI * Freq * g)) * 1e6; // uH
                if ((i+1) % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i+1, g, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i+1, g, cap, "-");
            }
        } else if (Top == 2) {
            printf("\nTee network selected (High-pass)\n");
            for (int i = 0; i < n; i++) {
                double g = bessel_g[n-1][i];
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
            double g = bessel_g[n-1][i];
            double C = (g * BW) / (2 * PI * Fc * Fc * R) * 1e9; // nF
            double L = (R * g) / (2 * PI * BW) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i+1, g, C, L);
        }
    } else if (filterType == 4) { // Band-reject
        for (int i = 0; i < n; i++) {
            double g = bessel_g[n-1][i];
            double C = (1.0 / (R * 2 * PI * BW * g)) * 1e9; // nF
            double L = (R / (2 * PI * BW * g)) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i+1, g, C, L);
        }
    } else {
        printf("\nInvalid filter type.");
    }

    return 0;
}