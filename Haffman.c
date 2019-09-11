#include<stdio.h>
#include<math.h>
struct Pinode 				//����������ڵ� 
{
	int FloorNum; 			//�ýڵ����ڵĲ��� 
	int OrderNum; 			//�ýڵ����ڲ��������(���ո��ʴ�С����) 
	double AllProbability;  //�ýڵ����̳�ǰ���ڵ�ĸ��ʺ� 
	char BitSign;           //�ýڵ����õ��Ķ����Ʒ���(1��0,û����2��ʾ) 
	int SourceNum[256];     //�ýڵ��ڰ����˸��ʺ͵���Щ��Դ�������(ASCII��Ϊ256��,���Ա���256���ڵķ�����) 
};

struct SourceSign			//���� ��Դ���Žṹ�� 
{
	double Probability;		//����Դ���ŵĸ��� 
	char code[20];			//����:����Դ���ŵĶ����Ʊ��� ,��λ��ǰ,��λ�ں�,��󳤶�Ϊ20,δ�������-1��ʾ 
	int codeLen;			// �볤 
};

void PrintHafnode(struct Pinode* Hafnode,int num,int N); 	//��ӡ���й������ڵ㺯������ 
void PrintAi(struct SourceSign *Ai,int N);					//��ӡ������Դ������Ϣ�������� 


//��ʼ���������ڵ�,ͬʱ����Դ���Ÿ��ʸ�ֵ����һ��Ĺ������ڵ�
//Ai��������Դ��������, Hafnode��������Ҫ���г�ʼ���Ĺ������ڵ������,N��ʾ��Դ���Ÿ��� 
void initHafnode(struct SourceSign *Ai,struct Pinode* Hafnode,int N)
{
	int i,j;
	int nodenum = (2+N)*(N-1)/2;  //��ʽ������Ҫʹ�õĹ������ڵ�ĸ��� 
	for (i=0;i<nodenum;i++) Hafnode[i].OrderNum=-1; //�Թ������ڵ����Ŷ�����ʼֵ-1 
	for (i=0;i<N;i++)                               		//��һ������ݳ�ʼ�� 
	{
		Hafnode[i].AllProbability=Ai[i].Probability;		//��ֵ��Դ���ŵĸ��� 
		Hafnode[i].SourceNum[0]=i+1;						//��һ�θ�ֵ����������Դ���� 
		Hafnode[i].FloorNum=1;								//��ֵ����Ϊ��һ��	
		Hafnode[i].OrderNum=i+1;							//����������Դ����(�Ѿ��ź���)��˳�� ��ֵ�ò��˳�� 
		Hafnode[i].BitSign=2;								//ͳһ�ȳ�ʼ������ķ���Ϊ2 
		for (j=1;j<N;j++) 	Hafnode[i].SourceNum[j]=-1;		// SourceNum[0]��¼�˵�һ������Դ����,����ĳ�ʼ��Ϊ-1 
	}
	Hafnode[N-1].BitSign=1;									//�Ե�һ������һ���ڵ㸳������Ʒ���1 
	Hafnode[N-2].BitSign=0;									//�Ե�һ������һ���ڵ㸳������Ʒ���0
	
}

//�Ըò�Ĺ������ڵ���з������е���Ϣ 
//���뵱ǰ�Ĳ��� FloorN,��Ҫ������Ϣ�Ľڵ�����Hafnode,��Դ���Ÿ���N 
void HafnodeLine(int FloorN,struct Pinode* Hafnode,int N)
{
	int count = N-FloorN+1; 										//���㵱ǰ�����еĽڵ���� 
	int beginNum = (FloorN-FloorN*FloorN)/2+N*FloorN+FloorN-N-1; 	//���㵱ǰ���е�һ���ڵ��±����ֵ(�Ȳʽ����) 
	int endNum = beginNum+count-1;									//���㵱ǰ���� ���һ���ڵ���±�ֵ 
	int i,j,k,lastA,lastB,Newnode;
	double NewProbability;
	double tempProbability = 0; 
//	printf("count=%d  ,beginNum=%d  ,endNum=%d\n",count,beginNum,endNum); //test
	for (i=beginNum-1;i>=beginNum-count-1;i--)     //Ѱ����һ���и�����С�������ڵ���±�(ͨ����Ų���) 
	{
		if (Hafnode[i].OrderNum==count+1) lastA=i;
		if (Hafnode[i].OrderNum==count)	lastB=i;
	}
	NewProbability = Hafnode[lastA].AllProbability+Hafnode[lastB].AllProbability; //��һ���и�����С�������ڵ������� 
//	printf("NewProbability=%.3f\n",NewProbability);//test
	i = beginNum;
	for (j=beginNum-1;j>=beginNum-count-1;j--)
	{
		if ((j!=lastA)&&(j!=lastB)) //�Է�������С���ʵĽڵ����ֱ�Ӹ�ֵ 
		{
			Hafnode[i].AllProbability=Hafnode[j].AllProbability;
			Hafnode[i].FloorNum = FloorN;
			Hafnode[i].BitSign = 2;
			for (k=0;k<N;k++) 	Hafnode[i].SourceNum[k] = Hafnode[j].SourceNum[k]; //�̳���һ���ڵ����Դ���� 
			i++;
		}
	}
	Newnode = i;
	//�����д����Ƕ������ɵĸ��ʺͽڵ���и�����Ϣ�ĸ�ֵ 
	Hafnode[Newnode].AllProbability =  NewProbability;//�õ����ʺ� 
	Hafnode[Newnode].FloorNum=FloorN;					//�õ����ڲ��� 
	Hafnode[Newnode].BitSign=2;						//Ԥ�ȸ���ֵ 
	for (k=0;k<N;k++) Hafnode[Newnode].SourceNum[k]= Hafnode[lastA].SourceNum[k];//ֱ�Ӽ̳���һ������С���ʽڵ����Դ���� 
	j=0;
	for (k=0;k<N;k++)
	{
		if (Hafnode[Newnode].SourceNum[k]==-1) //����ǰ�� SourceNum[k]��Ԥ��������Դ����, �̳���һ���еڶ�С���ʽڵ����Դ����Ҫ��ֵΪ-1��ֵ���и��� 
		{
			Hafnode[Newnode].SourceNum[k]= Hafnode[lastB].SourceNum[j];
			j++;	
		}
	}
	
	//���ݸ��ʴ�С��������,��ŷ����OrderNum
	for (i=0;i<count;i++)
	{
		tempProbability=0;
		for (j=beginNum;j<=endNum;j++)
		{
			if((tempProbability<Hafnode[j].AllProbability)&&(Hafnode[j].OrderNum==-1))
			{
				tempProbability=Hafnode[j].AllProbability;
				k=j;
			}
		}
	//	printf("k=%d  temp=%d ",k,tempProbability);//test
		Hafnode[k].OrderNum=i+1;
	}
	//������ʺͽڵ�ĸ���ֵ��ǰһ�����ĸ���ֵ����� ����������������������֤�ϲ����ʳ��������棬��ý�С���뷽�� 
	for (i=0;i<count;i++)
	{
		k=-1;
		for (j=beginNum;j<=endNum;j++) 
		{
			if((Hafnode[j].OrderNum==Hafnode[Newnode].OrderNum-1)&&(Hafnode[j].AllProbability==Hafnode[Newnode].AllProbability ))
			{
				Hafnode[j].OrderNum=Hafnode[j].OrderNum+1;
				Hafnode[Newnode].OrderNum=Hafnode[Newnode].OrderNum-1;
				break;
			}
		}
	}
	
	//�Ը�����С�������ڵ���з�������Ʒ���0��1 
	for (i=beginNum;i<=endNum;i++)
	{
		if (Hafnode[i].OrderNum==count) Hafnode[i].BitSign=1;
		if (Hafnode[i].OrderNum==count-1) Hafnode[i].BitSign=0;
	}		
}

//�ú�����ͨ���Ѿ���ֵ�õĽڵ���������Դ�����Ʒ��ŵķ��� 
// Ai��������Դ��������, Hafnode��������Ҫ���г�ʼ���Ĺ������ڵ������,N��ʾ��Դ���Ÿ��� 
void encoded(struct SourceSign *Ai,struct Pinode* Hafnode,int N)
{
	int nodenum = (2+N)*(N-1)/2; //��ʽ������Ҫʹ�õĹ������ڵ�ĸ���
	int i,j,k,cnt;
	for (i=0;i<N;i++) for (j=0;j<20;j++) Ai[i].code[j]=-1; //�Զ����Ʒ�����ȫ����ʼֵ-1 
	
	for (i=0;i<nodenum;i++)
	{
		if (Hafnode[i].BitSign!=2)  //�ҵ�������0,1�����Ʒ��ŵĹ������ڵ� 
		{
			//printf("i=%d bit=%d \n",i,Hafnode[i].BitSign);//test
			for (j=0;j<N;j++)
			{
				if (Hafnode[i].SourceNum[j]!=-1) //�ҳ��ù������ڵ��д��е���Դ 
				{
					//printf("i=%d. %d  j=%d  \n",i,Hafnode[i].SourceNum[j],j);//test
					cnt=Hafnode[i].SourceNum[j]-1; //�ҳ��ù������ڵ��д��е���Դ�����±����1 
					for (k=0;k<20;k++)
					{
						if (Ai[cnt].code[k]==-1) //����-1��ֵ���и���,����Ӱ��ǰ���Ѿ������ֵ 
						{
							Ai[cnt].code[k]=Hafnode[i].BitSign;
							break;
						}
					}
				}
			}
		}
	}
	//����ÿ����Դ���ŵ��ֳ� 
	for (i=0;i<N;i++)
	{
		cnt=0;
		for(j=0;j<20;j++)
		{
			if(Ai[i].code[j]==-1) break;
			else cnt++;
		}
		Ai[i].codeLen=cnt;	
	}
	
}

void HaffmanCode (double *ProbabilityArry,int N,struct SourceSign *Ai)
{
	int i;
	int nodenum = (2+N)*(N-1)/2 ;	//��ʽ������Ҫʹ�õĹ������ڵ�ĸ���
	for (i=0;i<N;i++)  Ai[i].Probability =	ProbabilityArry[i];  //�����������ֵ�����ṹ��	
	struct Pinode Hafnode[nodenum];
	initHafnode(Ai,Hafnode,N);
	//	PrintHafnode(Hafnode,7,N);//Test
	for (i=2;i<N;i++) HafnodeLine(i,Hafnode,N); //�ֲ���� HafnodeLine���� 
	//PrintHafnode(Hafnode,nodenum,N);//Test
	encoded(Ai,Hafnode,N); //���ı��� 
} 

int main()
{
	//double PArry[]={0.2,0.19,0.18,0.17,0.15,0.10,0.01,0}; //�Ѿ��ź������Դ����,��0��ʾ���� 
	double PArry[]={0.4,0.2,0.2,0.1,0.1,0};
	double ISEntropy=0;
	double AveCodeLen=0;
	int N=0;
	int i; 
	while (PArry[N]!=0) N++;  //������Դ���ŵĸ��� 
	
	struct SourceSign Ai[N];
	
	HaffmanCode(PArry,N,Ai);
	PrintAi(Ai,N);
	
	for (i=0;i<N;i++)
	{
		if(PArry[i]!=0) ISEntropy = ISEntropy+log10(PArry[i])*(PArry[i])*(-1)*3.322;
	}
	printf("  ��Դ�أ�%f\n",ISEntropy);
	
	for (i=0;i<N;i++)
	{
		AveCodeLen = AveCodeLen+Ai[i].Probability*Ai[i].codeLen;
	}
	printf("ƽ���볤��%f\n",AveCodeLen);
	printf("����Ч�ʣ�%f\n",ISEntropy/AveCodeLen);	
	return 0;
 } 
 
 
 void PrintHafnode(struct Pinode* Hafnode,int num,int N)
{
	int i,j;
	for (i=0;i<num;i++)
	{
		if (Hafnode[i].FloorNum!=Hafnode[i-1].FloorNum) printf("\n");
		printf("%3d. FloorNum=%d, OrderNum=%d, AllProbability=%.3f, BitSign=%d  ",i,Hafnode[i].FloorNum,Hafnode[i].OrderNum,Hafnode[i].AllProbability,Hafnode[i].BitSign);
		printf("include SourceNum:");
		for (j=0;j<N;j++)
		{
			if (Hafnode[i].SourceNum[j]==-1) break;
			else printf("  %d",Hafnode[i].SourceNum[j]);
		}
		printf("\n");		
	}
}
 
 void PrintAi(struct SourceSign *Ai,int N)
 {
 	printf("\n"); 
 	int i,j;
 	for (i=0;i<N;i++)
 	{
 		printf("%3d. Probability=%.3f  codelen=%d  ",i+1,Ai[i].Probability,Ai[i].codeLen);
 		printf("Code:");
 		for (j=19;j>=0;j--)  //�����ӡ ,��λ�ں��ȴ�ӡ,������λ������һ������ǰ�� 
 		{
 			if(Ai[i].code[j]!=-1)printf("%d",Ai[i].code[j]);
		 }
		 printf("\n");
	}
 }
 
