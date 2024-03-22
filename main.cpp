#include "FAT32.h"

int main ()
{
    string input_path_fat = "/dev/disk6s2";
    Fat32 fat32(input_path_fat);
    fat32.open();
    fat32.read_data_to_boot_sector();
    fat32.output_boot_sector();
    fat32.close();
}

