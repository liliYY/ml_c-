#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <queue> 
#include <map>
using namespace std;
#define Attribution_num 4                    //属性维度 
#define  MAX_SIZE_OF_TRAINING_SET  1000      //训练数据集的最大大小
#define  MAX_SIZE_OF_TEST_SET      100       //测试数据集的最大大小
#define  MAX_VALUE  10000.0                  //属性最大值
#define k 5                                 //k值
    
struct Vector {
 int ID;                      //ID号
 string classLabel;            //分类标号
 double attributes[Attribution_num]; //属性 
};
struct VectorDistance{
 Vector ve;                    //向量 
 double distance;             //距离
 friend operator < (VectorDistance v1,VectorDistance v2){
            return v1.distance > v2.distance;
        }
};

struct Vector gTrainingSet[MAX_SIZE_OF_TRAINING_SET]; //训练数据集
struct Vector gTestSet[MAX_SIZE_OF_TEST_SET];         //测试数据集
int curTrainingSetSize=0;                                 //训练数据集的大小
int curTestSetSize=0;                                     //测试数据集的大小

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
	priority_queue<VectorDistance> vdistance;            //距离值从小到大排序 
    map<string,int> vote;                                //投票器 
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
 //step.1---读文件 
 while(!filein.eof())
 {
  rowNo++;//第一组数据rowNo=1
  if(curTrainingSetSize>=MAX_SIZE_OF_TRAINING_SET)
  {
   cout<<"The training set has "<<MAX_SIZE_OF_TRAINING_SET<<" examples!"<<endl<<endl;
   break ;
  }  
  //rowNo%3!=0的100组数据作为训练数据集
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
  //剩下rowNo%3==0的50组做测试数据集
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
 //用KNN算法进行分类
 outfile <<"************************************程序说明***************************************\n";
 outfile <<"** 采用KNN算法对iris.data分类。为了操作方便，对各组数据添加rowNo属性***************\n";
 outfile <<"** 共有150组数据,选择rowNo模3不等于0的100组作为训练数据集，剩下的50组做测试数据集\n";
 outfile <<"***********************************************************************************\n\n";
 outfile <<"************************************实验结果***************************************\n\n";
 for(i=0;i<curTestSetSize;i++)
 {
        outfile <<"************************************第"<<i+1<<"组数据**************************************\n";
  classLabel =classify(gTestSet[i]);
     if(classLabel==gTestSet[i].classLabel)//相等时，分类正确
  {
   TruePositive++;
  }
  cout<<"rowNo: ";
  cout<<gTestSet[i].ID<<"    \t";
  cout<<"KNN分类结果:      ";

  cout<<classLabel<<"(正确类标号: ";
  cout<<gTestSet[i].classLabel<<")\n";
  outfile <<"rowNo:  "<<gTestSet[i].ID<<"   \t  KNN分类结果:  "<<classLabel<<" ( 正确类标号:  "<<gTestSet[i].classLabel<<" )\n";
  if(classLabel!=gTestSet[i].classLabel)//不等时，分类错误
  {
  // cout<<"   ***分类错误***\n";
   outfile <<"                                                                      ***分类错误***\n";
  }
 outfile <<"\n";
 }
    FalsePositive=curTestSetSize-TruePositive;
 outfile <<"***********************************结果分析**************************************\n";
 outfile <<"TP(True positive): "<<TruePositive<<"\nFP(False positive): "<<FalsePositive<<"\naccuracy: "<<double(TruePositive)/(curTestSetSize-1)<<"\n";
 outfile.close();
 getchar();
 return 0;
}
