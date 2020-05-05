﻿#pragma once

#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

class EncryptedDiskPartition {
    public:
        EncryptedDiskPartition();
        ~EncryptedDiskPartition();
        void JudgePartitionType(int device_number);
        BOOL EncryptMbr(int device_number);
        BOOL DecryptMbr(int device_number) const;
        BOOL DecryptGpt(int device_number) const;
        BOOL EncryptGpt(int device_number) const;
    private:
        const int BytesPerSector = 512;
        BOOL ReadPhysicalSector(LONGLONG sector_start, ULONG byte_counts, UCHAR* output_buffer, ULONG buffer_size,
                                int device_number) const;
        BOOL WritePhysicalSector(LONGLONG sector_start, ULONG byte_counts, UCHAR* input_buffer, ULONG offset,
                                 int device_number) const;
        BOOL ReadPhysicalSectorWithoutCutOff(LONGLONG sector_start, ULONG byte_counts, UCHAR* output_buffer,
                                             ULONG buffer_size,
                                             int device_number) const;
        BOOL WritePhysicalSectorWithoutOffset(LONGLONG sector_start, ULONG byte_counts, UCHAR* input_buffer,
                                              ULONG offset,
                                              int device_number) const;
};