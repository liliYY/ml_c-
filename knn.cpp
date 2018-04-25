#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <queue> 
#include <map>
using namespace std;
#define Attribution_num 4                    //����ά�� 
#define  MAX_SIZE_OF_TRAINING_SET  1000      //ѵ�����ݼ�������С
#define  MAX_SIZE_OF_TEST_SET      100       //�������ݼ�������С
#define  MAX_VALUE  10000.0                  //�������ֵ
#define k 5                                 //kֵ
    
struct Vector {
 int ID;                      //ID��
 string classLabel;            //������
 double attributes[Attribution_num]; //���� 
};
struct VectorDistance{
 Vector ve;                    //���� 
 double distance;             //����
 friend operator < (VectorDistance v1,VectorDistance v2){
            return v1.distance > v2.distance;
        }
};

struct Vector gTrainingSet[MAX_SIZE_OF_TRAINING_SET]; //ѵ�����ݼ�
struct Vector gTestSet[MAX_SIZE_OF_TEST_SET];         //�������ݼ�
int curTrainingSetSize=0;                                 //ѵ�����ݼ��Ĵ�С
int curTestSetSize=0;                                     //�������ݼ��Ĵ�С

double Distance(struct Vector vector1,struct Vector vector2)
{
 double dist,sum=0.0;
 for(int i=0;i<Attribution_num;i++)
 {
  sum+=(vector1.attributes[i]-vector2.attributes[i])*(vector1.attributes[i]-vector2.attributes[i]);
 }
 dist=sqrt(sum);
 return dist;
}


string classify(struct Vector vector)
{
	priority_queue<VectorDistance> vdistance;            //����ֵ��С�������� 
    map<string,int> vote;                                //ͶƱ�� 
	map<string,int>::iterator p,b;                            
	double dist=0;
	VectorDistance tem; 
	for(int i=0;i<curTrainingSetSize;i++)
	{
		dist=Distance(gTrainingSet[i],vector);
		tem.ve=gTrainingSet[i];
		tem.distance=dist;
		vdistance.push(tem);
	}	
	for(int i=0;i<k;i++)
	{
		tem=vdistance.top();
		vdistance.pop();
		p=vote.find(tem.ve.classLabel);  
        if(p!=vote.end()){  
            p->second++;  
        }  
        else{  
            vote.insert(pair<string,int>(tem.ve.classLabel,1));  
        }		  
	}
	double max=0;
	string classifyresult;
	for(b=vote.begin();b!=vote.end();b++)
	{
		if(b->second>max)
		{
			max=b->second;
			classifyresult=b->first;
		}
	}
	return classifyresult;
}
int main()
{
char c;
string classLabel;
 int i,j, rowNo=0,TruePositive=0,FalsePositive=0;
 ifstream filein("./data.txt");
 FILE *fp;
 if(filein.fail()){cout<<"Can't open data.txt"<<endl; return 1;}
 //step.1---���ļ� 
 while(!filein.eof())
 {
  rowNo++;//��һ������rowNo=1
  if(curTrainingSetSize>=MAX_SIZE_OF_TRAINING_SET)
  {
   cout<<"The training set has "<<MAX_SIZE_OF_TRAINING_SET<<" examples!"<<endl<<endl;
   break ;
  }  
  //rowNo%3!=0��100��������Ϊѵ�����ݼ�
  if(rowNo%3!=0)
  {   
   gTrainingSet[curTrainingSetSize].ID=rowNo;
   for(int i = 0;i <Attribution_num;i++)
   {     
    filein>>gTrainingSet[curTrainingSetSize].attributes[i];
    filein>>c;
   }   
   filein>>gTrainingSet[curTrainingSetSize].classLabel;
   curTrainingSetSize++;
   
  }
  //ʣ��rowNo%3==0��50�����������ݼ�
  else if(rowNo%3==0)
  {
   gTestSet[curTestSetSize].ID=rowNo;
   for(int i = 0;i < Attribution_num;i++)
   {    
    filein>>gTestSet[curTestSetSize].attributes[i];
    filein>>c;
   }  
   filein>>gTestSet[curTestSetSize].classLabel;
   curTestSetSize++;
  }
 }
 filein.close();
 ofstream outfile("./result.txt", ios::app); 
 //��KNN�㷨���з���
 outfile <<"************************************����˵��***************************************\n";
 outfile <<"** ����KNN�㷨��iris.data���ࡣΪ�˲������㣬�Ը����������rowNo����***************\n";
 outfile <<"** ����150������,ѡ��rowNoģ3������0��100����Ϊѵ�����ݼ���ʣ�µ�50�����������ݼ�\n";
 outfile <<"***********************************************************************************\n\n";
 outfile <<"************************************ʵ����***************************************\n\n";
 for(i=0;i<curTestSetSize;i++)
 {
        outfile <<"************************************��"<<i+1<<"������**************************************\n";
  classLabel =classify(gTestSet[i]);
     if(classLabel==gTestSet[i].classLabel)//���ʱ��������ȷ
  {
   TruePositive++;
  }
  cout<<"rowNo: ";
  cout<<gTestSet[i].ID<<"    \t";
  cout<<"KNN������:      ";

  cout<<classLabel<<"(��ȷ����: ";
  cout<<gTestSet[i].classLabel<<")\n";
  outfile <<"rowNo:  "<<gTestSet[i].ID<<"   \t  KNN������:  "<<classLabel<<" ( ��ȷ����:  "<<gTestSet[i].classLabel<<" )\n";
  if(classLabel!=gTestSet[i].classLabel)//����ʱ���������
  {
  // cout<<"   ***�������***\n";
   outfile <<"                                                                      ***�������***\n";
  }
 outfile <<"\n";
 }
    FalsePositive=curTestSetSize-TruePositive;
 outfile <<"***********************************�������**************************************\n";
 outfile <<"TP(True positive): "<<TruePositive<<"\nFP(False positive): "<<FalsePositive<<"\naccuracy: "<<double(TruePositive)/(curTestSetSize-1)<<"\n";
 outfile.close();
 getchar();
 return 0;
}
