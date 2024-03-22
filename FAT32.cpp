#include "FAT32.h"


void Fat32 :: open ()
{
    fat32_partition.open(directory, ios::binary);
    if (!fat32_partition)
    {
        cout<<"can't open file."<<endl;
    }
    return;
}

void Fat32 :: close ()
{
    fat32_partition.close();
}
void Fat32::read_data_to_boot_sector() {
    if (fat32_partition) {
        // Đọc số byte của một Sector tại offset 0x0B
        fat32_partition.seekg(0x0B, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.bytesPerSector), sizeof(BS.bytesPerSector));

        // Đọc số Sector của một Cluster tại offset 0x0D
        fat32_partition.seekg(0x0D, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.sectorsPerCluster), sizeof(BS.sectorsPerCluster));
        // Đọc số Sector trước bảng FAT tại offset 0x0E
        fat32_partition.seekg(0x0E, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.reservedSectors), sizeof(BS.reservedSectors));

        // Đọc số lượng bảng FAT tại offset 0x10
        fat32_partition.seekg(0x10, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.numberOfFATs), sizeof(BS.numberOfFATs));

        // Đọc kích thước Volume tại offset 0x20
        fat32_partition.seekg(0x20, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.totalSectors), sizeof(BS.totalSectors));

        // Đọc số Sector của một bảng FAT tại offset 0x24
        fat32_partition.seekg(0x24, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.sectorsPerFAT), sizeof(BS.sectorsPerFAT));

        // Đọc chỉ số Cluster bắt đầu của RDET tại offset 0x2C
        fat32_partition.seekg(0x2C, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.rootCluster), sizeof(BS.rootCluster));

        // Đọc loại FAT tại offset 0x52
        fat32_partition.seekg(0x52, ios::beg);

        fat32_partition.read(reinterpret_cast<char*>(&BS.fatType), sizeof(BS.fatType));

    } else {
        cerr << "File hệ thống chưa được mở." << endl;
        // Xử lý lỗi tại đây
    }
}

void Fat32 ::  output_boot_sector()
{
    cout << "Thông tin Boot Sector FAT32:" << endl;
    cout << "Số byte của một Sector: " << BS.bytesPerSector << endl;
    cout << "Số Sector của một Cluster: " << (int)BS.sectorsPerCluster << endl;
    cout << "Số Sector trước bảng FAT: " << BS.reservedSectors << endl;
    cout << "Số lượng bảng FAT: " << (int)BS.numberOfFATs << endl;
    cout << "Kích thước Volume (tổng số sectors): " << BS.totalSectors << endl;
    cout << "Số Sector của một bảng FAT: " << BS.sectorsPerFAT << endl;
    cout << "Chỉ số Cluster bắt đầu của RDET: " << BS.rootCluster << endl;
    cout << "Loại FAT: ";
    for (char c : BS.fatType) {
        cout << c;
    }
    cout << endl;
}

