#include "FAT32.h"

int main ()
{
    string input_path_fat = "/dev/disk6s2";
    Fat32Partition fat32(input_path_fat);
    fat32.open();
    fat32.read_data_boot_sector();
    // fat32.output_boot_sector();
    fat32.read_data_rdet();

    fat32.output_rdet_table();
    for (int i = 0 ; i < fat32.rdet.size(); i++)
    {
        for (int j = 0 ; j < 32 ; j++)
        {
            cout<<fat32.rdet[i][j] <<" ";
        }
        cout<<"\n--------------------------"<<endl;
    }
    // fat32.read_fat_table();
    // fat32.output_fat_table();
    // cout<<fat32.rdetOffset<<endl;
    fat32.close();




}

