/*
 * Extended Butterworth Filter Design Calculator with Table Output
 * Supports Low-pass, High-pass, Band-pass, and Band-reject filters
 * Author: kheng choong (extended by GitHub Copilot)
 * Date: 2023-10-12
 * Version: 2.1
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793 

int main() {
    int n, Top, filterType;
    uint32_t Freq = 0, Fc = 0, BW = 0;

    printf("Butterworth filter design calculator for equal source and load termination\n");
    printf("Select filter type:\n1. Low-pass\n2. High-pass\n3. Band-pass\n4. Band-reject\n");
    scanf("%d", &filterType);

    printf("Enter filter order: ");
    scanf("%d", &n);

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
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (coe) / (2 * PI * Freq * 50) * 1e9; // nF
                double ind = (50 * coe) / (2 * PI * Freq) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
            }
        } else if (Top == 2) {
            printf("\nTee network selected (Low-pass)\n");
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (coe) / (2 * PI * Freq * 50) * 1e9; // nF
                double ind = (50 * coe) / (2 * PI * Freq) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
            }
        } else {
            printf("\nInvalid topology option.");
        }
    } else if (filterType == 2) { // High-pass
        if (Top == 1) {
            printf("\nPI network selected (High-pass)\n");
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (1.0 / (50 * 2 * PI * Freq * coe)) * 1e9; // nF
                double ind = (50 / (2 * PI * Freq * coe)) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
            }
        } else if (Top == 2) {
            printf("\nTee network selected (High-pass)\n");
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (1.0 / (50 * 2 * PI * Freq * coe)) * 1e9; // nF
                double ind = (50 / (2 * PI * Freq * coe)) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
            }
        } else {
            printf("\nInvalid topology option.");
        }
    } else if (filterType == 3) { // Band-pass
        for (int i = 1; i <= n; i++) {
            double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
            double C = (coe * BW) / (2 * PI * Fc * Fc * 50) * 1e9; // nF
            double L = (50 * coe) / (2 * PI * BW) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i, coe, C, L);
        }
    } else if (filterType == 4) { // Band-reject
        for (int i = 1; i <= n; i++) {
            double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
            double C = (1.0 / (50 * 2 * PI * BW * coe)) * 1e9; // nF
            double L = (50 / (2 * PI * BW * coe)) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i, coe, C, L);
        }
    } else {
        printf("\nInvalid filter type.");
    }

    return 0;
}