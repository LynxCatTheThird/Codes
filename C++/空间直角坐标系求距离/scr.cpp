#include<cstdio>
#include<iostream>
#include<cmath>
using namespace std;
int main(){
	//定义
	long long x1,y1,z1,x2,y2,z2;
	double d,t,a;
	//输入
	cout<<"确保面向右手系正方向"<<endl;
	cout<<"两点坐标--->";
	scanf("%lld %lld %lld %lld %lld %lld",&x1,&y1,&z1,&x2,&y2,&z2);
	/*----------------------------------------判断方向------------------------------------------*/
	//1.前后相等
	if(x2==x1&&y2==y1&&z2==z1){
		cout<<"判断一：不动";
		return 0;
	}
	//2.全45°
	else if(x2-x1==y2-y1&&y2-y1==z2-z1){
		cout<<"判断二：向正";
		if(x2>x1){
			cout<<"后";
		}
		if(x2<x1){
			cout<<"前";
		}
		if(y2>y1){
			cout<<"右";
		}
		if(y2<y1){
			cout<<"左";
		}
		if(z2>z1){
			
			cout<<"上";
		}
		if(z2<z1){	
			cout<<"下";
		}
	}
	//3.沿坐标轴移动
	else if((x2==x1&&y2==y1)||(x2==x1&&z2==z1)||(y2==y1&&z2==z1)){
		cout<<"判断三：向正";
		if(y2==y1&&z2==z1){
			if(x2>x1){
				cout<<"后";
			}
			if(x2<x1){
				cout<<"前";
			}
		}
		else if(x2==x1&&z2==z1){
			if(y2>y1){
				cout<<"右";
			}
			if(y2<y1){
				cout<<"左";
			}
		}
		else if(x2==x1&&y2==y1){
			if(z2>z1){
				cout<<"上";
			}
			if(z2<z1){
				cout<<"下";
			}
		}
	}
	//4.有数值前后相等
	else if(x2==x1||y2==y1||z2==z1){
		cout<<"判断四：向";
		if(x2==x1){
			a=atan(1.0/abs(y2-y1))*(180/3.14159265);//执行acot并从弧度转角度
			if(z2>z1){
				cout<<"上偏";
			}
			if(z2<z1){
				cout<<"下偏";
			}
			if(y2>y1){
				cout<<"右";
			}
			if(y2<y1){
				cout<<"左";
			}
			printf("%.15lf°",a);
		}
		else if(y2==y1){
			a=atan(1.0/abs(x2-x1))*(180/3.14159265);//执行acot并从弧度转角度
			if(z2>z1){
				cout<<"上偏";
			}
			if(z2<z1){
				cout<<"下偏";
			}
			if(x2>x1){
				cout<<"后";
			}
			if(x2<x1){
				cout<<"前";
			}
			printf("%.15lf°",a);
		}
		else if(z2==z1){
			a=atan(1.0/abs(y2-y1))*(180/3.14159265);//执行acot并从弧度转角度
			if(x2>x1){
				cout<<"右偏";
			}
			if(x2<x1){
				cout<<"左偏";
			}
			if(y2>y1){
				cout<<"右";
			}
			if(y2<y1){
				cout<<"左";
			}
			printf("%.15lf°",a);
		}
	}
	//5.一般数值
	else{
		cout<<"判断五：向";
		a=atan(1.0/abs(y2-y1))*(180/3.14159265);//执行acot并从弧度转角度
		if(z2>z1){
			cout<<"上偏";
		}
		if(z2<z1){
			cout<<"下偏";
		}
		if(y2>y1){
			cout<<"右";
		}
		if(y2<y1){
			cout<<"左";
		}
		printf("%.15lf",a);
		a=atan(1.0/abs(x2-x1))*(180/3.14159265);//执行acot并从弧度转角度
		if(x2>x1){
			cout<<"°、偏后";	
		}
		if(x2<x1){
			cout<<"°、偏前";
		}
		printf("%.15lf°(大致方向为",a);
		if(x2>x1){
			cout<<"后";
		}
		if(x2<x1){
			cout<<"前";
		}
		if(y2>y1){
			cout<<"右";
		}
		if(y2<y1){
			cout<<"左";
		}
		if(z2>z1){
			
			cout<<"上方) ";
		}
		if(z2<z1){	
			cout<<"下方) ";
		}
	}
	cout<<"方向行进 ";
	/*------------------------------------------计算并输出距离-------------------------------------*/
	d=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
	t=sqrt(x2*x2+y2*y2+z2*z2);
	printf("%.15lf",d);
	cout<<" 单位长度"<<endl<<"目标点距离坐标轴原点 ";
	printf("%.15lf",t);
	cout<<" 单位长度"<<endl;
	return 0;
