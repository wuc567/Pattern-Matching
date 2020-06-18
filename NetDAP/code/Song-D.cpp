//SONG(Single-rOot Nettree for approximate pattern matchinG with flexible wildcards and length constraints)
//
//pattern P=a[1,3]t[2,4]c[5,13]g
//P stores in sub_ptns which are given as follows. 
//sub_ptn[0]=a[1,3]t
//sub_ptn[1]=t[2,4]c
//sub_ptn[2]=c[5,13]g



#include <string>
#include <cctype>
#include <malloc.h>
#include <wtypes.h>
#include <iostream>
#include <fstream>
using namespace std;



#define N 1000000
#define M 100
#define K 100000

struct sub_ptn_struct
{
	char start,end;		//
	int min,max;		//
};
int maxp_c=-1;
char S[N];

struct seqdb                 
{
	int id;                  // sequence id  
	char S[N];               // sequence  
} sDB[K];

sub_ptn_struct sub_ptn[N];  //pattern p[i]

int MinLen;
int MaxLen;
int d;

int ptn_len=0;  //the length of pattern
int seq_len=0;
int sum_min=0;
int sum_max=0;
int W=0;
int NODE;
int NONP_1=0;
int NONP_2=0;

int count=0;



struct node
{
	int name;
	int delta;
	int *Nb;
	int *child;
};
struct level
{
	int size;
	node *array;
};
level *SONettree;
// To make it easy, we use arrays to store a Single-root Nettree.

//minial leaf
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
//maxmal root
int max_root(int leaf)
{
	int a=leaf+1-MinLen;
	int b=leaf-ptn_len-sum_min;
	int result=min(a,b);	//min
	return result;
}
int *size;

//--------------------Ãƒâ€šÃ‚Â³ÃƒÆ’Ã‚ÂµÃƒÆ’Ã…Â Ãƒâ€šÃ‚Â¼Ãƒâ€šÃ‚Â»Ãƒâ€šÃ‚Â¯
void Inital(int G)
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
			SONettree[i].array[j].Nb =new int [G+1];
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


void Initial_thislevel(int j, int G)
{
	for (int i=0;i<SONettree[j].size ;i++)
	{
		SONettree[j].array [i].name =0;
        SONettree[j].array [i].delta =-1;
		for(int k=0;k<=G;k++)
			SONettree[j].array[i].Nb[k]=0;
		
	
	}
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int SONG(int d, int G)
{
	NODE=0;
	NONP_1=NONP_2=0;
	int sum=0;
	int maxrt=max_root(seq_len-1);//range of max-root
	Inital(G);
	//1:
	for (int r=0;r<=maxrt;r++)
	{//root
		//2:~~~~create root $n_1^r$ and compute $N_b(n_1^r,n_1^r,d)$ and $N_s(n_1^r,n_1^r)$ according to concepts \ref{de:SBN} and \ref{de:SN}, respectively;
        for(int l=0;l<=G;l++)
		    SONettree[0].array[0].Nb[l]=0;
		SONettree[0].array [0].name =r;
		if(sub_ptn[0].start -S[r]<=0)
			SONettree[0].array [0].delta=S[r]-sub_ptn[0].start ;
		else if(sub_ptn[0].start -S[r]>=0)
			SONettree[0].array [0].delta=sub_ptn[0].start-S[r];
		//cout<<"SONettree[0].array [0].delta="<<SONettree[0].array [0].delta<<endl;

		// ----------theorem 1----------
		if(SONettree[0].array [0].delta>d){
			continue;
		}
		// ----------theorem 1----------
		//initialise NR(n) of root node
		if (S[r]==sub_ptn[0].start)
		{
			SONettree[0].array[0].Nb[0]=1;
			for (int k=1;k<=G;k++)
				SONettree[0].array[0].Nb[k]=0;
		}
		else if (S[r]!=sub_ptn[0].start && G>0)
		{
			for (int k=0;k<=G;k++)
				SONettree[0].array[0].Nb[k]=0;
			SONettree[0].array[0].Nb[SONettree[0].array[0].delta]=1;
		}
		else if (S[r]!=sub_ptn[0].start && G==0)
		{
			continue;
		}
		//3:~~~~compute \textit{MinLf}($n_1^r$) and \textit{MaxLf}($n_1^r$) according to equations (\ref{eq:MinLf}) and (\ref{eq:MaxLf}), respectively;
		int minlf=min_leaf(r);
		int maxlf=max_leaf(r);
		int sum_minj=0,sum_maxj=0;
		
		for (int j=1;j<=ptn_len;j++)//ptn_len=m-1   
		{//P
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
			Initial_thislevel(j,G);      
			for ( k=0;k<SONettree[j-1].size ;k++ )
			{
				
				//if a nodoe great than d ,we delete this node.
				int parent=SONettree[j-1].array[k].name ;
				//
				int fs=max(parent+1+sub_ptn[j-1].min,   minbr);
				int ls=min(parent+1+sub_ptn[j-1].max,   maxbr);
					
				for (int child=fs; child<=ls; child++)
				{
					int position=child-minbr;//
					SONettree[j].array[position].name =child;
				//	cout<<"S[child]="<<S[child]<<"   sub_ptn[j].start="<<sub_ptn[j-1].start<<endl;
					if(S[child]-sub_ptn[j-1].end<=0)
						SONettree[j].array [position].delta=-S[child]+sub_ptn[j-1].end ;
					else if(S[child]-sub_ptn[j-1].end>=0)
						SONettree[j].array [position].delta=S[child]-sub_ptn[j-1].end;
				//	cout<<SONettree[j].array [position].delta<<"-----"<<endl;
					if(SONettree[j].array [position].delta>d){
						continue;
					}
					int D=SONettree[j].array [position].delta;
					//
					for (int b=0;b<=G-D;b++)
					{
						if(SONettree[j-1].array[k].Nb[b]!=0){
							break;
						}	
						SONettree[j].array[position].Nb[b+D]+=SONettree[j-1].array[k].Nb[b] ; 
					}
					//cout<<endl;	
					//useless node
					bool flag = false;
					for (int t = 0; t <= G; t++)
					{
						if (SONettree[j].array[position].Nb[t]!=0)
						{
							flag = true;
							//NONP_1++;
						}
						break;
					}
					if (flag == true)
					{
						//NODE++;
					 	SONettree[j].array[position].name =child;
					}
				}//c
			}
		//----------------------------------------------------------------------------------------------
		}//P*/
		
		int rs=0;
		for (int k=0; k<SONettree[ptn_len].size; k++)
		{
			for(int j=0;j<=G;j++)
				rs+=SONettree[ptn_len].array[k].Nb[j];
				//cout<<"rs="<<rs;
		}
		sum+=rs;
	}//root
	//cout<<"sum="<<sum<<endl;
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
//put sub-pattern "a[1,3]b" into sub_ptn and sub_ptn.start=aÃƒâ€šÃ‚Â£Ãƒâ€šÃ‚Â¬sub_ptn.end=b, sub_ptn.min=1Ãƒâ€šÃ‚Â£Ãƒâ€šÃ‚Â¬sub_ptn.max=3
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
	ptn_len = 0;
	char st,en;
	int l,r ;
	int len=strlen(p);
	st=p[0];
	en=p[len-1];
	if (isalpha(st))
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

void read_file(char *p,int d,int G,int MinLen,int MaxLen)
{
	//cout<<G<<" "<<d;
	fstream file;
	string buff;
	file.open("Protein//A0A3Q3L924.txt",ios::in); //open sequence file
	int i=0;
	//int ourrence;1
	convert_p_to_ruler(p);
	//cout<<"1 ";

	while(getline(file, buff))
	{
		strcpy(sDB[i].S, buff.c_str());

		strcpy(S,sDB[i].S);
		//cout<<S<<"\n";
		seq_len=strlen(S);

		count+=SONG(d,G);		
		cout<<i<<": "<<count<<"\n";
		//cout<<"NONP_1:"<<NONP_1<<" "<<"NONP_2:"<<NONP_2<<endl;
		//cout<<"NODE"<<NODE;
		i++;
	}
	file.close();
}


int main(int argc, char* argv[])
{
	int G,d;
	cout<<"input d:"<<endl;
	cin>>d;
	cout<<"input G:"<<endl;
	cin>>G;
	//char fsn[]="demo.txt";
	MinLen=11,MaxLen=35;//length constraits

//	char p[200]="D[2,9]F[1,8]E[0,10]K[2,6]A[3,7]G";
//	char p[200]="E[1,9]D[0,8]A[2,7]L[1,8]Q[4,9]R[3,8]U";
//	char p[200]="S[1,9]K[3,11]D[1,7]F[1,9]P[3,8]G[2,6]A[0,5]L";

	char p[200]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S";
//	char p[200]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S[0,7]D[0,7]F";

//	char p[200]="G[0,8]D[0,8]S[0,8]A[0,8]F[0,8]T";	
//	char p[200]="G[0,7]D[0,7]S[0,7]A[0,7]F[0,7]T";
//	char p[200]="G[0,6]D[0,6]S[0,6]A[0,6]F[0,6]T";

	//time===pattern
//	char p[200]="r[0,5]s[0,5]k[0,5]c[0,5]k";
//	char p[200]="c[0,5]e[0,5]q[0,5]r[0,5]e[0,5]m[0,5]o";

	DWORD   begintime1=GetTickCount();
	for(float t = 1;t<11;t++)
	{
			DWORD   begintime=GetTickCount();
			read_file(p,d,G,MinLen,MaxLen);//p1-p8
			DWORD   endtime=GetTickCount();
			cout<<"sum of count:"<<count<<endl;
			printf("time is:%0.3f\n",endtime-begintime);

	}
	DWORD   endtime1=GetTickCount();
	
	printf("The average cost of time is:%0.3f\n",(endtime1-begintime1)/10.0);
	printf("The number of the approximate matching is:%d\n",count);
	return 0;
}
   
 