#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include<conio.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#include <string.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <math.h>
#define N 100
char m_sequence[11000];//序列串
char m_pattern[100];//模式串
int m_result;
int m_timecost;
int pu_len;
char disp_all;
int seq_len;
int delta;
int d;//记录近似度
int count=0;//几录出现数
int countsum=0;
struct punit
{
	char start,end;
	int min,max;
};
struct row
{
	int *r;
};
int maxp_c=-1;
punit pu[N];
CWinApp theApp;
using namespace std;
int *match;
bool isalpha(char a)    
{
	if ((a>=97&&a<=96+26)||(a>=65&&a<=64+26))
		return true;
	else 
		return false;
}
void dealrange(char *p,int len)      
{
	pu[pu_len].start=p[0];
	pu[pu_len].end=p[len];
	if (len==1)
	{
		pu[pu_len].max =pu[pu_len].min=0;
	}
	else
	{
		int value=0;
		int i;
		bool flag=false;
		for (i=2;p[i]!=',';i++)
		{
			if(p[i]=='-')
				flag=true;
			else
				value=value*10+p[i]-48;
			if(flag==true)
				pu[pu_len].min=-value;
			else
				pu[pu_len].min=value;
		}
		value=0;
		flag=false;
		for (int j=i+1;p[j]!=']';j++)
		{
			if(p[j]=='-')
				flag=true;
			else
				value=value*10+p[j]-48;
			if(flag==true)
				pu[pu_len].max=-value;
			else
				pu[pu_len].max=value;
		}
	}
	if (pu[pu_len].max-pu[pu_len].min+1>maxp_c)
		maxp_c=pu[pu_len].max-pu[pu_len].min+1;
	pu_len++;
}
void convert_p_to_ruler(char *p)//将模式串转化成a[m,n]b的形式
{
	char st,en;
	int l,r;
		int len=strlen(p);
	st=p[0];
	en=p[len-1];
	if(isalpha(st)&&isalpha(en))
	{
		l=0;
		for(int i=1;i<len;i++)
		{
			if(isalpha(p[i]))
			{
				r=i;
				dealrange(p+l,r-l);
				l=i;
			}
		}
	}
	else
		printf("irregular pattern!");
}
void disp_pattern()//输出模式串
{
	for(int i=0;i<pu_len;i++)
	{
		printf("%c\t%d\t%d\t%c\n",pu[i].start,pu[i].min,pu[i].max,pu[i].end);
	}
}
int Plus=0;
int Plusmin=0;
void Calulate_Plus()
{
	int len1=0;
	int i=0;
	int len2=0;
	for(i=pu_len;i>=1;i--)
	{
		if(pu[i-1].min<0)
			len1=len1-pu[i-1].min;
		else
			len1=len1-pu[i-1].min-1;
		if(Plus<len1)
			Plus=len1;
	}
	for(i=pu_len;i>=1;i--)
	{
		if(pu[i-1].max<0)
			len2=len2-pu[i-1].max;
		else
			len2=len2-pu[i-1].max-1;
		if(Plusmin>len2)
			Plusmin=len2;
	}
	printf("Plus=%d,Plusmin=%d",Plus,Plusmin);
}
void ForwardPhase(int j,int start,row *table,bool *used)//填充一列
{
	int Sstart,Send;
	Sstart=j;
	Send=j+Plus;
	if(Send>=seq_len-1)
		Send=seq_len-1;
	for(int i=0;i<=pu_len;i++)
	{
		if(i==0)
		{
			for(int m=Sstart;m<=Send;m++)
			{
				if(used[m]==false)
				{
					int dis=abs(m_sequence[m]-pu[0].start);
					if(dis<=delta)
						table[i].r[m]=dis; //第一层直接存距离
					/*
					if(m_sequence[m]==pu[0].start)//相等的时候为0;
						table[i].r[m]=0;
					else
						table[i].r[m]=1;
						*/
				}
			}
		}
		else
		{
			int kmin=0;
			int kmax=0;
			{
				if(pu[i-1].min>=0)//全为正间隙
				{
					for(int m=Sstart;m<=Send;m++)
					{
						if(used[m]==false)
						{
							kmin=m-pu[i-1].max-1;
							kmax=m-pu[i-1].min-1;
							if(kmax<0)
								table[i].r[m]=1000;
							else
							{
								int MIN=1000; //NULL
								if(kmin<0)
									kmin=0;
								for(int k=kmin;k<=kmax;k++)
								{
									int pos=0;
									if(table[i-1].r[k]!=1000 && used[k]==false)
									{
										int dis=abs(m_sequence[m]-pu[i-1].end);
										if(dis<=delta)
											pos=table[i-1].r[k]+dis;
										else
											pos=1000;  /////////////////////////////////////////////
										/*
										if(m_sequence[m]==pu[i-1].end)
											pos=table[i-1].r[k];
										else
											pos=table[i-1].r[k]+1;
											*/
										if(pos<MIN)
											MIN=pos;
									}
								}
								if(MIN==1000)
									table[i].r[m]=1000;
								else
									table[i].r[m]=MIN;
							}
						}
					}
				}
			}
		}
	}
}
bool BackwardPhase(int j,row *table,bool *used,int *match)
{
	int MIN;
	int kmin,kmax;
	match[pu_len]=j;
	MIN=table[pu_len].r[j];
	for(j=pu_len-1;j>=0;j--)
	{
		if(pu[j].min>=0)//间隙为正正
		{
			int flag1=-1;//用来表记是否产生了最小的出现位置
			kmin=match[j+1]-pu[j].max-1;
			kmax=match[j+1]-pu[j].min-1;
			if(kmin<0)
				kmin=0;
			for(int k=kmax;k>=kmin;k--)
			{
				int flag=0;
				if(table[j].r[k]!=1000&&used[k]==false)
				{
					if(table[j].r[k]<=MIN)
					{
						for(int l=pu_len;l>=j+1;l--)
						{
							if(k==match[l])
							{
								flag=1;
								break;
							}
						}
						if(flag==1)
							continue;
						else
						{
							MIN=table[j].r[k];
							flag1=k;
						}
					}
				}
			}
			if(flag1!=-1)
				match[j]=flag1;
			else
				return false;
		}
	}
	return true;
}
void Update_Table(int pos,row *table,bool *used)//在小范围内更新table
{
	int Sstart,Send;
	int i,j;
	Sstart=pos+Plusmin;
	if(Sstart<0)
		Sstart=0;
	Send=pos+Plus;
	if(Send>=seq_len)
		Send=seq_len-1;
	for(i=0;i<=pu_len;i++)
	{
		for(j=Sstart;j<=Send;j++)
		{
			if(i==0)
			{
				if(used[j]==false)//没有被使用过
				{
					int dis=abs(m_sequence[j]-pu[0].start);
					if(dis<=delta)
						table[i].r[j]=dis; //第一层直接存距离
					/*
					if(m_sequence[j]==pu[0].start)
						table[i].r[j]=0;
					else
						table[i].r[j]=1;
						*/
				}
			}
			else//不是第一行，不仅要满足没有被使用过，而且也要满足间隙
			{
				int kmin=0;
				int kmax=0;
				if(pu[i-1].min>=0)
				{
					if(used[j]==false)
					{
						kmin=j-pu[i-1].max-1;
						kmax=j-pu[i-1].min-1;
						if(kmax<0)
							table[i].r[j]=1000;
						else
						{
							if(kmin<0)
								kmin=0;
							int MIN=1000;
							for(int k=kmin;k<=kmax;k++)
							{
								int pos1=0;
								if(table[i-1].r[k]!=1000&&used[k]==false)
								{
									int dis=abs(m_sequence[j]-pu[i-1].end);
										if(dis<=delta)
											pos1=table[i-1].r[k]+dis;
										else
											pos1=1000;  /////////////////////////////////////////////
									/*
									if(m_sequence[j]==pu[i-1].end)
										pos1=table[i-1].r[k];
									else
										pos1=table[i-1].r[k]+1;
										*/
									if(pos1<MIN)
										MIN=pos1;
								}
							}
							if(MIN==1000)
								table[i].r[j]=1000;
							else
								table[i].r[j]=MIN;
						}
					}
				}
			}
		}
	}
}
void Display_occ(int *match)
{
	int i=0;
	for(i=0;i<pu_len;i++)
	{
		cout<<match[i]<<" "<<m_sequence[match[i]];
		cout<<",";
	}
	cout<<match[pu_len]<<" "<<m_sequence[match[pu_len]];
	cout<<endl;
	
}
//输出二维表
void Display_Table(row *table)
{
	int len_s=strlen(m_sequence);  //获取序列串长度
	for(int i=0;i<=pu_len;i++)
	{
		for(int j=0;j<len_s;j++)
		{
			cout<<table[i].r[j]<<"\t";
		}
		cout<<endl;
	}
}
void display_oneoff()
{
	bool *used;
	int len_s=strlen(m_sequence);
	used=new bool[len_s];
	int i;
	for(i=0;i<len_s;i++)
		used[i]=false;
	int *match;
	match=new int[pu_len+1];
	row *table;
	table=new row[pu_len+1];
	for(i=0;i<pu_len+1;i++)
	{
		table[i].r=new int[len_s];
		for(int j=0;j<len_s;j++)
		{
			table[i].r[j]=1000;//初值附一个很大的值
		}
	}
	for(i=0;i<len_s;i++)
	{
		ForwardPhase(i,0,table,used);
		if(table[pu_len].r[i]<=d&&used[i]==false)
		{
			//Display_Table(table);
			for(int r=0;r<=pu_len;r++)
			{
				match[r]=-1;
			}
			bool result1=BackwardPhase(i,table,used,match);
			if(result1==true)
			{
				for(int j=0;j<pu_len+1;j++)
				{
					used[match[j]]=true;
				}
				count++;
				//Display_occ(match);
				Update_Table(i,table,used);
			}
			result1=false;
		}
	}
}
int main()
{
	//char ch;
	int k=0;

	//char p1[100]="a[0,3]e[2,3]f[0,3]b[0,3]g";
	//char p1[100]="a[0,10]f[0,8]k[0,12]h";
	//char p1[100]="a[0,10]f[0,10]e[0,10]f[0,10]b";
	//1

	/*
	char p1[100]="g[0,5]e[0,5]c[0,5]d[0,5]g[0,5]e[0,5]c[0,5]d";			
	char p2[100]="h[0,10]f[0,8]d[0,6]e[0,10]c";			
	char p3[100]="b[0,2]f[0,4]e[0,6]f[0,8]d[0,10]e";			
	char p4[100]="b[1,5]d[0,6]c[2,7]e[3,9]f[2,5]g[4,9]d[1,8]e[2,9]b";			
	char p5[100]="b[0,1]e[0,3]g[0,5]b[0,7]e[0,9]g";			
	char p6[100]="a[0,5]f[0,7]e[0,6]f[0,8]d[0,7]e[0,9]d";
	char p7[100]="c[0,5]g[0,5]f[0,5]a[0,5]e";			
	char p8[100]="a[1,5]d[2,5]e[3,5]c[2,5]h";			
	char p9[100]="b[2,8]f[2,8]e[2,8]j[2,8]c[2,8]b[2,8]d";			
	char p10[100]="e[1,6]f[0,5]b[3,8]d[2,7]h[1,5]e[3,7]b[1,9]d";			
	*/


	char p1[100]="l[0,10]r[0,10]e[0,10]q[0,10]d";
	char p2[100]="r[1,9]g[1,11]d[1,13]e[1,9]c[1,11]k";		
	char p3[100]="t[0,9]q[0,9]k[0,9]g[0,9]c";			
	char p4[100]="d[1,8]g[1,8]l[1,8]d[1,8]g[1,8]l";			
	char p5[100]="d[1,8]g[1,8]l[1,8]d[1,8]g[1,8]l[1,8]d[1,8]g[1,8]l";		
	char p6[100]="k[2,5]q[2,7]s[2,9]f[2,11]i";
	char p7[100]="k[2,7]q[2,9]s[2,11]f[2,13]i";					
	char p8[100]="k[2,9]q[2,11]s[2,13]f[2,15]i";

	    //char p9[100]="r[0,5]e[0,5]b[0,5]d[0,5]r[0,5]q";
	char p9[100]="b[0,6]f[0,6]k[0,6]r[0,6]r[0,6]q[0,6]b";

	//char p10[100]="c[0,5]e[0,5]q[0,5]r[0,5]e[0,5]m[0,5]o";
	//char p10[100]="r[0,6]m[0,6]e[0,6]b[0,6]d[0,6]m[0,6]q";
	//char p10[100]="r[0,10]m[0,10]e[0,10]b[0,10]d[0,10]m[0,10]q";

    //char p10[100]="r[0,6]q[0,6]e[0,6]i[0,6]o[0,6]q";
	char p10[100]="o[0,6]m[0,6]d[0,6]e[0,6]r[0,6]s[0,6]s";

	strcat(m_pattern,p8);
	//cout<<"please input pattern at first:"<<endl;
	//cin>>m_pattern;
	convert_p_to_ruler(m_pattern);
	printf("the pattern is:\n");
	disp_pattern();
	//DWORD dwBeginTime=GetTickCount();

	//Calulate_Plus();
	//if((fp=fopen("C:\\Users\\LeiLei\\Desktop\\DataSet\\SDB.txt","rt"))==NULL)
	FILE *fp;
	if((fp=fopen("C:\\Users\\LeiLei\\Desktop\\DataSet\\SDB.txt","r"))==NULL)
		//if((fp=fopen("C:\\Users\\LeiLei\\Desktop\\fuhao.txt","r"))==NULL)
	{
		cout<<"\nconnot open file strike any key exit!"<<endl;
		exit(1);
	}
	cout<<"请输入近似度：delta gama"<<endl;
    cin>>delta>>d;
	DWORD dwBeginTime=GetTickCount();

	Calulate_Plus();
	while(fscanf(fp,"%s",m_sequence)==1)
	{
		k=strlen(m_sequence);
		//cout<<"序列串长度为：%d"<<k<<endl;
		seq_len=k;
		//fclose(fp);
		//cout<<"All optimal matches are given as follows:"<<endl;
		display_oneoff();
		//cout<<"-------------------------------"<<endl;
		//printf("\n----------------------%d\n",count);
		countsum += count;
		count=0;
	}
	
	DWORD dwEndTime=GetTickCount();
	m_timecost=dwEndTime-dwBeginTime;
	cout<<"-------------------------------"<<endl;

	fclose(fp);
	//	printf ("The number of optimal matchess is : %d\n" ,count);
	printf("The sum of occ is %d\n",countsum);
	printf("The time cost is :%ld\n",m_timecost);
	return 0;
}
