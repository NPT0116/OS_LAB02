#include <iostream>
#include <cstdint>
#include <array>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
using namespace std;
string convert_to_binary(char[3]);
#pragma pack(push, 1) // Đảm bảo không có padding được thêm vào giữa các trường

struct Fat32BootSector {
    uint16_t bytesPerSector;      // 0B: Số byte của một Sector
    uint8_t sectorsPerCluster;    // 0D: Số Sector của một Cluster
    uint16_t sectorsBeforeFat;     // 0E: Số Sector trước bảng FAT
    uint8_t numberOfFATs;         // 10: Số lượng bảng FAT
    uint32_t totalSectors;        // 20: Kích thước Volume
    uint32_t sectorsPerFAT;       // 24: Số Sector của một bảng FAT
    uint32_t rootCluster;         // 2C: Chỉ số Cluster bắt đầu của RDET
    char fatType[8];  // 52: Loại FAT

    Fat32BootSector()
        : bytesPerSector(0), sectorsPerCluster(0), sectorsBeforeFat(0),
          numberOfFATs(0), totalSectors(0), sectorsPerFAT(0),
          rootCluster(0) {
            for (int i = 0; i < 8 ; i++)
            {
                fatType[i] = 0;
            }
          }
};

class entry{
    public:
    entry() {}
    virtual ~entry() {}
    virtual void output_entry(){};
};
class entry_chinh : public entry {
public:
    uint8_t name[8];
    char extension[3];
    uint8_t attribute;
    char reserve1;
    char time[3];
    uint16_t date;
    char reserve2[8];
    uint16_t starting_cluster;
    uint32_t size;

    entry_chinh() : entry() { 
        // Khởi tạo mặc định
    }

    // Copy constructor
    entry_chinh(const entry_chinh& src) : entry(src) { // Gọi copy constructor của lớp cơ sở
        memcpy(this->name, src.name, sizeof(this->name));
        memcpy(this->extension, src.extension, sizeof(this->extension));
        this->attribute = src.attribute;
        this->reserve1 = src.reserve1;
        memcpy(this->time, src.time, sizeof(this->time));
        this->date = src.date;
        memcpy(this->reserve2, src.reserve2, sizeof(this->reserve2));
        this->starting_cluster = src.starting_cluster;
        this->size = src.size;
    }
     string output_date();
   void output_time();

    void output_entry() override;
};

class entry_phu : public entry {
public:
    uint8_t index;
    char name_1[10];
    uint8_t signature;
    char reserve_1[2];
    char name_2[12];
    char reserve_2[2];
    char name_3[4];

    entry_phu() : entry() {
        // Khởi tạo mặc định
    }

    // Copy constructor
    entry_phu(const entry_phu& src) : entry(src) { // Gọi copy constructor của lớp cơ sở
        this->index = src.index;
        memcpy(this->name_1, src.name_1, sizeof(this->name_1));
        this->signature = src.signature;
        memcpy(this->reserve_1, src.reserve_1, sizeof(this->reserve_1));
        memcpy(this->name_2, src.name_2, sizeof(this->name_2));
        memcpy(this->reserve_2, src.reserve_2, sizeof(this->reserve_2));
        memcpy(this->name_3, src.name_3, sizeof(this->name_3));
    }
    void output_entry() override  {};
};

#pragma pack(pop) // Kết thúc vùng không padding
class Fat32Table
{
public:
    vector <uint32_t> fatTable;
    Fat32Table(vector <uint32_t> table) {
         fatTable.assign(table.begin(), table.end());
    }
    Fat32Table(){};
    bool operator = ( const vector<uint32_t> &src)
    {
        fatTable.assign(src.begin(), src.end());
        return 1;
    }

};
class Fat32Partition
{
private:
    string directory = " ";

public:
    Fat32Partition( string directory ) : directory(directory){};
    Fat32BootSector BS;
    ifstream fat32_partition;
    Fat32Table fat32_table;
    int fat1Offset = 0;
    int rdetOffset = 0;
    vector <entry*> entries;
    std::vector<std::vector<uint8_t>> rdet; // Khai báo này nên được thêm vào lớp
    //FAT1 - FAT2 - RDET - DATA
    void read_data_boot_sector();
    void read_data_rdet();
    void open();
    void close();
    void output_boot_sector();
    void read_fat_table();
    void output_rdet_table();
    void output_fat_table();
    int offset_with_cluster (int );
    ~Fat32Partition()
    {
        for (int  i = 0 ; i < entries.size(); i++)
        {
            delete entries[i];
        }
    }
};


