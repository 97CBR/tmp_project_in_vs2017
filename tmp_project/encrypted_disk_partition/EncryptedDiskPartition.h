#pragma once

#include <iostream>
#include <time.h>
#include <windows.h>

constexpr auto BYTES_PER_SECTOR = 512;
constexpr auto PARTITION_ENCRYPTED_DATA_SECTOR = 36;
constexpr auto DEVICE_TAG_DATA_SECTOR = 35;


using namespace std;

class EncryptedDiskPartition {
    public:
        EncryptedDiskPartition();
        ~EncryptedDiskPartition();
        void JudgePartitionType(int device_number) const;
        void WriteSm4Key(const int device_number, char* key) const;
        static void ReadSm4Key(const int device_number, char* key);
        static void ReadSm4Key(int device_number, byte* key);
        BOOL EncryptMbr(int device_number) const;
        BOOL DecryptMbr(int device_number) const;
        BOOL DecryptGpt(int device_number) const;
        BOOL EncryptGpt(int device_number) const;
    private:

        static BOOL ReadPhysicalSector(LONGLONG sector_start, ULONG byte_counts, UCHAR* output_buffer, ULONG buffer_size,
                                       int device_number);
        BOOL WritePhysicalSector(LONGLONG sector_start, ULONG byte_counts, UCHAR* input_buffer, ULONG offset,
                                 int device_number) const;
        static BOOL ReadPhysicalSectorWithoutCutOff(LONGLONG sector_start, ULONG byte_counts, UCHAR* output_buffer,
                                                    ULONG buffer_size,
                                                    int device_number);
        static BOOL WritePhysicalSectorWithoutOffset(LONGLONG sector_start, ULONG byte_counts, UCHAR* input_buffer,
                                                     ULONG offset,
                                                     int device_number);
        static int HexCharStr2UnsignedCharStr(char* src, unsigned long lsrc, int flag, unsigned char* out,
                                              unsigned long* lout) ;
};
