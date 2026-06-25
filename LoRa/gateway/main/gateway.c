#include <stdio.h>

void app_main(void)
{
    int arr[] = {1, 2, 3, 4, 5};
    printf("Array elements: ");
    fwrite(arr, sizeof(int), sizeof(arr) / sizeof(int), stdout);
    printf("\n");
}
