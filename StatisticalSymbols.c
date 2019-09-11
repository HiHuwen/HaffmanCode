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
		if(i==10) printf("Ascii��ֵ��%3d   �ַ�������   ������%6d       ��Դ���ʣ�%f\n",i,Pi[i],Pi[i]/sum); 
		if (i==32) printf("Ascii��ֵ��%3d   �ַ����ո�   ������%6d       ��Դ���ʣ�%f\n",i,Pi[i],Pi[i]/sum); 
		if ((i>32)&&(i<127)&&(Pi[i]!=0)) printf("Ascii��ֵ��%3d   �ַ���%c      ������%6d       ��Դ���ʣ�%f\n",i,i,Pi[i],Pi[i]/sum); 	
	}
	printf("��������%.0f ��\n",sum);
	for (i=0;i<256;i++)
	{
		if(Pi[i]!=0) ISEntropy=ISEntropy+log10(Pi[i]/sum)*(Pi[i]/sum)*(-1)*3.322;
	}
	printf("��Դ�أ�%f bit/��Դ����\n",ISEntropy);
	return 0;
 } 
