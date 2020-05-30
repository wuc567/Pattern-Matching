//SONG(Single-rOot Nettree for approximate pattern matchinG with flexible wildcards and length constraints)
//
//pattern P=a[1,3]t[2,4]c[5,13]g
//P stores in sub_ptns which are given as follows. 
//sub_ptn[0]=a[1,3]t
//sub_ptn[1]=t[2,4]c
//sub_ptn[2]=c[5,13]g


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <wtypes.h>


#define N 131075
struct sub_ptn_struct
{
	char start,end;		//
	int min,max;		//
};
int maxp_c=-1;
char S[N];
sub_ptn_struct sub_ptn[N];  //pattern p[i]

int MinLen;
int MaxLen;
int T;

int ptn_len=0;  //the length of pattern
int seq_len=0;
int sum_min=0;
int sum_max=0;
int W=0;

__int64 count=0;

struct node
{
	int name;
	__int64 *Nb,Ns;
	int *child;
};
struct level
{
	int size;
	node *array;
};
level *SONettree;
// To make it easy, we use arrays to store a Single-root Nettree.


int min_leaf(int root)
{
	int a=root+MinLen-1;
	int b=root+ptn_len+sum_min;
	int result=max(a,b);  //max
	return result;
}
int max_leaf(int root)
{
	int a=root+MaxLen-1;
	int b=seq_len-1;
	int c=root+ptn_len+sum_max;
	int result=min(a,b);  //min
	result=min(result,c);
	return result;
}

int max_root(int leaf)
{
	int a=leaf+1-MinLen;
	int b=leaf-ptn_len-sum_min;
	int result=min(a,b);	//min
	return result;
}
int *size;

void Inital()
{
	W=sub_ptn[0].max -sub_ptn[0].min +1;
	for (int i=1;i<ptn_len;i++)
	{	
		int temp=sub_ptn[i].max -sub_ptn[i].min +1;
		if (W<temp)
			W=temp;
	}
	SONettree=new level [ptn_len+1];
	size=new int [ptn_len+1];
	size[0]=1;
	for (i=1;i<=ptn_len;i++)
	{
		size[i]=size[i-1]+sub_ptn[i-1].max-sub_ptn[i-1].min; 
	}
	for (i=0;i<=ptn_len;i++)
	{
		SONettree[i].size =size[i];
		SONettree[i].array =new node [size[i]];
		
		
		for (int j=0;j<size[i];j++)
		{
			SONettree[i].array[j].child =new int [W];
			SONettree[i].array[j].Nb =new __int64 [T+1];
			SONettree[i].array[j].Ns =0;
		}
	}
	
}
void Release()
{
	for (int i=0;i<=ptn_len;i++)
	{
		for (int j=0;j<size[i];j++)
		{
			delete [] SONettree[i].array[j].child;
			delete [] SONettree[i].array[j].Nb;
		}
		delete [] SONettree[i].array;
	}
	delete [] size;
}
/*
Algorithm SONG.

\begin {algorithm}
\caption { SONG.\label{alg:SONG}}
Input: $P=p_0[min_0,max_0]p_1\cdots [min_{j-1},max_{j-1}]p_j\cdots [min_{m-2},max_{m-2}]p_{m-1}$,\\
$~~~~~~~~~~S=s_0\cdots s_i\cdots s_{n-1}$, $MinLen$, $MaxLen$, and $T$\\
Output: $N(P,S,LEN,T)$\\
1:for($r=0;r \leq  MaxRt;r++$ )\\
2:~~~~create root $n_1^r$ and initialize $N_b(n_1^r,n_1^r,d)$ and $N_s(n_1^r,n_1^r)$ according to concepts \ref{de:SBN} and \ref{de:SN}, respectively;\\
3:~~~~compute $MinLf(n_1^r)$ and $MaxLf(n_1^r)$ according to Equations (\ref{eq:MinLf}) and (\ref{eq:MaxLf}), respectively;\\
4:~~~~for($j=2;j \leq m ;j++$)\\
5:~~~~~~~~compute $MinBr_j(n^r_1)$ and $MaxBr_j(n^r_1)$ according to Equations (\ref{eq:MinBr}) and (\ref{eq:MaxBr}), respectively;\\
6:~~~~~~~~initialize the $j^{th}$ level nodes according to $MinBr_j(n^r_1)$ and $MaxBr_j(n^r_1)$, respectively;\\
7:~~~~~~~~for ($t=0;t<MaxBr_{j-1}(n^r_1)-MinBr_{j-1}(n^r_1)+1;t++$)\\
8:~~~~~~~~~~~~$k$=get the $t^{th}$ node on the $j-1^{th}$ level;\\
9:~~~~~~~~~~~~if $N_s(n_1^r, n_{j-1}^k)=0$ continue;  // according to Theorem \ref{le;nochild}\\
10:~~~~~~~~~~~compute $MinChd(n^k_{j-1})$ and $MaxChd(n^k_{j-1})$ according to Equations (\ref{eq:MinChd}) and (\ref{eq:MaxChd}), respectively;\\
11:~~~~~~~~~~~for($i=MinChd(n^k_{j-1});i \leq MaxChd(n^k_{j-1});i++$)\\
12:~~~~~~~~~~~~~~~compute $N_b(n_1^r,n_j^i,d)$ and $N_s(n_1^r,n_j^i)$ according to Equations (\ref{eq:SBN}) and (\ref{eq:SN}), respectively;\\
13:~~~~~~~~~~~end for\\
14:~~~~~~~~end for\\
15:~~~~end for\\
16:~~~~compute $R_s(n_1^r)$ according to Property 7;\\
17:end for\\
18:compute $N(P,S,LEN,T)$ according to Property 8;
\end {algorithm}


*/
void Initial_thislevel(int j)
{
	for (int i=0;i<SONettree[j].size ;i++)
	{
		SONettree[j].array [i].name =0;
		SONettree[j].array [i].Ns =0;

		for (int b=0;b<T+1;b++)
			SONettree[j].array [i].Nb[b]=0; 
	}
}
__int64 SONG()
{
	__int64 sum=0;
	int maxrt=max_root(seq_len-1);
	Inital();
	for (int r=0;r<=maxrt;r++)
	{
		//2:~~~~create root $n_1^r$ and compute $N_b(n_1^r,n_1^r,d)$ and $N_s(n_1^r,n_1^r)$ according to concepts \ref{de:SBN} and \ref{de:SN}, respectively;

		SONettree[0].array [0].name =r;
		if (S[r]==sub_ptn[0].start)
		{
			SONettree[0].array[0].Ns =1;
			SONettree[0].array[0].Nb[0]=1;
			for (int k=1;k<=T;k++)
				SONettree[0].array[0].Nb[k]=0;
		}
		else if (S[r]!=sub_ptn[0].start && T>0)
		{
			SONettree[0].array[0].Ns =1;
			for (int k=0;k<=T;k++)
				SONettree[0].array[0].Nb[k]=0;
			SONettree[0].array[0].Nb[1]=1;
		}
		else if (S[r]!=sub_ptn[0].start && T==0)
		{
			//
			continue;
		}
		//3:~~~~compute \textit{MinLf}($n_1^r$) and \textit{MaxLf}($n_1^r$) according to equations (\ref{eq:MinLf}) and (\ref{eq:MaxLf}), respectively;
		int minlf=min_leaf(r);
		int maxlf=max_leaf(r);

		int sum_minj=0,sum_maxj=0;
		for (int j=1;j<=ptn_len;j++)//ptn_len=m-1   
		{
			//create the $j^{th}$ level nodes according to rules 2 and 3;
			sum_minj+=sub_ptn[j-1].min ;
			sum_maxj+=sub_ptn[j-1].max;
	
			int sum_last_minj=0,sum_last_maxj=0;
			for (int k=j;k<ptn_len;k++)
			{
				sum_last_minj+=sub_ptn[ptn_len-k].min;
				sum_last_maxj+=sub_ptn[ptn_len-k].max;
			}

			int minbr,maxbr;
			// according to equations 13 and 14
			if (j<ptn_len)
			{	
				minbr=max(r+j+sum_minj,minlf - ptn_len+j-sum_last_maxj);
				maxbr=min(r+j+sum_maxj,maxlf - ptn_len+j-sum_last_minj) ;
			}
			else
			{
				minbr=minlf;
				maxbr=maxlf;
			}
			SONettree[j].size=maxbr-minbr+1;
			
			Initial_thislevel(j);

			for ( k=0;k<SONettree[j-1].size ;k++ )
			{
				int parent=SONettree[j-1].array[k].name ;
				if (SONettree[j-1].array[k].Ns ==0)//This node has no child. 
					continue;

				int fs=max(parent+1+sub_ptn[j-1].min,minbr);
				int ls=min(parent+1+sub_ptn[j-1].max,maxbr);
				
				for (int child=fs ;child<=ls;child++)
				{
					int position=child-minbr;
					SONettree[j].array [position].name =child;

					int distance;
					if (j<ptn_len)
						distance=(S[child]==sub_ptn[j].start )?0:1;
					else
						distance=(S[child]==sub_ptn[ptn_len-1].end )?0:1;

					//12:~~~~~~~~~~~~~~compute $N_b(n_1^r,n_j^i,d)$ and $N_s(n_1^r,n_j^i)$ according to equations (\ref{eq:SBN}) and (\ref{eq:SN}), respectively;

					if (distance==0)
					{
						//
						SONettree[j].array [position].Ns+=SONettree[j-1].array[k].Ns ;
						for (int b=0;b<=T;b++)
							SONettree[j].array [position].Nb[b]+=SONettree[j-1].array[k].Nb[b] ; 				
					}
					else
					{
						//
						for (int b=1;b<=T;b++)
						{
							SONettree[j].array [position].Nb[b]+=SONettree[j-1].array[k].Nb[b-1] ; 				
							SONettree[j].array [position].Ns+=SONettree[j-1].array[k].Nb[b-1];
						}
						
					}
				}
			}
		}
		__int64 rs=0;
		for (int k=0;k<SONettree[ptn_len].size ;k++)
		{
			rs+=SONettree[ptn_len].array [k].Ns ;
		}
		sum+=rs;
//		printf("The number of the approximate matching is:%I64d\n",sum);
	}
	Release();
	return sum;
}

bool isalpha(char a)     //determine a is a number or character
{
	if ((a>=97 && a<=96+26)||(a>=65 && a<=64+26))
		return 1;
	else 
		return 0;
}
void dealrange(char *p,int len)      
//put sub-pattern "a[1,3]b" into sub_ptn and sub_ptn.start=a£¬sub_ptn.end=b, sub_ptn.min=1£¬sub_ptn.max=3
{
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

		sum_min += sub_ptn[ptn_len].min;
		
		value=0;
		for (int j=i+1;p[j]!=']';j++)
			value=value*10+p[j]-48;
		sub_ptn[ptn_len].max=value;
		sum_max +=sub_ptn[ptn_len].max;
	}
	if (sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1>maxp_c)
		maxp_c=sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1;
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
		printf("irregular pattern");
}
void disp_pattern()         //display the array sub_ptn
{
	for (int i=0;i<ptn_len;i++)
	{
		printf("%c\t%d\t%d\t%c\n",sub_ptn[i].start ,sub_ptn[i].min , sub_ptn[i].max ,sub_ptn[i].end );
	}
}

void Inputstr(char *fn,char *str)
{
	FILE* fp=fopen(fn,"r+");
	if (NULL==fp)
	{
		printf("Cannot find the file\n");
		return;
	}
	fseek(fp,0,SEEK_END); 
	int len =ftell(fp);
	fseek(fp,0,0); 
	fscanf(fp, "%s",str);
}

int main(int argc, char* argv[])
{
	
	char fpn[]="p1.txt";
	char fsn[]="s44.txt";
	//MinLen=11,MaxLen=15,T=2;
	MinLen=1,MaxLen=10,T=1;
	char p[1000]="a[0,2]g[0,2]t";
	strcpy(S,"aaggtt");
	//Inputstr(fpn,p);
	//Inputstr(fsn,S);
	//printf("Please input pattern at first:");
	//scanf("%s",p);
	convert_p_to_ruler(p);
	printf("The pattern can written as follows:\n");
	disp_pattern();
	printf("%s\n",p);
	
	//printf("\n\nPlease input the Sequence:\n");
	//scanf("%s",S);

	printf("\n\nPlease input Minlen and Maxlen:\n");
	//scanf("%d%d",&MinLen,&MaxLen);
	
	seq_len=strlen(S);

	printf("Please input the threshold:\n");
	//scanf("%d",&T);
	DWORD begintime=GetTickCount();
	__int64 count;
	for (int i=0;i<10;i++)
		count=SONG();
	printf("The cost of time is:%5d\n",(GetTickCount()-begintime)/10);
	printf("The number of the approximate matching is:%I64d\n",count);
	return 0;
}

