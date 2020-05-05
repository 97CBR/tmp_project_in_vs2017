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
        void JudgePartitionType(int device_number);
        void WriteSm4Key(const int device_number, char* key) const;
        void ReadSm4Key(int device_number, char* key) const;
        BOOL EncryptMbr(int device_number);
        BOOL DecryptMbr(int device_number) ;
        BOOL DecryptGpt(int device_number) ;
        BOOL EncryptGpt(int device_number) ;
    private:

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
        int HexCharStr2UnsignedCharStr(char* src, unsigned long lsrc, int flag, unsigned char* out,
                                       unsigned long* lout) ;
};
