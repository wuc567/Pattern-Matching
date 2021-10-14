#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>	
#include <string.h>		// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <conio.h>
#include <cmath>
//#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// The one and only application object

#include <string.h>
#define NOLENCON true
using namespace std;
#define N 200000
#define M 1000
int maxp_c=-1;
char p[M];
/*********����ģʽ�����������***********/
struct sub_ptn_struct //ģʽ��
{
	char start,end;
	int min,max;
};
sub_ptn_struct sub_ptn[M];//����ģʽ�� 
int ptn_len=0;  //the length of pattern
/*******************�����������������Ϣ*********************/
struct node;
struct point
{
	node *p;
};
struct node
{
	char data;
	int position;
	int *downpath;//��Ҷ·����
	int *uppath;//����·����
	int *up_downpath;
	node *next;
	node *pre;
	point *child,*parent;
	int ch_count;
	int p_count;
	int *localpath;
	bool dis;//��dis=tureʱ��ȷƥ�䣬�������ƥ��
	
	int deltadistance; //��¼ÿ������delta����
};
/****************����������������Ϣ******************/
struct level_node
{
	int count;
	char ch;
	node *head;
	node *start;
	node *tail;
	bool needrep;
};
level_node *head_level;

/***************������������********************/
int d;  //٤��
int delta; //delta����
int sumocc=0;
int seq_sumlen=0;
int seq_len;
char t[N];

void dearlrange(char *p,int len)
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
		int flag=0;
		for ( i=2;p[i]!=',';i++)
			if(p[i]=='-')
				flag=1;
			else
				value = value*10+p[i]-48;
			if(flag==1)
				sub_ptn[ptn_len].min = -value;
			else
				sub_ptn[ptn_len].min = value;		
			value=0;
			flag=0;
			for (int j=i+1;p[j]!=']';j++)
				if(p[j]=='-')
					flag = 1;
				else
					value = value*10+p[j]-48;
				if(flag==1)
					sub_ptn[ptn_len].max = -value;
				else
					sub_ptn[ptn_len].max = value;
	}
	if(sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1>maxp_c)
		maxp_c=sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1;
	ptn_len++;
	cout<<ptn_len<<endl;
}
void convert_p_to_ruler(char *p)
{
	char st,en;
	int l,r;
	int len=strlen(p);
	st=p[0];
	en=p[len-1];
	if(isalpha(st)&&isalpha(en))
	{
		l = 0;
		for(int i=1;i<len;i++)
		{
			if(isalpha(p[i]))
			{
				r=i;
				dearlrange(p+l,r-l);
				l=r;
			}
		}	
	}
	else
	{
		cout<<"irrengular pattern.\n";
		exit(-1);
	}
}
void disp_pattern()
{
	for(int i=0;i<ptn_len;i++)
	{
        cout<<sub_ptn[i].start<<"\t"<<sub_ptn[i].min<<"\t"<<sub_ptn[i].max<<"\t"<<sub_ptn[i].end<<endl;
	}
}
/**********************�����������*********************/
void createlevel()
{
	head_level=new level_node[ptn_len+1];
	int i;
	for(i=0;i<ptn_len+1;i++)
	{
		head_level[i].count =-1;
		head_level[i].head  =NULL;
		head_level[i].start =NULL;
		head_level[i].tail  =NULL;
		head_level[i].needrep=false;
	}
	for(i=0;i<ptn_len;i++)
		head_level[i].ch=sub_ptn[i].start;
	head_level[ptn_len].ch=sub_ptn[ptn_len-1].end;
}
/********************����ڲ��Ƿ�����ظ�*********************/
void cal_needrep()
{
	int i,j;
	int flag=0;
	char h;
	for(i=0;i<ptn_len;i++)
	{
		if(sub_ptn[i].min<0)
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		return;
	}
	else
	{
		if(d==0)
		{
			for(i=ptn_len;i>=0;i--)
			{
				if(head_level[i].needrep==true)
					continue;
				else
				{
					h=head_level[i].ch;
					for(j=i-1;j>=0;j--)
					{
						if(head_level[j].ch==h)
						{
							int mini=0;
							for(int l=j;l<i;l++)
							{
								if(sub_ptn[l].min<0)
									mini+=sub_ptn[l].min;
								else
									mini+=sub_ptn[l].min+1;
							}
							if(mini<=0)
							{
								head_level[j].needrep=true;
							}
						}
					}
				}
			}
		}
		else
		{
			for(i=ptn_len;i>=0;i--)
			{
				if(head_level[i].needrep==true)
					continue;
				else
				{
					for(j=i-1;j>=0;j--)
					{
						int mini1=0;
						for(int l=j;l<i;l++)
						{
							if(sub_ptn[l].min<0)
								mini1+=sub_ptn[l].min;
							else
								mini1+=sub_ptn[l].min+1;
						}
						if(mini1<=0)
							head_level[j].needrep=true;
					}
				}
			}
		}
	}
	/***************��֤���*****************/
	cout<<"�ڲ��ظ������ƽ����"<<endl;
	for(i=0;i<ptn_len+1;i++)
	{
		cout<<head_level[i].needrep<<endl;
	}
}

void display_nettree()
{
	for(int i=0;i<ptn_len+1;i++)
	{
		node *tmp=head_level[i].head;
		cout<<"Levle["<<i<<"]:\t"<<endl;
		while(tmp!=NULL)
		{
			cout<<tmp->position+1<<":";
			for(int m=0;m<d+1;m++)
				cout<<(tmp->up_downpath)[m]<<" ";
			//cout<<"delta="<<tmp->dis;
			
			cout<<" child is "<<tmp->ch_count;
			cout<<"->";
			for(int n=0;n<=tmp->ch_count;n++)
				cout<<(tmp->child)[n].p->position;
			cout<<" ";
			tmp=tmp->next;
		}
		cout<<endl;
	}	
}
/************************�ܷ���뾫ȷƥ����**************/
bool can_insert_exactnode(int position,int depth,node *start,node *ptail)
{
	node *k=start;
	while(k!=ptail)
	{
		if(sub_ptn[depth-1].min>=0)//��϶Ϊ���������
		{
			if((position-k->position-1)<=sub_ptn[depth-1].max&&(position-k->position-1)>=sub_ptn[depth-1].min)
				return true;
			else
				k=k->next;
		}
		else 
		{
			if(sub_ptn[depth-1].max<0)//��Ϊ����϶
			{
				if((position-k->position)<=sub_ptn[depth-1].max&&(position-k->position)>=sub_ptn[depth-1].min)
					return true;
				else
					k=k->next;
			}
			else//һ��һ��
			{
				if((position-k->position)<=sub_ptn[depth-1].max+1&&(position-k->position)>=sub_ptn[depth-1].min&&position!=k->position)
					return true;
				else
					k=k->next;
			}
		}
	}
	if(sub_ptn[depth-1].min>=0)
	{
		if((position-k->position-1)<=sub_ptn[depth-1].max&&(position-k->position-1)>=sub_ptn[depth-1].min)
			return true;
		return false;
	}
	else if(sub_ptn[depth-1].max<0)
	{
		if((position-k->position)<=sub_ptn[depth-1].max&&(position-k->position)>=sub_ptn[depth-1].min)
			return true;
		return false;
	}
	else if(sub_ptn[depth-1].min<0&&sub_ptn[depth-1].max>=0)
	{
		if((position-k->position)<=sub_ptn[depth-1].max+1&&(position-k->position)>=sub_ptn[depth-1].min&&position!=k->position)
			return true;
		return false;
	}
	return false;
}

/************************���������е�һ�������Ϣ***************/
void create_node_table(char t,int position,int depth,int &nodecount)
{
	node *tmp;
	if(depth==0)//����0����
	{
		tmp=new node();
		tmp->data=t;
		tmp->position=position;
		tmp->uppath=new int[1];
		(tmp->uppath)[0]=0;
		
		tmp->deltadistance=0; //��ȷƥ��
		
		tmp->downpath=new int[1];
		tmp->up_downpath=new int[1];
		tmp->localpath=new int[1];
		tmp->next=NULL;
		tmp->child=new point[maxp_c];
		for(int i=0;i<maxp_c;i++)
			(tmp->child)[i].p=NULL;
		tmp->ch_count=-1;
		tmp->p_count=-1;
		tmp->dis=true;
		(head_level[0].count)++;
		if(head_level[0].count==0)//��һ��ĵ�һ�����
		{
			head_level[0].head=tmp;
			head_level[0].tail=tmp;
			head_level[0].start=tmp;
			tmp->pre=NULL;
		}
		else//���ǵ�һ�����
		{
			tmp->pre=head_level[0].tail;
			(head_level[0].tail)->next=tmp;
			head_level[0].tail=tmp;
		}
	}
	else
	{
		bool flag=false;
		node *k=head_level[depth-1].start;//�ж��Ƿ��ܹ������
		node *ptail=head_level[depth-1].tail;
		bool c_i_n;
		if(k!=NULL)
		{
			c_i_n= can_insert_exactnode(position,depth,k,ptail);
		}
		if(k!=NULL&&c_i_n)
		{
			int i;
			tmp=new node();
			tmp->data=t;
			tmp->position=position;
			tmp->uppath=new int[1];
			(tmp->uppath)[0]=0;
			
			tmp->deltadistance=0; //��ȷƥ��
			
			tmp->downpath=new int[1];
			tmp->up_downpath=new int[1];
			tmp->localpath=new int[1];
			tmp->localpath[0]=0;
			tmp->next=NULL;
			tmp->child=new point[maxp_c];//�Ժ��ӽ����д���
			for(i=0;i<maxp_c;i++)
				(tmp->child)[i].p=NULL;
			tmp->parent=new point[maxp_c];//�Ը��׽����д���
			for(i=0;i<maxp_c;i++)
				(tmp->parent)[i].p=NULL;
			tmp->ch_count=-1;
			tmp->p_count=-1;
			tmp->dis=true;
			(head_level[depth].count)++;
			if(head_level[depth].count==0)//�����һ�����
			{
				head_level[depth].head=tmp;
				head_level[depth].tail=tmp;
				head_level[depth].start=tmp;
				tmp->pre=NULL;
			}
			else
			{
				tmp->pre=head_level[depth].tail;
				(head_level[depth].tail)->next=tmp;
				head_level[depth].tail=tmp;
			}
			while(k!=NULL)
			{
				int l;
				if(sub_ptn[depth-1].max<0)//����϶����С��϶��Ϊ��ֵʱ
				{
					l=position-k->position;
					if(l<sub_ptn[depth-1].min)
						break;
					if(l>sub_ptn[depth-1].max)
					{
						if(k->next!=NULL)
							head_level[depth-1].start=k->next;//�ý�����ϲ������̫����������
					}
					else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)//�ý����k��������϶Լ�������Ӻ���˫�׽��
					{
						(k->ch_count)++;
						((k->child)[k->ch_count]).p=tmp;
						(tmp->p_count)++;
						((tmp->parent)[tmp->p_count]).p=k;
					}
				}
				else if(sub_ptn[depth-1].min>=0) //��Ϊ��ֵ
				{
					l=position-k->position-1;
					if(l<sub_ptn[depth-1].min)
						break;
					else if(l>sub_ptn[depth-1].max)
					{
						if(k->next!=NULL)  //ȷ������һ�������϶�Ŀ�ʼλ��
							head_level[depth-1].start=k->next;
					}
					else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)
					{
						(k->ch_count)++;
						((k->child)[k->ch_count]).p=tmp;
						(tmp->p_count)++;
						((tmp->parent)[tmp->p_count]).p=k;
					}
				}
				else if(sub_ptn[depth-1].min<0&&sub_ptn[depth-1].max>=0)//һ��һ��
				{
					l=position-k->position;
					if(l<sub_ptn[depth-1].min)
						break;
					else if(l>sub_ptn[depth-1].max+1)
					{
						if(k->next!=NULL)
							head_level[depth-1].start=k->next;
					}
					else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max+1&&position!=k->position)
					{
						(k->ch_count)++;
						((k->child)[k->ch_count]).p=tmp;
						(tmp->p_count)++;
						((tmp->parent)[tmp->p_count]).p=k;
					}
				}
				k=k->next;//k���������ƶ�
			}
		}
	}
}

/**********************�ж��ܷ������ƽ��*******************/
bool can_insert_appnode(int position,int depth,node *start,node *ptail)
{
	node *k=start;
	if(sub_ptn[depth-1].min>=0)//��϶Ϊ����
	{
		while(k!=ptail)
		{
			if((position-k->position-1)<sub_ptn[depth-1].min)
				return false;
			else 
			{
				if((position-k->position-1)<=sub_ptn[depth-1].max)
				{
					for(int m=0;m<d;m++)
					{
						if((k->uppath)[m]!=0)
							return true;
					}
					k=k->next;
				}
				else
					k=k->next;
			}
		}
		if((position-k->position-1)<=sub_ptn[depth-1].max&&(position-k->position-1)>=sub_ptn[depth-1].min)
		{
			for(int m=0;m<d;m++)
			{
				if((k->uppath)[m]!=0)
					return true;
			}
			return false;
		}
		return false;
	}
	else
	{
		if(sub_ptn[depth-1].max<0)//��϶Ϊ�������
		{
			while(k!=ptail)
			{
				if((position-k->position)<sub_ptn[depth-1].min)
					return false;
				else 
				{
					if((position-k->position)<=sub_ptn[depth-1].max)
					{
						for(int m=0;m<d;m++)
						{
							if((k->uppath)[m]!=0)
								return true;
						}
						k=k->next;
					}
					else
						k=k->next;
				}
			}
			if((position-k->position)>=sub_ptn[depth-1].min&&(position-k->position)<=sub_ptn[depth-1].max)
			{
				for(int m=0;m<d;m++)
				{
					if((k->uppath)[m]!=0)
						return true;
				}
				return false;
			}
			return false;
		}
		else//��϶Ϊ����
		{
			while(k!=ptail)
			{
				if((position-k->position)<sub_ptn[depth-1].min)
					return false;
				else
				{
					if((position-k->position-1)<=sub_ptn[depth-1].max&&position!=k->position)
					{
						for(int m=0;m<d;m++)
						{
							if((k->uppath)[m]!=0)
								return true;
						}
						k=k->next;
					}
					else
						k=k->next;
				}
			}
			if((position-k->position)>=sub_ptn[depth-1].min&&(position-k->position-1)<=sub_ptn[depth-1].max&&position!=k->position)
			{
				for(int m=0;m<d;m++)
				{
					if((k->uppath)[m]!=0)
						return true;
				}
				return false;
			}
			return false;
		}
	}
	return false;
}

/*************************����d>0ʱ������****************/
void create_appnode_table(int position,int depth)
{
	node *tmp;
	int deltadis=abs(t[position] - head_level[depth].ch); //delta����
	
	if(depth==0)//������һ��ֱ�ӽ������
	{
		tmp=new node();
		tmp->data=t[position];
		tmp->position=position;
		
		tmp->deltadistance=deltadis; //��¼����delta����
		
		tmp->uppath=new int[d+1];
		tmp->downpath=new int[d+1];
		tmp->up_downpath=new int[d+1];
		tmp->localpath=new int[d+1];
		for(int i=0;i<d+1;i++)
			(tmp->localpath)[i]=0;
		tmp->next=NULL;
		tmp->child=new point[maxp_c];
		for(i=0;i<maxp_c;i++)
			(tmp->child)[i].p=NULL;
		tmp->ch_count=-1;
		tmp->p_count=-1;
		//if(tmp->data==head_level[depth].ch)//��ȷƥ��
		
		if(tmp->deltadistance==0)
		{
			tmp->dis=true;
			(tmp->uppath)[0]=1;
			for(int k=1;k<d+1;k++)
				(tmp->uppath)[k]=0;
		}
		else//����ƥ��
		{
			
			tmp->dis=false;
			
			for(int k=0;k<d+1;k++)
				(tmp->uppath)[k]=0;
			
			(tmp->uppath)[tmp->deltadistance]=1;  //��delta���봦������·����Ϊ1
		}
		(head_level[0].count)++;
		if(head_level[0].count==0)//��һ���һ�����
		{
			head_level[0].head=tmp;
			head_level[0].tail=tmp;
			head_level[0].start=tmp;
			tmp->pre=NULL;
		}
		else
		{
			tmp->pre=head_level[0].tail;
			(head_level[0].tail)->next=tmp;
			head_level[0].tail=tmp;
		}
	}
	else//���������ĵ�һ��
	{
		node *k=head_level[depth-1].start;
		node *ptail=head_level[depth-1].tail;
		bool c_i_n;
		//if(t[position]==head_level[depth].ch)//��ȷƥ��
		if(deltadis==0) //��ȷƥ��
		{
			if(k!=NULL)
			{
				c_i_n=can_insert_exactnode(position,depth,k,ptail);
			}
			if(k!=NULL&&c_i_n)//�������
			{
				int i;
				tmp=new node();
				tmp->data=t[position];
				tmp->position=position;
				
				tmp->deltadistance=deltadis;  //��ֵdelta����
				tmp->uppath=new int[d+1];
				for(int m=0;m<d+1;m++)
					(tmp->uppath)[m]=0;
				tmp->downpath=new int[d+1];
				tmp->up_downpath=new int[d+1];
				tmp->localpath=new int[d+1];
				for(i=0;i<d+1;i++)
					(tmp->localpath)[i]=0;
				tmp->next=NULL;
				tmp->child=new point[maxp_c];
				for(i=0;i<maxp_c;i++)
					(tmp->child)[i].p=NULL;
				tmp->parent=new point[maxp_c];
				for(i=0;i<maxp_c;i++)
					(tmp->parent )[i].p=NULL;
				tmp->ch_count=-1;
				tmp->p_count=-1;
				tmp->dis=true;
				(head_level[depth].count)++;
				
				//���㽨����ϵ
				if(head_level[depth].count==0)
				{
					head_level[depth].head=tmp;
					head_level[depth].tail=tmp;
					head_level[depth].start=tmp;
					tmp->pre=NULL;
				}
				else
				{
					tmp->pre=head_level[depth].tail;
					(head_level[depth].tail)->next=tmp;
					head_level[depth].tail=tmp;
				}
				while(k!=NULL)  //��������ϵ
				{
					int l;
					if(sub_ptn[depth-1].max<0)//��϶Ϊ����
					{
						l=position-k->position;
						if(l<sub_ptn[depth-1].min)
							break;
						if(l>sub_ptn[depth-1].max)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;//�ý�����ϲ������̫����������
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)//�ý����k��������϶Լ�������Ӻ���˫�׽��
						{
							(k->ch_count)++;
							((k->child)[k->ch_count]).p=tmp;
							(tmp->p_count)++;
							((tmp->parent)[tmp->p_count]).p=k;
							for(int m=0;m<d+1;m++)
							{
								(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m];
							}
						}
					}
					else if(sub_ptn[depth-1].min>=0) //��Ϊ��ֵ
					{
						l=position-k->position-1;
						if(l<sub_ptn[depth-1].min)
							break;
						else if(l>sub_ptn[depth-1].max)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)
						{
							(k->ch_count)++;
							((k->child)[k->ch_count]).p=tmp;
							(tmp->p_count)++;
							((tmp->parent)[tmp->p_count]).p=k;
							for(int m=0;m<d+1;m++)         //��ֵ����·��������
							{
								(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m];
							}
						}
					}
					else if(sub_ptn[depth-1].min<0&&sub_ptn[depth-1].max>=0)//һ��һ��
					{
						l=position-k->position;
						if(l<sub_ptn[depth-1].min)
							break;
						else if(l>sub_ptn[depth-1].max+1)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max+1&&position!=k->position)
						{
							(k->ch_count)++;
							((k->child)[k->ch_count]).p=tmp;
							(tmp->p_count)++;
							((tmp->parent)[tmp->p_count]).p=k;
							for(int m=0;m<d+1;m++)
							{
								(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m];
							}
						}
					}
					k=k->next;
				}
			}
		}
		else //����ƥ��ʱ
		{
			if(k!=NULL)
			{
				c_i_n=can_insert_appnode(position,depth,k,ptail);
			}
			if(k!=NULL&&c_i_n)
			{
				int i;
				tmp=new node();
				tmp->data=t[position];
				tmp->position=position;
				
				tmp->deltadistance=deltadis;  //��ֵdelta����
				
				tmp->uppath=new int[d+1];
				for(int m=0;m<d+1;m++)
					(tmp->uppath)[m]=0;
				tmp->downpath=new int[d+1];
				tmp->up_downpath=new int[d+1];
				tmp->localpath=new int[d+1];
				for(i=0;i<d+1;i++)
					(tmp->localpath)[i]=0;
				tmp->next=NULL;
				tmp->child=new point[maxp_c];
				for(i=0;i<maxp_c;i++)
					(tmp->child)[i].p=NULL;
				tmp->parent=new point[maxp_c];
				for(i=0;i<maxp_c;i++)
					(tmp->parent )[i].p=NULL;
				tmp->ch_count=-1;
				tmp->p_count=-1;
				tmp->dis=false;
				(head_level[depth].count)++;
				if(head_level[depth].count==0)//�����һ�����
				{
					head_level[depth].head=tmp;
					head_level[depth].tail=tmp;
					head_level[depth].start=tmp;
					tmp->pre=NULL;
				}
				else
				{
					tmp->pre=head_level[depth].tail;
					(head_level[depth].tail)->next=tmp;
					head_level[depth].tail=tmp;
				}
				while(k!=NULL)
				{
					int l,m;
					if(sub_ptn[depth-1].max<0)//��϶Ϊ����
					{
						l=position-k->position;
						if(l<sub_ptn[depth-1].min)
							break;
						if(l>sub_ptn[depth-1].max)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)
						{
							for(m=0;m<d;m++)
							{
								if((k->uppath)[m]!=0)
									break;
							}
							if(m<d)//������ϵ
							{
								(k->ch_count)++;
								((k->child)[k->ch_count]).p=tmp;
								(tmp->p_count)++;
								((tmp->parent)[tmp->p_count]).p=k;
								(tmp->uppath)[0]=0;
								for(int m=1;m<d+1;m++)
								{
									(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m-1];
								}
							}
						}
					}
					else if(sub_ptn[depth-1].min>=0)//��϶Ϊ����
					{
						l=position-k->position-1;
						if(l<sub_ptn[depth-1].min)
							break;
						else if(l>sub_ptn[depth-1].max)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max)
						{
							for(m=0;m<d;m++)
							{
								if((k->uppath)[m]!=0)
									break;
							}
							if(m<d)  //˵������С��٤��ĵط����Բ��ȥ���
							{
								(k->ch_count)++;
								((k->child)[k->ch_count]).p=tmp;
								(tmp->p_count)++;
								((tmp->parent)[tmp->p_count]).p=k;
								for(m=0;m<tmp->deltadistance;m++)
								{
									(tmp->uppath)[m]+=0;
								}
								
								for(int m=tmp->deltadistance;m<d+1;m++)
								{
									(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m-tmp->deltadistance];
								}
							}
						}
					}
					else if(sub_ptn[depth-1].min<0&&sub_ptn[depth-1].max>=0)
					{
						l=position-k->position;
						if(l<sub_ptn[depth-1].min)
							break;
						else if(l>sub_ptn[depth-1].max+1)
						{
							if(k->next!=NULL)
								head_level[depth-1].start=k->next;
						}
						else if(l>=sub_ptn[depth-1].min&&l<=sub_ptn[depth-1].max+1&&position!=k->position)
						{
							for(m=0;m<d;m++)
							{
								if((k->uppath)[m]!=0)
									break;
							}
							if(m<d)
							{
								(k->ch_count)++;
								((k->child)[k->ch_count]).p=tmp;
								(tmp->p_count)++;
								((tmp->parent)[tmp->p_count]).p=k;
								(tmp->uppath)[0]=0;
								for(int m=1;m<d+1;m++)
								{
									(tmp->uppath)[m]+=(((tmp->parent)[tmp->p_count]).p->uppath)[m-1];
								}
							}
						}
					}
					k=k->next;
				}
			}
		}
	}
}

/***************************��������******************/
void create_store(char *c)
{
	if(d==0)//��d=0ʱ��������
	{
		int count=0;
		for(int i=0;i<ptn_len+1;i++)
		{
			for(int j=0;j<seq_len;j++)
			{
				if(head_level[i].ch==t[j])
				{
					create_node_table(c[i],j,i,count);
				}
			}
		}
	}
	else//��d>0ʱ
	{
		for(int i=0;i<ptn_len+1;i++)
		{
			for(int j=0;j<seq_len;j++)
			{
				if(abs(head_level[i].ch-t[j])<=delta)
				{
					create_appnode_table(j,i);
				}
			}
		}
	}
}



/**********************���������Ҷ·����********************/
void update_node_downpath(int **nodeinpath,bool *used)
{
	int i;
	for(i=ptn_len;i>=0;i--)
	{
		node *tmp=head_level[i].head;  //ÿ��ִ�У��Ƚ�����������ͷ��㣬Ū����
		int j;
		if(i==ptn_len)//���һ�������Ҷ·��������Ϊ1
		{
			for(j=0;j<=head_level[i].count;j++)
			{
				for(int m=0;m<d+1;m++)
					(tmp->downpath)[m]=0;
				if(used[tmp->position]==false)
				{
					for(m=0;m<d+1;m++)
					{
						if(m==tmp->deltadistance)
							(tmp->downpath)[m]=1;
						else
							(tmp->downpath)[m]=0;
					}
				}
				tmp=tmp->next;
			}
		}
		else
		{
			if(d==0)
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					int downpath=0;
					(tmp->downpath)[0]=0;
					if(used[tmp->position]==false)
					{
						for(int k=0;k<=tmp->ch_count;k++)  //�������ӽ�㣬ÿ��ֵ���
						{
							node *ppp=(tmp->child[k]).p;
							downpath+=ppp->downpath[0];
						}
						(tmp->downpath)[0]=downpath;  //��ֵ
					}
					tmp=tmp->next;
				}
			}
			else
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					for(int m=0;m<d+1;m++)
						(tmp->downpath)[m]=0;
					if(used[tmp->position]==false)
					{
						if(tmp->dis==true)//��ȷƥ�䣬����ֱ�����
						{
							for(m=0;m<d+1;m++)
							{
								for(int k=0;k<=tmp->ch_count;k++)
								{
									node *ppp=(tmp->child[k]).p;
									(tmp->downpath)[m]+=(ppp->downpath)[m]; //ֱ�����
								}
							}
						}
						else
						{
							for(int m=0;m<tmp->deltadistance;m++)
							{
								(tmp->downpath)[m]=0;
							}
							for(m=tmp->deltadistance;m<d+1;m++)
							{
								for(int k=0;k<=tmp->ch_count;k++)
								{
									node *ppp=(tmp->child[k]).p;
									(tmp->downpath)[m]+=(ppp->downpath)[m-tmp->deltadistance];  //����delta����λ
								}
							}
						}
					}
					tmp=tmp->next;
				}
			}
		}
	}
}


/****************�����������·����*****************/
void update_node_uppath(int **nodeinpath,bool *used)
{
	int i;
	if(d==0)  
	{
		for(i=0;i<=ptn_len;i++)
		{
			node *tmp=head_level[i].head;
			int j;
			if(i==0)  //0��
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					(tmp->uppath)[0]=0; 
					if(used[tmp->position]==false)
						(tmp->uppath)[0]=1;
					tmp=tmp->next;
				}
			}
			else
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					(tmp->uppath)[0]=0;
					if(used[tmp->position]==false)
					{
						for(int k=0;k<=tmp->p_count;k++)
						{
							node *ppp=(tmp->parent[k]).p;
							(tmp->uppath)[0]+=(ppp->uppath)[0];
						}
					}
					tmp=tmp->next;
				}
			}
		}
	}
	else   //d>0
	{
		for(i=0;i<ptn_len+1;i++)
		{
			node *tmp=head_level[i].head;
			int j;
			if(i==0)  //��һ��
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					
					for(int m=0;m<d+1;m++)
					{
						(tmp->uppath)[m]=0;
					}
					
					if(used[tmp->position]==false)
					{
						if(tmp->dis==true)  //��ȷƥ��
						{
							(tmp->uppath)[0]=1;
							for(int m=1;m<d+1;m++)
								(tmp->uppath)[m]=0;
						}
						else
						{							
							for(int m=0;m<d+1;m++)
								(tmp->uppath)[m]=0;
							(tmp->uppath)[tmp->deltadistance]=1;
						}
					}
					tmp=tmp->next;
				}
			}
			else
			{
				for(j=0;j<=head_level[i].count;j++)
				{
					for(int m=0;m<d+1;m++)
						(tmp->uppath)[m]=0;
					if(used[tmp->position]==false)
					{
						if(tmp->dis==true)//��ȷƥ�䣬˫��ֱ�����
						{
							for(m=0;m<d+1;m++)
							{
								for(int k=0;k<=tmp->p_count;k++)
								{
									node *ppp=(tmp->parent[k]).p;
									(tmp->uppath)[m]+=(ppp->uppath)[m]; //ֱ�����
								}
							}
						}
						else
						{
							//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
							for(int m=0;m<tmp->deltadistance;m++)
							{
								(tmp->uppath)[m]=0;
							}
							for(m=tmp->deltadistance;m<d+1;m++)
							{
								for(int k=0;k<=tmp->p_count;k++)
								{
									node *ppp=(tmp->parent[k]).p;
									(tmp->uppath)[m]+=(ppp->uppath)[m-tmp->deltadistance];  //����delta����λ
								}
							}
						}
					}
					tmp=tmp->next;
				}
			}
		}
	}
}


/*******************************�������������Ҷ·����********************/
void update_node_updownpath(int **nodeinpath,bool *used)
{
	for(int i=0;i<ptn_len+1;i++)
	{
		node *tmp=head_level[i].head;
		int j;
		for(j=0;j<head_level[i].count+1;j++)
		{
			for(int k=0;k<d+1;k++)
			{
				(tmp->up_downpath)[k]=0;
				for(int m=0;m<d+1;m++)
					(tmp->up_downpath)[k]+=((tmp->uppath)[k])*((tmp->downpath)[m]);
			}
			tmp=tmp->next;
		}
	}
}


/********************�������д�����������Ҷ·����******************/
void cal_nodeinpath(int **nodeinpath,bool *used)
{
	update_node_downpath(nodeinpath,used);//���������Ҷ·����
	update_node_uppath(nodeinpath,used);//�����������·����
	
	if(d>0)
	{
		update_node_updownpath(nodeinpath,used);//�����������-��Ҷ·����
	}
	int i,j;
	for(i=0;i<seq_len;i++)
	{
		for(j=0;j<d+1;j++)
			nodeinpath[i][j]=0;
	}
	if(d==0)
	{
		//����һ��
		for(i=0;i<ptn_len;i++)
		{
			node *tmp=head_level[i].head;
			for(j=0;j<=head_level[i].count;j++)
			{
				int nodename=tmp->position;
				nodeinpath[nodename][0]+=tmp->uppath[0]*tmp->downpath[0];
				tmp=tmp->next;
			}
		}
	}
	else
	{
		for(i=0;i<ptn_len+1;i++)
		{
			node *tmp=head_level[i].head;
			for(j=0;j<head_level[i].count+1;j++)
			{
				for(int m=0;m<d+1;m++)
				{
					nodeinpath[tmp->position][m]+=(tmp->up_downpath)[m];  //ÿ������������Ҷ·����֮�ͣ��˴�Ӧ��Ϊ����٤������������Ҷ·����
				}
				tmp=tmp->next;
			}
		}
	}
	
	//���λ�������
	/*
	int *aa=new int [seq_len];
	for(i=0;i<seq_len;i++)
	{
		aa[i]=0;
		for(int m=0;m<d+1;m++)
		{
			aa[i]+=nodeinpath[i][m];  //ÿ������������Ҷ·����֮�ͣ��˴�Ӧ��Ϊ����٤������������Ҷ·����
		}
		cout<<aa[i]<<" - ";
	}
	cout<<endl;
	*/
	
}
/*************************�ҳ���***********************/


/**********************************���Ҳ���********************************/
bool RightMost_Occ(node *start_off,int *apath,bool *used)
{
	point *array;
	array=new point[ptn_len+1];
	array[ptn_len].p=start_off;
	int start=start_off->position;
	apath[ptn_len]=start;
	node *parentnode,*childnode;
	childnode=start_off;
	int *pos=new int[ptn_len+1];
	int i;
	int n;
	for(i=0;i<ptn_len+1;i++)
		pos[i]=-1;
	pos[ptn_len]=0;
	int d1=d;
	if(start_off->dis==false)
		//d1--;
		d1=d1-start_off->deltadistance;  //----------------------
	for(i=ptn_len-1;i>=0;)
	{
		n=childnode->p_count;
		int nodename,j;
		if(pos[i]==-1)
			j=n;
		else
			j=pos[i]-1;
		int pos_j=-1;
		for(;j>=0;j--)
		{
			bool flag=false,flag1=false;//������¼�Ƿ������������
			parentnode=childnode->parent[j].p;
			nodename=parentnode->position;
			for(int m=0;m<=d1;m++)
			{
				if(parentnode->uppath[m]!=0)
				{
					flag1=true;
					break;
				}
			}
			if(flag1==true)
			{
				if(used[nodename]==false)
				{
					for(int h=ptn_len;h>i;h--)
					{
						if(nodename==apath[h])
						{
							flag=true;
							break;
						}
					}
					if(flag==true)
						continue;
					pos_j=j;
					break;
				}
			}
			
		}
		j=pos_j;
		if(j>=0)
		{
			parentnode=childnode->parent[j].p;
			nodename=parentnode->position;
			array[i].p=parentnode;
			apath[i]=nodename;
			childnode=parentnode;
			pos[i]=j;
			if(parentnode->dis==false)
				//d1--;
				d1=d1-parentnode->deltadistance;
			i--;
		}
		else
		{
			for(int j=0;j<=i;j++)
				pos[j]=-1;
			i++;
			if(i>=ptn_len)
			{
				delete[]array;
				delete[]pos;
				return false;
			}
			if((array[i].p)->dis==false)
				//d1++;
				d1=d1+(array[i].p)->deltadistance;
			childnode=array[i+1].p;
		}
	}
	delete[]array;
	delete[]pos;
	return true;
}

/**********************************�������********************************/
bool LeftMost_Occ(node *start_off,int *apath,bool *used)
{
	point *array;
	array=new point[ptn_len+1];
	array[ptn_len].p=start_off;
	int start=start_off->position;
	apath[ptn_len]=start;
	node *parentnode,*childnode;
	childnode=start_off;        //��ʼ�ڵ�
	int *pos=new int[ptn_len+1];
	int i;
	int n;
	for(i=0;i<ptn_len+1;i++)
		pos[i]=-1;
	pos[ptn_len]=0;
	int d1=d;
	if(start_off->dis==false)
		//d1--;
		d1=d1-start_off->deltadistance;  //----------------------
	for(i=ptn_len-1;i>=0;)
	{
		n=childnode->p_count;
		int nodename,j;
		if(pos[i]==-1)
			//j=n;         //������ʼ��
			j=0;
		else
			//j=pos[i]-1;
			j=pos[i]+1;    //�����ƶ�һ��˫�׽ڵ�
		int pos_j=-1;
		for(;j<=n;j++)     //���������ƶ�ÿһ��˫��
		{
			bool flag=false,flag1=false;//������¼�Ƿ������������
			parentnode=childnode->parent[j].p;
			nodename=parentnode->position;
			for(int m=0;m<=d1;m++)
			{
				if(parentnode->uppath[m]!=0)
				{
					flag1=true;
					break;
				}
			}
			if(flag1==true)
			{
				if(used[nodename]==false)
				{
					for(int h=ptn_len;h>i;h--)
					{
						if(nodename==apath[h])
						{
							flag=true;
							break;
						}
					}
					if(flag==true)
						continue;
					pos_j=j;
					break;
				}
			}
			
		}
		j=pos_j;
		//if(j>=0)
		if(j<=n && j>=0)         //ֻҪ�ҵ������λ�ã�֤�����Լ�����������
		{
			parentnode=childnode->parent[j].p;
			nodename=parentnode->position;
			array[i].p=parentnode;
			apath[i]=nodename;
			childnode=parentnode;
			pos[i]=j;
			if(parentnode->dis==false)
				//d1--;
				d1=d1-parentnode->deltadistance;
			i--;
		}
		else          //���򣬵�ǰλ���޷��ҵ�����س���
		{
			for(int j=0;j<=i;j++)     //�������λ��ȫ�����
				pos[j]=-1;
			i++;
			if(i>=ptn_len)            //�ҵ�ͷ�ˣ���û���ҵ����ͽ�������
			{
				delete[]array;
				delete[]pos;
				return false;
			}
			if((array[i].p)->dis==false)
				//d1++;
				d1=d1+(array[i].p)->deltadistance;
			childnode=array[i+1].p;
		}
	}
	delete[]array;
	delete[]pos;
	return true;
}
/***********************�����ҳ���***********************/
void disp_most_right_occ()
{
	int i;
	int count=0;
	int m1=head_level[ptn_len].count+1;
	int **nodeinpath;
	nodeinpath=new int*[seq_len];
	for(i=0;i<seq_len;i++)
		nodeinpath[i]=new int[d+1];
	bool *used;
	used=new bool[seq_len];
	for(i=0;i<seq_len;i++)
		used[i]=false;
	//	int *apath;
	//	apath=new int[ptn_len+1];
	int *apath2;
	apath2=new int[ptn_len+1];
	node *start_off=head_level[ptn_len].tail;
	while(start_off!=NULL)
	{
		if(used[start_off->position]==false)//������д�������ַ�û�б�ʹ��
		{
			cal_nodeinpath(nodeinpath,used);//����������Ҷ·����
			//	bool retval=Gready_Search_Occ(start_off,apath,used,nodeinpath);
			bool retval2=RightMost_Occ(start_off,apath2,used);
			if(retval2==true)
			{
				
				for(int j=0;j<ptn_len+1;j++)
					used[(apath2[j])]=true;
				
				printf("<");
				for(int k=0;k<ptn_len;k++)
					printf("%d,  ",apath2[k]);
				printf("%d>\n",apath2[ptn_len]);
				count++;
			}
			start_off=start_off->pre;
		}
		else
			start_off=start_off->pre;
	}
	cout<<"���ָ�����"<<count<<endl;
	sumocc+=count;
}

/***********************���������***********************/
void disp_most_left_occ()
{
	int i;
	int count=0;
	int m1=head_level[ptn_len].count+1;
	int **nodeinpath;
	nodeinpath=new int*[seq_len];
	for(i=0;i<seq_len;i++)
		nodeinpath[i]=new int[d+1];
	bool *used;
	used=new bool[seq_len];
	for(i=0;i<seq_len;i++)
		used[i]=false;
	//	int *apath;
	//	apath=new int[ptn_len+1];
	int *apath2;
	apath2=new int[ptn_len+1];
	node *start_off=head_level[ptn_len].head;
	while(start_off!=NULL)
	{
		if(used[start_off->position]==false)//������д�������ַ�û�б�ʹ��
		{
			cal_nodeinpath(nodeinpath,used);//����������Ҷ·����
			//	bool retval=Gready_Search_Occ(start_off,apath,used,nodeinpath);
			bool retval2=LeftMost_Occ(start_off,apath2,used);
			if(retval2==true)
			{
				
				for(int j=0;j<ptn_len+1;j++)
					used[(apath2[j])]=true;
				
				printf("<");
				for(int k=0;k<ptn_len;k++)
					printf("%d,  ",apath2[k]);
				printf("%d>\n",apath2[ptn_len]);
				count++;
			}
			start_off=start_off->next;
		}
		else
			start_off=start_off->next;
	}
	cout<<"���ָ�����"<<count<<endl;
	sumocc+=count;
}

/*************************������***********************/
int main()
{
	/*********����ģʽ����ת��**************/
	int k=0;
	//cout<<"please input pattern at first:"<<endl;
	//cin>>p;
	//char p0[100]="b[0,2]d[0,2]d[0,2]d";
	//char p0[100]="b[0,2]c[0,2]d[0,2]d";


	//char p0[100]="f[0,4]d[0,4]d[0,4]d";
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
	strcat(p,p1);
	convert_p_to_ruler(p);
	disp_pattern();
	
	/***************������ƶ�********************/
	cout<<"Input the delta:";
	cin>>delta;
	cout<<"Input the gama:";
	cin>>d;
	/********************��ȡ���д����洢******************/
	FILE *fp;
	if((fp=fopen("C:\\Users\\Administrator\\Desktop\\DataSet\\SDB7.txt","r"))==NULL)
		//if((fp=fopen("C:\\Users\\LeiLei\\Desktop\\fuhao.txt","r"))==NULL)
	{
		cout<<"\nconnot open file strike any key exit!"<<endl;
		exit(1);
	}
	DWORD   dwBeginTime=GetTickCount();
	while(fscanf(fp,"%s",t)==1)
	{
		createlevel();
		seq_len=strlen(t);
		seq_sumlen+=seq_len;
		//cout<<"S is "<<t<<endl;
		//cout<<"/////"<<seq_len<<endl;
		//cal_needrep();
		/******************��������******************/
		
		create_store(t);
		
		cout<<"-------------------------------"<<endl;
		//�ҳ���
		//disp_most_right_occ();
		disp_most_left_occ();
	}
	//cout<<fscanf(fp,"%s",t)<<endl;
	DWORD   dwEndTime0=GetTickCount();
	int time0=dwEndTime0-dwBeginTime;
	cout<<"-------------------------------"<<endl;
	
	fclose(fp);
	//cout<<"length of the seq is "<<seq_sumlen<<endl;
	cout<<"sum of occ is "<<sumocc<<endl;
	cout<<"The time cost is: "<<time0<<endl;	
	return 0;
}