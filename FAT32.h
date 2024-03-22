#include <iostream>
#include <cstdint>
#include <array>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#pragma pack(push, 1) // Đảm bảo không có padding được thêm vào giữa các trường

struct Fat32BootSector {
    uint16_t bytesPerSector;      // 0B: Số byte của một Sector
    uint8_t sectorsPerCluster;    // 0D: Số Sector của một Cluster
    uint16_t reservedSectors;     // 0E: Số Sector trước bảng FAT
    uint8_t numberOfFATs;         // 10: Số lượng bảng FAT
    uint32_t totalSectors;        // 20: Kích thước Volume
    uint32_t sectorsPerFAT;       // 24: Số Sector của một bảng FAT
    uint32_t rootCluster;         // 2C: Chỉ số Cluster bắt đầu của RDET
    char fatType[8];  // 52: Loại FAT

    Fat32BootSector()
        : bytesPerSector(0), sectorsPerCluster(0), reservedSectors(0),
          numberOfFATs(0), totalSectors(0), sectorsPerFAT(0),
          rootCluster(0) {
            for (int i = 0; i < 8 ; i++)
            {
                fatType[i] = 0;
            }
          }
};
#pragma pack(pop) // Kết thúc vùng không padding
class Fat32
{
private:
    string directory = " ";

public:
    Fat32( string directory ) : directory(directory){};
    Fat32BootSector BS;
    ifstream fat32_partition;
    //FAT1 - FAT2 - RDET - DATA
    void read_data_to_boot_sector();
    void open();
    void close();
    void output_boot_sector();
};


