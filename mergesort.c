//Paris Downing and Nikita Zala
//

#include "scannerCSVsorter.h"

//function declarations
int mergesort(struct movies *mo, int startIndex, int endIndex, int sortBy);
int combine (struct movies *mo, int startIndex, int midIndex, int endIndex, int sortBy);

//breaks the array down into subarrays
int mergesort(struct movies *mo, int startIndex, int endIndex, int sortBy)
{
	if(startIndex < endIndex)
        {
                int mid1 = ((endIndex +startIndex)/2);
                mergesort(mo, startIndex, mid1, sortBy);
                mergesort(mo, mid1 + 1, endIndex, sortBy);
                combine(mo, startIndex, mid1, endIndex, sortBy);
 	}
        return 0;
 }

//combines the subarrays into a sorted bigger array
int combine (struct movies *mo, int startIndex, int midIndex, int endIndex, int sortBy)
{
        int temp1Size = midIndex - startIndex + 1;
        int temp2Size = endIndex - midIndex;
        int temp28Size = endIndex - startIndex + 1;
        struct movies temp1[temp1Size];
        struct movies temp2[temp2Size];
        struct movies temp28[temp28Size];

	int i;

	//copies first half of subarray into temp1
        for(i = 0; i < temp1Size; i++)
        {
                temp1[i] = mo[startIndex + i]; 
        }
	//copies second half of subarray into temp2
        for(i = 0; i < temp2Size; i++)
        {
                temp2[i] = mo[midIndex + i + 1];
        }

        int count1 = 0;
        int count2 = 0;
        int count28 = 0;

        //goes through the two sorted temp arrays and sees which
        for(count28 = 0; count1 < temp1Size && count2 < temp2Size; count28++)
        {
                char *temp1CurrVal = temp1[count1].array[sortBy];
                char *temp2CurrVal = temp2[count2].array[sortBy];
		
		if(strcmp(temp1CurrVal, temp2CurrVal) < 0)
                {
                        temp28[count28] = temp1[count1];
                        count1++;
                }
                else
                {
                        temp28[count28] = temp2[count2];
                        count2++;
                }
        }

	//puts any of the leftover structs in order
        while (count1 < temp1Size)
        {
                temp28[count28] = temp1[count1];
                count1++;
                count28++;
        }
        while(count2 < temp2Size)
        {
                temp28[count28] = temp2[count2];
                count2++;
                count28++;
        }
	//temp28 used to be called temp3, but when I switched from tested a 3 sized csv into a 28 sized csv, I changed all my 3s to 28s and didn't need to change them back.
	for(i = 0; i < temp28Size; i++)
        {
                mo[startIndex + i] = temp28[i];
        }

}
