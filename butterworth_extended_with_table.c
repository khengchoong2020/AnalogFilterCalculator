/*
 * Butterworth Filter Design Calculator with Table Output
 * Supports Low-pass, High-pass, Band-pass, and Band-reject filters
 * with PI and Tee network topologies
 * Calculates component values based on user input
 * calculate order of the filter with attenuation at the offset frequency
 * and the center frequency for band-pass and band-reject filters
 * Author: kheng choong 
 * Date: 2025-05-21
 * Version: 2.1
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.141592653589793 

int main() {
    int n,n_cal, Top, filterType;
    uint32_t Fc = 0, BW = 0;// offsetFreq = 0;
    double R = 50.0; // System impedance
    double attenuation_dB = 0,CutoffFreq =0, offsetFreq = 0;

    printf("Butterworth filter design calculator for equal source and load termination\n");
    printf("Select filter type:\n1. Low-pass\n2. High-pass\n3. Band-pass\n4. Band-reject\n");
    scanf("%d", &filterType);

    // for LPF and HPF we will calculate the order if the filter based on attenuation and frequency
    // will accept input of attenuation and offset frequency from cutoff frequency
    // for BPF and BRF we will accept input of center frequency and bandwidth
    if (filterType == 1 || filterType == 2) {
        //double offsetFreq = 0.0;
        printf("Enter cutoff frequency in Hz (limit to 4.2GHz): ");
        scanf("%lf", &CutoffFreq);
        printf("Enter attenuation in dB: ");
        scanf("%lf", &attenuation_dB);
        printf("Enter offset frequency in Hz: ");
        scanf("%lf", &offsetFreq);

        if (filterType == 1) { // Low-pass
            if (attenuation_dB > 0 && offsetFreq > CutoffFreq) {
                n_cal = (int)ceil(log10(pow(10, attenuation_dB / 10) - 1) / (2 * log10(offsetFreq / CutoffFreq)));
                printf("Calculated filter order: %d\n", n_cal);
            } else {
                printf("Invalid input. For LPF, attenuation must be > 0 and offset frequency must be > cutoff frequency.\n");
                return 1;
            }
        } else if (filterType == 2) { // High-pass
            if (attenuation_dB > 0 && offsetFreq < CutoffFreq) {
                n_cal = (int)ceil(log10(pow(10, attenuation_dB / 10) - 1) / (2 * log10(CutoffFreq / offsetFreq)));
                printf("Calculated filter order: %d\n", n_cal);
            } else {
                printf("Invalid input. For HPF, attenuation must be > 0 and offset frequency must be < cutoff frequency.\n");
                return 1;
            }
        }
        printf("Enter filter order: ");
        scanf("%d", &n);

        if(n_cal >n ){
            printf("Warning !! Invalid input. Filter order must be greater than calculated order.\n");
            return 1;
        }
    }
    

    if (filterType == 3 || filterType == 4) {
        printf("Enter center frequency in Hz: ");
        scanf("%d", &Fc);
        printf("Enter bandwidth in Hz: ");
        scanf("%d", &BW);
        printf("Enter filter order: ");
        scanf("%d", &n_cal);
   // } else {
   //     printf("Enter cutoff frequency in Hz (limit to 4.2GHz): ");
   //     scanf("%d", &CutoffFreq);
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
                double cap = (coe) / (2 * PI * CutoffFreq * R) * 1e9; // nF
                double ind = (R * coe) / (2 * PI * CutoffFreq) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
                else
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
            }
        } else if (Top == 2) {
            printf("\nTee network selected (Low-pass)\n");
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (coe) / (2 * PI * CutoffFreq * R) * 1e9; // nF
                double ind = (R * coe) / (2 * PI * CutoffFreq) * 1e6; // uH
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
                double cap = (1.0 / (R * 2 * PI * CutoffFreq * coe)) * 1e9; // nF
                double ind = (R / (2 * PI * CutoffFreq * coe)) * 1e6; // uH
                if (i % 2 == 1)
                    printf("|   L%-5d| %11.4lf | %16s | %15.4f |\n", i, coe, "-", ind);
                else
                    printf("|   C%-5d| %11.4lf | %16.4f | %15s |\n", i, coe, cap, "-");
            }
        } else if (Top == 2) {
            printf("\nTee network selected (High-pass)\n");
            for (int i = 1; i <= n; i++) {
                double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
                double cap = (1.0 / (R * 2 * PI * CutoffFreq * coe)) * 1e9; // nF
                double ind = (R / (2 * PI * CutoffFreq * coe)) * 1e6; // uH
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
            double C = (coe * BW) / (2 * PI * Fc * Fc * R) * 1e9; // nF
            double L = (R * coe) / (2 * PI * BW) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i, coe, C, L);
        }
    } else if (filterType == 4) { // Band-reject
        for (int i = 1; i <= n; i++) {
            double coe = 2 * sin((((2 * i) - 1) * PI) / (2 * n));
            double C = (1.0 / (R * 2 * PI * BW * coe)) * 1e9; // nF
            double L = (R / (2 * PI * BW * coe)) * 1e6; // uH
            printf("|   %2d     | %11.4lf | %16.4f | %15.4f |\n", i, coe, C, L);
        }
    } else {
        printf("\nInvalid filter type.");
    }

    return 0;
}