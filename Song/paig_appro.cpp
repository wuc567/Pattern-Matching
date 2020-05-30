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
				int end=s[k]!=sub_ptn[j].end;
				if (end<=T)
				{
					table[i].r [j].gap [length].distance[end]=1;
					table[i].r [j].gap [length].total =1;
				}
				else
				{
					table[i].r [j].gap [length].total =0;
				}
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
	start_position[0]=sub_ptn[0].min+1;
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

	for (i=0;i<len;i++)
	{
		if(table[i].r [0].len ==-1)
		{
			//it is an empty row.
			continue;
		}
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
				detail current;
				current=table2[i].r [j-1].gap [k];
				if (current.total ==0)
				{
					continue;
				}
				int name=current.name;
				cell list=table[name].r [j];				
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
							int position=list.gap [ii].name -start_position[j]-i;
							table2[i].r [j].gap [position].distance [kk+d]+=current.distance [d]*list.gap [ii].distance [kk];
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
	cout <<"Please input MinLen and MaxLen:";
	//cin>>MinLen>>MaxLen;
	cout <<"Please input the Hamming distance:\n";
	//cin >>T;
	char fpn[]="p2.txt";
	char fsn[]="s22.txt";
	MinLen=20,MaxLen=40,T=2;
	char p[1000];
	//MinLen=2,MaxLen=10,T=2;

	//char p[1000]="a[0,2]t[0,2]g";
	//char p[1000]="a[0,2]g[0,2]a[0,2]g";
	//strcpy(s,"aattgg");
	//strcpy(s,"aaggaagg");
	//strcpy(s,"agagagaaggtggagagaaga");
	Inputstr(fpn,p);
	Inputstr(fsn,s);
	//char p[M]="a[0,2]t[0,2]c";
	//char s[100000]="aattcc";
	//Inputstr("ggaagacc.txt",s);
	DWORD dwBeginTime=GetTickCount();
	long occ;
	//for (int i=0;i<5;i++)
	//{
	convert_p_to_ruler(p);
	create_matching_lookup(s,T);
	create_matching_table(s,T);	
	occ=calculate(s,T);
	//}
	release(s);
	DWORD dwEndTime=GetTickCount();
	cout <<occ<<endl;
	//cout <<"Time cost:"<<(dwEndTime-dwBeginTime)/5<<"ms\n";
	cout <<"Time cost:"<<(dwEndTime-dwBeginTime)<<"ms\n";
}
