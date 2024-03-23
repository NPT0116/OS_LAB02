#include "FAT32.h"


void Fat32Partition :: open ()
{
    fat32_partition.open(directory, ios::binary);
    if (!fat32_partition)
    {
        cout<<"can't open file."<<endl;
    }
    return;

}
void Fat32Partition :: close ()
{
    fat32_partition.close();
}
void Fat32Partition::read_data_boot_sector() {
    if (fat32_partition) {
        // Đọc số byte của một Sector tại offset 0x0B
        fat32_partition.seekg(0x0B, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.bytesPerSector), sizeof(BS.bytesPerSector));

        // Đọc số Sector của một Cluster tại offset 0x0D
        fat32_partition.seekg(0x0D, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.sectorsPerCluster), sizeof(BS.sectorsPerCluster));
        // Đọc số Sector trước bảng FAT tại offset 0x0E
        fat32_partition.seekg(0x0E, ios::beg);
        fat32_partition.read(reinterpret_cast<char*>(&BS.sectorsBeforeFat), sizeof(BS.sectorsBeforeFat));

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


        // tính toán offset của rdet và bảng fat 1
        fat1Offset = BS.sectorsBeforeFat * BS.bytesPerSector;
        rdetOffset = (BS.sectorsBeforeFat + (BS.numberOfFATs * BS.sectorsPerFAT)) * BS.bytesPerSector + (BS.rootCluster - 2) * BS.sectorsPerCluster * BS.bytesPerSector;






    } else {
        cerr << "File hệ thống chưa được mở." << endl;
    }
}


// void Fat32Partition :: read_data_rdet()
// {
//     vector <uint8_t> table;
//     fat32_partition.seekg(rdetOffset , ios::beg);
//     int size = BS.sectorsPerCluster* BS.bytesPerSector;
//     while (size > 0 )
//     {
//         uint8_t buffer;
//         fat32_partition.read(reinterpret_cast<char*>(&buffer) , 1);

//         table.push_back(buffer);
//         size-=1;
//     }
//      int entriesPerRow = 16; // Số lượng entries bạn muốn hiển thị trên mỗi hàng
//     int totalEntries = table.size(); // Tổng số entries trong FAT table

//     // In header cho bảng với index
//     cout << "FAT Table:" << endl;
//     cout << "Index  ";
//     for (int i = 0; i < entriesPerRow; ++i) {
//         cout << i  << "       "; // Giới hạn chỉ số để không quá rộng, chỉ lấy hàng đơn vị
//     }
//     cout << endl;

//     // In dữ liệu
//     for (int i = 0; i < 1000; ++i) {
//         if (i % entriesPerRow == 0) {
//             cout << endl << i << "\t  "; // In chỉ số hàng mới khi cần
//         }
//         cout << (int)table[i] << "\t"; // In giá trị FAT entry

//         if ((i + 1) % entriesPerRow == 0 && i != 0) {
//             cout << endl; // Xuống hàng mới sau mỗi entriesPerRow entries
//         }
//     }
//     cout << endl;

// }

void Fat32Partition :: read_data_rdet()
{
if (fat32_partition.is_open()) {
            fat32_partition.seekg(rdetOffset, std::ios::beg);
            int size = BS.sectorsPerCluster * BS.bytesPerSector;

            while (size > 0) {
                uint8_t first_byte;
                fat32_partition.read(reinterpret_cast<char*>(&first_byte), 1);
                
                if (first_byte == 0x00 || first_byte == 0x50 || first_byte == 0x55) {
                    // Skip the current 32-byte block
                    fat32_partition.seekg(31, std::ios::cur); // Move forward 31 more bytes to complete 32 bytes
                } else {
                    // Reset the pointer back to read the full 32-byte entry
                    fat32_partition.seekg(-1, std::ios::cur);
                    std::vector<uint8_t> buffer(32); // Prepare buffer with size 32
                    fat32_partition.read(reinterpret_cast<char*>(buffer.data()), 32); // Read directly into buffer
                    rdet.push_back(buffer);
                }

                size -= 32; // Deduct the size of one entry from the remaining size
            }
        }
}
/*
if (fat32_partition.is_open()) {
            fat32_partition.seekg(rdetOffset, std::ios::beg);
            int size = BS.sectorsPerCluster * BS.bytesPerSector;

            while (size > 0) {
                uint8_t first_byte;
                fat32_partition.read(reinterpret_cast<char*>(&first_byte), 1);
                
                if (first_byte == 0x00 || first_byte == 0x50 || first_byte == 0x55) {
                    // Skip the current 32-byte block
                    fat32_partition.seekg(31, std::ios::cur); // Move forward 31 more bytes to complete 32 bytes
                } else {
                    // Reset the pointer back to read the full 32-byte entry
                    fat32_partition.seekg(-1, std::ios::cur);
                    std::vector<uint8_t> buffer(32); // Prepare buffer with size 32
                    fat32_partition.read(reinterpret_cast<char*>(buffer.data()), 32); // Read directly into buffer
                    rdet.push_back(buffer);
                }

                size -= 32; // Deduct the size of one entry from the remaining size
            }
        }
*/
void Fat32Partition ::  output_boot_sector()
{
    cout << "Thông tin Boot Sector FAT32:" << endl;
    cout << "Số byte của một Sector: " << BS.bytesPerSector << endl;
    cout << "Số Sector của một Cluster: " << (int)BS.sectorsPerCluster << endl;
    cout << "Số Sector trước bảng FAT: " << BS.sectorsBeforeFat << endl;
    cout << "Số lượng bảng FAT: " << (int)BS.numberOfFATs << endl;
    cout << "Kích thước Volume (tổng số sectors): " << BS.totalSectors << endl;
    cout << "Số Sector của một bảng FAT: " << BS.sectorsPerFAT << endl;
    cout << "Chỉ số Cluster bắt đầu của RDET: " << BS.rootCluster << endl;
    cout << "Loại FAT: ";
    for (char c : BS.fatType) {
        cout << c;
    }
    cout<<endl;
    cout<<"starting fat: "<<fat1Offset <<endl;
    cout<<"starting rdet: "  << rdetOffset <<endl; 
    cout << endl;
}
// void Fat32Partition:: output_rdet()
// {
//     cout<<"so luong entry: " << entries.size()<<endl;
//     cout << "Root Directory Entries:" << endl;
//     for (const auto& entry : entries) {
//         // Chuyển đổi tên và phần mở rộng từ dạng char array sang string và loại bỏ ký tự trắng
//         string name((char*)entry.name, 8);
//         name = name.c_str(); // Loại bỏ các ký tự rác sau null terminator
//         string extension(entry.extension, 3);
//         extension = extension.c_str(); // Tương tự như trên

//         // Xác định attribute của entry
//         string attrDesc;
//         if (entry.attribute & 0x10) {
//             attrDesc = "Directory";
//         } else if (entry.attribute & 0x20) {
//             attrDesc = "Archive";
//         } else if (entry.attribute & 0x01) {
//             attrDesc = "Read-only";
//         } else if (entry.attribute & 0x02) {
//             attrDesc = "Hidden";
//         } else if (entry.attribute & 0x04) {
//             attrDesc = "System";
//         } else {
//             attrDesc = "Other";
//         }

//         // In ra thông tin entry
//         cout << "Name: " << name;
//         if (!extension.empty()) {
//             cout << "." << extension;
//         }
//         cout<<endl;
//         cout << "Attribute: " << attrDesc<<endl;
//         cout << "Starting Cluster: " << entry.starting_cluster<<endl;
//         cout << "Size: " << entry.size << " bytes" << endl;
//         cout<<"-------------------------------------"<<endl;
//     }
// }
void Fat32Partition :: output_rdet_table()
{
    for (int i = 0 ; i < entries.size() ; i++)
    {
        if (dynamic_cast<entry_chinh*> (entries[i]) != nullptr)
        {
                cout<<"entry chinh "<<endl;
                
dynamic_cast<entry_chinh*> (entries[i]) -> output_entry();
        }
        else 
        {
            cout<<"entry phu"<<endl;
        }
    }
}


void Fat32Partition :: read_fat_table()
{
    if (fat32_partition)
    {
        vector <uint32_t > buffer ;
        int size =  BS.sectorsPerFAT * BS.bytesPerSector;
        fat32_partition.seekg(fat1Offset, ios::beg);
        
        uint32_t temp = 0;
        while (size > 0)
        {
            fat32_partition.read(reinterpret_cast<char*>(&temp), 4);
            buffer.push_back(temp);
            size -= 4;
        }
        fat32_table = buffer;
    }

}

void Fat32Partition :: output_fat_table()
{

    int entriesPerRow = 10; // Số lượng entries bạn muốn hiển thị trên mỗi hàng
    int totalEntries = fat32_table.fatTable.size(); // Tổng số entries trong FAT table

    // In header cho bảng với index
    cout << "FAT Table:" << endl;
    cout << "Index  ";
    for (int i = 0; i < entriesPerRow; ++i) {
        cout << i % 10 << "       "; // Giới hạn chỉ số để không quá rộng, chỉ lấy hàng đơn vị
    }
    cout << endl;

    // In dữ liệu
    for (int i = 0; i < 500; ++i) {
        if (i % entriesPerRow == 0) {
            cout << endl << i << "\t  "; // In chỉ số hàng mới khi cần
        }
        if (fat32_table.fatTable[i]  == 268435455)
        {
            cout<<"EOF"<<"\t";
        }
        else if (i == 1 || i == 0)
        {
            cout<<"-- \t";
        }
        else
        cout << fat32_table.fatTable[i] << "\t"; // In giá trị FAT entry

        if ((i + 1) % entriesPerRow == 0 && i != 0) {
            cout << endl; // Xuống hàng mới sau mỗi entriesPerRow entries
        }
    }
    cout << endl;
}
int Fat32Partition :: offset_with_cluster(int n)
{
    return (BS.sectorsBeforeFat + (BS.numberOfFATs * BS.sectorsPerFAT)) * BS.bytesPerSector + (BS.rootCluster - 2) * BS.sectorsPerCluster * BS.bytesPerSector;

}



string entry_chinh::output_date() {
    // Tính toán ngày, tháng, và năm từ trường date
    unsigned day = date & 0x1F; // Lấy 5 bits thấp nhất
    unsigned month = (date >> 5) & 0x0F; // Dịch phải 5 bits và lấy 4 bits tiếp theo
    unsigned year = ((date >> 9) & 0x7F) + 1980; // Dịch phải 9 bits, lấy 7 bits tiếp theo và cộng với 1980

    // Sử dụng stringstream để tạo chuỗi định dạng ngày tháng
    stringstream ss;
    ss << setfill('0') << setw(2) << day << "-" 
       << setfill('0') << setw(2) << month << "-" 
       << year;
    return ss.str();
}

string convert_to_binary(char src[3])
{
    string s = "";
    for (int  i = 0 ; i < 3 ; i ++)
    {
      s += bitset<8>(src[i]).to_string();
    }

    return s;
}
void entry_chinh :: output_time() {
    string binary = convert_to_binary(this -> time);
    std::bitset<5> hourBits(binary.substr(0, 5));
    std::bitset<6> minuteBits(binary.substr(5, 6));
    std::bitset<6> secondBits(binary.substr(11, 6));
    std::bitset<7> millisecondBits(binary.substr(17, 7));

    unsigned hour = hourBits.to_ulong();
    unsigned minute = minuteBits.to_ulong();
    unsigned second = secondBits.to_ulong(); // Giả sử giây không cần chia 2 trong cấu trúc tùy chỉnh này
    unsigned millisecond = millisecondBits.to_ulong();

    std::cout << "Time: " << hour << ":" << minute << ":" << second << "." << millisecond << std::endl;
}
void entry_chinh :: output_entry()
{
        string name((char*)this->name, 8);
        name = name.c_str(); // Loại bỏ các ký tự rác sau null terminator
        string extension(this->extension, 3);
        extension = extension.c_str(); // Tương tự như trên

        // Xác định attribute của entry
        string attrDesc;
        if (this->attribute & 0x10) {
            attrDesc = "Directory";
        } else if (this->attribute & 0x20) {
            attrDesc = "Archive";
        } else if (this->attribute & 0x01) {
            attrDesc = "Read-only";
        } else if (this->attribute & 0x02) {
            attrDesc = "Hidden";
        } else if (this->attribute & 0x04) {
            attrDesc = "System";
        } else {
            attrDesc = "Other";
        }

        // In ra thông tin entry
        cout << "Name: " << name;
        if (!extension.empty()) {
            cout << "." << extension;
        }
        cout<<endl;
        cout << "Attribute: " << attrDesc<<endl;
        cout << "Starting Cluster: " << this->starting_cluster<<endl;
        cout<<"Date: "<<output_date()<<endl;
        output_time();
        cout << "Size: " << this->size << " bytes" << endl;
        cout<<"-------------------------------------"<<endl;
    }

