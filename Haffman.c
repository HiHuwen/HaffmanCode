#include<stdio.h>
#include<math.h>
struct Pinode 				//定义哈夫曼节点 
{
	int FloorNum; 			//该节点所在的层数 
	int OrderNum; 			//该节点所在层数的序号(按照概率大小排序) 
	double AllProbability;  //该节点所继承前父节点的概率和 
	char BitSign;           //该节点分配得到的二进制符号(1或0,没有用2表示) 
	int SourceNum[256];     //该节点内包含了概率和的那些信源符号序号(ASCII码为256个,可以编译256以内的符号数) 
};

struct SourceSign			//定义 信源符号结构体 
{
	double Probability;		//该信源符号的概率 
	char code[20];			//码字:该信源符号的二进制编码 ,低位在前,高位在后,最大长度为20,未分配的用-1表示 
	int codeLen;			// 码长 
};

void PrintHafnode(struct Pinode* Hafnode,int num,int N); 	//打印所有哈夫曼节点函数声明 
void PrintAi(struct SourceSign *Ai,int N);					//打印所有信源符号信息函数声明 


//初始化哈夫曼节点,同时把信源符号概率赋值给第一层的哈夫曼节点
//Ai参数是信源符号数组, Hafnode参数是需要进行初始化的哈夫曼节点的数组,N表示信源符号个数 
void initHafnode(struct SourceSign *Ai,struct Pinode* Hafnode,int N)
{
	int i,j;
	int nodenum = (2+N)*(N-1)/2;  //公式计算需要使用的哈夫曼节点的个数 
	for (i=0;i<nodenum;i++) Hafnode[i].OrderNum=-1; //对哈夫曼节点的序号都赋初始值-1 
	for (i=0;i<N;i++)                               		//第一层的数据初始化 
	{
		Hafnode[i].AllProbability=Ai[i].Probability;		//赋值信源符号的概率 
		Hafnode[i].SourceNum[0]=i+1;						//第一次赋值所包含的信源符号 
		Hafnode[i].FloorNum=1;								//赋值层数为第一层	
		Hafnode[i].OrderNum=i+1;							//根据输入信源符号(已经排好序)的顺序 赋值该层的顺序 
		Hafnode[i].BitSign=2;								//统一先初始化分配的符号为2 
		for (j=1;j<N;j++) 	Hafnode[i].SourceNum[j]=-1;		// SourceNum[0]记录了第一个的信源符号,后面的初始化为-1 
	}
	Hafnode[N-1].BitSign=1;									//对第一层的最后一给节点赋予二进制符号1 
	Hafnode[N-2].BitSign=0;									//对第一层的最后一给节点赋予二进制符号0
	
}

//对该层的哈夫曼节点进行分配所有的信息 
//输入当前的层数 FloorN,需要分配信息的节点数组Hafnode,信源符号个数N 
void HafnodeLine(int FloorN,struct Pinode* Hafnode,int N)
{
	int count = N-FloorN+1; 										//计算当前层数中的节点个数 
	int beginNum = (FloorN-FloorN*FloorN)/2+N*FloorN+FloorN-N-1; 	//计算当前层中第一个节点下标的数值(等差公式计算) 
	int endNum = beginNum+count-1;									//计算当前层中 最后一个节点的下标值 
	int i,j,k,lastA,lastB,Newnode;
	double NewProbability;
	double tempProbability = 0; 
//	printf("count=%d  ,beginNum=%d  ,endNum=%d\n",count,beginNum,endNum); //test
	for (i=beginNum-1;i>=beginNum-count-1;i--)     //寻找上一层中概率最小的两个节点的下标(通过序号查找) 
	{
		if (Hafnode[i].OrderNum==count+1) lastA=i;
		if (Hafnode[i].OrderNum==count)	lastB=i;
	}
	NewProbability = Hafnode[lastA].AllProbability+Hafnode[lastB].AllProbability; //将一层中概率最小的两个节点概率求和 
//	printf("NewProbability=%.3f\n",NewProbability);//test
	i = beginNum;
	for (j=beginNum-1;j>=beginNum-count-1;j--)
	{
		if ((j!=lastA)&&(j!=lastB)) //对非以上最小概率的节点进行直接赋值 
		{
			Hafnode[i].AllProbability=Hafnode[j].AllProbability;
			Hafnode[i].FloorNum = FloorN;
			Hafnode[i].BitSign = 2;
			for (k=0;k<N;k++) 	Hafnode[i].SourceNum[k] = Hafnode[j].SourceNum[k]; //继承上一个节点的信源符号 
			i++;
		}
	}
	Newnode = i;
	//以下行代码是对新生成的概率和节点进行各项信息的赋值 
	Hafnode[Newnode].AllProbability =  NewProbability;//得到概率和 
	Hafnode[Newnode].FloorNum=FloorN;					//得到所在层数 
	Hafnode[Newnode].BitSign=2;						//预先赋初值 
	for (k=0;k<N;k++) Hafnode[Newnode].SourceNum[k]= Hafnode[lastA].SourceNum[k];//直接继承上一层中最小概率节点的信源符号 
	j=0;
	for (k=0;k<N;k++)
	{
		if (Hafnode[Newnode].SourceNum[k]==-1) //由于前面 SourceNum[k]中预先有了信源符号, 继承上一层中第二小概率节点的信源符号要对值为-1的值进行覆盖 
		{
			Hafnode[Newnode].SourceNum[k]= Hafnode[lastB].SourceNum[j];
			j++;	
		}
	}
	
	//根据概率大小进行排序,序号分配给OrderNum
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
	//如果概率和节点的概率值和前一排名的概率值相等则 交换他两的排名，这样保证合并概率出现在上面，获得较小的码方差 
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
	
	//对概率最小的两个节点进行分配二进制符号0和1 
	for (i=beginNum;i<=endNum;i++)
	{
		if (Hafnode[i].OrderNum==count) Hafnode[i].BitSign=1;
		if (Hafnode[i].OrderNum==count-1) Hafnode[i].BitSign=0;
	}		
}

//该函数是通过已经赋值好的节点来进行信源二进制符号的分配 
// Ai参数是信源符号数组, Hafnode参数是需要进行初始化的哈夫曼节点的数组,N表示信源符号个数 
void encoded(struct SourceSign *Ai,struct Pinode* Hafnode,int N)
{
	int nodenum = (2+N)*(N-1)/2; //公式计算需要使用的哈夫曼节点的个数
	int i,j,k,cnt;
	for (i=0;i<N;i++) for (j=0;j<20;j++) Ai[i].code[j]=-1; //对二进制符号先全赋初始值-1 
	
	for (i=0;i<nodenum;i++)
	{
		if (Hafnode[i].BitSign!=2)  //找到分配了0,1二进制符号的哈夫曼节点 
		{
			//printf("i=%d bit=%d \n",i,Hafnode[i].BitSign);//test
			for (j=0;j<N;j++)
			{
				if (Hafnode[i].SourceNum[j]!=-1) //找出该哈夫曼节点中带有的信源 
				{
					//printf("i=%d. %d  j=%d  \n",i,Hafnode[i].SourceNum[j],j);//test
					cnt=Hafnode[i].SourceNum[j]-1; //找出该哈夫曼节点中带有的信源数和下标相差1 
					for (k=0;k<20;k++)
					{
						if (Ai[cnt].code[k]==-1) //对是-1的值进行覆盖,以免影响前面已经分配的值 
						{
							Ai[cnt].code[k]=Hafnode[i].BitSign;
							break;
						}
					}
				}
			}
		}
	}
	//计算每个信源符号的字长 
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
	int nodenum = (2+N)*(N-1)/2 ;	//公式计算需要使用的哈夫曼节点的个数
	for (i=0;i<N;i++)  Ai[i].Probability =	ProbabilityArry[i];  //将概率数组的值赋给结构体	
	struct Pinode Hafnode[nodenum];
	initHafnode(Ai,Hafnode,N);
	//	PrintHafnode(Hafnode,7,N);//Test
	for (i=2;i<N;i++) HafnodeLine(i,Hafnode,N); //分层调用 HafnodeLine函数 
	//PrintHafnode(Hafnode,nodenum,N);//Test
	encoded(Ai,Hafnode,N); //最后的编码 
} 

int main()
{
	//double PArry[]={0.2,0.19,0.18,0.17,0.15,0.10,0.01,0}; //已经排好序的信源概率,以0表示结束 
	double PArry[]={0.4,0.2,0.2,0.1,0.1,0};
	double ISEntropy=0;
	double AveCodeLen=0;
	int N=0;
	int i; 
	while (PArry[N]!=0) N++;  //计算信源符号的个数 
	
	struct SourceSign Ai[N];
	
	HaffmanCode(PArry,N,Ai);
	PrintAi(Ai,N);
	
	for (i=0;i<N;i++)
	{
		if(PArry[i]!=0) ISEntropy = ISEntropy+log10(PArry[i])*(PArry[i])*(-1)*3.322;
	}
	printf("  信源熵：%f\n",ISEntropy);
	
	for (i=0;i<N;i++)
	{
		AveCodeLen = AveCodeLen+Ai[i].Probability*Ai[i].codeLen;
	}
	printf("平均码长：%f\n",AveCodeLen);
	printf("编码效率：%f\n",ISEntropy/AveCodeLen);	
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
 		for (j=19;j>=0;j--)  //倒序打印 ,高位在后先打印,这样高位出现在一列数的前面 
 		{
 			if(Ai[i].code[j]!=-1)printf("%d",Ai[i].code[j]);
		 }
		 printf("\n");
	}
 }
 
