#include <stdio.h>

struct MenuItem
{
	char szFoodName[100];
	unsigned int iCountOfOrder ;
	float fCostPerServing;
	float fTotalCost;  
} menuItemArray[] = {
						{"BUN MASAKA        ", 0, 40.0f,0.0f },
						{"BUN MASAKA JAM    ", 0, 50.0f,0 },
						{"BROOM MASAKA      ", 0, 30.0f,0 },
						{"Veg Samosa        ", 0, 50.0f,0 },
						{"Omlette           ", 0, 30.0f,0 },
						{"MASALA OMLLET     ", 0, 80.0f,0 },
						{"MASALA CHEES OMLLET", 0, 100.0f,0 },
						{"PAVBHAJI           ", 0, 69.0f,0 },
						{"CHEESE CAKE        ", 0, 95.0f,0 },
						{"RED VELVET         ", 0, 95.0f,0 },
					 };


int main(void)
{
	printf("\n\n");

	printf("---------------------------------------------------------------------\n");
	printf("\t\t\tPRATIK IRANI CAFE\n");
	printf("---------------------------------------------------------------------\n");


	unsigned int iSizeOfMenusItems = sizeof(menuItemArray) / sizeof(menuItemArray[0]);
	int i;
	unsigned int iChoice;

	do{
		printf("Please Enter your Choice.\n");
		printf("\t[ No . ]    NAME             \tOrders\t\t Costs\n\n");
		for(i = 0 ; i < iSizeOfMenusItems ; i++)
			printf("\t[ %d. ] %s \t %u \t, COST$ : %.2f \n",i + 101 , menuItemArray[i].szFoodName,menuItemArray[i].iCountOfOrder ,menuItemArray[i].fCostPerServing);
		printf("\t[ 111. ] EXIT\n->");
		scanf("%d", &iChoice);

		if(iChoice < 100 && iChoice > 112)
		{
			printf("INVALID CHOICE ...Please Enter Valid Choice...\n");
			continue;
		}
		
		switch(iChoice)
		{
		case 1:
		//	menuItemArray[iChoice].
		default:
			break;
		}
		printf("---------------------------------------------------------------------\n");

	}while(iChoice != 111);



	return (0);
}
