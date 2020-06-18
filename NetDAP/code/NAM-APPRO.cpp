#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <windows.h>
#include <fstream>
using namespace std;

#define N 400000
#define M 100
#define D 50
#define K 600

struct node
{
	int name;
	vector <int> parent;
	vector <int> children;		
	int Npath[D];		
	int rootname;
	int delta;
};
struct sub_ptn_struct
{
	char start,end;		//
	int min,max;		//
};
struct occurrence
{
	vector <int > position;
};
char S[N];

struct seqdb                 
{
	int id;                  // sequence id  
	char S[N];               // sequence  
} sDB[K];

vector <occurrence> store;
sub_ptn_struct sub_ptn[M];  
int ptn_len=0;  
int seq_len=0;
int maxgap=-1;
int count=0;

void dealrange(char *p,int len)      
//put sub-pattern "a[1,3]b" into sub_ptn and sub_ptn.start=a，sub_ptn.end=b, sub_ptn.min=1，sub_ptn.max=3
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
		value=0;
		for (int j=i+1;p[j]!=']';j++)
			value=value*10+p[j]-48;
		sub_ptn[ptn_len].max=value;
	}
	if (sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1>maxgap)
		maxgap=sub_ptn[ptn_len].max-sub_ptn[ptn_len].min+1;
	ptn_len++;
}
void convert_p_to_ruler(char *p)    
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
	{
		cout<<"irregular pattern.\n";
		exit(-1);
	}
}
void disp_pattern()         //display the array sub_ptn
{
	for (int i=0;i<ptn_len;i++)
	{	
		cout<<sub_ptn[i].start<<"\t"<<sub_ptn[i].min<<"\t"
			<<sub_ptn[i].max<<"\t"<<sub_ptn[i].end<<endl;
	}
}
void initial(node & anode,int i,int T)
{
	anode.name =i;
	anode.parent.resize (0);
	anode.children.resize (0);
	anode.delta=-1;
	for(int h=0;h<=T;h++){
		anode.Npath[h]=0;
	}	
}
void createnettree(vector <node> *nettree,char *s, int d,int G)
{
	for (int i=0;i<ptn_len+1;i++)
		nettree[i].resize (0);//网树层初始化
	int *start;
	start=new int[ptn_len+1];
	for (i=0;i<ptn_len+1;i++)
		start[i]=0;
	node enode;
	initial(enode,0,G);
	for (i=0;i<strlen(s);i++)
	{
		//对每个字符逐一检查
		//对结点进行初始化；
		node root=enode;
		root.name =i;
		
		//*******************************第一层
		if(i<strlen(s)-ptn_len)
		{
			//直接存储该结点作为树根层
			int len=nettree[0].size ();
			
            //计算树根节点树根路径数
			if(sub_ptn[0].start -s[i]<=0)
				root.delta=s[i]-sub_ptn[0].start ;
			else if(sub_ptn[0].start -s[i]>=0)
				root.delta=sub_ptn[0].start-s[i];
			if(root.delta<=d){
				nettree[0].resize (len+1);
				for(int h=0;h<=G;h++){
					root.Npath[h]=0;	
				}
				root.Npath[root.delta]=1;
				nettree[0][len]=root;
			}
		}
		//*****************************************************第一层结束
		for (int j=0;j<ptn_len;j++)
		{
			node anode;
			anode=enode;
			anode.name =i;
			
			//上一层中找双亲
			int prev_len=nettree[j].size ();
			if (prev_len==0)
			{
				//说明尚无结点，可以退出
				break;
			}
			//更新起点  。。。
			for (int k=start[j];k<prev_len;k++)
			{
				int name=nettree[j][k].name;
				if (i-nettree[j][k].name -1>sub_ptn[j].max )
				{
					//超过最大上届，游标后移
					start[j]++;
				}
			}
			//判断间隙约束
			if (i-nettree[j][prev_len-1].name -1>sub_ptn[j].max)
			{
				//超过了最大间隙约束
				continue;
			}
			if (i-nettree[j][start[j]].name -1<sub_ptn[j].min)
			{
				continue;
			}	
			bool createnode=false;
			for (k=start[j];k<prev_len;k++)
			{
				//判断是否满足间隙约束
				int name=nettree[j][k].name;
				if (i-nettree[j][k].name -1>sub_ptn[j].max )
				{
					//超过最大上届，游标后移
					start[j]++;
					continue;
				}
				if (i-nettree[j][k].name -1<sub_ptn[j].min )
				{
					//超过最大下界，退出
					break;
				}
				//满足间隙约束，开始判断是否满足近似距离阈值
				if(sub_ptn[j].end-S[i]<=0)
					anode.delta=-(sub_ptn[j].end-S[i]);	
				else if(sub_ptn[j].end-S[i]>=0)
					anode.delta=sub_ptn[j].end-S[i];
				
				bool notAllZero = false;
				//剪枝
				if(anode.delta<=d){
					for(int h=0;h<=G-anode.delta;h++){
					if(nettree[j][k].Npath[h]!=0){
							createnode=true;
							notAllZero=true;
							break;
						}
					}
				}
				if (!notAllZero)//
					continue;
				//
				//在间隙约束之间，满足近似距离约束
				//建立父子关系
				int nc=nettree[j][k].children .size ();
				nettree[j][k].children.resize (nc+1);
				int len=nettree[j+1].size ();
				nettree[j][k].children [nc]=len;//存储位置
				
				int np=anode.parent .size ();
				anode.parent.resize (np+1);
				anode.parent [np]=k;					
				for(int e=0;e<=G-anode.delta;e++){
					anode.Npath[anode.delta+e]+=nettree[j][k].Npath[e];	
				}	
			}//k
			
			if (createnode==true)
			{
				int jlen=nettree[j+1].size ();
				nettree[j+1].resize (jlen+1);
				nettree[j+1][jlen]=anode;				
			}
		}//j
	}
	delete []start;
}

/*display the nettree*/
void displaynettree(vector <node> *nettree,int G)
{
	for (int i=0;i<ptn_len+1;i++)
	{
		cout <<i<<":   ";
		
		int size=nettree[i].size();
		for (int j=0;j<size;j++){
			//cout <<"["<<nettree[i][j].sum_delte<<"]  "<<nettree[i][j].name<<"、\t";
			//cout<<nettree[i][j].name<<" ";
			cout<<nettree[i][j].name<<":[";
			for(int h=0;h<=G;h++)
				cout<<nettree[i][j].Npath[h]<<" ";
			cout<<"]";

		}
		
		cout <<endl;
	}
	cout <<endl<<endl;
} 

int retnum(int rootposition,vector <node> *nettree,  int d,int G,int minLen,int maxLen)
{
	node *root, *current,*child;
	root=& nettree[0][rootposition];
	root->rootname =root->name ;
	int rootname=root->name;
	//cout<<"rootname="<<rootname<<endl;
	int start,end,childposition;
	start=end=rootposition;
	
	for (int ii=0;ii<=G;ii++)
	{
		root->Npath[ii]=0; 
	}
	if (root->delta >0)
	{
		root->Npath[root->delta]=1;
	}
	else
	{
		root->Npath[0]=1;
	}
	int startc,endc;
	for (int i=0;i<ptn_len;i++)
	{
		bool thefirst=true;
		for (int j=start;j<=end;j++)
		{
			current=& nettree[i][j];
			
			//当前结点现在不能被使用
			if (current->rootname  !=rootname)
				continue;
		//	if(current->name-rootname+1>maxLen)
		//		continue;
			int number=0;
			for (int ii=0;ii<=G;ii++)
				number+=current->Npath [ii];
			if (number==0)
				continue;
			
			int childnumber=current->children .size ();
			for (int k=0;k<childnumber;k++)
			{
				childposition=current->children [k];
				child= & nettree[i+1][childposition];
				
				bool notzero=false;
				if (child->rootname != rootname)
				{
					//表明第一次访问该结点
					child->rootname =rootname;
					for(int h=0;h<=G;h++){
						child->Npath[h]=0;
					}
					for (int ii=0;ii<=G-child->delta;ii++)
					{	
						child->Npath[child->delta+ii]=current->Npath[ii];	
					}
					for(ii=0;ii<=G;ii++){
						if(child->Npath[ii]!=0 ){
							//&&child->name-rootname+1<=maxLen&&child->name-rootname+1>=minLen
							notzero=true;
							break;
						}
					}
				}
				else//不是第一次访问
				{
					for (int ii=0;ii<=G-child->delta;ii++)
					{	
						child->Npath[child->delta+ii]+=current->Npath[ii];	
					}
					for( ii=0;ii<=G;ii++){
						if(child->Npath[ii]!=0){
							notzero=true;
							break;
						}
					}
				}//else
				if (notzero==true)
				{
					if (thefirst==true)
					{
						thefirst=false;
						startc=childposition;
						endc=childposition;
					}
					if (startc>childposition)
						startc=childposition;
					if (endc<childposition)
						endc=childposition;
					
				}
			}
		}
		if (thefirst==true)
		{
			//说明无孩子结点，不能抵达叶子；
			return 0;
		}
		else
		{	//更新下一层结点开始与结束
			start=startc;
			end=endc;
		}
	}
	//cout<<"根结点：-------------------------"<<rootname<<endl;
	int a=minLen-1+rootname;
	int b=maxLen-1+rootname;
	int sum=0;
	for( i=start;i<=end;i++){
		if(nettree[ptn_len][i].name<=b && nettree[ptn_len][i].name>=a){
			//cout<<nettree[ptn_len][i].name+1<<":[";
			/*for(int h=0;h<=G;h++)
				cout<<nettree[ptn_len][i].Npath[h]<<" ";
			cout<<"]";*/
			for(int j = 0;j <=G; j++)
			{

				sum += nettree[ptn_len][i].Npath[j];	
				//cout<<sum;
			}
		}
		

	}
	//cout<<endl;
	return sum;
	
}
void appro_nonoverlevel(int G,char *p,int d,char *s,int minLen,int maxLen)
{
   
	//disp_pattern();
	vector <node> *nettree;
	nettree=new vector <node> [ptn_len+1];
	createnettree(nettree,s,d,G);
	//displaynettree(nettree,G);
	for(int y=0;y<nettree[0].size();y++)
	   count+=retnum(y,nettree,   d, G,minLen, maxLen);
	
}
void read_file(char *p,int d,int G,int minLen,int maxLen)
{
	fstream file;
	string buff;
//	file.open("Protein\\A0A3Q3L924.txt",ios::in); //open sequence file S1
//	file.open("Protein\\A0A3B4UA59.txt",ios::in); //open sequence file S1
//	file.open("Protein\\A0A2N8LHF0.txt",ios::in); //open sequence file S3
//	file.open("Protein\\A0A078K571.txt",ios::in); //open sequence file S4
//	file.open("Protein\\A0A383R7Y4.txt",ios::in); //open sequence file S5
//	file.open("Protein\\A0A1E7WGV1.txt",ios::in); //open sequence file S6
//	file.open("Protein\\A0A3B0KSX7.txt",ios::in); //open sequence file S7
//	file.open("Protein\\A0A388K3K1.txt",ios::in); //open sequence file S8
  file.open("Protein\\time_data1.txt",ios::in); //open sequence file S9
// file.open("Protein\\time_data2.txt",ios::in); //open sequence file S10

	int i=0;
	convert_p_to_ruler(p);
	while(getline(file, buff))
	{
		strcpy(sDB[i].S, buff.c_str());
		strcpy(S,sDB[i].S);
		//cout<<S<<"\n";
		//cout<<i<<": "<<"\n";
		appro_nonoverlevel(G,p,d,S,minLen,maxLen);
		i++;
	}
	//cout<<d<<" "<<G;
}

int main(){
	int G,d,MinLen,MaxLen;
    cout<<"input d:"<<endl;
	cin>>d;
	cout<<"input G:"<<endl;
	cin>>G;

//	MinLen=11,MaxLen=40;//length constraits 
//	MinLen=13,MaxLen=38;//length constraits
//	MinLen=15,MaxLen=55;//length constraits

//	MinLen=16,MaxLen=40;//length constraits
	MinLen=11,MaxLen=35;//length constraits

//	char p[200]="D[2,9]F[1,8]E[0,10]K[2,6]A[3,7]G";
//	char p[200]="E[1,9]D[0,8]A[2,7]L[1,8]Q[4,9]R[3,8]U";
//	char p[200]="S[1,9]K[3,11]D[1,7]F[1,9]P[3,8]G[2,6]A[0,5]L";

//	char p[200]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S";
//	char p[200]="D[0,7]F[0,7]S[0,7]D[0,7]F[0,7]S[0,7]D[0,7]F";

//	char p[200]="G[0,8]D[0,8]S[0,8]A[0,8]F[0,8]T";
//	char p[200]="G[0,7]D[0,7]S[0,7]A[0,7]F[0,7]T";
	char p[200]="G[0,6]D[0,6]S[0,6]A[0,6]F[0,6]T";

//	char p[200]="r[0,5]s[0,5]k[0,5]c[0,5]k";
//	char p[200]="c[0,5]e[0,5]q[0,5]r[0,5]e[0,5]m[0,5]o":

 	DWORD begintime1=GetTickCount();
	for(int t = 1;t<4;t++)
	{
			DWORD begintime=GetTickCount();

			read_file(p,d,G,MinLen,MaxLen);
			DWORD endtime=GetTickCount();
			cout<<"共有"<<count<<"个出现"<<endl;

			cout<<t<<":"<<(endtime-begintime)<<"\n";
	}
	DWORD endtime1=GetTickCount();
	
	printf("The average cost of time is:%0.2f\n",(endtime1-begintime1)/3.0);
	cout<<"共有"<<count/3<<"个出现"<<endl;
	return 0;
}
