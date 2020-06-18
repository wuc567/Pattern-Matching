#include <iostream.h>
#include <afxwin.h>
struct punit
{
	char start,end; //
	int min,max; //
};
struct detail
{
	int name;
	int *distance;
	int total;
};
struct cell
{	
	int len;
	detail *gap;
	//int *p;
};
struct row
{
	cell *r;
};
row *table,*table2;

int maxgap=-1;
int delta;
#define N 100
#define M 1000
int MaxLen,MinLen;
punit sub_ptn[N];  //pattern p[i]
int ptn_len=0;  //the length of pattern
void dealrange(char *p,int len)      
{
//put sub-pattern "a[1,3]b" into sub_ptn and sub_ptn.start=a£¬sub_ptn.end=b, sub_ptn.min=1£¬sub_ptn.max=3
	sub_ptn[ptn_len].start =p[0];
	sub_ptn[ptn_len].end =p[len];
	if (len==1)
	{
		sub_ptn[ptn_len].max =sub_ptn[ptn_len].min=0;
	}
	else
	{
		int value=0;
		int i;
		for ( i=2;p[i]!=',';i++)
			value=value*10+p[i]-48;
		sub_ptn[ptn_len].min=value;		
		value=0;
		for (int j=i+1;p[j]!=']';j++)
			value=value*10+p[j]-48;
		sub_ptn[ptn_len].max=value;
	}
	if (sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1>maxgap)
		maxgap=sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1;
	ptn_len++;
}
void convert_p_to_ruler(char *p)     //transform p into sub_ptn
{
	char st,en;
	int l,r ;
	int len=strlen(p);
	st=p[0];
	en=p[len-1];
	if (isalpha(st) && isalpha(en))
	{
		l=0;
		for (int i=1;i<len;i++)
		{
			if (isalpha(p[i]))
			{
				r=i;
				dealrange(p+l,r-l);
				l=i;
			}
		}
	}
	else
	{
		cout<<"irregular pattern.\n";
		exit(-1);
	}
}
void Inputstr(char *fn,char *str)
{
	FILE* fp=fopen(fn,"r+");
	if (NULL==fp)
	{
		cout<<"Cannot find the file.\n";
		return;
	}
	fseek(fp,0,SEEK_END); 
	int len =ftell(fp);
	fseek(fp,0,0); 
	fscanf(fp, "%s",str);
}
void display(char *s,int T)
{
	int len=strlen(s);
	int col=ptn_len;
	int i,j,k,ii;
	for (i=0;i<len;i++)
	{
		for (j=0;j<col;j++)
		{
			for ( k=0;k<=table[i].r [j].len ;k++)
			{
				cout <<table[i].r [j].gap [k].name <<" "<<table[i].r [j].gap [k].total<<" " ;
				for (ii=0;ii<T+1;ii++)
				{
					cout<<table[i].r [j].gap [k].distance [ii]<<",";
				}
				cout <<"\t";
			}
			cout <<"\t\t";
		}
		cout <<endl<<endl;
	}
}
int calculate_delta_dis(int a,int b)
{
	/*
	if(a>b)
		return a-b;
	return b-a;
	*/
	return a-b>0?(a-b):(b-a);
}
void create_matching_lookup(char *s,int T)
{
	int len=strlen(s);
	int col=ptn_len;
	
	table=new row[len];
	int i,j,k,ii;
	for (i=0;i<len;i++)
	{
		table[i].r =new cell [col];
		for (j=0;j<col;j++)
		{
			int gaplen=sub_ptn[j].max-sub_ptn[j].min +1;
			table[i].r [j].gap =new detail [gaplen];
			for ( k=0;k<gaplen;k++)
			{
				table[i].r [j].gap [k].distance =new int [T+1];
				for (ii=0;ii<T+1;ii++)
				{
					table[i].r [j].gap [k].distance[ii]=0;
				}
			}
			table[i].r [j].len =-1;
		}
	}
	for (i=0;i<len;i++)
	{		
		for (j=0;j<col;j++)
		{
			int length=0;
			for (k=i+sub_ptn[j].min +1;k<len &&k<=i+sub_ptn[j].max +1;k++)
			{
				table[i].r [j].len =length;
				table[i].r [j].gap [length].name =k;
				//determine the start and end
				//int d1=calculate_delta_dis(s[i],sub_ptn[j].start);
				//int d2=calculate_delta_dis(s[k],sub_ptn[j].end);
				int d2;
				if (s[k] >= sub_ptn[j].end)
				{
					d2 = s[k] - sub_ptn[j].end;
				}
				else if (s[k] < sub_ptn[j].end)
				{
					d2 = sub_ptn[j].end - s[k];
				}

				//if(d1 <=delta && d2 <=delta && d1+d2 <=T){
				if(d2 <=delta){
					//the delta and gamma distances are satisfied
					table[i].r [j].gap [length].distance[d2]=1;
					table[i].r [j].gap [length].total =1;
				}else{
					table[i].r [j].gap [length].total =0;
				}

				/* Hamming distance code
				int end=(s[k]!=sub_ptn[j].end);
				if (end<=T)
				{
					table[i].r [j].gap [length].distance[end]=1;
					table[i].r [j].gap [length].total =1;
				}
				else
				{
					table[i].r [j].gap [length].total =0;
				}

				*/
				length++;
			}
		}
	}
	//display(s,T);
}


void create_matching_table(char *s,int T)
{
	int len=strlen(s);
	int col=ptn_len;
	int maxl=col;
	int *start_position;
	start_position=new int [col+1];
	int *size;
	size=new int [col+1];

	table2=new row[len];
	int i,j,k;
	start_position[0]=sub_ptn[0].min+1;//s start position
	size[0]=sub_ptn[0].max - sub_ptn[0].min+1;
	for (i=1;i<col;i++)
	{
		size[i]=size[i-1]+sub_ptn[i].max - sub_ptn[i].min+1;
		start_position[i]=start_position[i-1]+sub_ptn[i].min+1;
	}
	for (i=0;i<len;i++)
	{
		table2[i].r =new cell [col];
		for (j=0;j<col;j++)
		{
			table2[i].r [j].gap =new detail [size[j]];
			for ( k=0;k<size[j];k++)
			{
				table2[i].r [j].gap [k].distance =new int [T+1];
				for (int ii=0;ii<T+1;ii++)
				{
					table2[i].r [j].gap [k].distance[ii]=0;
					table2[i].r [j].gap [k].total =0;
				}
			}
			table2[i].r [j].len =-1;
		}
	}
	
	/*

	for(i=0;i<len;i++)
	{
		if (table[i].r[0].len ==-1)
		{
			continue;
		}
		
		//Assign Table 1 to Table 2
		table2[i].r[0].len=table[i].r [0].len;
		for (k=0;k<=table[i].r [0].len;k++ )
		{
			table2[i].r [0].gap [k]=table[i].r [0].gap [k];
		}
		
		for (k=0;k<=table2[i].r[0].len;k++)
		{
			detail current;  //The current node is the k-th node in column j-1
			current=table2[i].r[0].gap [k];
			if (current.total ==0)
			{
				continue;
			}
			for (int d=0;d<=T;d++)
			{
				if (current.distance [d]==0)
				{
					continue;
				}
				//j=1时，将第一个字符的距离乘进去
				//i为总的行数， j为总的列数， ii为下一列的行数	
				//判断s[0]的距离
				//初始化第一个字符的近似度数组
				int *ss1 =new int [T+1];
				if (calculate_delta_dis(s[i],sub_ptn[0].start)<=delta)
				{
					//iii为当前第一个节点的数组下标
					for (int iii=0;iii<T+1;iii++)
					{
						ss1[iii]=0;
					}
					ss1[calculate_delta_dis(s[i],sub_ptn[0].start)]=1;
				}else{
					for (int iii=0;iii<T+1;iii++)
					{
						ss1[iii]=0;
					}
				}
				//int position=list.gap [ii].name -start_position[0]-i;//s中的位置
				//当前结点，路径数交叉相乘
				for (int p = 0; p < table2[i].r [0].len; ++p)
				{
					for (int kk=0;kk<=T-d;kk++)
					{
						table2[i].r [0].gap [p].distance [kk+d]+=table2[i].r [0].gap [p].distance [d]*ss1[kk];
					}
				}
			}
		}
	}
	*/

	for (i=0;i<len;i++)
	{
		if(table[i].r [0].len ==-1)
		{
			//it is an empty row.
			continue;
		}
		//Assign Table 1 to Table 2
		table2[i].r[0].len=table[i].r [0].len;
		for (k=0;k<=table[i].r [0].len;k++ )
		{
			table2[i].r [0].gap [k]=table[i].r [0].gap [k];
		}

		for (j=1 ;j<col;j++)
		{
			int len=table2[i].r[j-1].len;
			for (k=0;k<=len;k++)
			{
				detail current;  //The current node is the k-th node in column j-1
				current=table2[i].r [j-1].gap [k];
				if (current.total ==0)
				{
					continue;
				}
				int name=current.name;
				cell list=table[name].r [j];//list is the position where the next layer of current meets the condition			
				for (int d=0;d<=T;d++)
				{
					if (current.distance [d]==0)
					{
						continue;
					}
					for (int ii=0;ii<=list.len ;ii++)
					{					
						//merge tables
						for (int kk=0;kk<=T-d;kk++)
						{
							int position=list.gap [ii].name -start_position[j]-i;//position in s
							//Multiply the current node and the number of paths
							table2[i].r [j].gap [position].distance [kk+d]+=current.distance [d]*list.gap [ii].distance [kk];
							//
							table2[i].r [j].gap [position].total +=current.distance [d]*list.gap [ii].distance [kk];
							table2[i].r [j].gap [position].name=list.gap [ii].name; 
							if (table2[i].r [j].len <position+1)
							{
								table2[i].r [j].len=position+1;
							}
						}
					}
				}
			}
		}	
	}
	// cout<<"================================================"<<endl;
	// for (i=0;i<len;i++)
	// {
	// 	for (j=0;j<col;j++)
	// 	{
	// 		for ( k=0;k<=table2[i].r [j].len ;k++)
	// 		{
	// 			cout <<table2[i].r [j].gap [k].name <<" "<<table2[i].r [j].gap [k].total<<" " ;
	// 			for (int ii=0;ii<T+1;ii++)
	// 			{
	// 				cout<<table2[i].r [j].gap [k].distance [ii]<<",";
	// 			}
	// 			cout <<"  ";
	// 		}
	// 		cout <<"-----";
	// 	}
	// 	cout <<endl<<endl;
	// }

	delete []start_position;
}
long calculate(char *s,int T)
{
	int len=strlen(s);
	int col=ptn_len;
	int maxl=0;
	int i,j;
	for (i=0;i<col;i++)
	{
		maxl+=sub_ptn[i].max-sub_ptn[i].min+1 ;
	}
	/*for (i=0;i<len;i++)
	{
		cout <<i<<":\t";
		for (j=0;j<maxl;j++)	
		{
			detail tmp=table2[i].r [col-1].gap [j];
			//cout <<tmp.name <<"\t";
			int kk=tmp.name-i+1;
			if (kk<=MaxLen&&kk>=MinLen)
			{
				cout <<tmp.name <<":";
				for (int k=0;k<=T;k++)
				{
					cout <<"<"<<tmp.distance [k]<<">, ";
				}
				cout <<tmp.total <<"\t";
			}
		}
		cout <<endl;
	}*/

	/*
	long sum=0;
	for (i=0;i<len;i++)
	{
		for (j=0;j<maxl;j++)	
		{
			detail tmp=table2[i].r [col-1].gap [j];
			int kk=tmp.name-i+1;
			if (kk<=MaxLen&&kk>=MinLen)
			{

				if (s[i]==sub_ptn[0].start )
					sum+=tmp.total ;
				else
				{
					for (int k=0;k<=T-1;k++)
					{
						sum+=tmp.distance [k];
					}
				}
			}
		}
	}
	*/
	long sum=0;
	for (i=0;i<len;i++)
	{
		for (j=0;j<maxl;j++)	
		{
			detail tmp=table2[i].r [col-1].gap [j];
			int kk=tmp.name-i+1;

			
			if (kk<=MaxLen&&kk>=MinLen)
			{
				if (s[i]==sub_ptn[0].start)
					sum+=tmp.total;
				else
				
				{
					int d;
					if (s[i] >= sub_ptn[0].start)
					{
						d = s[i] - sub_ptn[0].start;
					}
					else if (s[i] < sub_ptn[0].start)
					{
						d = sub_ptn[0].start - s[i];
					}
					for (int k=0;k<=T-d;k++)					
					{
						sum+=tmp.distance [k];
					}
				}
			}
		}
	}
	return sum;
}
void release(char *s)
{
	int len=strlen(s);
	int col=ptn_len;
	int i;
	int *size;
	size=new int [col+1];
	size[0]=sub_ptn[0].max - sub_ptn[0].min+1;
	for (i=1;i<col;i++)
	{
		size[i]=size[i-1]+sub_ptn[i].max - sub_ptn[i].min+1;
	}		
	for (i=0;i<len;i++)
	{
		for (int j=col-1;j>=0;j--)
		{
			for (int k=sub_ptn[j].max-sub_ptn[j].min ;k>=0;k--)
			{
 				delete []table[i].r [j].gap [k].distance ;
			}
			delete []table[i].r[j].gap ;
		}
		delete []table[i].r ;
	}
	delete []table;
	for (i=0;i<len;i++)
	{
		for (int j=col-1;j>0;j--)
		{
			for (int k=size[j]-1;k>=0;k--)
			{
				delete []table2[i].r [j].gap [k].distance ;
			}
			delete []table2[i].r[j].gap ;
		}
		delete []table2[i].r;
	}
	delete []table2;
}
char s[10000];
void main()
{
	int T;
	//cout <<"Please input MinLen and MaxLen:";
	//cin>>MinLen>>MaxLen;
	cout <<"Please input the distance:\n";
	cin >>delta>>T;
	// char fpn[]="p2.txt";
	
	char fsn1[]="Protein\\A0A3Q3L924.txt";
	char fsn2[]="Protein\\A0A3B4UA59.txt";
	char fsn3[]="Protein\\A0A2N8LHF0.txt";
	char fsn4[]="Protein\\A0A078K571.txt";
	char fsn5[]="Protein\\A0A383R7Y4.txt";
	char fsn6[]="Protein\\A0A1E7WGV1.txt";
	char fsn7[]="Protein\\A0A3B0KSX7.txt";
	char fsn8[]="Protein\\A0A388K3K1.txt";
	char fsn9[]="Protein\\time_data1.txt";
	char fsn10[]="Protein\\time_data2.txt";
//	char p[1000];
	//MinLen=2,MaxLen=10,T=2;

	MinLen=11,MaxLen=40;
//	MinLen=13,MaxLen=38;
//	MinLen=15,MaxLen=55;
//	MinLen=16,MaxLen=40;
//	MinLen=11,MaxLen=35;

	char p1[1000]="D[2,9]F[1,8]E[0,10]K[2,6]A[3,7]G";
	char p2[1000]="E[1,9]D[0,8]A[2,7]L[1,8]Q[4,9]R[3,8]U";
	char p3[1000]="S[1,9]K[3,11]D[1,7]F[1,9]P[3,8]G[2,6]A[0,5]L";
	char p4[1000]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S";
	char p5[1000]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S[0,7]D[0,7]F";
	char p6[1000]="G[0,8]D[0,8]S[0,8]A[0,8]F[0,8]T";
	char p7[1000]="G[0,7]D[0,7]S[0,7]A[0,7]F[0,7]T";
	char p8[1000]="G[0,6]D[0,6]S[0,6]A[0,6]F[0,6]T";

	//Inputstr(fsn1,s);
	//Inputstr(fsn2,s);
	//Inputstr(fsn3,s);
	//Inputstr(fsn4,s);
//	Inputstr(fsn5,s);
	//Inputstr(fsn6,s);
	//Inputstr(fsn7,s);
//	Inputstr(fsn8,s);
	Inputstr(fsn9,s);
	//Inputstr(fsn9,s);

	//char p[M]="a[0,2]t[0,2]c";
	//char s[100000]="aattcc";
	//Inputstr("ggaagacc.txt",s);
	DWORD dwBeginTime=GetTickCount();
	long occ;
	
	convert_p_to_ruler(p1);
	create_matching_lookup(s,T);
	create_matching_table(s,T);	
	occ=calculate(s,T);
	
	//release(s);
	DWORD dwEndTime=GetTickCount();
	cout <<"Time cost1:"<<(dwEndTime-dwBeginTime)<<"ms\n";
	release(s);
	cout <<occ<<endl;
	//cout <<"Time cost:"<<(dwEndTime-dwBeginTime)/5<<"ms\n";
	//cout <<"Time cost2:"<<(dwEndTime-dwBeginTime)<<"ms\n";

}
