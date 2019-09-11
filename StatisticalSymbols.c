#include <stdio.h>
#include <math.h>

int main()
{
	FILE *fp;
	char c;
	int i;
	double sum=0;
	double ISEntropy=0; 
	int Pi[256]={0};
	fp=fopen("testbook.txt","r");
	while (!feof(fp))
	{
		c=fgetc(fp); 
		//printf("%c",c);
		Pi[c]++;
	}
	for (i=0;i<256;i++) sum=sum+Pi[i];
	
	
	
	for (i=0;i<256;i++)
	{
		if(i==10) printf("Ascii码值：%3d   字符：换行   个数：%6d       信源概率：%f\n",i,Pi[i],Pi[i]/sum); 
		if (i==32) printf("Ascii码值：%3d   字符：空格   个数：%6d       信源概率：%f\n",i,Pi[i],Pi[i]/sum); 
		if ((i>32)&&(i<127)&&(Pi[i]!=0)) printf("Ascii码值：%3d   字符：%c      个数：%6d       信源概率：%f\n",i,i,Pi[i],Pi[i]/sum); 	
	}
	printf("总字数：%.0f 个\n",sum);
	for (i=0;i<256;i++)
	{
		if(Pi[i]!=0) ISEntropy=ISEntropy+log10(Pi[i]/sum)*(Pi[i]/sum)*(-1)*3.322;
	}
	printf("信源熵：%f bit/信源符号\n",ISEntropy);
	return 0;
 } 
