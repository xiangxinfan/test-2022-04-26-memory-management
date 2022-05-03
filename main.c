#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

//内存管理（是什么，为什么，怎么办）

//什么是动态内存？
//为什么要有动态内存
//什么是野指针
//对应到C空间布局，malloc在哪里申请空间（堆区）
//常见的内存错误和对策
//C中动态内存“管理”体现在哪里？


////什么是动态内存？？
//#include <stdlib.h>
//#define N 10
//int main()
//{
//	int* p = (int*)malloc(sizeof(int) * N);//动态开辟空间
//	if (NULL == p)
//	{
//		perror("malloc\n");
//		exit(1);
//	}
//	for (int i = 0; i < N; i++)
//	{
//		p[i] = i;//*(p+i)=i;//一个指针，两种写法
//	}
//	for (int i = 0; i < N; i++)
//	{
//		printf("p[%d]:%d\n", i, p[i]);
//	}
//	printf("\n");
//	free(p);	//开辟完之后，要程序员自主释放,如果不释放，可能会导致内存泄漏的问题
//	return 0;
//}


//为什么要有动态内存

	//在数组开辟大小的时候，当给数组变量分配1000000 的时候，系统就会崩溃，原因是出现了栈溢出
//也就是数组开辟的空间是有效的，用于小的内存空间分配的时候，可以运用数组进行，当使用大量的空间开辟的时候
//这时就需要使用动态内存malloc进行动态开辟空间

		//int a[1024 * 1024];//这种系统会直接而不能崩溃

	//1、在技术方面，普通的空间申请，都是在全局或者栈区，全局一般不太建议大量使用，而栈空间有限，那么如果
//一个应用需要大量的内存空间的时候，需要通过申请堆空间来支持基本业务。
	//2、在应用方面，程序员很难一次预估好自己总共需要花费多大的空间，想想之前我们定义的所有数组，因为其语法
//的约束，我们必须地明确“指出”空间的大小，但是如果用动态内存申请（malloc），因为malloc是函数，而函数就
//可以传参，也就意味着，我们可以通过具体的情况，对需要的内存大小进行动态计算，进而在传参申请，提供了很大
//的灵活性
	//3、题外话：
//如何辨别的看动态内存给程序员带来的灵活性
	
	//在给程序员中有这样一个好处：申请空间和用空间是确定的，用多少申请多少，这样的话可以通过程序员的合理
//编码，将运行的成本降到最低，任何空间的浪费，空间释放的延后性都会不存在
	//带来的问题就是：可能会因为程序BUG的问题将申请空间忘记释放


//malloc是函数，在C语言中函数在运行之后才会被调用，换句话说，malloc在运行之后才会被调用，动态空间也就
//在程序运行之后才会给动态进行分配空间

//如果在不用考虑一资源的分配，比较赶时间写一个项目，不用考虑动态资源的分配，可以考虑更高阶的语言；
//但是一般在写一些底层的开发，驱动程序，操作系统源代码或者嵌入式开发，资源本身就很有限，就需要在我们的程序
//当中，明确以更少的资源得到最高的效率，同时也要给代码带来极大的确定性，这里就可以选择C语言。


//栈、堆和静态区
	//C程序中的动态地址空间分布
		//自下而上：（也就是从低地址到高地址处）
			//代码区、字符常量区、已初始化全局数据区、未初始化全局数据区、堆区、栈区

//这个东西在Linux中看比较好，下面代码进行验证：

//这两个验证已初始化全局数据区和未初始化全局数据区，一个初始化，一个未初始化，反着定义就是为了看编译器能不能先编译已初始化，再编译未初始化

//#include <stdlib.h>
//int g_val2;
//int g_val1 = 10;
//int main()
//{
//	printf("code addr:%p\n", main);//代码区，打印函数的地址，哪个函数，main函数的地址
//	const char* str = "hello bit！";
//	printf("read only:%p\n", str);//字符常量区，应该输出的是str里面的内容，也就是hellobit的起始地址，并非str变量的地址，str是一个变量，是在栈上开辟的
//	printf("init g_val:%p\n", &g_val1);
//	printf("uninit g_val:%p\n", &g_val2);
//	//这下是堆区（就要进行malloc函数（内存管理））
//	char* p = (char*)malloc(10 * sizeof(char));//强转成char*
//	printf("heap addr:%p\n", &p);
//
//	//栈上
//	printf("stack addr:%p\n", &str);//str就是在栈上开辟的空间
//	printf("stack addr:%p\n", &p);//p就是在栈上开辟的空间
//
//	free(p);
//	//打印出来之后可以观察地址的排布，可以发现的确是由低到高进行排布的
//	return 0;
//}

//在Linux中验证是符合图中的动态地址空间分布

//堆区和栈区：(堆栈相对而生)
	//堆区向上增长，栈区向下增长，38到30可以说明是栈明显是向下增长的，每次后续定义的变量一定是后入栈，
//只要是后入栈就一定会是后开辟空间的，后开辟空间也就意味着地址一定是相对较小的，

//验证堆区：
	//char* p = (char*)malloc(10 * sizeof(char));
	//char* p1 = (char*)malloc(10 * sizeof(char));
	//char* p2 = (char*)malloc(10 * sizeof(char));
	//printf("heap addr:%p\n", &p);
	//printf("heap addr:%p\n", &p1);
	//printf("heap addr:%p\n", &p2);

//地址是10 30 50，明显是增长的，也就明显验证了是向上增长的

//栈区也可以打印一下：
	//printf("stack addr:%p\n", &p);
	//printf("stack addr:%p\n", &p1);
	//printf("stack addr:%p\n", &p2);
//地址是a0 98 90，明显是递减的，也就明显验证了是向下增长的

//VS编译器会对我们的内存布局情况进行各种各样的处理，所以实际看到的可能并不是这样的状态

//定义变量，一定是在栈上进行定义的，所以随机定义一个变量，其地址都可能出现在上面栈空间地址大

//当用static修饰这个临时变量的时候，这个变量的地址已经变成了一个临时变量的地址，这个变量在编译的时候已经
//被放在了全局数据区中

//放在了全局数据区，理所当然的肯定会使生命周期随着程序一直存在的


	//在C语言中，为何一个临时变量，使用static修饰之后，他的生命周期就变成全局的了？

//还是在Linux中：
//给一个变量加上static修饰之后，打印出来的地址与init g_val的值是一样的，也就是放在了已初始化全局数据区

	//代码区，字符常量区，已初始化全局数据区，未初始化全局数据区，堆区：基本上这些都是随着整个程序的运行而
//一直存在。全局变量会随着程序运行一直存在，static修饰的变量，为何改变的是生命周期呢？
	
	//原因是程序在编译的时候被编译进了全局数据区，所以具有全局特性

	//栈区：随着申请与释放而进行空间管理


	
//常见内存错误与对策
	//指针没有指向一块合法的内存
		//定义了指针变量，但是没有为指针分配内存，即指针没有指向一块合法的内存
	//1、结构体成员指针未初始化
//struct student
//{
//	char* name;
//	int score;
//}stu, * pstu;//结构体变量，结构体变量指针
//虽然定义了stu，*pstu，但是在name哪里一是没有进行初始化，二是没有进行分配内存，所以name就是典型的野指针
//int main()
//{
//	strcpy(stu.name, "Jimy");
//	stu.score = 99;
//	return 0;
//}

//书上的解释是这样的：这里定义了结构体变量stu，但是他没想到这个结构体内部char* name,该成员在定义结构体变量
//stu时，只是给name这个指针变量本身分配了4个字节；name指针并没有指向一个合法的地址，这时候其内部存的只是一
//些乱码。所以在电泳strcpy函数时，会将字符串"Jimy"往乱码所指的内存上复制，而这块内存name指针根本就无权访问
//导致出错。解决的办法是为name指针进行malloc一块空间。


//主函数这样子写呢：
//int main()
//{
//	pstu = (struct student*)malloc(sizeof(struct student));
//	strcpy(pstu->name, "Jimy");
//	pstu->score = 99;
//	free(pstu);
//	return 0;
//}
//这样更不行，只malloc了一个结构体变量的大小，但是name指针所指向的那块空间没有进行申请好，代码也是崩溃的

//书上的解释：为指针变量pstu分配了内存，但是同样没有给name指针分配内存，错误上与上面情况类似，解决办法也
//一样，用一个malloc给人一种错觉，以为也给name指针分配了内存。


	//2、没有为结构体指针分配足够的内存
//int main()
//{
//	pstu = (struct student*)malloc(sizeof(struct student*));
//	strcpy(pstu->name, "Jimy");
//	pstu->score = 99;
//	free(pstu);
//	return 0;
//}

//书上的解释：为pstu分配内存的时候，分配的内存大小不合适。这里把sizeof(struct student)误写为
//sizeof(struct student*)。当然name指针同样没有被分配内存，解决办法同上。

//下面这个例子是修改：
//#include <stdlib.h>
//struct stu {
//	char* name;
//	int score;
//};
//int main()
//{
//	struct stu zhangsan = { NULL,0 };
//	struct stu* p = NULL;
//	//修改这里，加上一句：
//	zhangsan.name = malloc(32);//给name同样申请一块空间
//	strcpy(zhangsan.name, "Tom");
//	return 0;
//}
//这种代码肯定是错的，修改在上面
	
//#include <stdlib.h>
//#include <assert.h>
//struct stu {
//	char* name;
//	int score;
//};
//void showname(char* name)
//{
//	//指针的合法性：指针如果有具体的指向(包括野指针),
//	//对应得合法性我们是没无法进行验证的，确认指针具体值得“合法性”，不是用户能做到的，判断这类指针问题本质是由操作系统决定的
//	//1、所有的指针，如果没有被直接使用，必须设置成NULL（编程规范的问题）
//	//2、在函数内部，要验证指针的合法性，本质是验证指针！=NULL
//	//if (name != NULL)
//	//{
//	//	//"合法"：能被用户直接使用的！应用层面解决的问题
//	//}
//	//else if (name == NULL)
//	//{
//	//	return;
//	//}
//	////应用层面的问题
//	assert(name == NULL);//内部条件如果不满足，则中断运行，调试代码的时候用，只在Debug下是有效的
//	//assert是检测不到野指针的
//	//assert本质是一个宏，就是用来检测指针是否合法，检测指针合法性更建议成写成if判断if(name==NULL) return;
//
//	printf("%s\n", name);//这个检测的代码报错是在这一行进行报错的，所以assert是检测不出来这个的
//}
//int main()
//{
//	struct stu zhangsan = { NULL,0 };
//	struct stu* p = NULL;
//	//修改这里，加上一句：
//	zhangsan.name = malloc(32);//给name同样申请一块空间
//	strcpy(zhangsan.name, "Tom");
//	//showname(zhangsan.name);//条件不满足
//
//	//assert检测一下野指针
//	int* q = 0x11223344;
//	showname(q);
//	
//	//showname(NULL);//条件满足
//	
//	return 0;
//}

//有一个检测条件是否合法assert,Evaluates an expression and when the result is FALSE, prints a diagnostic message and aborts the program.
//头文件是：<assert.h>

	//函数入口校验


	//为指针分配的内存太小
	//内存分配成功，但未初始化（初始化不是必须的，但是建议初始化）

//#include <stdlib.h>
//int main()
//{
//	int* p = malloc(sizeof(int) * 30);
//	//建议进行下面这个：
//	memset(p, 0, sizeof(int) * 30);//对p所指向的空间全部清0，一共有多少字节?
//	for (int i = 0; i < 30; i++)
//	{
//		printf("%d ", p[i]);//打印出来的都是随机值，这样打印出来就全部都是0了
//	}
//	printf("\n");
//	return 0;
//}

//初始化需要大量进行遍历取0


	//内存越界
//#include <stdlib.h>
//int main()
//{
//	int* p = malloc(sizeof(int) * 30);
//	memset(p, 0, sizeof(int) * 30);
//	for (int i = 0; i <= 30; i++)//本应该是小于30并不是小于等于30，所以这里就发生了内存越界问题，
//								 //越界并不等于报错，越界到一定的程度，系统就会崩溃
//	{
//		printf("%d ", p[i]);
//	}
//	printf("\n");
//	free(p);
//	return 0;
//}

//在遍历内存空间的时候，不一定“一定会报错”

////下面这个是个典型的越界问题：
//int main()
//{
//	int i = 0, j = 0;
//	int a[10] = { 0 };
//	for (int k = 0; k <= 10; k++)//这里访问的时候就产生了越界
//	{
//		a[k] = k;
//	}
//	return 0;
//}


		//内存泄露
//int main()
//{
//	while (1)
//	{
//		int* p = malloc(1024);//看任务管理器中的内存，就会发现内存在逐渐濒于满
//							  //产生了内存泄露，，只给系统要内存，但并没有归还，这就是内存泄漏
//							  
//	}
//}

	//如果程序退出了，内存泄露问题还存在吗？
	//这肯定是不在的，这也可以认为是一个常识，申请内存是在给系统要内存，当程序退出时，这个程序泄露问题就会
//自动消失，内存泄漏：什么样的程序，最怕内存泄漏问题？？
			//永远不会主动退出的程序
	//常驻（内存）进程（程序）
//什么样的程序不会主动退出：操作系统，杀毒软件，服务器程序

//int main()
//{
//	char* p = (char*)malloc(sizeof(char) * 10);//释放的字节实际上要比10个字节多得多，申请肯定不止10个字节
//	free(p);//我们目前只知道堆空间得起始地址，并没有传入要释放多少字节
//
//	//关于free的一个结论：
//		//实际malloc申请空间的时候，系统给你的其实更多！，多出来的部分：记录这次申请的更详细的信息；
//	//申请了多大的空间，确定大小(的问题)---->这里大小叫做cookie---->申请堆空间，是申请大的空间好呢，还是
//		//申请小空间好呢？？
//	//肯定是申请大空间比较好，这个cookie的比率会减小
//	return 0;
//}

//int main()
//{
//	char* p = (char*)malloc(sizeof(char) * 10);
//	printf("before:%p\n", p);
//	free(p);//目前只知道堆空间的起始地址，并没有传入要释放多少个字节
//	printf("after:%p\n", p);//会不会设置为NULL，以及为什么？
//	return 0;
//}

//与上面代码相同
	//free如何理解
//#include <windows.h>
//#include <stdlib.h>
//int main()
//{
//	int* p = (int*)malloc(sizeof(int) * 5);
//	int i = 0;
//	for (; i < 5; i++)
//	{
//		p[i] = i;
//	}
//	printf("before:%p\n", p);//before:000001A0C97BED00
//	free(p);//free就是取消指针和堆空间之间的关系
//	printf("after:%p\n", p);//after:000001A0C97BED00
//	//这两个地址空间并没有任何改变
//	
//	//问题：after这里此时还可以访问堆空间吗？
//		//但是当然是不能，所谓的释放，究竟是在做什么？？
//
//	system("pause");
//	return 0;
//}

	//计算机中所谓的关系，需要“数据”去维护
		//free之后，p就不能被使用了



//C中动态内存“管理”体现在哪里？
	//我们前面说到了malloc和free，能够进行有效的空间申请和释放
	//通常书中所说的内存“管理”体现在哪里呢？难道就是malloc和free？？
	
	//目前所有的书所表达的含义，其实是想表达两个含义
	//内存管理的本质其实是：空间什么时候申请，申请多少，什么时候释放，释放多少的问题。
	//1、场景：C的内存管理工作是由程序员决定的，而程序员什么时候申请，申请多少，什么时候释放，释放多少都是
//有场景决定的（比如上面的链表操作），而大部分书中，是讲具体操作，很少有场景，所以管理工作体现的并不直观。
//不过我们现在能理解即可
	//2、其他高级语言：像java这样的高级语言，语言本身自带了内存管理，所以程序员只管使用即可。换句话说，内存
//管理工作，程序员是不用关心的。但是C是较为底层的语言，它的内存管理工作是暴露给程序员的，从而给程序员提供了
//更多的灵活性，不过，管理工作也同时交给了程序员。

	//所以，因为上面的两点，C中的内存章节，基本都叫做内存管理
	//在C中，程序员+场景=内存管理


//内存泄露短期之内是没有什么影响的，跑完之后直接释放也是没什么影响的

//实现一个关于链表的实际场景（使用内存管理）
	//每隔一秒，往测试用例中插入一个节点，插入结点并且打印出来，插入完毕之后，再慢慢的把结点拿到手，
//最后再释放掉
	//可以展现出一个链表由无到有，由有到多，再从有到无的一个过程，其中会大量的使用内存申请，这是一个动态
//的效果


//#include <stdlib.h>
//#include <assert.h>
//#include <windows.h>
//
//#define N 10
////带头结点首先必须得有结点，所以先创建头结点
//typedef struct Node {
//	int data;//链表结点所携带的数据
//	struct Node* next;//指向下一个结点的指针
//}Node_t;//声明一个链表结点的类型
//
//Node_t* AllocNode(int x)//向系统内部要一个结点，什么样的结点，Node_t这样类型的一个结点，申请成功，返回就是这个结点，申请失败，就让程序中止
//{
//	//空间申请
//	Node_t* node = (Node_t*)malloc(sizeof(Node_t));//node叫做结构体指针变量
//	if (NULL == node)
//	{
//		perror("malloc!");//Print an error message.
//		exit(1);//直接退出，终止程序
//	}
//	//对结构体进行初始化
//	node->data = x;
//	node->next = NULL;
//
//	return node;//返回值的本质是通过寄存器进行拿到手的
//}
//
//void InsertNode(Node_t* head, int x)
//{
//	assert(head);//这里是判断head是否为False，如果为，则直接终止此程序
//	Node_t* node = AllocNode(x);
//	node->next = head->next;
//	head->next = node;
//}
//
//void ShowList(Node_t* head)
//{
//	assert(head);//先进行校验，校验head不为空
//
//	////如果觉得下面这种写法不好理解，就可以重新定义一个变量，这样写是因为指针也是可以发生临时拷贝的
//	//head = head->next;
//	//while ()
//	//{
//	//	printf("%d ", head->data);
//	//}
//
//	Node_t* p = head->next;
//	while (p) {
//		printf("%d ", p->data);
//		p = p->next;//绝对不能p++;
//	}
//	printf("\n");
//
//}
//
//void DeleteNode(Node_t* head)
//{
//	assert(head);
//	Node_t* p = head->next;
//	head->next = p->next;
//	free(p);
//	p = NULL;
//}
//
//int main()
//{
//	Node_t* head = AllocNode(0);//这里表示要申请一个结点，默认将其data的值设为0
//	//测试用例，自顶向下进行写
//	//插入
//	printf("插入测试.....\n");
//	for (int i=0; i <= N; i++)//遍历10次
//	{
//		//插入的是头插
//		InsertNode(head,i);//插入结点，往那里插，就是往head所指向的链表中插入结点，这个结点是什么，是i
//		ShowList(head);// 将链表内容打印出来 
//		Sleep(1000);//1000毫秒，也就是1秒
//	}
//	//删除
//	printf("删除测试.....\n");
//	for (int i = 1; i <= N; i++)
//	{
//		//删除这里是头删
//		DeleteNode(head);//删除结点
//		ShowList(head);// 将链表内容打印出来 
//		Sleep(1000);//1000毫秒，也就是1秒
//	}
//	free(head);
//	return 0;
//}
//打印出来就是：头插和头删
//插入测试.....
//0
//1 0
//2 1 0
//3 2 1 0
//4 3 2 1 0
//5 4 3 2 1 0
//6 5 4 3 2 1 0
//7 6 5 4 3 2 1 0
//8 7 6 5 4 3 2 1 0
//9 8 7 6 5 4 3 2 1 0
//10 9 8 7 6 5 4 3 2 1 0
//删除测试.....
//9 8 7 6 5 4 3 2 1 0
//8 7 6 5 4 3 2 1 0
//7 6 5 4 3 2 1 0
//6 5 4 3 2 1 0
//5 4 3 2 1 0
//4 3 2 1 0
//3 2 1 0
//2 1 0
//1 0
//0
